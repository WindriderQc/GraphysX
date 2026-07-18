#!/usr/bin/env node

// Evidence-bounded converter for the two remaining BallZ2015 XML documents.
// Each document's embedded action table is authoritative: the archived C++
// enum changed after these files were serialized, so current enum ordinals must
// not be projected backward onto the documents.

import { createHash } from "node:crypto";
import { copyFileSync, mkdirSync, readFileSync, statSync, writeFileSync } from "node:fs";
import { basename, dirname, join, relative, resolve } from "node:path";
import { fileURLToPath } from "node:url";

const here = dirname(fileURLToPath(import.meta.url));
const project = resolve(here, "..");
const workspace = resolve(project, "..");
const archive = join(workspace, "Archive", "bckup", "BallZ2015.bckup");
const media = join(archive, "Media");
const stockRoom = "E:\\Media\\Datalake\\Tech\\StockRoom";
const output = join(project, "src", "legacy", "ballz-xml-worlds.json");
const publicOutput = join(project, "public", "assets", "ballz-xml-worlds");

const sha256 = (value) => createHash("sha256").update(value).digest("hex").toUpperCase();
const round = (value) => Math.round(value * 1_000_000) / 1_000_000;
const workspacePath = (path) => relative(workspace, path).replaceAll("\\", "/");

function fileEvidence(path) {
  const bytes = readFileSync(path);
  return { path: workspacePath(path), bytes: bytes.length, sha256: sha256(bytes) };
}

function parseAttributes(source) {
  return Object.fromEntries([...source.matchAll(/([A-Za-z_][\w.-]*)\s*=\s*"([^"]*)"/g)].map((match) => [match[1], match[2]]));
}

function childText(body, tag) {
  return body.match(new RegExp(`<${tag}\\b[^>]*>([\\s\\S]*?)<\\/${tag}>`, "i"))?.[1].trim() ?? "";
}

function numberValue(value, fallback = 0) {
  const parsed = Number(value);
  return Number.isFinite(parsed) ? parsed : fallback;
}

function boolValue(value, fallback = true) {
  if (String(value).toLowerCase() === "true") return true;
  if (String(value).toLowerCase() === "false") return false;
  return fallback;
}

function vector(body, tag, fallback) {
  const match = body.match(new RegExp(`<${tag}\\b([^>]*?)(?:\\/\\s*>|>([\\s\\S]*?)<\\/${tag}>)`, "i"));
  if (!match) return [...fallback];
  const attributes = parseAttributes(match[1] ?? "");
  const inner = match[2] ?? "";
  return ["x", "y", "z"].map((axis, index) => numberValue(attributes[axis] ?? childText(inner, axis), fallback[index]));
}

function parseActions(xml) {
  const body = xml.match(/<Actions\b[^>]*>([\s\S]*?)<\/Actions>/i)?.[1] ?? "";
  return Object.fromEntries([...body.matchAll(/\b(\d+)\s*=\s*([A-Z][A-Z0-9_]*)/g)].map((match) => [Number(match[1]), match[2]]));
}

function resolveMediaPath(pathValue) {
  const normalized = pathValue.replaceAll("\\", "/").replace(/^media\//i, "").replace(/^c:\/test\//i, "");
  const direct = join(media, ...normalized.split("/"));
  try {
    if (statSync(direct).isFile()) return direct;
  } catch { /* searched below by basename */ }
  const wanted = basename(normalized).toLowerCase();
  const candidates = [
    join(media, "Airplane", "Airplane.tvm"),
    join(media, "Airplane", "AirplaneLP.TVM"),
    join(media, "Suzanne1.DoorGate.x"),
    join(media, "Suzanne1.blend.x")
  ].filter((candidate) => basename(candidate).toLowerCase() === wanted);
  return candidates.length === 1 ? candidates[0] : null;
}

function parseDocument(id, filename, classification, assessment) {
  const path = join(media, filename);
  const bytes = readFileSync(path);
  const xml = bytes.toString("utf8").replace(/^\uFEFF/, "");
  const actions = parseActions(xml);
  const objects = [];
  for (const match of xml.matchAll(/<Object3D\b([^>]*)>([\s\S]*?)<\/Object3D>/gi)) {
    const attributes = parseAttributes(match[1]);
    const body = match[2];
    const type = numberValue(attributes.Type ?? childText(body, "Type"), -1);
    const action = actions[type] ?? `UNKNOWN_${type}`;
    const pathToMesh = childText(body, "PathToMesh");
    const priorNames = objects.map((object) => object.name);
    let resolution;
    let assetId = null;
    if (/^(PHYSICCUBE|PHYSICSPHERE|PHYSICSCYLINDER|PHYSICCONE)$/.test(action) && !pathToMesh) {
      resolution = { status: "exact-procedural-primitive" };
    } else if (action === "DUPLICATE") {
      const targetName = pathToMesh || null;
      const coincident = pathToMesh ? resolveMediaPath(pathToMesh) : null;
      resolution = priorNames.includes(pathToMesh)
        ? { status: "exact-duplicate-target", targetName }
        : {
            status: "invalid-duplicate-target",
            targetName,
            reason: "MeshFactory::CreateDuplicate searches prior mesh names; this field is not loaded as a file path.",
            coincidentFile: coincident ? fileEvidence(coincident) : null
          };
    } else {
      const exact = pathToMesh ? resolveMediaPath(pathToMesh) : null;
      if (exact) {
        assetId = basename(exact).toLowerCase() === "airplane.tvm" ? "airplane-high" :
          basename(exact).toLowerCase() === "airplanelp.tvm" ? "airplane-lp" : null;
        resolution = { status: "exact-file", ...fileEvidence(exact) };
      } else {
        resolution = {
          status: pathToMesh ? "missing-file" : "missing-path",
          requestedPath: pathToMesh || null
        };
      }
    }
    objects.push({
      index: objects.length,
      type,
      action,
      name: (attributes.Name ?? childText(body, "Name")) || `Object${objects.length}`,
      enabled: boolValue(attributes.Enabled ?? childText(body, "bEnable"), true),
      mass: numberValue(attributes.masse ?? childText(body, "masse"), 0),
      meshControlled: boolValue(attributes.MeshControlled ?? childText(body, "MeshControlled"), false),
      newtonMaterial: numberValue(attributes.NewtonMat ?? childText(body, "iNewtonMat"), -1),
      position: vector(body, "Pos", [0, 0, 0]),
      rotation: vector(body, "Rot", [0, 0, 0]),
      scale: vector(body, "Scale", [1, 1, 1]),
      pathToMesh,
      textureName: childText(body, "TextureName"),
      assetId,
      resolution
    });
  }
  const resolutionCounts = {};
  const actionCounts = {};
  for (const object of objects) {
    resolutionCounts[object.resolution.status] = (resolutionCounts[object.resolution.status] ?? 0) + 1;
    actionCounts[object.action] = (actionCounts[object.action] ?? 0) + 1;
  }
  return {
    id,
    source: workspacePath(path),
    bytes: bytes.length,
    sha256: sha256(bytes),
    serializedAt: statSync(path).mtime.toISOString(),
    root: "CScene3D",
    schema: "embedded 2015 CScene3D/Object3D action table",
    classification,
    assessment,
    actions,
    objectCount: objects.length,
    actionCounts,
    resolutionCounts,
    renderedObjectCount: objects.filter((object) => ["exact-procedural-primitive", "exact-file", "exact-duplicate-target"].includes(object.resolution.status)).length,
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
  const mgr4 = get("MGR4");
  if (!msta || !mver || !indicesChunk || !matt || !mgro || (!mgrp && !mgr4)) throw new Error(`${path}: incomplete TVM`);
  const stride = data.readUInt32LE(msta.start + 24);
  const vertexCount = mver.size / stride;
  if (!Number.isInteger(vertexCount) || stride < 32) throw new Error(`${path}: unsupported vertex stride ${stride}`);
  const positions = [];
  const normals = [];
  const uvs = [];
  for (let vertex = 0; vertex < vertexCount; vertex += 1) {
    const start = mver.start + vertex * stride;
    for (let axis = 0; axis < 3; axis += 1) positions.push(round(data.readFloatLE(start + axis * 4)));
    for (let axis = 0; axis < 3; axis += 1) normals.push(round(data.readFloatLE(start + 12 + axis * 4)));
    uvs.push(round(data.readFloatLE(start + 24)), round(1 - data.readFloatLE(start + 28)));
  }
  const indexBytes = indicesChunk.tag === "MI16" ? 2 : 4;
  const indices = [];
  for (let offset = 0; offset < indicesChunk.size; offset += indexBytes) {
    indices.push(indexBytes === 2 ? data.readUInt16LE(indicesChunk.start + offset) : data.readUInt32LE(indicesChunk.start + offset));
  }
  const triangleCount = indices.length / 3;
  const materialIndexByTriangle = [];
  for (let triangle = 0; triangle < triangleCount; triangle += 1) materialIndexByTriangle.push(data.readUInt32LE(matt.start + triangle * 4));
  const groups = [];
  for (let offset = 0; offset < mgro.size; offset += 16) {
    groups.push({
      vertexCount: data.readUInt32LE(mgro.start + offset),
      vertexStart: data.readUInt32LE(mgro.start + offset + 4),
      triangleCount: data.readUInt32LE(mgro.start + offset + 8),
      triangleStart: data.readUInt32LE(mgro.start + offset + 12)
    });
  }
  const materialFormat = mgrp ? "MGRP-416" : "MGR4-172";
  const materialChunk = mgrp ?? mgr4;
  const recordSize = materialChunk.size / groups.length;
  const materials = groups.map((_group, index) => {
    const start = materialChunk.start + index * recordSize;
    if (mgrp) {
      const rawName = data.toString("latin1", start, start + 64).split("\0")[0];
      const textureName = data.toString("latin1", start + 64, start + 128).split("\0")[0];
      const diffuse = [0, 1, 2, 3].map((component) => round(data.readFloatLE(start + 348 + component * 4)));
      return { index, name: rawName || `Material${index}`, textureName, diffuse };
    }
    return {
      index,
      name: `LegacyMaterial${index}`,
      textureName: "",
      diffuse: [0, 1, 2, 3].map((component) => round(data.readFloatLE(start + 84 + component * 4)))
    };
  });
  return {
    ...fileEvidence(path),
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
    materials,
    materialFormat
  };
}

const scenes = [
  parseDocument(
    "myworld",
    "MyWorld.xml",
    "distinct but broken editor-save composition",
    "Not the MyWorld - Copie composition: it places the high-detail airplane at (0,10,0), a tiny cylinder at (0.5,-0.005,0), and contains two malformed DUPLICATE records."
  ),
  parseDocument(
    "testworld",
    "TestWorld.xml",
    "distinct scene-loader coverage fixture",
    "Four primitive tests and AirplaneLP form an exact line; the missing bush source and two empty duplicate targets prove incomplete loader coverage, not a finished discoverable world."
  )
];

const tvmAssets = {
  "airplane-high": decodeTvm(join(media, "Airplane", "Airplane.tvm")),
  "airplane-lp": decodeTvm(join(media, "Airplane", "AirplaneLP.TVM"))
};

const exactCopies = [
  [join(media, "Airplane", "Airplane.tvm"), join(stockRoom, "Airplane", "Airplane.tvm")],
  [join(media, "Airplane", "AirplaneLP.TVM"), join(stockRoom, "Airplane", "AirplaneLP.TVM")],
  ...["FUS.BMP", "HTAIL.BMP", "RED.BMP", "VTAIL.BMP", "WHEEL.BMP", "WING.BMP"].map((name) => [
    join(media, "Airplane", name),
    join(stockRoom, "Airplane", name)
  ])
].map(([archivePath, stockPath]) => ({
  archive: fileEvidence(archivePath),
  stockRoom: { path: stockPath.replaceAll("\\", "/"), bytes: statSync(stockPath).size, sha256: sha256(readFileSync(stockPath)) },
  byteIdentical: sha256(readFileSync(archivePath)) === sha256(readFileSync(stockPath))
}));

const manifest = {
  generated: new Date().toISOString(),
  format: "graphysx-ballz-xml-worlds/v1",
  classification: {
    sceneCount: 2,
    assembledDiscoverableWorldCount: 0,
    distinctSerializedCompositionCount: 2,
    reason: "Both are distinct serialized compositions, but source and callsite evidence classify them as editor/loader tests rather than finished exploration worlds."
  },
  parserRule: "Resolve Type with each XML document's own Actions header. Do not use the later archived stdafx.h enum, which inserted PHYSICXSTATIC and shifted DUPLICATE.",
  scenes,
  tvmAssets,
  exactCopies,
  assetSearch: {
    searchedRoots: [workspace.replaceAll("\\", "/"), stockRoom.replaceAll("\\", "/")],
    missing: ["C:/media/Nature/bush1.X"],
    presentButNotLoadableAsSerialized: [
      workspacePath(join(media, "Suzanne1.DoorGate.x")),
      workspacePath(join(media, "Suzanne1.blend.x"))
    ]
  },
  hostEvidence: {
    parser: [
      "Archive/bckup/BallZ2015.bckup/SceneBuilder.cpp:49-54 XmlSerializer deserialization",
      "Archive/bckup/BallZ2015.bckup/SceneBuilder.cpp:60-177 BuildScene action dispatch",
      "Archive/bckup/BallZ2015.bckup/MeshFactory.cpp:113-125 DUPLICATE resolves PathToMesh as a prior mesh name"
    ],
    callsites: [
      "Archive/bckup/BallZ2015.bckup/MainForm.h:451-459 generic c:/test/MyWorld.xml load/save buttons",
      "Archive/bckup/BallZ2015.bckup/MainForm.h:44-46 startup loads ../StockRoom/Suzanne1.xml, not either archived Media document",
      "No surviving literal TestWorld.xml callsite was found"
    ],
    primitiveRules: [
      "MeshFactory.h:45-49 cube dimensions use Scale x/y/z",
      "MeshFactory.h:28-32 sphere radius uses Scale.x",
      "MeshFactory.h:57-61 and 69-73 cylinder/cone use Scale.x radius and Scale.y height with 12 segments",
      "MeshFactory.h:180-207 rotates physics cylinders/cones -90 degrees about Z"
    ]
  },
  evidenceBoundary: {
    exact: [
      "All serialized object records, action tables, transforms and physics metadata.",
      "Five recoverable TestWorld objects and two recoverable MyWorld objects.",
      "Both decoded TVM meshes and all six airplane textures, including byte-identical StockRoom copies."
    ],
    unresolved: [
      "MyWorld's two DUPLICATE records contain file paths where the host requires prior mesh names; the coincident X files are not substituted.",
      "TestWorld's bush1.X is absent and its two DUPLICATE records have empty targets.",
      "Neither XML stores camera, light, sky, gravity, timestep or interaction configuration."
    ],
    inference: [
      "Standalone preview camera, lights and diagnostic ground grid are inspection aids only.",
      "Physics metadata is reported but not simulated without a surviving host-world configuration."
    ]
  }
};

mkdirSync(dirname(output), { recursive: true });
writeFileSync(output, JSON.stringify(manifest));
mkdirSync(join(publicOutput, "airplane"), { recursive: true });
for (const name of ["FUS.BMP", "HTAIL.BMP", "RED.BMP", "VTAIL.BMP", "WHEEL.BMP", "WING.BMP"]) {
  copyFileSync(join(media, "Airplane", name), join(publicOutput, "airplane", name));
}
copyFileSync(join(media, "yellowtwoway.jpg"), join(publicOutput, "yellowtwoway.jpg"));

console.log(`wrote ${output}`);
for (const scene of scenes) console.log(`${scene.id}: ${scene.renderedObjectCount}/${scene.objectCount} exact renderable records; ${scene.classification}`);
console.log(`airplane-high: ${tvmAssets["airplane-high"].vertexCount} vertices / ${tvmAssets["airplane-high"].triangleCount} triangles`);
console.log(`airplane-lp: ${tvmAssets["airplane-lp"].vertexCount} vertices / ${tvmAssets["airplane-lp"].triangleCount} triangles`);
