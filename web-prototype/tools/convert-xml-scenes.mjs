// Deterministic converter/audit for the two generations of GraphysX XML scenes.
//
// The parser intentionally handles both serializer layouts found in the archive:
// - CScene3D/Object3D (2015, attributes or expanded child elements)
// - Scene3D/Obj3D (2016+, compact attributes with optional Rot/map metadata)
//
// It normalizes all six censused documents, then converts the exact TVM assets
// referenced by the best-preserved authored composition, `MyWorld - Copie.xml`.

import { createHash } from "node:crypto";
import { copyFileSync, mkdirSync, readFileSync, readdirSync, statSync, writeFileSync } from "node:fs";
import { basename, dirname, isAbsolute, join, relative, resolve } from "node:path";
import { fileURLToPath } from "node:url";

const here = dirname(fileURLToPath(import.meta.url));
const project = resolve(here, "..");
const workspace = resolve(project, "..");
const archiveRoot = join(workspace, "Archive", "bckup", "BallZ2015.bckup");
const mediaRoot = join(archiveRoot, "Media");
const stockRoom = "E:\\Media\\Datalake\\Tech\\StockRoom";
const auditOutput = join(project, "src", "legacy", "xml-scene-documents.json");
const selectedOutput = join(project, "src", "legacy", "xml-myworld-copy-scene.json");
const publicRoot = join(project, "public", "assets", "xml-scenes");

const DOCUMENTS = [
  {
    id: "myworld",
    path: join(mediaRoot, "MyWorld.xml"),
    classification: "broken authored editor save",
    assessment: "Two loadable objects plus two invalid DUPLICATE references that contain file paths instead of source object names."
  },
  {
    id: "myworld-copy",
    path: join(mediaRoot, "MyWorld - Copie.xml"),
    classification: "authored scene-builder test composition",
    assessment: "Best-preserved nontrivial document: six of seven objects resolve exactly; only ArcheChinois.TVM is absent."
  },
  {
    id: "testworld",
    path: join(mediaRoot, "TestWorld.xml"),
    classification: "scene-loader coverage test",
    assessment: "Primitive line and airplane resolve; bush1.X is absent and two PHYSICXMESH entries have empty paths."
  },
  {
    id: "object-library",
    path: join(mediaRoot, "ObjectLibrary.xml"),
    classification: "editor object-library grid, not a composed world",
    assessment: "Catalog rows intentionally place unrelated assets on a grid; this is not evidence for a village or Dominus layout."
  },
  {
    id: "base-scene",
    path: join(stockRoom, "BaseScene.xml"),
    classification: "serializer/template artifact",
    assessment: "Eighteen identical default cubes overlap at the origin; no authored spatial composition is present."
  },
  {
    id: "test1",
    path: join(stockRoom, "test1.xml"),
    classification: "minimal serializer smoke document",
    assessment: "One default physics cube proves the v1.2 schema but is too trivial to represent an authored discovery scene."
  }
];

const sha256 = (data) => createHash("sha256").update(data).digest("hex").toUpperCase();
const round = (value) => Math.round(value * 1_000_000) / 1_000_000;

function walkFiles(root) {
  const files = [];
  for (const entry of readdirSync(root, { withFileTypes: true })) {
    const path = join(root, entry.name);
    if (entry.isDirectory()) files.push(...walkFiles(path));
    else if (entry.isFile()) files.push(path);
  }
  return files;
}

const mediaFiles = walkFiles(mediaRoot);
const byRelative = new Map();
const byBasename = new Map();
for (const file of mediaFiles) {
  const relativePath = relative(mediaRoot, file).replaceAll("\\", "/").toLowerCase();
  byRelative.set(relativePath, file);
  const name = basename(file).toLowerCase();
  const entries = byBasename.get(name) ?? [];
  entries.push(file);
  byBasename.set(name, entries);
}

function decodeEntities(value) {
  return value
    .replaceAll("&quot;", '"')
    .replaceAll("&apos;", "'")
    .replaceAll("&lt;", "<")
    .replaceAll("&gt;", ">")
    .replaceAll("&amp;", "&");
}

function parseAttributes(source) {
  const attributes = {};
  for (const match of source.matchAll(/([A-Za-z_][\w.-]*)\s*=\s*"([^"]*)"/g)) {
    attributes[match[1]] = decodeEntities(match[2]);
  }
  return attributes;
}

function childText(body, tag) {
  const match = body.match(new RegExp(`<${tag}\\b[^>]*>([\\s\\S]*?)<\\/${tag}>`, "i"));
  return match ? decodeEntities(match[1].trim()) : "";
}

function numberValue(value, fallback = 0) {
  const parsed = Number(value);
  return Number.isFinite(parsed) ? parsed : fallback;
}

function booleanValue(value, fallback = true) {
  if (String(value).toLowerCase() === "true") return true;
  if (String(value).toLowerCase() === "false") return false;
  return fallback;
}

function parseVector(body, tag, fallback) {
  const match = body.match(new RegExp(`<${tag}\\b([^>]*?)(?:\\/\\s*>|>([\\s\\S]*?)<\\/${tag}>)`, "i"));
  if (!match) return [...fallback];
  const attributes = parseAttributes(match[1] ?? "");
  const inner = match[2] ?? "";
  return ["x", "y", "z"].map((axis, index) =>
    numberValue(attributes[axis] ?? childText(inner, axis), fallback[index])
  );
}

function actionsFrom(xml) {
  const match = xml.match(/<(Actions|ActionsHeader)\b[^>]*>([\s\S]*?)<\/\1>/i);
  if (!match) return {};
  const text = decodeEntities(match[2]);
  const version = text.match(/\bV\d+_\d+\b/);
  const objectSection = version ? text.slice(0, (version.index ?? 0) + version[0].length) : text;
  const actions = {};
  for (const pair of objectSection.matchAll(/\b(\d+)\s*=\s*([A-Z][A-Z0-9_]*)/g)) {
    actions[Number(pair[1])] = pair[2];
  }
  return actions;
}

function resolveAsset(pathValue, action, priorNames) {
  const normalized = pathValue.trim().replaceAll("\\", "/");
  if (/DUPLICATE/.test(action)) {
    return priorNames.includes(pathValue)
      ? { status: "duplicate-target", targetName: pathValue }
      : { status: "invalid-duplicate-target", targetName: pathValue || null };
  }
  if (!normalized) {
    const primitive = /^(CUBE|SPHERE|CYLINDER|CONE|FLOOR|BILLBOARD|PHYSICCUBE|PHYSICSPHERE|PHYSICSCYLINDER|PHYSICCYLINDER|PHYSICCONE)$/.test(action);
    return { status: primitive ? "procedural-primitive" : "missing-path" };
  }

  let relativeCandidate = normalized.replace(/^[A-Za-z]:\//, "");
  relativeCandidate = relativeCandidate.replace(/^media\//i, "").replace(/^test\//i, "");
  const exact = byRelative.get(relativeCandidate.toLowerCase());
  const candidates = exact ? [exact] : (byBasename.get(basename(normalized).toLowerCase()) ?? []);
  if (candidates.length === 1) {
    return {
      status: "exact-file",
      source: relative(workspace, candidates[0]).replaceAll("\\", "/"),
      bytes: statSync(candidates[0]).size,
      sha256: sha256(readFileSync(candidates[0]))
    };
  }
  return { status: candidates.length > 1 ? "ambiguous-file" : "missing-file", candidateCount: candidates.length };
}

function parseDocument(definition) {
  const bytes = readFileSync(definition.path);
  const xml = bytes.toString("utf8").replace(/^\uFEFF/, "");
  const root = xml.match(/<(CScene3D|Scene3D)\b/i)?.[1] ?? "unknown";
  const actions = actionsFrom(xml);
  const objects = [];
  const objectPattern = /<(Object3D|Obj3D)\b([^>]*)>([\s\S]*?)<\/\1>/gi;
  for (const match of xml.matchAll(objectPattern)) {
    const attributes = parseAttributes(match[2]);
    const body = match[3];
    const type = numberValue(attributes.Type ?? childText(body, "Type"), -1);
    const name = (attributes.Name ?? childText(body, "Name")) || `Object${objects.length}`;
    const action = actions[type] ?? `UNKNOWN_${type}`;
    const pathToMesh = childText(body, "PathToMesh");
    const object = {
      index: objects.length,
      type,
      action,
      name,
      enabled: booleanValue(attributes.Enabled ?? childText(body, "bEnable"), true),
      mass: numberValue(attributes.masse ?? attributes.Masse ?? childText(body, "masse"), 0),
      meshControlled: booleanValue(attributes.MeshControlled ?? childText(body, "MeshControlled"), false),
      newtonMaterial: numberValue(attributes.NewtonMat ?? childText(body, "iNewtonMat"), -1),
      position: parseVector(body, "Pos", [0, 0, 0]),
      rotation: parseVector(body, "Rot", [0, 0, 0]),
      scale: parseVector(body, "Scale", [1, 1, 1]),
      pathToMesh,
      textureName: childText(body, "TextureName"),
      resolution: resolveAsset(pathToMesh, action, objects.map((entry) => entry.name))
    };
    objects.push(object);
  }

  const resolutionCounts = {};
  const actionCounts = {};
  for (const object of objects) {
    resolutionCounts[object.resolution.status] = (resolutionCounts[object.resolution.status] ?? 0) + 1;
    actionCounts[object.action] = (actionCounts[object.action] ?? 0) + 1;
  }
  return {
    id: definition.id,
    source: isAbsolute(definition.path) && !definition.path.toLowerCase().startsWith(workspace.toLowerCase())
      ? definition.path.replaceAll("\\", "/")
      : relative(workspace, definition.path).replaceAll("\\", "/"),
    bytes: bytes.length,
    sha256: sha256(bytes),
    root,
    schemaGeneration: root === "CScene3D" ? "2015 CScene3D/Object3D" : "2016+ Scene3D/Obj3D",
    classification: definition.classification,
    assessment: definition.assessment,
    actions,
    filepathASCII: childText(xml, "filepathASCII"),
    mapSize: numberValue(childText(xml, "mapSize"), 0),
    objectCount: objects.length,
    actionCounts,
    resolutionCounts,
    objects
  };
}

function chunksOf(data) {
  const chunks = [];
  let offset = 0;
  while (offset <= data.length - 8) {
    const tag = data.toString("latin1", offset, offset + 4);
    const size = data.readUInt32LE(offset + 4);
    if (/^[A-Z0-9! ]{4}$/.test(tag) && offset + 8 + size <= data.length) {
      chunks.push({ tag, start: offset + 8, size });
      offset += 8 + size;
    } else offset += 1;
  }
  return chunks;
}

function boundsOf(positions) {
  const min = [Infinity, Infinity, Infinity];
  const max = [-Infinity, -Infinity, -Infinity];
  for (let index = 0; index < positions.length; index += 3) {
    for (let axis = 0; axis < 3; axis += 1) {
      min[axis] = Math.min(min[axis], positions[index + axis]);
      max[axis] = Math.max(max[axis], positions[index + axis]);
    }
  }
  return { min: min.map(round), max: max.map(round), size: max.map((value, axis) => round(value - min[axis])) };
}

function decodeTvm(path) {
  const data = readFileSync(path);
  const chunks = chunksOf(data);
  const get = (tag) => chunks.find((chunk) => chunk.tag === tag);
  const msta = get("MSTA");
  const mver = get("MVER");
  const indicesChunk = get("MI16") ?? get("MI32");
  const matt = get("MATT");
  const mgro = get("MGRO");
  const mgrp = get("MGRP");
  if (!msta || !mver || !indicesChunk || !matt || !mgro || !mgrp) {
    throw new Error(`${path}: required TVM chunks are missing`);
  }
  const stride = data.readUInt32LE(msta.start + 24);
  if (![24, 32, 36, 40, 44, 48, 56, 64].includes(stride)) throw new Error(`${path}: unsupported stride ${stride}`);
  const vertexCount = mver.size / stride;
  const positions = [];
  const normals = [];
  const uvs = [];
  for (let vertex = 0; vertex < vertexCount; vertex += 1) {
    const start = mver.start + vertex * stride;
    for (let axis = 0; axis < 3; axis += 1) positions.push(round(data.readFloatLE(start + axis * 4)));
    for (let axis = 0; axis < 3; axis += 1) normals.push(round(data.readFloatLE(start + 12 + axis * 4)));
    if (stride >= 32) uvs.push(round(data.readFloatLE(start + 24)), round(data.readFloatLE(start + 28)));
  }
  const indexBytes = indicesChunk.tag === "MI16" ? 2 : 4;
  const indices = [];
  for (let offset = 0; offset < indicesChunk.size; offset += indexBytes) {
    indices.push(indexBytes === 2 ? data.readUInt16LE(indicesChunk.start + offset) : data.readUInt32LE(indicesChunk.start + offset));
  }
  const triangleCount = indices.length / 3;
  const materialIndexByTriangle = [];
  for (let triangle = 0; triangle < triangleCount; triangle += 1) {
    materialIndexByTriangle.push(data.readUInt32LE(matt.start + triangle * 4));
  }
  const groups = [];
  for (let offset = 0; offset < mgro.size; offset += 16) {
    groups.push({
      vertexCount: data.readUInt32LE(mgro.start + offset),
      vertexStart: data.readUInt32LE(mgro.start + offset + 4),
      triangleCount: data.readUInt32LE(mgro.start + offset + 8),
      triangleStart: data.readUInt32LE(mgro.start + offset + 12)
    });
  }
  const materialRecordSize = groups.length > 0 && mgrp.size % groups.length === 0 ? mgrp.size / groups.length : 416;
  const materials = groups.map((_group, index) => {
    const start = mgrp.start + index * materialRecordSize;
    const rawName = data.toString("latin1", start, start + 64).split("\0")[0];
    const textureName = data.toString("latin1", start + 64, start + 128).split("\0")[0];
    let name = rawName;
    if (textureName) {
      const rawLower = rawName.toLowerCase();
      const textureLower = textureName.toLowerCase();
      for (let length = textureLower.length; length >= 3; length -= 1) {
        if (rawLower.endsWith(textureLower.slice(0, length))) {
          name = rawName.slice(0, -length);
          break;
        }
      }
    }
    const diffuse = materialRecordSize >= 364
      ? [0, 1, 2, 3].map((component) => round(data.readFloatLE(start + 348 + component * 4)))
      : [1, 1, 1, 1];
    return { index, name: name || rawName || `Material${index}`, textureName, diffuse };
  });
  return {
    source: relative(workspace, path).replaceAll("\\", "/"),
    bytes: data.length,
    sha256: sha256(data),
    vertexStride: stride,
    vertexCount,
    triangleCount,
    bounds: boundsOf(positions),
    positions,
    normals,
    uvs,
    indices,
    materialIndexByTriangle,
    groups,
    materials
  };
}

const documents = DOCUMENTS.map(parseDocument);
const selected = documents.find((document) => document.id === "myworld-copy");
if (!selected) throw new Error("MyWorld - Copie audit is missing.");
const exactTvmAssets = {};
for (const object of selected.objects) {
  if (object.resolution.status !== "exact-file" || !/\.tvm$/i.test(object.resolution.source)) continue;
  const sourcePath = join(workspace, object.resolution.source);
  exactTvmAssets[object.name] = decodeTvm(sourcePath);
}

const audit = {
  generated: new Date().toISOString(),
  parser: {
    id: "graphysx-xml-scene/v1",
    supportedRoots: ["CScene3D", "Scene3D"],
    supportedObjects: ["Object3D", "Obj3D"],
    note: "Action IDs are resolved from each document's own header; schema generations are not conflated."
  },
  selection: {
    selectedId: "myworld-copy",
    reason: "Smallest nontrivial authored composition with the highest exact-resolution coverage (6/7 objects).",
    evidenceBoundary: "No camera, sky, light, physics timestep or ArcheChinois asset survives in this document. Those are not invented by the converter."
  },
  documents
};

const selectedScene = {
  generated: new Date().toISOString(),
  format: "graphysx-xml-scene/v1",
  sourceDocument: selected,
  exactTvmAssets,
  runtimeEvidence: {
    exact: [
      "All object types, names, enabled flags, masses, mesh-control flags, Newton material IDs, positions, scales, paths and texture names come from MyWorld - Copie.xml.",
      "Primitive dimensions and cylinder/cone -90 degree Z physics alignment come from BallZ2015 SceneBuilder.cpp and MeshFactory.h.",
      "AirplaneLP.TVM and Level2.TVM preserve exact positions, normals, source UVs, indices, face-material assignments and groups.",
      "All referenced BMP/PNG/JPG files copied for the preview are byte-identical archive assets."
    ],
    missing: [
      "C:/Media/Building/ArcheChinois.TVM is absent from the searched workspace and external StockRoom.",
      "The XML contains no authored camera, lighting, sky, gravity, physics timestep or interaction rules."
    ],
    inference: [
      "Preview camera and lights are restoration choices used only to inspect the exact composition.",
      "Dynamic physics bodies are displayed at their serialized positions; motion is not simulated without an evidenced host-scene configuration."
    ]
  }
};

mkdirSync(dirname(auditOutput), { recursive: true });
writeFileSync(auditOutput, JSON.stringify(audit));
writeFileSync(selectedOutput, JSON.stringify(selectedScene));

mkdirSync(join(publicRoot, "airplane"), { recursive: true });
for (const file of ["FUS.BMP", "HTAIL.BMP", "RED.BMP", "VTAIL.BMP", "WHEEL.BMP", "WING.BMP"]) {
  copyFileSync(join(mediaRoot, "Airplane", file), join(publicRoot, "airplane", file));
}
copyFileSync(join(mediaRoot, "Level2.png"), join(publicRoot, "Level2.png"));
copyFileSync(join(mediaRoot, "twoway.jpg"), join(publicRoot, "twoway.jpg"));

console.log(`wrote ${auditOutput}`);
console.log(`wrote ${selectedOutput}`);
for (const document of documents) {
  console.log(`${document.id}: ${document.objectCount} objects; ${JSON.stringify(document.resolutionCounts)}; ${document.classification}`);
}
console.log(`selected myworld-copy: ${Object.keys(exactTvmAssets).join(", ")} exact TVM assets; 8 exact texture files copied`);
