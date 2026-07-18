// Deterministic audit/converter for the distinct 2008-era CLCubX actor lineage.
//
// Scope:
//   - Media/CubXActor/CubXGet2..8.tva
//   - Media/CubXActor/CubXRot1..7.tva
//   - Media/CubXActor/CubXOpen.tva and CubeOpensolo.tva
//   - the companion CubXMesh/CubXBtn TVMs needed to map click geometry
//   - the surviving CLCubX and CLCubXScene host calls
//
// This writes evidence only. It does not integrate the lineage into the web UI.
// Usage: node tools/convert-cubx-actor-lineage.mjs

import { createHash } from "node:crypto";
import { existsSync, readdirSync, readFileSync, statSync, writeFileSync } from "node:fs";
import { basename, dirname, extname, join, relative } from "node:path";
import { fileURLToPath } from "node:url";

const here = dirname(fileURLToPath(import.meta.url));
const webRoot = join(here, "..");
const root = join(webRoot, "..");
const backupRoot = join(root, "Yanik C++ BCKUP");
const actorRoot = join(backupRoot, "Media", "CubXActor");
const outputPath = join(webRoot, "src", "legacy", "cubx-actor-lineage.json");
const inspectionGeometryOutputPath = join(webRoot, "src", "legacy", "cubx-actor-inspection-geometry.json");
const primaryCubXSource = join(root, "AtmelCubx", "AtmelCubx", "CubX.cpp");
const primarySceneSource = join(root, "AtmelCubx", "AtmelCubx", "CubXScene.cpp");
const alternateAreaSource = join(backupRoot, "AtmelCubx 1", "Area.cpp");

const tvaDefinitions = [
  ...Array.from({ length: 7 }, (_, index) => ({
    id: `get-${index + 2}`,
    family: "host-loaded-get",
    filename: `CubXGet${index + 2}.tva`,
    filenameBoxLabel: `Box${String(index + 2).padStart(2, "0")}`,
    hostActorSlot: index
  })),
  ...Array.from({ length: 7 }, (_, index) => ({
    id: `rot-${index + 1}`,
    family: "unreferenced-rotation-prototype",
    filename: `CubXRot${index + 1}.tva`,
    prototypeFor: `CubXGet${index + 2}.tva`
  })),
  { id: "open-full", family: "host-loaded-open", filename: "CubXOpen.tva" },
  { id: "open-solo", family: "unreferenced-open-component", filename: "CubeOpensolo.tva" }
];

const actorGeometryPayloads = new Map();
const companionGeometryAssets = new Map();

const round = (value, places = 9) => {
  const factor = 10 ** places;
  return Math.round(value * factor) / factor;
};
const slash = (value) => value.replaceAll("\\", "/");
const relativeToRoot = (value) => slash(relative(root, value));
const sha256 = (value) => createHash("sha256").update(value).digest("hex").toUpperCase();

function assert(condition, message) {
  if (!condition) throw new Error(message);
}

function walk(directory) {
  if (!existsSync(directory)) return [];
  const files = [];
  for (const entry of readdirSync(directory, { withFileTypes: true })) {
    const path = join(directory, entry.name);
    if (entry.isDirectory()) files.push(...walk(path));
    else if (entry.isFile()) files.push(path);
  }
  return files;
}

function fileRecord(path) {
  const data = readFileSync(path);
  return { path: relativeToRoot(path), bytes: statSync(path).size, sha256: sha256(data) };
}

function groupByHash(records) {
  const groups = new Map();
  for (const record of records) {
    const paths = groups.get(record.sha256) ?? [];
    paths.push(record.path);
    groups.set(record.sha256, paths);
  }
  return [...groups.entries()].map(([hash, paths]) => ({ sha256: hash, count: paths.length, paths }));
}

function chunkStream(data, endTag) {
  const chunkStart = data.readUInt32LE(12);
  assert(chunkStart === 16, `Unsupported chunk start ${chunkStart}.`);
  const chunks = [];
  let offset = chunkStart;
  while (offset <= data.length - 8) {
    const tag = data.toString("latin1", offset, offset + 4);
    const size = data.readUInt32LE(offset + 4);
    assert(/^[A-Z0-9! ]{4}$/.test(tag), `Invalid chunk tag at ${offset}.`);
    assert(offset + 8 + size <= data.length, `${tag} chunk at ${offset} overruns the file.`);
    chunks.push({ tag, offset, start: offset + 8, size, end: offset + 8 + size });
    offset += 8 + size;
    if (tag === endTag) break;
  }
  assert(chunks.at(-1)?.tag === endTag, `Chunk stream has no ${endTag}.`);
  return chunks;
}

function fixedString(data, offset, length) {
  return data.toString("latin1", offset, offset + length).split("\0", 1)[0];
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
    min: min.map((value) => round(value)),
    max: max.map((value) => round(value)),
    center: max.map((value, axis) => round((value + min[axis]) / 2)),
    size: max.map((value, axis) => round(value - min[axis]))
  };
}

function transformPositions(positions, matrix) {
  const transformed = [];
  for (let index = 0; index < positions.length; index += 3) {
    const x = positions[index];
    const y = positions[index + 1];
    const z = positions[index + 2];
    transformed.push(
      x * matrix[0] + y * matrix[4] + z * matrix[8] + matrix[12],
      x * matrix[1] + y * matrix[5] + z * matrix[9] + matrix[13],
      x * matrix[2] + y * matrix[6] + z * matrix[10] + matrix[14]
    );
  }
  return transformed;
}

function readFloatRecords(data, offset, count, width) {
  return Array.from({ length: count }, (_, record) =>
    Array.from({ length: width }, (_, field) => data.readFloatLE(offset + (record * width + field) * 4))
  );
}

function componentSpan(records, start, count) {
  return Array.from({ length: count }, (_, component) => {
    const values = records.map((record) => record[start + component]);
    return Math.max(...values) - Math.min(...values);
  });
}

function quaternionDistance(left, right) {
  const leftNorm = Math.hypot(...left);
  const rightNorm = Math.hypot(...right);
  const dot = left.reduce((sum, value, index) => sum + value * right[index], 0);
  return 1 - Math.abs(dot / (leftNorm * rightNorm));
}

function quaternionMetrics(rotations) {
  const first = rotations[0].slice(1);
  let maximumDistanceFromFirst = 0;
  let maximumNormError = 0;
  for (const rotation of rotations) {
    const quaternion = rotation.slice(1);
    maximumDistanceFromFirst = Math.max(maximumDistanceFromFirst, quaternionDistance(first, quaternion));
    maximumNormError = Math.max(maximumNormError, Math.abs(Math.hypot(...quaternion) - 1));
  }
  return { maximumDistanceFromFirst, maximumNormError };
}

function parseHierarchy(data, chunk, expectedCount) {
  const recordBytes = 464;
  assert(chunk.size % recordBytes === 0, "MHI2 has a partial hierarchy record.");
  const count = chunk.size / recordBytes;
  assert(count === expectedCount, `MHI2 count ${count} differs from ASTA ${expectedCount}.`);
  return Array.from({ length: count }, (_, id) => {
    const offset = chunk.start + id * recordBytes;
    const meshDataIndex = data.readInt32LE(offset + 268);
    return {
      id,
      parent: data.readInt32LE(offset),
      firstChild: data.readInt32LE(offset + 4),
      nextSibling: data.readInt32LE(offset + 8),
      name: fixedString(data, offset + 12, 256),
      meshDataIndex,
      initialWorldMatrix: Array.from({ length: 16 }, (_, index) => data.readFloatLE(offset + 272 + index * 4)),
      initialLocalMatrix: Array.from({ length: 16 }, (_, index) => data.readFloatLE(offset + 336 + index * 4)),
      rawTailSha256: sha256(data.subarray(offset + 400, offset + 464))
    };
  });
}

function nearestNamedOwner(hierarchy, node) {
  let current = node;
  while (current) {
    if (current.name) return current.name;
    current = current.parent >= 0 ? hierarchy[current.parent] : null;
  }
  return "";
}

function parseMamd(data, chunk, ordinal, hierarchy) {
  const prefixBytes = 124;
  const vertexFormat = data.readUInt32LE(chunk.start + 64);
  const vertexStride = data.readUInt32LE(chunk.start + 68);
  const vertexCount = data.readUInt32LE(chunk.start + 72);
  const triangleCount = data.readUInt32LE(chunk.start + 76);
  const storedOrdinal = data.readUInt32LE(chunk.start + 80);
  const groupCount = data.readUInt32LE(chunk.start + 84);
  assert(storedOrdinal === ordinal, `MAMD ordinal ${storedOrdinal} differs from stream order ${ordinal}.`);
  assert(vertexStride === 32, `Unsupported MAMD vertex stride ${vertexStride}.`);
  const expectedSize = prefixBytes + vertexCount * vertexStride + triangleCount * 22;
  assert(expectedSize === chunk.size, `MAMD ${ordinal} counted payload does not fill its chunk.`);

  const vertexStart = chunk.start + prefixBytes;
  const positions = [];
  const normals = [];
  const uvs = [];
  for (let vertex = 0; vertex < vertexCount; vertex += 1) {
    const offset = vertexStart + vertex * vertexStride;
    for (let axis = 0; axis < 3; axis += 1) positions.push(data.readFloatLE(offset + axis * 4));
    for (let axis = 0; axis < 3; axis += 1) normals.push(data.readFloatLE(offset + 12 + axis * 4));
    uvs.push(data.readFloatLE(offset + 24), data.readFloatLE(offset + 28));
  }
  const indexStart = vertexStart + vertexCount * vertexStride;
  const indices = Array.from({ length: triangleCount * 3 }, (_, index) => data.readUInt16LE(indexStart + index * 2));
  assert(indices.every((index) => index < vertexCount), `MAMD ${ordinal} has an out-of-range vertex index.`);
  const materialStart = indexStart + triangleCount * 6;
  const materialIndices = Array.from({ length: triangleCount }, (_, face) => data.readUInt32LE(materialStart + face * 4));
  const remapStart = materialStart + triangleCount * 4;
  const sourceVertexRemap = Array.from({ length: triangleCount * 3 }, (_, index) => data.readUInt32LE(remapStart + index * 4));
  const node = hierarchy.find((candidate) => candidate.meshDataIndex === ordinal);
  assert(node, `No hierarchy node owns MAMD ${ordinal}.`);
  const ownerName = nearestNamedOwner(hierarchy, node);
  const localBounds = boundsOf(positions);
  const initialWorldBounds = boundsOf(transformPositions(positions, node.initialWorldMatrix));
  const geometryPayloadSha256 = sha256(data.subarray(vertexStart, chunk.end));
  if (!actorGeometryPayloads.has(geometryPayloadSha256)) {
    actorGeometryPayloads.set(geometryPayloadSha256, {
      geometryPayloadSha256,
      vertexStride,
      vertexCount,
      triangleCount,
      localBounds,
      positions,
      normals,
      uvs,
      indices,
      materialIndices,
      sourceVertexRemap
    });
  }
  return {
    ordinal,
    hierarchyNodeId: node.id,
    hierarchyNodeName: node.name,
    ownerName,
    vertexFormat,
    vertexStride,
    vertexCount,
    triangleCount,
    groupCount,
    localBounds,
    initialWorldBounds,
    uniqueMaterialIndices: [...new Set(materialIndices)].sort((a, b) => a - b),
    maximumVertexIndex: Math.max(...indices),
    maximumSourceVertexRemap: Math.max(...sourceVertexRemap),
    geometryPayloadSha256,
    positionsSha256: sha256(data.subarray(vertexStart, vertexStart + vertexCount * vertexStride)),
    indicesSha256: sha256(data.subarray(indexStart, materialStart)),
    rawChunkSha256: sha256(data.subarray(chunk.offset, chunk.end))
  };
}

function parseRanges(data, chunk, expectedCount) {
  const recordBytes = 280;
  assert(chunk.size % recordBytes === 0, "MANS has a partial animation record.");
  const count = chunk.size / recordBytes;
  assert(count === expectedCount, `MANS count ${count} differs from ASTA ${expectedCount}.`);
  return Array.from({ length: count }, (_, id) => {
    const offset = chunk.start + id * recordBytes;
    const startFrame = data.readFloatLE(offset + 260);
    const endFrame = data.readFloatLE(offset + 264);
    const framesPerSecond = data.readFloatLE(offset + 268);
    return {
      id,
      name: fixedString(data, offset, 256),
      sourceAnimation: data.readInt32LE(offset + 256),
      startFrame,
      endFrame,
      framesPerSecond,
      durationSeconds: (endFrame - startFrame) / framesPerSecond,
      rawRangeField: data.readInt32LE(offset + 272),
      trackChunkCount: data.readUInt32LE(offset + 276)
    };
  });
}

function parseTrack(data, chunk, hierarchy) {
  const nodeId = data.readInt32LE(chunk.start);
  const sourceAnimation = data.readInt32LE(chunk.start + 4);
  const positionCount = data.readUInt32LE(chunk.start + 8);
  const rotationCount = data.readUInt32LE(chunk.start + 12);
  const scaleCount = data.readUInt32LE(chunk.start + 16);
  const rawFlags = data.readUInt32LE(chunk.start + 20);
  const positionStart = chunk.start + 24;
  const rotationStart = positionStart + positionCount * 16;
  const scaleStart = rotationStart + rotationCount * 20;
  assert(scaleStart + scaleCount * 16 === chunk.end, `MANI node ${nodeId} does not fill its chunk.`);
  assert(nodeId >= 0 && nodeId < hierarchy.length, `MANI node ${nodeId} is outside MHI2.`);
  const positions = readFloatRecords(data, positionStart, positionCount, 4);
  const rotations = readFloatRecords(data, rotationStart, rotationCount, 5);
  const scales = readFloatRecords(data, scaleStart, scaleCount, 4);
  for (const records of [positions, rotations, scales]) {
    for (let index = 1; index < records.length; index += 1) {
      assert(records[index][0] >= records[index - 1][0], `MANI node ${nodeId} has decreasing keys.`);
    }
  }
  const positionSpan = componentSpan(positions, 1, 3);
  const scaleSpan = componentSpan(scales, 1, 3);
  const quaternion = quaternionMetrics(rotations);
  assert(quaternion.maximumNormError < 0.000002, `MANI node ${nodeId} quaternion norm drifted.`);
  const substantive = {
    position: Math.hypot(...positionSpan) > 0.001,
    rotation: quaternion.maximumDistanceFromFirst > 0.000001,
    scale: Math.hypot(...scaleSpan) > 0.0001
  };
  return {
    nodeId,
    nodeName: hierarchy[nodeId].name,
    sourceAnimation,
    rawFlags,
    counts: { position: positionCount, rotation: rotationCount, scale: scaleCount },
    frameBounds: {
      position: [positions[0][0], positions.at(-1)[0]],
      rotation: [rotations[0][0], rotations.at(-1)[0]],
      scale: [scales[0][0], scales.at(-1)[0]]
    },
    positionSpan: positionSpan.map((value) => round(value)),
    scaleSpan: scaleSpan.map((value) => round(value)),
    maximumQuaternionDistanceFromFirst: round(quaternion.maximumDistanceFromFirst, 12),
    maximumQuaternionNormError: round(quaternion.maximumNormError, 12),
    substantive,
    firstPosition: positions[0],
    lastPosition: positions.at(-1),
    firstRotation: rotations[0],
    lastRotation: rotations.at(-1),
    firstScale: scales[0],
    lastScale: scales.at(-1),
    rawChunkSha256: sha256(data.subarray(chunk.offset, chunk.end)),
    positions,
    rotations,
    scales
  };
}

function plateauStartFrame(rotations) {
  const terminal = rotations.at(-1).slice(1);
  let index = rotations.length - 2;
  while (index >= 0 && quaternionDistance(rotations[index].slice(1), terminal) <= 0.000000001) index -= 1;
  return rotations[Math.min(index + 1, rotations.length - 1)][0];
}

function decodeTva(definition) {
  const path = join(actorRoot, definition.filename);
  assert(existsSync(path), `Missing ${definition.filename}.`);
  const data = readFileSync(path);
  const chunks = chunkStream(data, "AEND");
  const chunk = (tag) => chunks.find((candidate) => candidate.tag === tag);
  for (const tag of ["MSTR", "ASTA", "MAG3", "MHI2", "MANS", "AEND"]) {
    assert(chunk(tag), `${definition.filename} is missing ${tag}.`);
  }
  const asta = chunk("ASTA");
  assert(asta.size === 48, `${definition.filename} ASTA size changed.`);
  const astaWords = Array.from({ length: 12 }, (_, index) => data.readUInt32LE(asta.start + index * 4));
  const animationRangeCount = astaWords[4];
  const hierarchyNodeCount = astaWords[8];
  const hierarchy = parseHierarchy(data, chunk("MHI2"), hierarchyNodeCount);
  const mamdChunks = chunks.filter((candidate) => candidate.tag === "MAMD");
  const geometry = mamdChunks.map((candidate, index) => parseMamd(data, candidate, index, hierarchy));
  const ranges = parseRanges(data, chunk("MANS"), animationRangeCount);
  const tracks = chunks.filter((candidate) => candidate.tag === "MANI").map((candidate) => parseTrack(data, candidate, hierarchy));
  assert(ranges.every((range) => range.trackChunkCount === tracks.length), `${definition.filename} MANS/MANI count changed.`);
  assert(astaWords[1] === geometry.reduce((sum, item) => sum + item.vertexCount, 0), `${definition.filename} ASTA vertex total changed.`);
  assert(astaWords[2] === geometry.reduce((sum, item) => sum + item.triangleCount, 0), `${definition.filename} ASTA triangle total changed.`);
  assert(astaWords[3] === geometry.length && astaWords[7] === geometry.length, `${definition.filename} ASTA geometry count changed.`);
  assert(hierarchy.filter((node) => node.meshDataIndex >= 0).length === geometry.length, `${definition.filename} hierarchy/geometry ownership changed.`);
  const substantiveTracks = tracks.filter((track) => Object.values(track.substantive).some(Boolean));
  const motionCompletedAtFrame = substantiveTracks.length > 0
    ? Math.max(...substantiveTracks.filter((track) => track.substantive.rotation).map((track) => plateauStartFrame(track.rotations)))
    : ranges[0].startFrame;
  const range = ranges[0];
  return {
    ...definition,
    source: relativeToRoot(path),
    bytes: data.length,
    sha256: sha256(data),
    header: {
      magicHex: data.subarray(0, 4).toString("hex").toUpperCase(),
      rawWord1: data.readUInt32LE(4),
      rawWord2: data.readUInt32LE(8),
      chunkStart: data.readUInt32LE(12)
    },
    structure: {
      chunkTagsInOrder: chunks.map((candidate) => candidate.tag),
      chunks: chunks.map(({ tag, offset, size }) => ({ tag, offset, size })),
      astaWords,
      animationRangeCount,
      hierarchyNodeCount,
      geometryChunkCount: geometry.length,
      animationTrackCount: tracks.length,
      postAendBytes: data.length - chunk("AEND").end,
      postAendSha256: sha256(data.subarray(chunk("AEND").end))
    },
    ranges,
    timing: {
      rangeStartFrame: range.startFrame,
      rangeEndFrame: range.endFrame,
      framesPerSecond: range.framesPerSecond,
      durationSeconds: range.durationSeconds,
      motionCompletedAtFrame,
      motionDurationSeconds: (motionCompletedAtFrame - range.startFrame) / range.framesPerSecond,
      terminalHoldFrames: range.endFrame - motionCompletedAtFrame,
      terminalHoldSeconds: (range.endFrame - motionCompletedAtFrame) / range.framesPerSecond
    },
    hierarchy,
    geometryTotals: {
      vertices: geometry.reduce((sum, item) => sum + item.vertexCount, 0),
      triangles: geometry.reduce((sum, item) => sum + item.triangleCount, 0),
      uniqueMaterialIndices: [...new Set(geometry.flatMap((item) => item.uniqueMaterialIndices))].sort((a, b) => a - b),
      rawMag3Bytes: chunk("MAG3").size,
      rawMag3Sha256: sha256(data.subarray(chunk("MAG3").start, chunk("MAG3").end))
    },
    geometry,
    trackSummaries: tracks.map(({ positions, rotations, scales, ...summary }) => summary),
    decodedSubstantiveTracks: substantiveTracks.map((track) => ({
      nodeId: track.nodeId,
      nodeName: track.nodeName,
      sourceAnimation: track.sourceAnimation,
      counts: track.counts,
      substantive: track.substantive,
      positions: track.positions,
      rotations: track.rotations,
      scales: track.scales,
      rawChunkSha256: track.rawChunkSha256
    }))
  };
}

function decodeTvm(filename) {
  const path = join(actorRoot, filename);
  const data = readFileSync(path);
  const chunks = chunkStream(data, "MEND");
  const chunk = (tag) => chunks.find((candidate) => candidate.tag === tag);
  const msta = chunk("MSTA");
  const mver = chunk("MVER");
  const indicesChunk = chunk("MI16") ?? chunk("MI32");
  const matt = chunk("MATT");
  const mgro = chunk("MGRO");
  const materialChunk = chunk("MGR4") ?? chunk("MGRP");
  assert(msta && mver && indicesChunk && matt && mgro && materialChunk, `${filename} is missing a geometry chunk.`);
  const vertexStride = data.readUInt32LE(msta.start + 24);
  assert(vertexStride === 32, `${filename} has unsupported stride ${vertexStride}.`);
  const vertexCount = mver.size / vertexStride;
  const positions = [];
  const normals = [];
  const uvs = [];
  for (let vertex = 0; vertex < vertexCount; vertex += 1) {
    const offset = mver.start + vertex * vertexStride;
    positions.push(data.readFloatLE(offset), data.readFloatLE(offset + 4), data.readFloatLE(offset + 8));
    normals.push(data.readFloatLE(offset + 12), data.readFloatLE(offset + 16), data.readFloatLE(offset + 20));
    uvs.push(data.readFloatLE(offset + 24), data.readFloatLE(offset + 28));
  }
  const indexBytes = indicesChunk.tag === "MI16" ? 2 : 4;
  const indices = Array.from({ length: indicesChunk.size / indexBytes }, (_, index) =>
    indexBytes === 2 ? data.readUInt16LE(indicesChunk.start + index * 2) : data.readUInt32LE(indicesChunk.start + index * 4)
  );
  const triangleCount = indices.length / 3;
  assert(matt.size === triangleCount * 4, `${filename} material assignment count changed.`);
  const materialIndices = Array.from({ length: triangleCount }, (_, face) => data.readUInt32LE(matt.start + face * 4));
  assert(mgro.size % 16 === 0, `${filename} MGRO size changed.`);
  const groups = Array.from({ length: mgro.size / 16 }, (_, index) => {
    const offset = mgro.start + index * 16;
    return {
      vertexCount: data.readUInt32LE(offset),
      vertexStart: data.readUInt32LE(offset + 4),
      triangleCount: data.readUInt32LE(offset + 8),
      triangleStart: data.readUInt32LE(offset + 12)
    };
  });
  companionGeometryAssets.set(filename, {
    filename,
    sourceSha256: sha256(data),
    vertexStride,
    vertexCount,
    triangleCount,
    bounds: boundsOf(positions),
    positions,
    normals,
    uvs,
    indices,
    materialIndices,
    groups
  });
  return {
    filename,
    source: relativeToRoot(path),
    bytes: data.length,
    sha256: sha256(data),
    chunkTagsInOrder: chunks.map((candidate) => candidate.tag),
    vertexStride,
    vertexCount,
    triangleCount,
    bounds: boundsOf(positions),
    materialGroups: mgro.size / 16,
    uniqueMaterialIndices: [...new Set(materialIndices)].sort((a, b) => a - b),
    rawMaterialChunk: { tag: materialChunk.tag, bytes: materialChunk.size, sha256: sha256(data.subarray(materialChunk.start, materialChunk.end)) }
  };
}

function sourceEvidence(path, patterns) {
  const text = readFileSync(path, "latin1");
  const lines = text.split(/\r?\n/);
  return patterns.map(({ pattern, fact }) => {
    const index = lines.findIndex((line) => line.includes(pattern));
    assert(index >= 0, `Source evidence not found: ${pattern}`);
    return { fact, source: relativeToRoot(path), line: index + 1, code: lines[index].trim() };
  });
}

function canonicalBoxLabel(name) {
  const match = /(?:Bo_te|Box)(\d{2})$/.exec(name);
  return match ? `Box${match[1]}` : null;
}

function distance(left, right) {
  return Math.hypot(...left.map((value, axis) => value - right[axis]));
}

const assets = tvaDefinitions.map(decodeTva);
assert(assets.length === 16, "CubXActor TVA census must contain 16 assets.");
assert(new Set(assets.map((asset) => asset.sha256)).size === 16, "CubXActor TVAs are expected to remain byte-distinct.");
assert(assets.every((asset) => asset.ranges.length === 1 && asset.ranges[0].framesPerSecond === 30), "Every CubXActor TVA must expose one 30 fps range.");

const getAssets = assets.filter((asset) => asset.family === "host-loaded-get");
const rotAssets = assets.filter((asset) => asset.family === "unreferenced-rotation-prototype");
const fullOpen = assets.find((asset) => asset.id === "open-full");
const soloOpen = assets.find((asset) => asset.id === "open-solo");
assert(getAssets.every((asset) => asset.decodedSubstantiveTracks.length === 1 && asset.decodedSubstantiveTracks[0].nodeName === "Group01"), "Get actors must animate only Group01.");
assert(rotAssets.every((asset) => asset.decodedSubstantiveTracks.length === 1 && asset.decodedSubstantiveTracks[0].nodeName === "Group01"), "Rot actors must animate only Group01.");
assert(JSON.stringify(fullOpen.decodedSubstantiveTracks.map((track) => track.nodeName).sort()) === JSON.stringify(["Left", "Right", "Top"]), "CubXOpen panel set changed.");
assert(JSON.stringify(soloOpen.decodedSubstantiveTracks.map((track) => track.nodeName).sort()) === JSON.stringify(["Left08", "Right08", "Top08"]), "CubeOpensolo panel set changed.");

const companions = {
  closedMesh: decodeTvm("CubXMesh.tvm"),
  clickMeshes: Array.from({ length: 8 }, (_, index) => decodeTvm(`CubXBtn${index + 1}.tvm`))
};
const referenceRotation = assets.find((asset) => asset.id === "rot-1");
const closedBoxes = referenceRotation.geometry
  .map((geometry) => ({ geometry, boxLabel: canonicalBoxLabel(geometry.ownerName) }))
  .filter((entry) => entry.boxLabel);
assert(closedBoxes.length === 8, "Reference rotation actor must contain eight closed BoxNN cells.");

const clickFlow = companions.clickMeshes.map((button, index) => {
  const ranked = closedBoxes
    .map((entry) => ({ ...entry, centerDistance: distance(button.bounds.center, entry.geometry.initialWorldBounds.center) }))
    .sort((left, right) => left.centerDistance - right.centerDistance);
  assert(ranked[0].centerDistance < 1, `${button.filename} does not spatially match a closed actor cell.`);
  const clickIndex = index + 1;
  const initialized = clickIndex <= 7;
  return {
    clickIndex,
    clickedMesh: button.filename,
    clickedMeshLocalCenter: button.bounds.center,
    exactSpatialBoxLabel: ranked[0].boxLabel,
    exactSpatialBoxCenter: ranked[0].geometry.initialWorldBounds.center,
    centerDistance: round(ranked[0].centerDistance),
    sourceSelectedCube: clickIndex,
    actorArrayIndex: clickIndex - 1,
    actorSlotInitialized: initialized,
    actorFilename: initialized ? `CubXGet${clickIndex + 1}.tva` : null,
    actorFilenameBoxLabel: initialized ? `Box${String(clickIndex + 1).padStart(2, "0")}` : null,
    renderedByHostLoop: initialized,
    result: initialized
      ? "Host plays the separately authored Get actor and later swaps to CubXOpen."
      : "Source indexes the uninitialized eighth actor slot; the render loop also stops at slot 6."
  };
});

const rotationPrototypePairs = rotAssets.map((rotation, index) => {
  const get = getAssets[index];
  const rotationTrack = rotation.decodedSubstantiveTracks[0];
  const getTrack = get.decodedSubstantiveTracks[0];
  return {
    rotationPrototype: rotation.filename,
    hostLoadedGet: get.filename,
    filenameRelationship: `Rot${index + 1} -> Get${index + 2}`,
    terminalQuaternionDistance: round(quaternionDistance(rotationTrack.rotations.at(-1).slice(1), getTrack.rotations.at(-1).slice(1)), 12),
    rotationPrototypeTiming: rotation.timing,
    hostLoadedGetTiming: get.timing
  };
});

const openingPanelPairs = ["Right", "Top", "Left"].map((panel) => {
  const fullTrack = fullOpen.decodedSubstantiveTracks.find((track) => track.nodeName === panel);
  const soloTrack = soloOpen.decodedSubstantiveTracks.find((track) => track.nodeName === `${panel}08`);
  assert(fullTrack && soloTrack, `Missing opening track pair ${panel}.`);
  let maximumDifferenceFirst51Keys = 0;
  for (let key = 0; key < soloTrack.rotations.length; key += 1) {
    for (let field = 0; field < 5; field += 1) {
      maximumDifferenceFirst51Keys = Math.max(maximumDifferenceFirst51Keys, Math.abs(fullTrack.rotations[key][field] - soloTrack.rotations[key][field]));
    }
  }
  const fullGeometry = fullOpen.geometry.find((geometry) => geometry.ownerName === panel);
  const soloGeometry = soloOpen.geometry.find((geometry) => geometry.ownerName === `${panel}08`);
  assert(fullGeometry && soloGeometry, `Missing opening geometry pair ${panel}.`);
  return {
    fullPanel: panel,
    soloPanel: `${panel}08`,
    fullRotationKeyCount: fullTrack.rotations.length,
    soloRotationKeyCount: soloTrack.rotations.length,
    maximumDifferenceFirst51Keys,
    fullTerminalHoldKey: fullTrack.rotations.at(-1),
    panelGeometryPayloadIdentical: fullGeometry.geometryPayloadSha256 === soloGeometry.geometryPayloadSha256,
    geometryPayloadSha256: fullGeometry.geometryPayloadSha256
  };
});

const openClosedLabels = fullOpen.hierarchy.map((node) => canonicalBoxLabel(node.name)).filter(Boolean).sort();
assert(!openClosedLabels.includes("Box05") && openClosedLabels.length === 7, "CubXOpen must replace Box05 with the Cube panel assembly.");
assert(soloOpen.hierarchy.some((node) => node.name === "Box08"), "CubeOpensolo must retain its Box08 label.");

const allFiles = walk(root);
const cxxFiles = allFiles.filter((path) => [".cpp", ".h"].includes(extname(path).toLowerCase()));
function literalSourceReferences(pattern) {
  const references = [];
  for (const path of cxxFiles) {
    const lines = readFileSync(path, "latin1").split(/\r?\n/);
    for (let index = 0; index < lines.length; index += 1) {
      if (lines[index].includes(pattern)) references.push({ source: relativeToRoot(path), line: index + 1, code: lines[index].trim() });
    }
  }
  return references;
}
const implementationSources = allFiles
  .filter((path) => basename(path).toLowerCase() === "cubx.cpp")
  .filter((path) => readFileSync(path, "latin1").includes("CLCubX::CLCubX(float Size"))
  .map(fileRecord);
const sceneSources = allFiles
  .filter((path) => basename(path).toLowerCase() === "cubxscene.cpp")
  .filter((path) => readFileSync(path, "latin1").includes("CLCubXScene::CubXClickedAction"))
  .map(fileRecord);
assert(implementationSources.length === 4 && groupByHash(implementationSources).length === 1, "Expected four byte-identical CLCubX implementations.");
assert(sceneSources.length === 4 && groupByHash(sceneSources).length === 1, "Expected four byte-identical CLCubXScene implementations.");

const cubXEvidence = sourceEvidence(primaryCubXSource, [
  { pattern: "pMatFactory->SetAmbient(iStdMaterial, 0.1, 0.1, 0.1, 1);", fact: "Actor material ambient is explicitly overridden." },
  { pattern: "pMatFactory->SetDiffuse(iStdMaterial, 0.8, 0.8, 0.8, 1);", fact: "Actor material diffuse is explicitly overridden." },
  { pattern: "pMatFactory->SetSpecular(iStdMaterial, 0.2, 0.2, 0.2, 1);", fact: "Actor material specular is explicitly overridden." },
  { pattern: "pMatFactory->SetPower(iStdMaterial, 20);", fact: "Actor material power is explicitly overridden." },
  { pattern: "CubXMesh->LoadTVM(\"Media\\\\CubX\\\\CubXMesh.tvm\",true,true);", fact: "Closed mesh is loaded from the missing Media/CubX directory name." },
  { pattern: "CubXMesh->SetPosition(Pos.x, Pos.y, Pos.z);", fact: "Closed mesh uses the constructor position." },
  { pattern: "//CubXMesh->SetScale(Size,Size,Size);", fact: "Constructor Size scaling is commented out." },
  { pattern: "CubXOpenActor->LoadTVA(\"Media\\\\CubX\\\\CubXOpen.tva\", true, false);", fact: "The full open actor is the only open TVA loaded by CLCubX." },
  { pattern: "CubXOpenActor->SetMaterial(iStdMaterial);", fact: "Full open actor receives the source StdMat override." },
  { pattern: "for(i=1; i<8;i++)", fact: "Only seven rotation actor slots are initialized." },
  { pattern: "sprintf(BufferTVAName, \"Media\\\\CubX\\\\CubXGet%d.tva\", i+1);", fact: "Slots 0..6 load Get2..Get8." },
  { pattern: "CubXRotActor[i-1]->SetMaterial(iStdMaterial);", fact: "Every initialized Get actor receives the source StdMat override." },
  { pattern: "sprintf(String, \"Media\\\\CubX\\\\CubXBtn%d.tvm\", i+1);", fact: "Eight click meshes are loaded from the missing Media/CubX directory name." },
  { pattern: "CubXRotActor[iSelectedCube-1]->PlayAnimation(1);", fact: "Selection directly indexes selectedCube-1 and plays forward." },
  { pattern: "if(CubXRotActor[iSelectedCube-1]->GetKeyFrame()    >  99)", fact: "Selection waits for the authored frame-100 terminal hold instead of IsAnimationFinished." },
  { pattern: "CubXOpenActor->PlayAnimation(1);", fact: "Opening plays CubXOpen forward." },
  { pattern: "if(CubXOpenActor->IsAnimationFinished())", fact: "Opening changes to OPENED only after the full 0..100 range." },
  { pattern: "pCubXBtn[i]->Enable(true);", fact: "Both EnableCubXButtons and the misnamed DisableCubXButtons enable click meshes." },
  { pattern: "for(i=0; i<7; i++)", fact: "Rendering covers only actor slots 0..6." }
]);
const sceneEvidence = sourceEvidence(primarySceneSource, [
  { pattern: "clCubX = new CLCubX(4, NULL, cTV_3DVECTOR(-400,500,0), true);", fact: "Scene constructs CLCubX at the exact host position; the Size argument is ignored by commented scaling." },
  { pattern: "if(iMeshClickedIndex == clCubX->pCubXBtn[0]->iMeshIndex)", fact: "Click mesh 1 enters the explicit selection chain." },
  { pattern: "clCubX->iSelectedCube = 1;", fact: "Click mesh 1 selects integer 1." },
  { pattern: "else if(iMeshClickedIndex == clCubX->pCubXBtn[7]->iMeshIndex)", fact: "Click mesh 8 is accepted despite the missing actor slot." },
  { pattern: "clCubX->iSelectedCube = 8;", fact: "Click mesh 8 selects integer 8." }
]);
const alternateClickEvidence = sourceEvidence(alternateAreaSource, [
  { pattern: "if(iMeshClickedIndex == clCubX->pCubXBtn[0]->iMeshIndex)", fact: "The alternate direct Area host repeats click mesh 1 selection." },
  { pattern: "else if(iMeshClickedIndex == clCubX->pCubXBtn[7]->iMeshIndex)", fact: "The alternate direct Area host also accepts click mesh 8." }
]);
const hostReferenceAudit = {
  formattedGetLoader: literalSourceReferences("CubXGet%d.tva"),
  fullOpenLoader: literalSourceReferences("CubXOpen.tva"),
  numberedRotAssetLiterals: Array.from({ length: 7 }, (_, index) => literalSourceReferences(`CubXRot${index + 1}.tva`)).flat(),
  soloOpenAssetLiterals: literalSourceReferences("CubeOpensolo.tva")
};
assert(hostReferenceAudit.formattedGetLoader.length === 4, "CubXGet formatted-loader source census changed.");
assert(hostReferenceAudit.fullOpenLoader.length === 4, "CubXOpen source census changed.");
assert(hostReferenceAudit.numberedRotAssetLiterals.length === 0, "A numbered CubXRot asset gained a host reference.");
assert(hostReferenceAudit.soloOpenAssetLiterals.length === 0, "CubeOpensolo gained a host reference.");

const expectedSourceDirectory = join(backupRoot, "Media", "CubX");
const assetPathEvidence = {
  hardCodedDirectory: "Media/CubX",
  hardCodedDirectoryExistsInArchive: existsSync(expectedSourceDirectory),
  survivingDirectory: relativeToRoot(actorRoot),
  survivingDirectoryExists: existsSync(actorRoot),
  requiredRemap: "Media/CubX -> Yanik C++ BCKUP/Media/CubXActor",
  status: "source-path-mismatch"
};
assert(!assetPathEvidence.hardCodedDirectoryExistsInArchive && assetPathEvidence.survivingDirectoryExists, "CubX source-path evidence changed.");

const tvaRecords = assets.map(({ source: path, bytes, sha256: hash }) => ({ path, bytes, sha256: hash }));
const output = {
  schema: "graphysx.cubx-actor-lineage-audit/v1",
  generated: new Date().toISOString(),
  decoder: {
    format: "TrueVision3D 6.5 TVA chunk stream",
    exactLayouts: {
      MHI2: "464-byte node records: links, 256-byte name, signed mesh-data index, initial world/local 4x4 matrices, retained tail hash",
      MAMD: "124-byte header; 32-byte position/normal/UV vertices; uint16 triangle indices; uint32 material-per-face list; uint32 source-vertex remap",
      MANS: "280-byte single-range record",
      MANI: "24-byte counted header; float32 position/quaternion/scale key arrays"
    },
    coordinateBoundary: "Positions, matrices and quaternions remain in exact TV3D source order. No browser handedness or matrix convention conversion is asserted.",
    materialBoundary: "MAG3 is retained by size/hash and every decoded MAMD face uses embedded material index 0. Runtime appearance is source-backed by the stronger CLCubX StdMat override."
  },
  assetCensus: {
    totalTvaAssets: assets.length,
    uniqueTvaHashes: new Set(assets.map((asset) => asset.sha256)).size,
    tvaRecords,
    hashGroups: groupByHash(tvaRecords),
    assetPathEvidence
  },
  companionClickGeometry: companions,
  clickFlow,
  rotationPrototypePairs,
  openingLineage: {
    fullActor: fullOpen.filename,
    soloComponent: soloOpen.filename,
    fullActorClosedBoxLabels: openClosedLabels,
    fullActorMissingClosedBoxLabel: "Box05",
    fullActorAssemblyLabels: ["Cube", "Right", "Top", "Left"],
    soloAssemblyLabels: ["Box08", "Right08", "Top08", "Left08"],
    panelPairs: openingPanelPairs,
    interpretation: "The three panel payloads are byte-identical after the MAMD header, and their first 51 animation keys differ by at most float32 export noise. However the full actor replaces Box05 while the solo source names the same panel lineage Box08, so BoxNN is not a stable semantic identifier across these exports."
  },
  materialAndHostTransform: {
    actorPosition: [-400, 500, 0],
    constructorSizeArgument: 4,
    scaleStatus: "not-applied-source-commented-out",
    closedMeshScale: [1, 1, 1],
    actorScale: [1, 1, 1],
    materialOverride: {
      name: "StdMat",
      ambient: [0.1, 0.1, 0.1, 1],
      diffuse: [0.8, 0.8, 0.8, 1],
      specular: [0.2, 0.2, 0.2, 1],
      power: 20,
      emissive: [0, 0, 0, 1],
      textureOverride: null,
      appliesTo: ["CubXMesh", "CubXOpenActor", "CubXGet2..8 actors"]
    }
  },
  hostSourceAudit: {
    implementationSources,
    implementationHashGroups: groupByHash(implementationSources),
    sceneSources,
    sceneHashGroups: groupByHash(sceneSources),
    cubXEvidence,
    sceneEvidence,
    alternateClickEvidence,
    hostReferenceAudit,
    defects: [
      "Seven actor slots are initialized from Get2..Get8, but eight click indices are accepted and selection 8 indexes uninitialized slot 7.",
      "The render loop covers only slots 0..6.",
      "DisableCubXButtons enables the click meshes instead of disabling them.",
      "CLOSING, DESELECTING, FOCUSED, OPENED and DEFOCUSING contain no transition behavior; the internal menu registration block is commented out.",
      "Hard-coded Media/CubX asset paths do not exist in the surviving tree; the files live under Media/CubXActor."
    ]
  },
  assets,
  mappingAssessment: {
    resolvesMissingCubzClickToBoxMapping: false,
    exactNewEvidence: [
      "Click mesh 1..8 local centers and their spatial overlap with the eight closed BoxNN cells.",
      "Click index 1..7 selects Get2..Get8; click 8 selects an uninitialized actor slot.",
      "Get2..8 and Rot1..7 each animate only Group01; CubXOpen animates Right/Top/Left; CubeOpensolo animates Right08/Top08/Left08.",
      "CubXOpen omits closed Box05 and supplies Cube/Right/Top/Left, while CubeOpensolo calls its homologous panel lineage Box08/Right08/Top08/Left08."
    ],
    whyNotResolved: "The click meshes spatially map to Box05, Box04, Box03, Box02, Box08, Box06, Box07, Box01, but the host maps click indices 1..7 to filenames Get2..Get8 and has no valid click-8 actor. The full and solo opening exports also disagree between Box05 and Box08 naming. Those contradictions prevent a defensible semantic click-index-to-BoxNN mapping for CubZ.",
    safeReuse: "Use exact geometry, transforms, timing and per-file animation data only after repairing path/slot defects. Keep click index, spatial BoxNN, actor filename, and opening-assembly label as separate fields rather than collapsing them into one invented mapping."
  },
  evidenceBoundary: {
    exact: [
      "All hashes, chunk counts, hierarchy names/links/mesh indices/matrices, MAMD geometry summaries, 30 fps ranges and MANI keys.",
      "Companion CubXMesh/CubXBtn bounds and exact source host calls/material/position constants.",
      "The source defects and the separate unused Rot/CubeOpensolo asset families."
    ],
    derived: [
      "Button-to-BoxNN spatial association is nearest exact local-center overlap with less than one source unit of export drift.",
      "Rotation-prototype pairing follows the exact RotN/GetN+1 filenames and near-equal terminal quaternions.",
      "Motion-complete time is the first key of the terminal quaternion plateau; the remaining authored frames are a terminal hold."
    ],
    notClaimed: [
      "TV3D-to-web handedness, quaternion application order, engine interpolation or visual actor-swap continuity.",
      "That a BoxNN modeling label equals a user-facing click number or CubZ selection number.",
      "That unreferenced CubXRot/CubeOpensolo assets were executed by the surviving CLCubX host."
    ]
  }
};

writeFileSync(outputPath, `${JSON.stringify(output, null, 2)}\n`);
const inspectionGeometryOutput = {
  schema: "graphysx.cubx-actor-inspection-geometry/v1",
  generated: output.generated,
  coordinateBoundary: "Exact TV3D source-order geometry. No handedness conversion, recentering or display scaling is baked into these arrays.",
  actorGeometryPayloads: [...actorGeometryPayloads.values()],
  companionGeometryAssets: Object.fromEntries(companionGeometryAssets)
};
writeFileSync(inspectionGeometryOutputPath, `${JSON.stringify(inspectionGeometryOutput)}\n`);
console.log(`wrote ${relativeToRoot(outputPath)}`);
console.log(`wrote ${relativeToRoot(inspectionGeometryOutputPath)} (${actorGeometryPayloads.size} deduplicated actor payloads)`);
console.log(`decoded ${assets.length} byte-distinct TVA assets at 30 fps`);
console.log(`click spatial order: ${clickFlow.map((entry) => entry.exactSpatialBoxLabel).join(", ")}`);
console.log("mapping verdict: newer CubXActor evidence does not resolve CubZ click-index-to-BoxNN semantics");
