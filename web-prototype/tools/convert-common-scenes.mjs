// Conversion-only audit for the standalone TrueVision3D common scenes.
//
// Decodes geometry, normals, UVs, face-material assignments and TVM groups
// without touching the web runtime. Material dependencies and the room2 demo
// assembly are recorded from the archived files/source code so a later scene
// implementation does not have to guess.
//
// Usage: node tools/convert-common-scenes.mjs
// Writes: src/legacy/common-scenes.json

import { createHash } from "node:crypto";
import { copyFileSync, mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { dirname, join, relative } from "node:path";
import { fileURLToPath } from "node:url";

const here = dirname(fileURLToPath(import.meta.url));
const root = join(here, "..", "..");
const common = join(root, "common");
const output = join(here, "..", "src", "legacy", "common-scenes.json");
const publicTextures = join(here, "..", "public", "assets", "textures", "common");

const definitions = [
  {
    id: "common-room",
    file: "room.tvm",
    classification: "unassembled inward-facing room-shell asset",
    materials: [
      {
        id: 0,
        name: "TV3D logo tangent-space bump material",
        colorMap: "common/tv3dlogo_d.dds",
        normalMap: "common/tv3dlogo_n.dds"
      }
    ]
  },
  {
    id: "common-room2-shadow-demo",
    file: "room2.tvm",
    classification: "standalone room used by archived HLSL Shadow Mapping #23 demo",
    materials: [
      {
        id: 0,
        name: "TV3D logo tangent-space bump material",
        colorMap: "common/tv3dlogo_d.dds",
        normalMap: "common/tv3dlogo_n.dds"
      }
    ]
  },
  {
    id: "common-sky-space",
    file: "sky.tvm",
    classification: "inward-facing skydome environment component, not an evidenced standalone scene",
    materials: [
      { id: 0, name: "+X inward sky face", colorMap: "common/mid_day.dds" },
      { id: 1, name: "+Z inward sky face", colorMap: "common/mid_day.dds" },
      { id: 2, name: "-X inward sky face", colorMap: "common/mid_day.dds" },
      { id: 3, name: "-Z inward sky face", colorMap: "common/mid_day.dds" },
      { id: 4, name: "+Y inward sky pole", colorMap: "common/top_pole.dds" },
      { id: 5, name: "-Y inward sky pole", colorMap: "common/bottom_pole.dds" }
    ]
  }
];

const knownStrides = new Set([24, 32, 36, 40, 44, 48, 56, 64]);
const round = (value) => Math.round(value * 1_000_000) / 1_000_000;

function sha256(data) {
  return createHash("sha256").update(data).digest("hex").toUpperCase();
}

function chunksOf(data) {
  const chunks = [];
  let offset = 0;
  while (offset <= data.length - 8) {
    const tag = data.toString("latin1", offset, offset + 4);
    const size = data.readUInt32LE(offset + 4);
    if (/^[A-Z0-9! ]{4}$/.test(tag) && offset + 8 + size <= data.length) {
      chunks.push({ tag, offset, start: offset + 8, size });
      offset += 8 + size;
    } else {
      offset += 1;
    }
  }
  return chunks;
}

function boundsOf(positions) {
  const min = [Infinity, Infinity, Infinity];
  const max = [-Infinity, -Infinity, -Infinity];
  for (let i = 0; i < positions.length; i += 3) {
    for (let axis = 0; axis < 3; axis += 1) {
      min[axis] = Math.min(min[axis], positions[i + axis]);
      max[axis] = Math.max(max[axis], positions[i + axis]);
    }
  }
  return {
    min: min.map(round),
    max: max.map(round),
    size: max.map((value, axis) => round(value - min[axis]))
  };
}

function embeddedDdsNames(data) {
  const names = [];
  let run = "";
  for (const byte of data) {
    if (byte >= 32 && byte <= 126) {
      run += String.fromCharCode(byte);
    } else {
      if (/\.dds$/i.test(run)) names.push(run);
      run = "";
    }
  }
  if (/\.dds$/i.test(run)) names.push(run);
  return names;
}

function decodeTvm(definition) {
  const path = join(common, definition.file);
  const data = readFileSync(path);
  const chunks = chunksOf(data);
  const msta = chunks.find((chunk) => chunk.tag === "MSTA");
  const mver = chunks.find((chunk) => chunk.tag === "MVER");
  const indexChunk = chunks.find((chunk) => chunk.tag === "MI16" || chunk.tag === "MI32");
  const matt = chunks.find((chunk) => chunk.tag === "MATT");
  const mgro = chunks.find((chunk) => chunk.tag === "MGRO");

  if (!msta || !mver || !indexChunk || !matt || !mgro) {
    throw new Error(`${definition.file}: required TVM geometry chunks are missing`);
  }

  const stride = data.readUInt32LE(msta.start + 24);
  if (!knownStrides.has(stride) || stride < 24) {
    throw new Error(`${definition.file}: unsupported vertex stride ${stride}`);
  }

  const vertexCount = Math.floor(mver.size / stride);
  const positions = new Array(vertexCount * 3);
  const normals = new Array(vertexCount * 3);
  const uvs = stride >= 32 ? new Array(vertexCount * 2) : null;
  for (let vertex = 0; vertex < vertexCount; vertex += 1) {
    const start = mver.start + vertex * stride;
    for (let axis = 0; axis < 3; axis += 1) {
      positions[vertex * 3 + axis] = round(data.readFloatLE(start + axis * 4));
      normals[vertex * 3 + axis] = round(data.readFloatLE(start + 12 + axis * 4));
    }
    if (uvs) {
      uvs[vertex * 2] = round(data.readFloatLE(start + 24));
      uvs[vertex * 2 + 1] = round(1 - data.readFloatLE(start + 28));
    }
  }

  const indexBytes = indexChunk.tag === "MI16" ? 2 : 4;
  const indices = new Array(Math.floor(indexChunk.size / indexBytes));
  for (let index = 0; index < indices.length; index += 1) {
    indices[index] = indexBytes === 2
      ? data.readUInt16LE(indexChunk.start + index * indexBytes)
      : data.readUInt32LE(indexChunk.start + index * indexBytes);
    if (indices[index] >= vertexCount) {
      throw new Error(`${definition.file}: index ${indices[index]} exceeds ${vertexCount} vertices`);
    }
  }

  const triangleCount = indices.length / 3;
  if (matt.size / 4 !== triangleCount) {
    throw new Error(`${definition.file}: material-face count does not match triangle count`);
  }
  const materialIndexByTriangle = new Array(triangleCount);
  for (let triangle = 0; triangle < triangleCount; triangle += 1) {
    materialIndexByTriangle[triangle] = data.readUInt32LE(matt.start + triangle * 4);
  }

  if (mgro.size % 16 !== 0) {
    throw new Error(`${definition.file}: unexpected MGRO size ${mgro.size}`);
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

  return {
    id: definition.id,
    classification: definition.classification,
    source: relative(root, path).replaceAll("\\", "/"),
    bytes: data.length,
    sha256: sha256(data),
    vertexStride: stride,
    vertexCount,
    triangleCount,
    bounds: boundsOf(positions),
    embeddedTextureNames: embeddedDdsNames(data),
    materials: definition.materials,
    groups,
    positions,
    normals,
    uvs,
    indices,
    materialIndexByTriangle
  };
}

const result = {
  generated: new Date().toISOString(),
  note: "Conversion-only standalone-scene audit; no runtime integration implied.",
  decoderCompatibility: {
    format: "TrueVision3D 6.5 TVM chunk stream",
    chunksUsed: ["MSTA", "MVER", "MI16/MI32", "MATT", "MGRO"],
    status: "Geometry, normals, UVs, indices and material groups decode without fallback."
  },
  recoveryAssessment: {
    evidence: "TVM geometry/material chunks, embedded DDS names, file dates, and repository-wide source search",
    exact: [
      "room.tvm is a 25-unit inward-facing room shell with one TV3D logo diffuse/normal material.",
      "sky.tvm is a unit-radius inward-facing subdivided skydome with four mid_day side groups and top/bottom pole groups.",
      "No archived source in this repository loads room.tvm or sky.tvm; only room2.tvm has a recovered authored scene assembly."
    ],
    inference: [
      "Room 1 preview camera, light, and transform are restoration choices because no authored assembly was found.",
      "The sky preview scales sky.tvm around its camera to demonstrate the component; this does not promote it to a distinct archived scene."
    ]
  },
  room2ArchiveAssembly: {
    evidence: "#23 HLSL_Shadow_Mapping/main.cpp",
    room: { position: [0, -12.5, 0], scale: [2, 2, 2] },
    teapot: { position: [0, 0, 0], scale: [3, 3, 3] },
    lightMarker: { type: "sphere", radius: 0.25, position: [0, 15, 0] },
    light: { position: [0, 15, 0], color: [1, 1, 1] },
    camera: { orbitCenter: [0, 20, 0], orbitRadius: 50, lookAt: [0, 0, 0], controls: "A/D orbit" },
    shading: "Tangent-space bump mapping plus point-light cube shadow map"
  },
  scenes: definitions.map(decodeTvm)
};

writeFileSync(output, JSON.stringify(result));
mkdirSync(publicTextures, { recursive: true });
for (const file of ["tv3dlogo_d.dds", "tv3dlogo_n.dds", "mid_day.dds", "top_pole.dds", "bottom_pole.dds"]) {
  copyFileSync(join(common, file), join(publicTextures, file));
}
console.log(`wrote ${output}`);
console.log(`copied archived DDS dependencies to ${publicTextures}`);
for (const scene of result.scenes) {
  console.log(`${scene.id}: ${scene.vertexCount} vertices, ${scene.triangleCount} triangles, ${scene.groups.length} material group(s)`);
}
