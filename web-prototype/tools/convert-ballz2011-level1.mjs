// Deterministic TrueVision3D conversion for the distinct BallZ 2011 Level1.TVM.
//
// This is not the later StockRoom `Level1_base.ASCII` board.  The source is a
// single TVM object named `Line01`, made from two closed adjacent components.
// Archive coordinates are preserved in metadata; renderer positions are only
// translated to a stable origin and uniformly scaled by the explicitly
// presentation-only `displayScale` value.
//
// Usage: node tools/convert-ballz2011-level1.mjs
// Writes: src/legacy/ballz2011-level1.json

import { createHash } from "node:crypto";
import { readFileSync, writeFileSync } from "node:fs";
import { dirname, join, relative } from "node:path";
import { fileURLToPath } from "node:url";

const here = dirname(fileURLToPath(import.meta.url));
const root = join(here, "..", "..");
const sourcePath = join(root, "Yanik C++ BCKUP", "BallZ 2011", "Release", "Media", "Level1.TVM");
const outputPath = join(here, "..", "src", "legacy", "ballz2011-level1.json");
const knownStrides = new Set([24, 32, 36, 40, 44, 48, 56, 64]);

const round = (value) => Math.round(value * 1_000_000) / 1_000_000;
const sha256 = (data) => createHash("sha256").update(data).digest("hex").toUpperCase();

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

function connectedComponents(vertexCount, indices, positions) {
  const adjacency = Array.from({ length: vertexCount }, () => []);
  const edgeUses = new Map();
  for (let triangle = 0; triangle < indices.length; triangle += 3) {
    const triplet = [indices[triangle], indices[triangle + 1], indices[triangle + 2]];
    for (const [from, to] of [[triplet[0], triplet[1]], [triplet[1], triplet[2]], [triplet[2], triplet[0]]]) {
      adjacency[from].push(to);
      adjacency[to].push(from);
      const key = from < to ? `${from},${to}` : `${to},${from}`;
      edgeUses.set(key, (edgeUses.get(key) ?? 0) + 1);
    }
  }

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
      componentPositions.push(
        positions[vertex * 3],
        positions[vertex * 3 + 1],
        positions[vertex * 3 + 2]
      );
    }
    components.push({ vertexCount: vertices.length, bounds: boundsOf(componentPositions) });
  }

  const edgeUseCounts = {};
  for (const uses of edgeUses.values()) {
    edgeUseCounts[uses] = (edgeUseCounts[uses] ?? 0) + 1;
  }
  return { components, edgeCount: edgeUses.size, edgeUseCounts };
}

function decode() {
  const data = readFileSync(sourcePath);
  const chunks = chunksOf(data);
  const chunk = (tag) => chunks.find((entry) => entry.tag === tag);
  const msta = chunk("MSTA");
  const mver = chunk("MVER");
  const indexChunk = chunk("MI16") ?? chunk("MI32");
  const matt = chunk("MATT");
  const mgro = chunk("MGRO");
  const mgrp = chunk("MGRP");
  if (!msta || !mver || !indexChunk || !matt || !mgro || !mgrp) {
    throw new Error("Level1.TVM is missing a required TVM geometry chunk.");
  }

  const stride = data.readUInt32LE(msta.start + 24);
  if (!knownStrides.has(stride) || stride < 32) {
    throw new Error(`Unsupported Level1.TVM vertex stride ${stride}.`);
  }
  const vertexCount = Math.floor(mver.size / stride);
  const archivePositions = new Array(vertexCount * 3);
  const normals = new Array(vertexCount * 3);
  const uvs = new Array(vertexCount * 2);
  for (let vertex = 0; vertex < vertexCount; vertex += 1) {
    const start = mver.start + vertex * stride;
    for (let axis = 0; axis < 3; axis += 1) {
      archivePositions[vertex * 3 + axis] = round(data.readFloatLE(start + axis * 4));
      normals[vertex * 3 + axis] = round(data.readFloatLE(start + 12 + axis * 4));
    }
    uvs[vertex * 2] = round(data.readFloatLE(start + 24));
    uvs[vertex * 2 + 1] = round(1 - data.readFloatLE(start + 28));
  }

  const indexBytes = indexChunk.tag === "MI16" ? 2 : 4;
  const indices = new Array(Math.floor(indexChunk.size / indexBytes));
  for (let index = 0; index < indices.length; index += 1) {
    indices[index] = indexBytes === 2
      ? data.readUInt16LE(indexChunk.start + index * indexBytes)
      : data.readUInt32LE(indexChunk.start + index * indexBytes);
    if (indices[index] >= vertexCount) throw new Error(`Out-of-range TVM index ${indices[index]}.`);
  }
  const triangleCount = indices.length / 3;
  if (matt.size !== triangleCount * 4) throw new Error("TVM material assignments do not match triangle count.");
  const materialIndexByTriangle = Array.from(
    { length: triangleCount },
    (_, triangle) => data.readUInt32LE(matt.start + triangle * 4)
  );
  if (mgro.size % 16 !== 0) throw new Error(`Unexpected MGRO size ${mgro.size}.`);
  const groups = [];
  for (let offset = 0; offset < mgro.size; offset += 16) {
    groups.push({
      vertexCount: data.readUInt32LE(mgro.start + offset),
      vertexStart: data.readUInt32LE(mgro.start + offset + 4),
      triangleCount: data.readUInt32LE(mgro.start + offset + 8),
      triangleStart: data.readUInt32LE(mgro.start + offset + 12)
    });
  }

  const archiveBounds = boundsOf(archivePositions);
  const anchor = [
    round((archiveBounds.min[0] + archiveBounds.max[0]) / 2),
    archiveBounds.min[1],
    round((archiveBounds.min[2] + archiveBounds.max[2]) / 2)
  ];
  const displayScale = 0.1;
  const positions = archivePositions.map((value, index) => round((value - anchor[index % 3]) * displayScale));
  let maxArchiveRoundTripError = 0;
  for (let index = 0; index < positions.length; index += 1) {
    const restored = positions[index] / displayScale + anchor[index % 3];
    maxArchiveRoundTripError = Math.max(maxArchiveRoundTripError, Math.abs(restored - archivePositions[index]));
  }
  maxArchiveRoundTripError = round(maxArchiveRoundTripError);
  const uvPairs = new Set();
  for (let index = 0; index < uvs.length; index += 2) {
    uvPairs.add(`${uvs[index]},${uvs[index + 1]}`);
  }
  const materialValues = [...new Set(materialIndexByTriangle)].sort((a, b) => a - b);
  const topology = connectedComponents(vertexCount, indices, archivePositions);
  const groupName = data.toString("latin1", mgrp.start, mgrp.start + 128).replace(/\0.*$/s, "") || "Line01";

  if (topology.components.length !== 2 || topology.components.some((component) => component.vertexCount !== 414)) {
    throw new Error("Unexpected Level1 topology: expected two 414-vertex components.");
  }
  if (Object.keys(topology.edgeUseCounts).length !== 1 || topology.edgeUseCounts[2] !== topology.edgeCount) {
    throw new Error("Unexpected Level1 topology: both components should be closed two-manifold surfaces.");
  }
  if (uvPairs.size !== 1 || !uvPairs.has("0,1")) throw new Error("Unexpected Level1 UV evidence.");
  if (materialValues.length !== 1 || materialValues[0] !== 0) throw new Error("Unexpected Level1 material evidence.");
  if (maxArchiveRoundTripError > 0.00001) throw new Error(`Normalization round trip drifted by ${maxArchiveRoundTripError}.`);

  const sortedComponents = [...topology.components].sort((a, b) => a.bounds.min[0] - b.bounds.min[0]);
  const adjacentXSeamGap = round(sortedComponents[1].bounds.min[0] - sortedComponents[0].bounds.max[0]);

  return {
    id: "ballz2011-level1",
    name: "BallZ 2011 Level 1",
    source: relative(root, sourcePath).replaceAll("\\", "/"),
    sourceSha256: sha256(data),
    bytes: data.length,
    format: "TrueVision3D 6.5 TVM chunk stream",
    objectName: groupName,
    vertexStride: stride,
    vertexCount,
    triangleCount,
    archiveBounds,
    normalization: {
      status: "inferred-display-only",
      method: "Translate archive X/Z center and archive minimum Y to the web origin, then apply one uniform scale.",
      anchor,
      displayScale,
      inverse: "archivePosition = displayPosition / displayScale + anchor",
      preservesAspectAndOrientation: true,
      maxArchiveRoundTripError
    },
    displayBounds: boundsOf(positions),
    topology: {
      connectedComponents: topology.components,
      edgeCount: topology.edgeCount,
      edgeUseCounts: topology.edgeUseCounts,
      closedTwoManifold: true,
      adjacentXSeamGap
    },
    materialEvidence: {
      exact: true,
      groups,
      materialIndices: materialValues,
      allUvsAreIdentical: true,
      decodedUv: [0, 1],
      embeddedTextureNames: [],
      assessment: "The TVM has one material slot, no usable UV map, and no embedded texture filename. Any colored, lit, or textured presentation is necessarily inferred."
    },
    interpretation: {
      exact: `One object named Line01; two adjacent, closed 414-vertex solids have identical Y/Z bounds and an ${adjacentXSeamGap}-unit X seam gap.`,
      inferred: "The geometry reads as a paired/two-lane long-form BallZ course. No archived runtime source was found loading Level1.TVM, so spawn, rules, camera, collision settings, and finish semantics remain unknown."
    },
    chunks: chunks.map(({ tag, offset, size }) => ({ tag, offset, size })),
    positions,
    normals,
    uvs,
    indices,
    materialIndexByTriangle
  };
}

const result = decode();
writeFileSync(outputPath, JSON.stringify(result));
console.log(`wrote ${outputPath}`);
console.log(`${result.vertexCount} vertices / ${result.triangleCount} triangles / ${result.topology.connectedComponents.length} components`);
console.log(`archive span ${result.archiveBounds.size.join(" x ")}`);
console.log(`display span ${result.displayBounds.size.join(" x ")} at scale ${result.normalization.displayScale}`);
