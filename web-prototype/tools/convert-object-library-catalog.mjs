#!/usr/bin/env node

// Converts only the recoverable entries in ObjectLibrary.xml. The serialized
// positions remain the authored editor/catalog grid; no village arrangement is
// introduced. Missing and unsupported entries remain explicit records.

import assert from "node:assert/strict";
import { createHash } from "node:crypto";
import { copyFileSync, existsSync, mkdirSync, readFileSync, readdirSync, statSync, writeFileSync } from "node:fs";
import { execFileSync } from "node:child_process";
import { basename, dirname, extname, join, relative, resolve } from "node:path";
import { fileURLToPath } from "node:url";

const here = dirname(fileURLToPath(import.meta.url));
const project = resolve(here, "..");
const workspace = resolve(project, "..");
const archiveMedia = join(workspace, "Archive", "bckup", "BallZ2015.bckup", "Media");
const dominusRoot = join(workspace, "Yanik C++ BCKUP", "Media", "Models", "Dominus Art");
const sourceAudit = join(project, "src", "legacy", "xml-scene-documents.json");
const output = join(project, "src", "legacy", "object-library-catalog.json");
const publicRoot = join(project, "public", "assets", "object-library", "textures");

const IDENTITY = [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1];
const round = (value) => Math.round(value * 1_000_000) / 1_000_000;
const sha256 = (data) => createHash("sha256").update(data).digest("hex").toUpperCase();
const sourceLabel = (path) => relative(workspace, path).replaceAll("\\", "/");
const cleanName = (value) => value.split(/[\\/]/).filter(Boolean).at(-1) ?? value;

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

function tokenizeX(text) {
  return text.replace(/\/\/[^\n]*/g, "").replace(/#[^\n]*/g, "").match(/"[^"]*"|[{}]|[^\s{}]+/g) ?? [];
}

function parseNumbers(tokens, state, count) {
  const numbers = [];
  while (numbers.length < count) {
    if (state.queue.length > 0) {
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

function defaultMaterial(index) {
  return { name: `material-${index}`, color: [0.72, 0.72, 0.72, 1], specularPower: 0, specular: [0, 0, 0], emissive: [0, 0, 0], textureName: null, textureUrl: null };
}

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
    if (indices.some((index) => !(index >= 0 && index < vertexCount))) throw new Error(`${name}: out-of-range face index`);
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
  if (uvs && uvs.length !== vertexCount * 2) throw new Error(`${name}: UV count mismatch`);
  if (faceMaterialIndices.length !== faceCount) throw new Error(`${name}: material face count mismatch`);
  if (!materials.length) materials.push(defaultMaterial(0));

  const indicesByMaterial = new Map();
  faces.forEach((face, faceIndex) => {
    const materialIndex = faceMaterialIndices[faceIndex] ?? 0;
    const indices = indicesByMaterial.get(materialIndex) ?? [];
    for (let index = 1; index < face.length - 1; index += 1) indices.push(face[0], face[index + 1], face[index]);
    indicesByMaterial.set(materialIndex, indices);
  });
  const indices = [];
  const groups = [];
  for (const [materialIndex, materialIndices] of [...indicesByMaterial].sort((left, right) => left[0] - right[0])) {
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
  const signature = data.subarray(0, 16).toString("latin1");
  if (!signature.includes("txt")) throw new Error(`${path}: unsupported binary DirectX X`);
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

function chunksOf(data) {
  const chunks = [];
  for (let offset = 0; offset <= data.length - 8;) {
    const tag = data.toString("latin1", offset, offset + 4);
    const size = data.readUInt32LE(offset + 4);
    if (/^[A-Z0-9! ]{4}$/.test(tag) && offset + 8 + size <= data.length) {
      chunks.push({ tag, start: offset + 8, size });
      offset += 8 + size;
    } else offset += 1;
  }
  return chunks;
}

function decodeTvm(path) {
  const data = readFileSync(path);
  const chunks = chunksOf(data);
  const get = (tag) => chunks.find((chunk) => chunk.tag === tag);
  const msta = get("MSTA");
  const mver = get("MVER");
  const indexChunk = get("MI16") ?? get("MI32");
  const matt = get("MATT");
  const mgro = get("MGRO");
  const mgrp = get("MGRP");
  if (!msta || !mver || !indexChunk || !matt || !mgro || !mgrp) throw new Error(`${path}: required TVM chunks missing`);
  const stride = data.readUInt32LE(msta.start + 24);
  const vertexCount = mver.size / stride;
  const positions = [];
  const normals = [];
  const uvs = [];
  for (let vertex = 0; vertex < vertexCount; vertex += 1) {
    const start = mver.start + vertex * stride;
    for (let axis = 0; axis < 3; axis += 1) positions.push(round(data.readFloatLE(start + axis * 4)));
    for (let axis = 0; axis < 3; axis += 1) normals.push(round(data.readFloatLE(start + 12 + axis * 4)));
    uvs.push(round(data.readFloatLE(start + 24)), round(data.readFloatLE(start + 28)));
  }
  const indexBytes = indexChunk.tag === "MI16" ? 2 : 4;
  const indices = [];
  for (let offset = 0; offset < indexChunk.size; offset += indexBytes) indices.push(indexBytes === 2 ? data.readUInt16LE(indexChunk.start + offset) : data.readUInt32LE(indexChunk.start + offset));
  const triangleCount = indices.length / 3;
  const materialIndexByTriangle = [];
  for (let triangle = 0; triangle < triangleCount; triangle += 1) materialIndexByTriangle.push(data.readUInt32LE(matt.start + triangle * 4));
  const groups = [];
  for (let offset = 0; offset < mgro.size; offset += 16) groups.push({
    vertexCount: data.readUInt32LE(mgro.start + offset),
    vertexStart: data.readUInt32LE(mgro.start + offset + 4),
    triangleCount: data.readUInt32LE(mgro.start + offset + 8),
    triangleStart: data.readUInt32LE(mgro.start + offset + 12)
  });
  const recordSize = groups.length && mgrp.size % groups.length === 0 ? mgrp.size / groups.length : 416;
  const materials = groups.map((_group, index) => {
    const start = mgrp.start + index * recordSize;
    const name = data.toString("latin1", start, start + 64).split("\0")[0] || `Material${index}`;
    const textureName = cleanName(data.toString("latin1", start + 64, start + 128).split("\0")[0]) || null;
    const diffuse = recordSize >= 364 ? [0, 1, 2, 3].map((component) => round(data.readFloatLE(start + 348 + component * 4))) : [1, 1, 1, 1];
    return { index, name, textureName, textureUrl: null, diffuse };
  });
  for (const group of groups) {
    const assigned = materialIndexByTriangle.slice(group.triangleStart, group.triangleStart + group.triangleCount);
    assert.equal(new Set(assigned).size, 1, `${basename(path)} TVM group mixes material assignments`);
    group.materialIndex = assigned[0] ?? 0;
  }
  return { source: sourceLabel(path), bytes: data.length, sha256: sha256(data), vertexCount, triangleCount, positions, normals, uvs, indices, groups, materials };
}

function familyOf(object) {
  const path = object.pathToMesh.toLowerCase();
  if (object.resolution.status === "procedural-primitive") return "primitive";
  if (path.includes("pipeset")) return "pipe";
  if (object.name === "Airplane") return "aircraft";
  if (object.name === "MainBoard") return "technology";
  if (path.includes("nature") || /^bush|^tree_|^green/i.test(object.name)) return "nature";
  if (path.includes("camp")) return "camp";
  if (path.includes("port")) return "port";
  return "building";
}

const audit = JSON.parse(readFileSync(sourceAudit, "utf8"));
const sourceDocument = audit.documents.find((document) => document.id === "object-library");
assert.ok(sourceDocument, "ObjectLibrary audit is missing");
const dominusFiles = readdirSync(dominusRoot);
const dominusByName = new Map(dominusFiles.filter((name) => /\.x$/i.test(name)).map((name) => [name.toLowerCase(), name]));
const xAssets = {};
const tvmAssets = {};
const objects = sourceDocument.objects.map((object) => ({ ...object, family: familyOf(object), archiveResolution: object.resolution, catalogResolution: null }));

for (const object of objects) {
  if (object.archiveResolution.status === "procedural-primitive") {
    object.catalogResolution = { status: "recovered-procedural" };
    continue;
  }
  if (object.archiveResolution.status === "exact-file" && /\.tvm$/i.test(object.archiveResolution.source ?? "")) {
    const path = join(workspace, object.archiveResolution.source);
    tvmAssets[object.name] = decodeTvm(path);
    object.catalogResolution = { status: "recovered-tvm", assetId: object.name };
    continue;
  }
  if (/\.x$/i.test(object.pathToMesh)) {
    const requestedName = cleanName(object.pathToMesh).toLowerCase();
    const actualName = dominusByName.get(requestedName);
    if (actualName) {
      const path = join(dominusRoot, actualName);
      const signature = readFileSync(path).subarray(0, 16).toString("latin1");
      if (signature.includes("bin")) object.catalogResolution = { status: "unsupported-binary-x", source: sourceLabel(path), reason: "Binary DirectX X decoder is not yet available." };
      else {
        xAssets[object.name] = parseXAsset(path);
        object.catalogResolution = { status: "recovered-text-x", assetId: object.name, source: sourceLabel(path) };
      }
      continue;
    }
  }
  if (object.archiveResolution.status === "duplicate-target") {
    const target = objects.find((candidate) => candidate.index < object.index && candidate.name === object.archiveResolution.targetName);
    object.catalogResolution = target?.catalogResolution?.status?.startsWith("recovered-")
      ? { status: "recovered-duplicate", targetName: target.name, assetId: target.catalogResolution.assetId }
      : { status: "missing-duplicate-source", targetName: object.archiveResolution.targetName, reason: "The referenced source object has no recovered mesh." };
  } else object.catalogResolution = { status: "missing-file", reason: "No exact archive asset matched this serialized path/name." };
}

mkdirSync(publicRoot, { recursive: true });
const dominusFileIndex = new Map(dominusFiles.map((name) => [name.toLowerCase(), join(dominusRoot, name)]));
const mediaFiles = [];
const walk = (root) => {
  for (const entry of readdirSync(root, { withFileTypes: true })) {
    const path = join(root, entry.name);
    if (entry.isDirectory()) walk(path);
    else if (entry.isFile()) mediaFiles.push(path);
  }
};
walk(archiveMedia);
const mediaByBasename = new Map();
for (const path of mediaFiles) {
  const key = basename(path).toLowerCase();
  const paths = mediaByBasename.get(key) ?? [];
  paths.push(path);
  mediaByBasename.set(key, paths);
}
const textureRecords = [];
const textureUrlByKey = new Map();

function exportTexture(sourcePath) {
  const sourceKey = sourcePath.toLowerCase();
  if (textureUrlByKey.has(sourceKey)) return textureUrlByKey.get(sourceKey);
  const sourceExtension = extname(sourcePath).toLowerCase().slice(1);
  const base = basename(sourcePath, extname(sourcePath)).toLowerCase().replace(/[^a-z0-9_-]/g, "_");
  const outputExtension = sourceExtension === "tga" ? "png" : sourceExtension;
  const outputName = `${base}_${sourceExtension}.${outputExtension}`;
  const outputPath = join(publicRoot, outputName);
  if (sourceExtension === "tga") {
    execFileSync("python", ["-c", "from PIL import Image; import sys; Image.open(sys.argv[1]).save(sys.argv[2], format='PNG')", sourcePath, outputPath]);
  } else copyFileSync(sourcePath, outputPath);
  const url = `/assets/object-library/textures/${outputName}`;
  textureRecords.push({
    source: sourceLabel(sourcePath),
    sourceBytes: statSync(sourcePath).size,
    sourceSha256: sha256(readFileSync(sourcePath)),
    url,
    outputBytes: statSync(outputPath).size,
    outputSha256: sha256(readFileSync(outputPath)),
    conversion: sourceExtension === "tga" ? "lossless-pixel TGA to PNG" : "byte-identical copy"
  });
  textureUrlByKey.set(sourceKey, url);
  return url;
}

function resolveTvmTexture(asset, textureName) {
  const sibling = join(workspace, dirname(asset.source), textureName);
  if (existsSync(sibling)) return sibling;
  const candidates = mediaByBasename.get(textureName.toLowerCase()) ?? [];
  if (candidates.length === 1) return candidates[0];
  if (candidates.length > 1 && new Set(candidates.map((path) => sha256(readFileSync(path)))).size === 1) return candidates[0];
  return null;
}

for (const asset of Object.values(xAssets)) {
  for (const mesh of asset.meshes) {
    for (const material of mesh.materials) {
      if (!material.textureName) continue;
      const sourcePath = dominusFileIndex.get(material.textureName.toLowerCase());
      assert.ok(sourcePath, `${asset.source}: missing referenced texture ${material.textureName}`);
      material.textureUrl = exportTexture(sourcePath);
    }
  }
}
for (const asset of Object.values(tvmAssets)) {
  for (const material of asset.materials) {
    if (!material.textureName) continue;
    const sourcePath = resolveTvmTexture(asset, material.textureName);
    assert.ok(sourcePath, `${asset.source}: missing TVM texture ${material.textureName}`);
    material.textureUrl = exportTexture(sourcePath);
  }
}
const primitiveTexturePath = join(archiveMedia, "twoway.jpg");
const primitiveTextureUrl = exportTexture(primitiveTexturePath);

// Several Dominus textures use pure RGB magenta as an old real-time color-key
// field. Keep the exported image bytes untouched and record the exact pixel
// ratio so the browser runtime can discard the key in its shader.
const colorKeyAnalysis = JSON.parse(execFileSync("python", [
  "-c",
  "from PIL import Image; import json,sys; out=[]\nfor p in sys.argv[1:]:\n im=Image.open(p).convert('RGB'); pixels=list(im.get_flattened_data()); count=sum(1 for pixel in pixels if pixel==(255,0,255)); out.append(count/len(pixels) if pixels else 0)\nprint(json.dumps(out))",
  ...textureRecords.map((record) => join(workspace, ...record.source.split("/")))
], { encoding: "utf8" }));
assert.equal(colorKeyAnalysis.length, textureRecords.length);
for (let index = 0; index < textureRecords.length; index += 1) {
  const ratio = round(colorKeyAnalysis[index]);
  textureRecords[index].exactMagentaPixelRatio = ratio;
  textureRecords[index].runtimeColorKey = ratio >= 0.05 ? "magenta" : null;
}

const recoveryCounts = {};
const familyCounts = {};
for (const object of objects) {
  recoveryCounts[object.catalogResolution.status] = (recoveryCounts[object.catalogResolution.status] ?? 0) + 1;
  familyCounts[object.family] = (familyCounts[object.family] ?? 0) + 1;
}
assert.equal(objects.length, 61);
assert.deepEqual(recoveryCounts, {
  "recovered-procedural": 5,
  "missing-file": 11,
  "recovered-tvm": 2,
  "missing-duplicate-source": 2,
  "recovered-text-x": 40,
  "unsupported-binary-x": 1
});
assert.equal(Object.keys(xAssets).length, 40);
assert.deepEqual(Object.keys(tvmAssets), ["Prisme", "Airplane"]);
assert.ok(textureRecords.every((record) => record.conversion !== "byte-identical copy" || record.sourceSha256 === record.outputSha256));
assert.equal(textureRecords.filter((record) => record.runtimeColorKey === "magenta").length, 11);

const report = {
  generated: new Date().toISOString(),
  format: "graphysx-object-library-catalog/v1",
  sourceDocument: {
    id: sourceDocument.id,
    source: sourceDocument.source,
    bytes: sourceDocument.bytes,
    sha256: sourceDocument.sha256,
    classification: sourceDocument.classification,
    objectCount: sourceDocument.objectCount,
    archiveResolutionCounts: sourceDocument.resolutionCounts
  },
  evidenceBoundary: {
    exact: [
      "All 61 names, actions, enabled flags, physics metadata, paths, positions, rotations and scales are preserved from ObjectLibrary.xml.",
      "The four source primitive definitions and BallFloor are procedural from SceneBuilder/MeshFactory behavior.",
      "Prisme.TVM and AirplaneLP.TVM preserve exact vertices, normals, UVs, indices and material groups.",
      "Forty exact-basename text X assets preserve source frame transforms, vertices, UVs, face-material assignment and all material groups.",
      "All copied JPG/BMP/PNG textures are byte-identical; TGA textures are losslessly decoded to browser PNG files."
    ],
    inspectionOnly: [
      "Camera, lights, selection labels and missing/unsupported markers are browser inspection aids; ObjectLibrary.xml contains none of them.",
      "Normals for text X meshes are derived at runtime from the exact transformed vertices and triangulated faces because the converter does not remap per-face source normals.",
      "Eleven textures whose exact source pixels contain at least 5% pure RGB magenta use a shader-side magenta color key; the archived and exported image bytes remain unchanged."
    ],
    absent: [
      "Eleven requested files are absent, two duplicates depend on an absent bush source, and port_crateshed.X is binary/unsupported.",
      "This is the authored mixed asset-catalog grid, not a village, port, race or gameplay composition."
    ]
  },
  familyCounts,
  recoveryCounts,
  primitiveTextureUrl,
  textureRecords,
  objects,
  assets: { x: xAssets, tvm: tvmAssets }
};
writeFileSync(output, JSON.stringify(report));
console.log(`wrote ${output}`);
console.log(`ObjectLibrary ${objects.length}: ${JSON.stringify(recoveryCounts)}`);
console.log(`geometry: ${Object.keys(xAssets).length} text X + ${Object.keys(tvmAssets).length} TVM + 5 procedural`);
console.log(`textures: ${textureRecords.length} exported (${textureRecords.filter((record) => record.conversion.startsWith("lossless")).length} TGA conversions)`);
console.log(`runtime color keys: ${textureRecords.filter((record) => record.runtimeColorKey === "magenta").length} exact-magenta textures`);
