// Deterministic audit/conversion for the remaining BallZ slide/track family.
//
// The family contains aliases, binary revisions with the same filename, unused
// experiments, and one pipe whose only evidenced host is FlightX. This script
// records all of those boundaries and exports full geometry only for the
// strongest distinct next candidate: the active Atmel-era Slide1 + Ball.tvm
// assembly used by CLBallZ/CLLevel.
//
// Usage: node tools/convert-ballz-slide-track-family.mjs
// Writes: src/legacy/ballz-slide-track-family.json

import { createHash } from "node:crypto";
import { copyFileSync, existsSync, mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { dirname, join, relative } from "node:path";
import { fileURLToPath } from "node:url";

const here = dirname(fileURLToPath(import.meta.url));
const root = join(here, "..", "..");
const outputPath = join(here, "..", "src", "legacy", "ballz-slide-track-family.json");
const galleryTextureOutput = join(here, "..", "public", "assets", "ballz-track-gallery");
const archiveMedia = join(root, "Archive", "bckup", "BallZ2015.bckup", "Media");
const modelMedia = join(root, "Yanik C++ BCKUP", "Media", "Models");
const releaseMedia = join(root, "Yanik C++ BCKUP", "BallZ 2011", "Release", "Media");
const cppReleaseMedia = join(root, "Yanik C++ BCKUP", "cpp", "Release", "Media");
const stockRoom = "E:/Media/Datalake/Tech/StockRoom";
const repoStockRoom = "E:/Media/Datalake/Tech/Repos/GraphysX/StockRoom";
const knownStrides = new Set([24, 32, 36, 40, 44, 48, 56, 64]);
const galleryAssetIds = [
  "slide1a-legacy-active",
  "level-slides",
  "level-steps",
  "slide-bump",
  "slide-bump-gridtex",
  "ballz-track1"
];
const galleryAssetIdSet = new Set(galleryAssetIds);
const galleryTextureSources = {
  "grass.jpg": { source: join(stockRoom, "Grass.jpg"), output: "grass.jpg" },
  "concrete.png": { source: join(stockRoom, "concrete.png"), output: "concrete.png" },
  "wood.jpg": { source: join(stockRoom, "wood.jpg"), output: "wood.jpg" },
  "earthgri.bmp": { source: join(stockRoom, "EarthGri.bmp"), output: "earthgri.bmp" }
};

const round = (value) => Math.round(value * 1_000_000) / 1_000_000;
const sha256 = (data) => createHash("sha256").update(data).digest("hex").toUpperCase();
const hashValues = (values) => sha256(Buffer.from(values.join(",")));

function readFixedString(data, start, length) {
  return data.toString("latin1", start, start + length).replace(/\0.*$/s, "");
}

function rgbaAt(data, start) {
  return Array.from({ length: 4 }, (_, index) => round(data.readFloatLE(start + index * 4)));
}

const definitions = [
  {
    id: "slide1-atmel-active",
    label: "Slide 1 — active Atmel/CubX revision",
    file: join(modelMedia, "Slide1.tvm"),
    role: "selected-playable-candidate",
    loaderEvidence: [
      "AtmelCubx/AtmelCubx/BallZ.cpp::CLBallZ -> LoadLevel(media\\slide1.tvm,1)",
      "AtmelCubx/AtmelCubx/Level.cpp::LoadLevel -> scale 1, position (0,-5000,0), static mesh body",
      "AtmelCubx/AtmelCubx/Ball.cpp -> Ball.tvm scale 0.5, spawn (-50,-4750,-250), mass 5000",
      "AtmelCubx/AtmelCubx/Area.cpp::BALLZ_MODE -> chase offset (0,350,300), gravity (0,-9.800908285,0)"
    ]
  },
  {
    id: "slide1-stockroom-revision",
    label: "Slide 1 — later StockRoom revision",
    file: join(archiveMedia, "Slide1.TVM"),
    role: "distinct-binary-revision",
    loaderEvidence: []
  },
  {
    id: "slide1a-legacy-active",
    label: "Slide 1A — legacy active revision",
    file: join(modelMedia, "Slide1a.tvm"),
    role: "source-loaded-older-candidate",
    loaderEvidence: [
      "Yanik C++ BCKUP/BallZ.cpp::CLBallZ -> LoadLevel(media\\slide1a.tvm,1)",
      "Yanik C++ BCKUP/Level.cpp::LoadLevel -> scale 1, position (0,-5000,0), static mesh body"
    ]
  },
  {
    id: "level-slides",
    label: "Level.Slides",
    file: join(stockRoom, "Level.Slides.TVM"),
    role: "unhosted-stockroom-experiment",
    loaderEvidence: []
  },
  {
    id: "level-steps",
    label: "Level.Steps",
    file: join(stockRoom, "Level.Steps.TVM"),
    role: "unhosted-stockroom-experiment",
    loaderEvidence: []
  },
  {
    id: "slide-bump",
    label: "Slide Bump",
    file: join(archiveMedia, "SlideBump.TVM"),
    role: "unhosted-experiment",
    loaderEvidence: []
  },
  {
    id: "slide-bump-gridtex",
    label: "Slide Bump GridTex",
    file: join(archiveMedia, "SlideBumpGridTex.TVM"),
    role: "unhosted-texture-variant",
    loaderEvidence: []
  },
  {
    id: "slide-long1",
    label: "Slide Long 1",
    file: join(archiveMedia, "SlideLong1.TVM"),
    role: "byte-alias-of-ballz2011-level1",
    loaderEvidence: []
  },
  {
    id: "ballz-track1",
    label: "BallZ Track 1",
    file: join(archiveMedia, "BallZTrack1.tvm"),
    role: "source-hosted-static-scene",
    loaderEvidence: [
      "Yanik C++ BCKUP/RacinX/3DScenes.h::CLBallZScene -> scale 1, position (0,0,0)",
      "RacinX CLBallZ spawn (-20,310,225), initial camera (400,300,200), chase offset (25,100,30)",
      "CLBallZScene::UpdateScene and MeshClickedAction are empty; its 100 diagonal rings are explicitly placeholder/test positions"
    ]
  },
  {
    id: "pipe1",
    label: "Pipe 1",
    file: join(archiveMedia, "pipe1.tvm"),
    role: "flightx-component-not-distinct-ballz-scene",
    loaderEvidence: [
      "AtmelCubx/AtmelCubx/FlightXScene.cpp -> scale 5, position (0,-500,0)",
      "No searched source loads pipe1.tvm as a BallZ slide or dedicated BallZ scene"
    ]
  }
];

const copyPathsById = {
  "slide1-atmel-active": [join(modelMedia, "Slide1.tvm"), join(releaseMedia, "Slide1.tvm"), join(cppReleaseMedia, "Slide1.tvm")],
  "slide1-stockroom-revision": [join(archiveMedia, "Slide1.TVM"), join(stockRoom, "Slide1.TVM"), join(repoStockRoom, "Slide1.TVM")],
  "slide1a-legacy-active": [join(modelMedia, "Slide1a.tvm"), join(releaseMedia, "Slide1a.tvm"), join(cppReleaseMedia, "Slide1a.tvm")],
  "level-slides": [join(stockRoom, "Level.Slides.TVM"), join(repoStockRoom, "Level.Slides.TVM")],
  "level-steps": [join(stockRoom, "Level.Steps.TVM"), join(repoStockRoom, "Level.Steps.TVM")],
  "slide-bump": [join(archiveMedia, "SlideBump.TVM"), join(stockRoom, "SlideBump.TVM"), join(repoStockRoom, "SlideBump.TVM")],
  "slide-bump-gridtex": [join(archiveMedia, "SlideBumpGridTex.TVM"), join(stockRoom, "SlideBumpGridTex.TVM"), join(repoStockRoom, "SlideBumpGridTex.TVM")],
  "slide-long1": [join(archiveMedia, "SlideLong1.TVM"), join(stockRoom, "SlideLong1.TVM"), join(repoStockRoom, "SlideLong1.TVM")],
  "ballz-track1": [join(archiveMedia, "BallZTrack1.tvm"), join(modelMedia, "BallZTrack1.tvm"), join(releaseMedia, "BallZTrack1.tvm"), join(cppReleaseMedia, "BallZTrack1.tvm")],
  "pipe1": [join(archiveMedia, "pipe1.tvm"), join(modelMedia, "pipe1.tvm"), join(releaseMedia, "pipe1.tvm"), join(cppReleaseMedia, "pipe1.tvm"), join(stockRoom, "pipe1.tvm"), join(repoStockRoom, "pipe1.tvm")]
};

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
  for (let index = 0; index < positions.length; index += 3) {
    for (let axis = 0; axis < 3; axis += 1) {
      min[axis] = Math.min(min[axis], positions[index + axis]);
      max[axis] = Math.max(max[axis], positions[index + axis]);
    }
  }
  return {
    min: min.map(round),
    max: max.map(round),
    size: max.map((value, axis) => round(value - min[axis]))
  };
}

function printableNames(data) {
  const names = [];
  let run = "";
  for (const byte of data) {
    if (byte >= 32 && byte <= 126) run += String.fromCharCode(byte);
    else {
      if (/\.(dds|bmp|jpg|jpeg|png|tga)$/i.test(run)) names.push(run);
      run = "";
    }
  }
  return [...new Set(names)];
}

function topologyOf(vertexCount, indices, positions) {
  const adjacency = Array.from({ length: vertexCount }, () => []);
  const edges = new Map();
  for (let index = 0; index < indices.length; index += 3) {
    const triangle = [indices[index], indices[index + 1], indices[index + 2]];
    for (const [from, to] of [[triangle[0], triangle[1]], [triangle[1], triangle[2]], [triangle[2], triangle[0]]]) {
      adjacency[from].push(to);
      adjacency[to].push(from);
      const key = from < to ? `${from},${to}` : `${to},${from}`;
      edges.set(key, (edges.get(key) ?? 0) + 1);
    }
  }
  const edgeUseCounts = {};
  for (const uses of edges.values()) edgeUseCounts[uses] = (edgeUseCounts[uses] ?? 0) + 1;
  const visited = new Set();
  const components = [];
  for (let start = 0; start < vertexCount; start += 1) {
    if (visited.has(start)) continue;
    const stack = [start];
    const vertices = [];
    visited.add(start);
    while (stack.length > 0) {
      const vertex = stack.pop();
      vertices.push(vertex);
      for (const neighbor of adjacency[vertex]) {
        if (!visited.has(neighbor)) {
          visited.add(neighbor);
          stack.push(neighbor);
        }
      }
    }
    const componentPositions = [];
    for (const vertex of vertices) {
      componentPositions.push(positions[vertex * 3], positions[vertex * 3 + 1], positions[vertex * 3 + 2]);
    }
    components.push({ vertexCount: vertices.length, bounds: boundsOf(componentPositions) });
  }
  return {
    edgeCount: edges.size,
    edgeUseCounts,
    boundaryEdgeCount: edgeUseCounts[1] ?? 0,
    nonManifoldEdgeCount: Object.entries(edgeUseCounts)
      .filter(([uses]) => Number(uses) > 2)
      .reduce((sum, [, count]) => sum + count, 0),
    connectedComponents: components.sort((a, b) => b.vertexCount - a.vertexCount)
  };
}

function decodeTvm(definition, includeGeometry = false) {
  if (!existsSync(definition.file)) throw new Error(`${definition.id}: missing source ${definition.file}`);
  const data = readFileSync(definition.file);
  const chunks = chunksOf(data);
  const chunk = (tag) => chunks.find((entry) => entry.tag === tag);
  const msta = chunk("MSTA");
  const mver = chunk("MVER");
  const indexChunk = chunk("MI16") ?? chunk("MI32");
  const matt = chunk("MATT");
  const mgro = chunk("MGRO");
  const mgrp = chunk("MGRP");
  const mgr4 = chunk("MGR4");
  if (!msta || !mver || !indexChunk) throw new Error(`${definition.id}: required geometry chunks missing`);
  const stride = data.readUInt32LE(msta.start + 24);
  if (!knownStrides.has(stride) || stride < 24) throw new Error(`${definition.id}: unsupported stride ${stride}`);
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
    if (indices[index] >= vertexCount) throw new Error(`${definition.id}: index ${indices[index]} exceeds ${vertexCount}`);
  }
  const triangleCount = indices.length / 3;
  const materialIndexByTriangle = matt && matt.size === triangleCount * 4
    ? Array.from({ length: triangleCount }, (_, triangle) => data.readUInt32LE(matt.start + triangle * 4))
    : [];
  const groups = [];
  if (mgro && mgro.size % 16 === 0) {
    for (let offset = 0; offset < mgro.size; offset += 16) {
      groups.push({
        vertexCount: data.readUInt32LE(mgro.start + offset),
        vertexStart: data.readUInt32LE(mgro.start + offset + 4),
        triangleCount: data.readUInt32LE(mgro.start + offset + 8),
        triangleStart: data.readUInt32LE(mgro.start + offset + 12)
      });
    }
  }
  const uvPairs = new Set();
  if (uvs) {
    for (let index = 0; index < uvs.length; index += 2) uvPairs.add(`${uvs[index]},${uvs[index + 1]}`);
  }
  const embeddedMaterialRecords = [];
  if (mgrp && mgrp.size % 416 === 0) {
    for (let offset = 0; offset < mgrp.size; offset += 416) {
      const start = mgrp.start + offset;
      const name = readFixedString(data, start, 64);
      const textureName = readFixedString(data, start + 64, 64) || null;
      embeddedMaterialRecords.push({
        format: "MGRP-416",
        materialIndex: embeddedMaterialRecords.length,
        name,
        textureName,
        rgbaSlots: [rgbaAt(data, start + 348), rgbaAt(data, start + 364), rgbaAt(data, start + 380)],
        rawRecordSha256: sha256(data.subarray(start, start + 416))
      });
    }
  }
  const legacyMaterialRecords = [];
  if (mgr4 && mgr4.size % 172 === 0) {
    for (let offset = 0; offset < mgr4.size; offset += 172) {
      const start = mgr4.start + offset;
      legacyMaterialRecords.push({
        format: "MGR4-172",
        materialIndex: legacyMaterialRecords.length,
        rgbaSlots: [rgbaAt(data, start + 84), rgbaAt(data, start + 116)],
        rawRecordSha256: sha256(data.subarray(start, start + 172))
      });
    }
  }
  const objectNames = embeddedMaterialRecords.map((record) => record.name).filter(Boolean);
  const topology = topologyOf(vertexCount, indices, positions);
  const geometryHash = hashValues([...positions, ...indices]);
  const knownCopies = (copyPathsById[definition.id] ?? [definition.file])
    .filter((path) => existsSync(path))
    .map((path) => {
      const copyData = readFileSync(path);
      return {
        source: relative(root, path).replaceAll("\\", "/"),
        bytes: copyData.length,
        sha256: sha256(copyData)
      };
    });
  const sourceHash = sha256(data);
  const result = {
    id: definition.id,
    label: definition.label,
    role: definition.role,
    source: relative(root, definition.file).replaceAll("\\", "/"),
    bytes: data.length,
    sha256: sourceHash,
    knownCopies,
    copiesByteIdentical: knownCopies.every((copy) => copy.sha256 === sourceHash),
    geometrySha256: geometryHash,
    vertexStride: stride,
    vertexCount,
    triangleCount,
    bounds: boundsOf(positions),
    objectNames,
    groups,
    materialIndices: [...new Set(materialIndexByTriangle)].sort((a, b) => a - b),
    materialAssignmentCount: materialIndexByTriangle.length,
    uv: {
      present: Boolean(uvs),
      uniquePairCount: uvPairs.size,
      allIdentical: uvPairs.size <= 1,
      finite: uvs ? uvs.every(Number.isFinite) : false
    },
    embeddedTextureNames: printableNames(data),
    embeddedMaterialRecords,
    legacyMaterialRecords,
    topology,
    loaderEvidence: definition.loaderEvidence,
    chunks: chunks.map(({ tag, offset, size }) => ({ tag, offset, size }))
  };
  if (includeGeometry) {
    result.geometry = { positions, normals, uvs, indices, materialIndexByTriangle };
  }
  return result;
}

const assets = definitions.map((definition) => decodeTvm(
  definition,
  definition.id === "slide1-atmel-active" || galleryAssetIdSet.has(definition.id)
));
const selected = assets.find((asset) => asset.id === "slide1-atmel-active");
if (!selected?.geometry) throw new Error("Selected Slide1 geometry was not exported.");

const ballDefinition = {
  id: "ball-tvm-atmel",
  label: "Ball.tvm used by the active Slide1 host",
  file: join(modelMedia, "Ball.tvm"),
  role: "selected-candidate-companion",
  loaderEvidence: ["AtmelCubx/AtmelCubx/Ball.cpp -> scale 0.5, spawn (-50,-4750,-250), mass 5000"]
};
const ball = decodeTvm(ballDefinition, true);

mkdirSync(galleryTextureOutput, { recursive: true });
const galleryTextureCatalog = Object.fromEntries(Object.entries(galleryTextureSources).map(([embeddedName, entry]) => {
  if (!existsSync(entry.source)) throw new Error(`Gallery texture ${embeddedName} is missing at ${entry.source}.`);
  const sourceData = readFileSync(entry.source);
  const outputPath = join(galleryTextureOutput, entry.output);
  copyFileSync(entry.source, outputPath);
  const outputData = readFileSync(outputPath);
  if (sha256(outputData) !== sha256(sourceData)) throw new Error(`Gallery texture copy ${entry.output} changed bytes.`);
  return [embeddedName, {
    embeddedName,
    source: relative(root, entry.source).replaceAll("\\", "/"),
    bytes: sourceData.length,
    sha256: sha256(sourceData),
    browserPath: `/assets/ballz-track-gallery/${entry.output}`,
    byteIdenticalOutput: true
  }];
}));

const galleryHostEvidence = {
  "slide1a-legacy-active": {
    status: "source-hosted-static-level",
    position: [0, -5000, 0],
    scale: [1, 1, 1],
    materialMode: "host-overrides-all-groups-with-StdMat",
    staticMeshBody: true,
    gameplayBoundary: "The older host loads this slide and placeholder rings, but the later complete Ball spawn/physics assembly belongs to a different Slide1 revision."
  },
  "level-slides": {
    status: "no-located-host",
    position: null,
    scale: null,
    materialMode: "embedded-MGRP-colors-no-textures",
    staticMeshBody: null,
    gameplayBoundary: "No transform, spawn, collision setup, camera or objective was located."
  },
  "level-steps": {
    status: "no-located-host",
    position: null,
    scale: null,
    materialMode: "embedded-MGRP-texture-bindings",
    staticMeshBody: null,
    gameplayBoundary: "No transform, spawn, collision setup, camera or objective was located."
  },
  "slide-bump": {
    status: "no-located-host",
    position: null,
    scale: null,
    materialMode: "embedded-MGRP-record-is-zeroed",
    staticMeshBody: null,
    gameplayBoundary: "No host was located; open/non-manifold topology and a zero material record are preserved as evidence."
  },
  "slide-bump-gridtex": {
    status: "no-located-host",
    position: null,
    scale: null,
    materialMode: "embedded-MGRP-EarthGri.bmp-binding",
    staticMeshBody: null,
    gameplayBoundary: "No host was located; this is a distinct UV/vertex re-export of the same bump form."
  },
  "ballz-track1": {
    status: "source-hosted-static-scene",
    position: [0, 0, 0],
    scale: [1, 1, 1],
    materialMode: "CL3DObject-host-overrides-all-groups-with-StdMat",
    staticMeshBody: true,
    initialCameraPosition: [400, 300, 200],
    ballSpawn: [-20, 310, 225],
    chaseOffset: [25, 100, 30],
    gameplayBoundary: "UpdateScene is empty; ManageCamCtrl is commented out; the 100 diagonal rings are explicitly temporary placeholders."
  }
};

function galleryNormalization(asset, host) {
  const position = host.position ?? [0, 0, 0];
  const scale = host.scale ?? [1, 1, 1];
  const sourceWorldBounds = {
    min: asset.bounds.min.map((value, axis) => round(value * scale[axis] + position[axis])),
    max: asset.bounds.max.map((value, axis) => round(value * scale[axis] + position[axis]))
  };
  sourceWorldBounds.size = sourceWorldBounds.max.map((value, axis) => round(value - sourceWorldBounds.min[axis]));
  const anchor = [
    round((sourceWorldBounds.min[0] + sourceWorldBounds.max[0]) / 2),
    sourceWorldBounds.min[1],
    round((sourceWorldBounds.min[2] + sourceWorldBounds.max[2]) / 2)
  ];
  const displayScale = round(72 / Math.max(...sourceWorldBounds.size));
  const displayBounds = {
    min: sourceWorldBounds.min.map((value, axis) => round((value - anchor[axis]) * displayScale)),
    max: sourceWorldBounds.max.map((value, axis) => round((value - anchor[axis]) * displayScale))
  };
  displayBounds.size = displayBounds.max.map((value, axis) => round(value - displayBounds.min[axis]));
  return {
    status: "inferred-reversible-display-only",
    basis: host.position ? "exact-source-world-transform" : "unhosted-local-mesh-coordinates",
    sourceWorldBounds,
    anchor,
    displayScale,
    displayBounds,
    inverse: "sourceWorldPosition = displayPosition / displayScale + anchor",
    preservesAspectAndOrientation: true
  };
}

const galleryAssets = galleryAssetIds.map((id) => {
  const asset = assets.find((candidate) => candidate.id === id);
  if (!asset?.geometry) throw new Error(`Gallery asset ${id} has no exported geometry.`);
  const host = galleryHostEvidence[id];
  const resolvedTextureBindings = asset.embeddedMaterialRecords
    .filter((record) => record.textureName)
    .map((record) => ({
      materialIndex: record.materialIndex,
      textureName: record.textureName,
      resolved: galleryTextureCatalog[record.textureName.toLowerCase()] ?? null
    }));
  return {
    id,
    host,
    normalization: galleryNormalization(asset, host),
    resolvedTextureBindings,
    unresolvedTextureBindings: resolvedTextureBindings.filter((binding) => !binding.resolved).map((binding) => binding.textureName)
  };
});

const selectedWorldBounds = {
  min: [selected.bounds.min[0], selected.bounds.min[1] - 5000, selected.bounds.min[2]].map(round),
  max: [selected.bounds.max[0], selected.bounds.max[1] - 5000, selected.bounds.max[2]].map(round)
};
selectedWorldBounds.size = selectedWorldBounds.max.map((value, axis) => round(value - selectedWorldBounds.min[axis]));
const displayAnchor = [
  round((selectedWorldBounds.min[0] + selectedWorldBounds.max[0]) / 2),
  selectedWorldBounds.min[1],
  round((selectedWorldBounds.min[2] + selectedWorldBounds.max[2]) / 2)
];
const displayScale = 0.02;
const displayBounds = {
  min: selectedWorldBounds.min.map((value, axis) => round((value - displayAnchor[axis]) * displayScale)),
  max: selectedWorldBounds.max.map((value, axis) => round((value - displayAnchor[axis]) * displayScale))
};
displayBounds.size = displayBounds.max.map((value, axis) => round(value - displayBounds.min[axis]));

const byHash = new Map();
for (const asset of assets) {
  const ids = byHash.get(asset.sha256) ?? [];
  ids.push(asset.id);
  byHash.set(asset.sha256, ids);
}

const result = {
  generated: new Date().toISOString(),
  note: "Slide/track family audit. Geometry facts are exact; scene/playability assessments follow only located source loaders.",
  deDuplication: {
    exactBinaryAliases: [
      {
        ids: ["slide-long1", "ballz2011-level1"],
        sha256: assets.find((asset) => asset.id === "slide-long1")?.sha256,
        assessment: "SlideLong1.TVM and BallZ 2011 Level1.TVM are byte-identical; do not create another scene."
      },
      {
        ids: ["pipe1", "flightx-pipe-component"],
        sha256: assets.find((asset) => asset.id === "pipe1")?.sha256,
        assessment: "All located pipe1 copies are byte-identical and only FlightX source loads them."
      }
    ],
    sameNameDistinctBinaries: [
      {
        name: "Slide1.TVM",
        ids: ["slide1-atmel-active", "slide1-stockroom-revision"],
        hashes: [
          assets.find((asset) => asset.id === "slide1-atmel-active")?.sha256,
          assets.find((asset) => asset.id === "slide1-stockroom-revision")?.sha256
        ],
        assessment: "These are distinct geometry revisions and must not be silently substituted."
      }
    ],
    auditedHashGroups: [...byHash.entries()].map(([hash, ids]) => ({ hash, ids }))
  },
  remainingGallery: {
    schema: "graphysx.ballz-track-gallery/v1",
    assetIds: galleryAssetIds,
    purpose: "Exact-geometry archive visits only. No gameplay, spawn, physics or objectives are inferred.",
    excludedAliases: [
      { id: "slide-long1", reason: "Byte-identical to the recovered BallZ 2011 Level1 visit." },
      { id: "pipe1", reason: "Already classified as a FlightX component, not a distinct BallZ scene." }
    ],
    excludedRevision: {
      id: "slide1-stockroom-revision",
      reason: "Distinct same-name binary remains audited, but this requested gallery is scoped to Slide1A, Slides, Steps, both Bump revisions and BallZTrack1."
    },
    materialPolicy: "Use exact host StdMat overrides where source exists; otherwise use exact MGRP group colors/textures when valid. A zeroed source material is shown with a disclosed neutral inspection fallback.",
    textureCatalog: galleryTextureCatalog,
    assets: galleryAssets,
    statusRecommendations: [
      { id: "slide1a-legacy-active", status: "PARTIAL", reason: "Exact geometry and older static host transform are visitable; the complete later Ball assembly cannot be borrowed from the distinct Slide1 revision." },
      { id: "level-slides", status: "PARTIAL", reason: "Exact three-group geometry/colors are visitable, but no host transform, camera, collision, spawn or objective survives." },
      { id: "level-steps", status: "PARTIAL", reason: "Exact three-group geometry and grass/concrete/wood bindings are visitable, but no host behavior survives." },
      { id: "slide-bump", status: "PARTIAL", reason: "Exact geometry/topology is visitable; no host survives and the embedded material record is zeroed." },
      { id: "slide-bump-gridtex", status: "PARTIAL", reason: "Exact distinct UV/vertex revision and EarthGri binding are visitable; no host behavior survives." },
      { id: "ballz-track1", status: "PARTIAL", reason: "Exact static host track, Ball spawn and camera evidence survive, but Update/Input are empty and rings are explicit placeholders." }
    ]
  },
  sourceBehavior: {
    selectedAssembly: {
      exact: true,
      slide: { position: [0, -5000, 0], scale: [1, 1, 1], staticMeshBody: true },
      ball: { source: "Yanik C++ BCKUP/Media/Models/Ball.tvm", position: [-50, -4750, -250], scale: [0.5, 0.5, 0.5], mass: 5000 },
      camera: { kind: "TV3D ChaseCamera", offset: [0, 350, 300], lookAtOffset: [0, 0, 0], smoothing: 1000 },
      physics: {
        gravity: [0, -9.800908285, 0],
        slideDefaultFriction: [1, 0.9],
        ballSlideFriction: [0.2, 0.075],
        ballSlideBounciness: 0.05,
        softness: 1
      },
      material: { ambient: [0.1, 0.1, 0.1, 1], diffuse: [0.8, 0.8, 0.8, 1], specular: [0.2, 0.2, 0.2, 1], power: 20, emissive: [0, 0, 0, 1], textureOverride: null },
      rings: {
        count: 100,
        first: [0, 100, 0],
        last: [990, 1, 990],
        status: "source-exact-placeholder-not-gameplay-fidelity",
        evidence: "Level.cpp calls these offsets 'pour le fun' and says XML/table placement is still to implement."
      },
      controls: {
        directions: ["front", "back", "left", "right"],
        impulseMagnitude: 100,
        status: "partial-source-implementation",
        evidence: "PushBall accumulates direction impulses, but the rendered impulse block only executes for RotBack; other torque blocks are commented out."
      }
    }
  },
  selection: {
    assetId: "slide1-atmel-active",
    recommendation: "PARTIAL non-race visit first; playable only after preserving the source's incomplete-control disclosure and validating collision/spawn in the browser.",
    rationale: "Unlike the unhosted variants, this binary has a complete located assembly: exact transform, Ball.tvm companion, spawn, chase camera, gravity, material and contact settings. Its original input path is incomplete, so a faithful playable claim would currently be false.",
    exactBoundary: "TVM geometry, world transform, Ball.tvm transform, camera offset, gravity, material and contact constants.",
    inferredBoundary: "Web display normalization, managed-lighting translation, overview camera, and any future corrected four-direction controls or objective.",
    displayNormalization: {
      status: "inferred-display-only",
      method: "Apply the exact source world transform, then center X/Z, ground the transformed slide minimum Y, and apply one uniform scale.",
      sourceWorldBounds: selectedWorldBounds,
      anchor: displayAnchor,
      displayScale,
      displayBounds,
      inverse: "sourceWorldPosition = displayPosition / displayScale + anchor",
      preservesAspectAndOrientation: true
    }
  },
  statusRecommendations: [
    { id: "slide1-atmel-active", status: "PARTIAL", reason: "Expose this source-complete assembly as a non-race visit; do not claim playable until collision and the incomplete original input path are handled honestly." },
    { id: "slide1-stockroom-revision", status: "PIPELINE", reason: "Distinct compact binary revision, but no located loader or behavior binds it." },
    { id: "slide1a-legacy-active", status: "PIPELINE", reason: "An older CLBallZ loader exists, but its Ball implementation lacks the later source-backed spawn/physics assembly." },
    { id: "level-slides", status: "PIPELINE", reason: "Closed multi-piece authored geometry with no located host behavior." },
    { id: "level-steps", status: "PIPELINE", reason: "Small textured step assembly with no located host behavior." },
    { id: "slide-bump", status: "PIPELINE", reason: "Unhosted geometry experiment; topology contains open/non-manifold edges." },
    { id: "slide-bump-gridtex", status: "PIPELINE", reason: "Same bounds and triangle count as Slide Bump but a distinct UV/vertex revision with embedded EarthGri.bmp evidence." },
    { id: "slide-long1", status: "ALIAS", reason: "Byte-identical to recovered BallZ 2011 Level1; no additional scene record or mode." },
    { id: "ballz-track1", status: "PIPELINE", reason: "A static CLBallZScene host exists, but input/update are empty and its 100 rings are explicitly placeholders." },
    { id: "pipe1", status: "COMPONENT_OF_PARTIAL_FLIGHTX", reason: "All copies are the already-integrated FlightX pipe; no distinct BallZ pipe host was found." }
  ],
  relatedVariantEvidence: [
    {
      ids: ["slide-bump", "slide-bump-gridtex"],
      exact: "Bounds and triangle count match exactly, but vertex counts, binary hashes, geometry hashes and UV cardinality differ.",
      inference: "GridTex is a deliberate remap/re-export of the same bump form, not a byte-identical duplicate."
    }
  ],
  assets,
  selectedCompanion: ball
};

writeFileSync(outputPath, JSON.stringify(result));
console.log(`wrote ${outputPath}`);
for (const asset of assets) {
  console.log(`${asset.id}: ${asset.vertexCount}v ${asset.triangleCount}t; ${asset.bounds.size.join(" x ")}; ${asset.sha256.slice(0, 12)}`);
}
