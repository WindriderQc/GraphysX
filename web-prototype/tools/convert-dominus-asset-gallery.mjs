#!/usr/bin/env node

// Converts the 63 recoverable DirectX text meshes in the audited Dominus asset
// family into independent lazy gallery payloads. It deliberately creates no
// inter-asset transforms or scene composition. The two binary X files remain
// manifest records with no proxy geometry.

import assert from "node:assert/strict";
import { execFileSync } from "node:child_process";
import { createHash } from "node:crypto";
import { copyFileSync, mkdirSync, readFileSync, statSync, writeFileSync } from "node:fs";
import { basename, dirname, extname, join, relative, resolve } from "node:path";
import { fileURLToPath } from "node:url";

const here = dirname(fileURLToPath(import.meta.url));
const project = resolve(here, "..");
const workspace = resolve(project, "..");
const dominusRoot = join(workspace, "Yanik C++ BCKUP", "Media", "Models", "Dominus Art");
const auditPath = join(project, "src", "legacy", "dominus-family-audit.json");
const manifestPath = join(project, "src", "legacy", "dominus-asset-gallery.json");
const publicRoot = join(project, "public", "assets", "dominus-gallery");
const meshOutputRoot = join(publicRoot, "meshes");
const textureOutputRoot = join(publicRoot, "textures");
const IDENTITY = [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1];
const round = (value) => Math.round(value * 1_000_000) / 1_000_000;
const sha256 = (data) => createHash("sha256").update(data).digest("hex").toUpperCase();
const sourceLabel = (path) => relative(workspace, path).replaceAll("\\", "/");
const cleanName = (value) => value.split(/[\\/]/).filter(Boolean).at(-1) ?? value;
const assetId = (name) => basename(name, extname(name)).toLowerCase().replace(/[^a-z0-9_-]/g, "_");

function multiplyMatrix(a, b) {
  const output = new Array(16).fill(0);
  for (let row = 0; row < 4; row += 1) {
    for (let column = 0; column < 4; column += 1) {
      for (let inner = 0; inner < 4; inner += 1) output[row * 4 + column] += a[row * 4 + inner] * b[inner * 4 + column];
    }
  }
  return output;
}

function transformPoint(matrix, x, y, z) {
  return [
    round(x * matrix[0] + y * matrix[4] + z * matrix[8] + matrix[12]),
    round(x * matrix[1] + y * matrix[5] + z * matrix[9] + matrix[13]),
    round(x * matrix[2] + y * matrix[6] + z * matrix[10] + matrix[14])
  ];
}

const tokenizeX = (text) => text.replace(/\/\/[^\n]*/g, "").replace(/#[^\n]*/g, "").match(/"[^"]*"|[{}]|[^\s{}]+/g) ?? [];

function parseNumbers(tokens, state, count) {
  const numbers = [];
  while (numbers.length < count) {
    if (state.queue.length) {
      numbers.push(state.queue.shift());
      continue;
    }
    if (state.index >= tokens.length) break;
    const token = tokens[state.index];
    const parsed = token.split(/[;,]+/).filter(Boolean).map(Number).filter(Number.isFinite);
    if (!parsed.length) {
      if (token === "{" || token === "}") break;
      state.index += 1;
      continue;
    }
    state.index += 1;
    state.queue.push(...parsed);
  }
  return numbers;
}

function seekOpenBrace(tokens, state) {
  while (state.index < tokens.length && tokens[state.index] !== "{") state.index += 1;
  if (tokens[state.index] !== "{") throw new Error("DirectX text block has no opening brace.");
  state.index += 1;
  state.queue = [];
}

function consumeBlockRemainder(tokens, state) {
  let depth = 1;
  while (depth > 0 && state.index < tokens.length) {
    const token = tokens[state.index++];
    if (token === "{") depth += 1;
    else if (token === "}") depth -= 1;
  }
  state.queue = [];
}

function skipTemplate(tokens, state) {
  state.index += 1;
  while (state.index < tokens.length && tokens[state.index] !== "{") state.index += 1;
  if (tokens[state.index] === "{") {
    state.index += 1;
    consumeBlockRemainder(tokens, state);
  }
}

const defaultMaterial = (index) => ({
  name: `material-${index}`,
  color: [0.72, 0.72, 0.72, 1],
  specularPower: 0,
  specular: [0, 0, 0],
  emissive: [0, 0, 0],
  textureName: null,
  textureUrl: null
});

function parseMaterial(tokens, state) {
  let name = "material";
  if (tokens[state.index] !== "{") name = tokens[state.index++];
  seekOpenBrace(tokens, state);
  const color = parseNumbers(tokens, state, 4).map(round);
  const specularPower = round(parseNumbers(tokens, state, 1)[0] ?? 0);
  const specular = parseNumbers(tokens, state, 3).map(round);
  const emissive = parseNumbers(tokens, state, 3).map(round);
  let textureName = null;
  let depth = 1;
  while (depth > 0 && state.index < tokens.length) {
    const token = tokens[state.index];
    if (token === "TextureFilename") {
      state.index += 1;
      seekOpenBrace(tokens, state);
      const value = tokens[state.index];
      if (value?.startsWith('"')) textureName = cleanName(value.replaceAll('"', "").replace(/;$/, ""));
      consumeBlockRemainder(tokens, state);
    } else if (token === "{") {
      state.index += 1;
      depth += 1;
    } else if (token === "}") {
      state.index += 1;
      depth -= 1;
    } else state.index += 1;
  }
  return { name, color, specularPower, specular, emissive, textureName, textureUrl: null };
}

function parseMaterialList(tokens, state) {
  seekOpenBrace(tokens, state);
  const materialCount = parseNumbers(tokens, state, 1)[0] ?? 0;
  const faceMaterialCount = parseNumbers(tokens, state, 1)[0] ?? 0;
  const faceMaterialIndices = parseNumbers(tokens, state, faceMaterialCount);
  const materials = [];
  let depth = 1;
  while (depth > 0 && state.index < tokens.length) {
    const token = tokens[state.index];
    if (token === "Material") {
      state.index += 1;
      materials.push(parseMaterial(tokens, state));
    } else if (token === "{") {
      state.index += 1;
      depth += 1;
    } else if (token === "}") {
      state.index += 1;
      depth -= 1;
    } else state.index += 1;
  }
  while (materials.length < materialCount) materials.push(defaultMaterial(materials.length));
  return { faceMaterialIndices, materials };
}

function parseXMesh(tokens, state, worldMatrix, defaultName) {
  let name = defaultName;
  if (tokens[state.index] !== "{") name = tokens[state.index++];
  seekOpenBrace(tokens, state);
  const vertexCount = parseNumbers(tokens, state, 1)[0];
  const rawPositions = parseNumbers(tokens, state, vertexCount * 3);
  const faceCount = parseNumbers(tokens, state, 1)[0];
  const faces = [];
  for (let face = 0; face < faceCount; face += 1) {
    const count = parseNumbers(tokens, state, 1)[0];
    const indices = parseNumbers(tokens, state, count);
    assert(indices.every((index) => index >= 0 && index < vertexCount), `${name}: out-of-range face index`);
    faces.push(indices);
  }
  let uvs = null;
  let faceMaterialIndices = new Array(faceCount).fill(0);
  let materials = [];
  let depth = 1;
  while (depth > 0 && state.index < tokens.length) {
    const token = tokens[state.index];
    if (token === "MeshTextureCoords") {
      state.index += 1;
      seekOpenBrace(tokens, state);
      const count = parseNumbers(tokens, state, 1)[0];
      uvs = parseNumbers(tokens, state, count * 2).map(round);
      consumeBlockRemainder(tokens, state);
    } else if (token === "MeshMaterialList") {
      state.index += 1;
      const parsed = parseMaterialList(tokens, state);
      faceMaterialIndices = parsed.faceMaterialIndices;
      materials = parsed.materials;
    } else if (token === "{") {
      state.index += 1;
      depth += 1;
    } else if (token === "}") {
      state.index += 1;
      depth -= 1;
    } else state.index += 1;
  }
  const positions = [];
  for (let index = 0; index < vertexCount; index += 1) positions.push(...transformPoint(worldMatrix, rawPositions[index * 3], rawPositions[index * 3 + 1], rawPositions[index * 3 + 2]));
  assert(uvs === null || uvs.length === vertexCount * 2, `${name}: UV count mismatch`);
  assert.equal(faceMaterialIndices.length, faceCount, `${name}: material face count mismatch`);
  if (!materials.length) materials.push(defaultMaterial(0));
  const byMaterial = new Map();
  faces.forEach((face, faceIndex) => {
    const materialIndex = faceMaterialIndices[faceIndex] ?? 0;
    const indices = byMaterial.get(materialIndex) ?? [];
    for (let index = 1; index < face.length - 1; index += 1) indices.push(face[0], face[index + 1], face[index]);
    byMaterial.set(materialIndex, indices);
  });
  const indices = [];
  const groups = [];
  for (const [materialIndex, materialIndices] of [...byMaterial].sort((left, right) => left[0] - right[0])) {
    while (materials.length <= materialIndex) materials.push(defaultMaterial(materials.length));
    groups.push({ start: indices.length, count: materialIndices.length, materialIndex });
    indices.push(...materialIndices);
  }
  return { name, vertexCount, faceCount, triangleCount: indices.length / 3, positions, uvs, indices, groups, materials };
}

function parseXFrame(tokens, state, parentMatrix, meshes) {
  let name = "unnamed";
  if (tokens[state.index] !== "{") name = tokens[state.index++];
  seekOpenBrace(tokens, state);
  let local = IDENTITY;
  let depth = 1;
  while (depth > 0 && state.index < tokens.length) {
    const token = tokens[state.index];
    if (token === "FrameTransformMatrix") {
      state.index += 1;
      seekOpenBrace(tokens, state);
      local = parseNumbers(tokens, state, 16);
      consumeBlockRemainder(tokens, state);
    } else if (token === "Frame") {
      state.index += 1;
      parseXFrame(tokens, state, multiplyMatrix(local, parentMatrix), meshes);
    } else if (token === "Mesh") {
      state.index += 1;
      meshes.push(parseXMesh(tokens, state, multiplyMatrix(local, parentMatrix), name));
    } else if (token === "{") {
      state.index += 1;
      depth += 1;
    } else if (token === "}") {
      state.index += 1;
      depth -= 1;
    } else state.index += 1;
  }
}

function boundsOfMeshes(meshes) {
  const min = [Infinity, Infinity, Infinity];
  const max = [-Infinity, -Infinity, -Infinity];
  for (const mesh of meshes) {
    for (let index = 0; index < mesh.positions.length; index += 3) {
      for (let axis = 0; axis < 3; axis += 1) {
        min[axis] = Math.min(min[axis], mesh.positions[index + axis]);
        max[axis] = Math.max(max[axis], mesh.positions[index + axis]);
      }
    }
  }
  return { min: min.map(round), max: max.map(round), size: max.map((value, axis) => round(value - min[axis])) };
}

function parseXAsset(path) {
  const data = readFileSync(path);
  assert(data.subarray(0, 16).toString("latin1").includes("txt"), `${path}: text X expected`);
  const text = data.toString("latin1");
  const tokens = tokenizeX(text.slice(16));
  const state = { index: 0, queue: [] };
  const meshes = [];
  while (state.index < tokens.length) {
    if (tokens[state.index] === "template") skipTemplate(tokens, state);
    else if (tokens[state.index] === "Frame") {
      state.index += 1;
      parseXFrame(tokens, state, IDENTITY, meshes);
    } else if (tokens[state.index] === "Mesh") {
      state.index += 1;
      meshes.push(parseXMesh(tokens, state, IDENTITY, basename(path)));
    } else state.index += 1;
  }
  const rawReferences = [...text.matchAll(/TextureFilename\s*\{\s*"([^"]+)"/gi)].map((match) => cleanName(match[1]).toLowerCase());
  const parsedReferences = meshes.flatMap((mesh) => mesh.materials.map((material) => material.textureName?.toLowerCase()).filter(Boolean));
  assert.deepEqual([...new Set(parsedReferences)].sort(), [...new Set(rawReferences)].sort(), `${basename(path)} texture/material parse mismatch`);
  return {
    source: sourceLabel(path),
    bytes: data.length,
    sha256: sha256(data),
    bounds: boundsOfMeshes(meshes),
    meshCount: meshes.length,
    vertexCount: meshes.reduce((sum, mesh) => sum + mesh.vertexCount, 0),
    faceCount: meshes.reduce((sum, mesh) => sum + mesh.faceCount, 0),
    triangleCount: meshes.reduce((sum, mesh) => sum + mesh.triangleCount, 0),
    materialGroupCount: meshes.reduce((sum, mesh) => sum + mesh.groups.length, 0),
    normals: "derived at runtime from exact transformed vertices and triangulated faces",
    meshes
  };
}

mkdirSync(meshOutputRoot, { recursive: true });
mkdirSync(textureOutputRoot, { recursive: true });
const audit = JSON.parse(readFileSync(auditPath, "utf8"));
const textureFiles = new Map();
for (const mesh of audit.inventory.meshes) {
  for (const textureName of mesh.textureReferences) {
    const key = textureName.toLowerCase();
    if (!textureFiles.has(key)) textureFiles.set(key, join(dominusRoot, textureName));
  }
}
assert.equal(textureFiles.size, 83);

const textureRecords = [];
const textureUrlByName = new Map();
for (const [key, sourcePath] of [...textureFiles].sort(([left], [right]) => left.localeCompare(right))) {
  const sourceExtension = extname(sourcePath).toLowerCase().slice(1);
  const base = basename(sourcePath, extname(sourcePath)).toLowerCase().replace(/[^a-z0-9_-]/g, "_");
  const outputExtension = sourceExtension === "tga" ? "png" : sourceExtension;
  const outputName = `${base}_${sourceExtension}.${outputExtension}`;
  const outputPath = join(textureOutputRoot, outputName);
  if (sourceExtension === "tga") {
    execFileSync("python", ["-c", "from PIL import Image; import sys; Image.open(sys.argv[1]).save(sys.argv[2], format='PNG')", sourcePath, outputPath]);
  } else copyFileSync(sourcePath, outputPath);
  const url = `/assets/dominus-gallery/textures/${outputName}`;
  textureRecords.push({
    source: sourceLabel(sourcePath),
    sourceBytes: statSync(sourcePath).size,
    sourceSha256: sha256(readFileSync(sourcePath)),
    url,
    outputBytes: statSync(outputPath).size,
    outputSha256: sha256(readFileSync(outputPath)),
    conversion: sourceExtension === "tga" ? "lossless-pixel TGA to PNG" : "byte-identical copy",
    exactMagentaPixelRatio: 0,
    runtimeColorKey: null
  });
  textureUrlByName.set(key, url);
}

const colorKeyAnalysis = JSON.parse(execFileSync("python", [
  "-c",
  "from PIL import Image; import json,sys; out=[]\nfor p in sys.argv[1:]:\n im=Image.open(p).convert('RGB'); pixels=list(im.get_flattened_data()); count=sum(1 for pixel in pixels if pixel==(255,0,255)); out.append(count/len(pixels) if pixels else 0)\nprint(json.dumps(out))",
  ...textureRecords.map((record) => join(workspace, ...record.source.split("/")))
], { encoding: "utf8" }));
for (let index = 0; index < textureRecords.length; index += 1) {
  textureRecords[index].exactMagentaPixelRatio = round(colorKeyAnalysis[index]);
  textureRecords[index].runtimeColorKey = colorKeyAnalysis[index] >= 0.05 ? "magenta" : null;
}

const assets = [];
for (const [index, record] of audit.inventory.meshes.entries()) {
  const id = assetId(record.name);
  const sourcePath = join(workspace, ...record.source.split("/"));
  assert.equal(sha256(readFileSync(sourcePath)), record.sha256, `${record.name}: audit source hash changed`);
  if (!record.structurallyDecoded) {
    assets.push({
      index,
      id,
      name: record.name,
      family: record.category,
      status: "unsupported-binary-x",
      format: record.format,
      source: record.source,
      bytes: record.bytes,
      sha256: record.sha256,
      textureReferences: [],
      payloadUrl: null,
      payloadBytes: 0,
      payloadSha256: null,
      bounds: null,
      meshCount: 0,
      vertexCount: 0,
      faceCount: 0,
      triangleCount: 0,
      materialGroupCount: 0
    });
    continue;
  }
  const payload = parseXAsset(sourcePath);
  assert.equal(payload.sha256, record.sha256);
  for (const mesh of payload.meshes) {
    for (const material of mesh.materials) {
      if (material.textureName) material.textureUrl = textureUrlByName.get(material.textureName.toLowerCase()) ?? null;
      assert(!material.textureName || material.textureUrl, `${record.name}: unresolved texture ${material.textureName}`);
    }
  }
  const payloadPath = join(meshOutputRoot, `${id}.json`);
  writeFileSync(payloadPath, JSON.stringify(payload));
  const payloadBytes = readFileSync(payloadPath);
  assets.push({
    index,
    id,
    name: record.name,
    family: record.category,
    status: "recovered-text-x",
    format: record.format,
    source: record.source,
    bytes: record.bytes,
    sha256: record.sha256,
    textureReferences: record.textureReferences,
    payloadUrl: `/assets/dominus-gallery/meshes/${id}.json`,
    payloadBytes: payloadBytes.length,
    payloadSha256: sha256(payloadBytes),
    bounds: payload.bounds,
    meshCount: payload.meshCount,
    vertexCount: payload.vertexCount,
    faceCount: payload.faceCount,
    triangleCount: payload.triangleCount,
    materialGroupCount: payload.materialGroupCount
  });
}

const familyCounts = Object.fromEntries(Object.keys(audit.inventory.meshCategories).map((family) => [family, assets.filter((asset) => asset.family === family).length]));
const recoveryCounts = {
  "recovered-text-x": assets.filter((asset) => asset.status === "recovered-text-x").length,
  "unsupported-binary-x": assets.filter((asset) => asset.status === "unsupported-binary-x").length
};
assert.deepEqual(familyCounts, audit.inventory.meshCategories);
assert.deepEqual(recoveryCounts, { "recovered-text-x": 63, "unsupported-binary-x": 2 });
assert.equal(new Set(assets.map((asset) => asset.id)).size, 65);
assert.equal(new Set(assets.map((asset) => asset.sha256)).size, 65);
assert.equal(textureRecords.length, 83);
assert(textureRecords.every((record) => record.conversion !== "byte-identical copy" || record.sourceSha256 === record.outputSha256));

const manifest = {
  generated: new Date().toISOString(),
  format: "graphysx-dominus-asset-gallery/v1",
  id: "dominus-asset-gallery",
  classification: "source asset family only; no authored composition survives",
  sourceAudit: { path: "src/legacy/dominus-family-audit.json", id: audit.id, canonicalRoot: audit.scope.canonicalRoot },
  evidenceBoundary: {
    exact: [
      "All 65 source mesh names, family classifications, byte counts and SHA-256 hashes come from the full Dominus forensic audit.",
      "Sixty-three text X payloads preserve local frame transforms, vertices, UVs, face-material assignment and every decoded material group.",
      "All 83 referenced texture filenames resolve; JPG/PNG outputs are byte-identical and TGA files are losslessly decoded to PNG."
    ],
    inspectionOnly: [
      "The browser centers and uniformly scales one selected local model at a time; that display normalization, camera and lighting are gallery inspection aids.",
      "Text-X normals are derived at runtime from exact transformed vertices and triangulated faces because source per-face normals are not remapped.",
      "Textures with at least 5% exact pure RGB magenta use a shader-side compatibility color key while source/output image bytes remain unchanged."
    ],
    absent: [
      "port_crateshed.X and renzokscale.X are binary DirectX files and receive no decoded or proxy geometry.",
      "No authored scene composition, inter-asset transforms, camera, spawn, navigation, water, terrain or interaction rules survive.",
      "This gallery is not a village, port environment, world or gameplay restoration."
    ]
  },
  familyCounts,
  recoveryCounts,
  textureRecords,
  assets
};
writeFileSync(manifestPath, JSON.stringify(manifest));
console.log(`wrote ${manifestPath}`);
console.log(`Dominus assets: ${assets.length} (${recoveryCounts["recovered-text-x"]} text X + ${recoveryCounts["unsupported-binary-x"]} binary unsupported)`);
console.log(`geometry: ${assets.reduce((sum, asset) => sum + asset.vertexCount, 0)} vertices / ${assets.reduce((sum, asset) => sum + asset.triangleCount, 0)} triangles / ${assets.reduce((sum, asset) => sum + asset.materialGroupCount, 0)} material groups`);
console.log(`textures: ${textureRecords.length} (${textureRecords.filter((record) => record.conversion.startsWith("lossless")).length} TGA conversions; ${textureRecords.filter((record) => record.runtimeColorKey === "magenta").length} magenta keys)`);
