// Deterministic TrueVision3D 6.5 TVA animation decoder for the CubZ menu.
//
// Scope is deliberately narrow: CubeRot.tva, CubeOpen.tva, the surviving
// CubZ.cpp implementations that play them, and hashes for the related but
// distinct CubXActor TVA revision. Geometry/material chunks are inventoried
// but not promoted as decoded animation evidence.
//
// Usage: node tools/convert-cubz-tva.mjs
// Writes: src/legacy/cubz-tva-animations.json

import { createHash } from "node:crypto";
import { existsSync, readdirSync, readFileSync, statSync, writeFileSync } from "node:fs";
import { basename, dirname, extname, join, relative } from "node:path";
import { fileURLToPath } from "node:url";

const here = dirname(fileURLToPath(import.meta.url));
const webRoot = join(here, "..");
const root = join(webRoot, "..");
const cppRoot = join(root, "Yanik C++ BCKUP");
const animationRoot = join(cppRoot, "Media", "PartSys", "Animations");
const cubXActorRoot = join(cppRoot, "Media", "CubXActor");
const outputPath = join(webRoot, "src", "legacy", "cubz-tva-animations.json");

const targetDefinitions = [
  { id: "cube-rotation", filename: "CubeRot.tva" },
  { id: "cube-open", filename: "CubeOpen.tva" }
];

const sha256 = (data) => createHash("sha256").update(data).digest("hex").toUpperCase();
const slash = (value) => value.replaceAll("\\", "/");
const relativeToRoot = (value) => slash(relative(root, value));
const round = (value, places = 9) => {
  const factor = 10 ** places;
  return Math.round(value * factor) / factor;
};

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

function chunkStream(data) {
  const chunkStart = data.readUInt32LE(12);
  assert(chunkStart === 16, `Unsupported TVA chunk start ${chunkStart}.`);
  const chunks = [];
  let offset = chunkStart;
  while (offset <= data.length - 8) {
    const tag = data.toString("latin1", offset, offset + 4);
    const size = data.readUInt32LE(offset + 4);
    assert(/^[A-Z0-9! ]{4}$/.test(tag), `Invalid TVA chunk tag at ${offset}.`);
    assert(offset + 8 + size <= data.length, `TVA chunk ${tag} overruns the file.`);
    chunks.push({ tag, offset, start: offset + 8, size, end: offset + 8 + size });
    offset += 8 + size;
    if (tag === "AEND") break;
  }
  assert(chunks.at(-1)?.tag === "AEND", "TVA stream has no AEND chunk.");
  return chunks;
}

function fixedString(data, offset, length) {
  return data.toString("latin1", offset, offset + length).split("\0", 1)[0];
}

function readFloatRecords(data, offset, count, width) {
  return Array.from({ length: count }, (_, record) =>
    Array.from({ length: width }, (_, field) => data.readFloatLE(offset + (record * width + field) * 4))
  );
}

function componentSpan(records, startField, componentCount) {
  return Array.from({ length: componentCount }, (_, component) => {
    const values = records.map((record) => record[startField + component]);
    return Math.max(...values) - Math.min(...values);
  });
}

function quaternionMetrics(rotationKeys) {
  const first = rotationKeys[0].slice(1);
  const firstNorm = Math.hypot(...first);
  let maximumDistanceFromFirst = 0;
  let maximumNormError = 0;
  for (const key of rotationKeys) {
    const quaternion = key.slice(1);
    const norm = Math.hypot(...quaternion);
    const dot = quaternion.reduce((sum, value, index) => sum + value * first[index], 0);
    maximumDistanceFromFirst = Math.max(maximumDistanceFromFirst, 1 - Math.abs(dot / (norm * firstNorm)));
    maximumNormError = Math.max(maximumNormError, Math.abs(norm - 1));
  }
  return { maximumDistanceFromFirst, maximumNormError };
}

function parseHierarchy(data, chunk, expectedCount) {
  const recordBytes = 464;
  assert(chunk.size % recordBytes === 0, "MHI2 does not contain whole 464-byte records.");
  const count = chunk.size / recordBytes;
  assert(count === expectedCount, `MHI2 hierarchy count ${count} does not match ASTA ${expectedCount}.`);
  return Array.from({ length: count }, (_, id) => {
    const offset = chunk.start + id * recordBytes;
    return {
      id,
      rawLinks: [data.readInt32LE(offset), data.readInt32LE(offset + 4), data.readInt32LE(offset + 8)],
      parent: data.readInt32LE(offset),
      firstChild: data.readInt32LE(offset + 4),
      nextSibling: data.readInt32LE(offset + 8),
      name: fixedString(data, offset + 12, 256)
    };
  });
}

function parseAnimationRanges(data, chunk, expectedCount) {
  const recordBytes = 280;
  assert(chunk.size % recordBytes === 0, "MANS does not contain whole 280-byte records.");
  const count = chunk.size / recordBytes;
  assert(count === expectedCount, `MANS range count ${count} does not match ASTA ${expectedCount}.`);
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
  const positionOffset = chunk.start + 24;
  const rotationOffset = positionOffset + positionCount * 16;
  const scaleOffset = rotationOffset + rotationCount * 20;
  const expectedEnd = scaleOffset + scaleCount * 16;
  assert(expectedEnd === chunk.end, `MANI node ${nodeId} counted arrays do not fill the chunk.`);
  assert(nodeId >= 0 && nodeId < hierarchy.length, `MANI node ${nodeId} is outside MHI2.`);

  const positions = readFloatRecords(data, positionOffset, positionCount, 4);
  const rotations = readFloatRecords(data, rotationOffset, rotationCount, 5);
  const scales = readFloatRecords(data, scaleOffset, scaleCount, 4);
  const positionSpan = componentSpan(positions, 1, 3);
  const scaleSpan = componentSpan(scales, 1, 3);
  const quaternion = quaternionMetrics(rotations);
  const positionMagnitude = Math.hypot(...positionSpan);
  const scaleMagnitude = Math.hypot(...scaleSpan);
  const substantive = {
    position: positionMagnitude > 0.001,
    rotation: quaternion.maximumDistanceFromFirst > 0.000001,
    scale: scaleMagnitude > 0.0001
  };

  for (const records of [positions, rotations, scales]) {
    for (let index = 1; index < records.length; index += 1) {
      assert(records[index][0] >= records[index - 1][0], `MANI node ${nodeId} has decreasing frame keys.`);
    }
  }
  assert(quaternion.maximumNormError < 0.000001, `MANI node ${nodeId} has non-unit quaternion data.`);

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
    maximumQuaternionDistanceFromFirst: round(quaternion.maximumDistanceFromFirst, 12),
    maximumQuaternionNormError: round(quaternion.maximumNormError, 12),
    scaleSpan: scaleSpan.map((value) => round(value)),
    substantive,
    rawChunkSha256: sha256(data.subarray(chunk.offset, chunk.end)),
    positions,
    rotations,
    scales
  };
}

function exactRotationAt(track, frame) {
  const key = track.rotations.find((candidate) => Math.abs(candidate[0] - frame) < 0.000001);
  return key ?? null;
}

function publicTrack(track) {
  return {
    nodeId: track.nodeId,
    nodeName: track.nodeName,
    sourceAnimation: track.sourceAnimation,
    rawFlags: track.rawFlags,
    counts: track.counts,
    frameBounds: track.frameBounds,
    fixedPositionAtFirstKey: track.positions[0],
    fixedScaleAtFirstKey: track.scales[0],
    rotationKeys: track.rotations,
    maximumQuaternionNormError: track.maximumQuaternionNormError,
    rawChunkSha256: track.rawChunkSha256
  };
}

function decodeTarget(definition) {
  const path = join(animationRoot, definition.filename);
  assert(existsSync(path), `Missing target TVA ${definition.filename}.`);
  const data = readFileSync(path);
  const chunks = chunkStream(data);
  const chunk = (tag) => chunks.find((candidate) => candidate.tag === tag);
  for (const required of ["MSTR", "ASTA", "MAG3", "MHI2", "MANS", "AEND"]) {
    assert(chunk(required), `${definition.filename} is missing ${required}.`);
  }

  const header = {
    magicHex: data.subarray(0, 4).toString("hex").toUpperCase(),
    rawWord1: data.readUInt32LE(4),
    rawWord2: data.readUInt32LE(8),
    chunkStart: data.readUInt32LE(12)
  };
  assert(header.magicHex === "22446418", `${definition.filename} has unexpected TVA magic.`);
  // rawWord2 is consistently one greater than the parsed MSTR..AEND chunk
  // count in these files. Preserve that observation without naming an
  // undocumented engine field more strongly than the evidence supports.
  assert(chunks.length + 1 === header.rawWord2, `${definition.filename} header/chunk relation changed.`);

  const asta = chunk("ASTA");
  assert(asta.size === 48, `${definition.filename} has an unexpected ASTA size.`);
  const astaWords = Array.from({ length: 12 }, (_, index) => data.readUInt32LE(asta.start + index * 4));
  const animationRangeCount = astaWords[4];
  const hierarchyNodeCount = astaWords[8];
  const hierarchy = parseHierarchy(data, chunk("MHI2"), hierarchyNodeCount);
  const ranges = parseAnimationRanges(data, chunk("MANS"), animationRangeCount);
  const mansIndex = chunks.indexOf(chunk("MANS"));
  const maniChunks = chunks.slice(mansIndex + 1).filter((candidate) => candidate.tag === "MANI");
  assert(ranges.every((range) => range.trackChunkCount === maniChunks.length), `${definition.filename} MANS track count changed.`);
  const tracks = maniChunks.map((candidate) => parseTrack(data, candidate, hierarchy));
  const substantiveTracks = tracks.filter((track) => Object.values(track.substantive).some(Boolean));
  const aend = chunk("AEND");

  return {
    id: definition.id,
    filename: definition.filename,
    source: relativeToRoot(path),
    bytes: data.length,
    sha256: sha256(data),
    header,
    structure: {
      chunkTagsInOrder: chunks.map((candidate) => candidate.tag),
      chunks: chunks.map(({ tag, offset, size }) => ({ tag, offset, size })),
      astaWords,
      animationRangeCount,
      hierarchyNodeCount,
      maniTrackCount: tracks.length,
      postAendBytes: data.length - aend.end,
      postAendSha256: sha256(data.subarray(aend.end))
    },
    hierarchy,
    ranges,
    trackSummaries: tracks.map((track) => ({
      nodeId: track.nodeId,
      nodeName: track.nodeName,
      sourceAnimation: track.sourceAnimation,
      rawFlags: track.rawFlags,
      counts: track.counts,
      frameBounds: track.frameBounds,
      positionSpan: track.positionSpan,
      maximumQuaternionDistanceFromFirst: track.maximumQuaternionDistanceFromFirst,
      maximumQuaternionNormError: track.maximumQuaternionNormError,
      scaleSpan: track.scaleSpan,
      substantive: track.substantive,
      rawChunkSha256: track.rawChunkSha256
    })),
    decodedSubstantiveTracks: substantiveTracks.map(publicTrack),
    rangeBoundaryRotations: ranges.map((range) => ({
      rangeId: range.id,
      rangeName: range.name,
      startFrame: range.startFrame,
      endFrame: range.endFrame,
      tracks: substantiveTracks.map((track) => ({
        nodeId: track.nodeId,
        nodeName: track.nodeName,
        exactStartKey: exactRotationAt(track, range.startFrame),
        exactEndKey: exactRotationAt(track, range.endFrame)
      }))
    }))
  };
}

function fileRecord(path) {
  const data = readFileSync(path);
  return {
    path: relativeToRoot(path),
    bytes: statSync(path).size,
    sha256: sha256(data)
  };
}

function groupRecordsByHash(records) {
  const groups = new Map();
  for (const record of records) {
    const group = groups.get(record.sha256) ?? [];
    group.push(record.path);
    groups.set(record.sha256, group);
  }
  return [...groups.entries()].map(([hash, paths]) => ({ sha256: hash, count: paths.length, paths }));
}

function sourceEvidence(path, patterns) {
  const text = readFileSync(path, "latin1");
  const lines = text.split(/\r?\n/);
  return patterns.map(({ pattern, fact }) => {
    const lineIndex = lines.findIndex((line) => line.includes(pattern));
    assert(lineIndex >= 0, `Source evidence pattern not found: ${pattern}`);
    return { fact, line: lineIndex + 1, code: lines[lineIndex].trim() };
  });
}

const assets = targetDefinitions.map(decodeTarget);
const cubeRot = assets.find((asset) => asset.id === "cube-rotation");
const cubeOpen = assets.find((asset) => asset.id === "cube-open");
assert(cubeRot.ranges.length === 8, "CubeRot must expose eight ranges including Animation0.");
assert(cubeRot.decodedSubstantiveTracks.length === 1, "CubeRot must have one substantive transform track.");
assert(cubeRot.decodedSubstantiveTracks[0].nodeName === "GlobalCube", "CubeRot substantive node changed.");
assert(cubeOpen.ranges.length === 1, "CubeOpen must expose one source range.");
assert(
  JSON.stringify(cubeOpen.decodedSubstantiveTracks.map((track) => track.nodeName).sort()) ===
    JSON.stringify(["Left08", "Right08", "Top08"].sort()),
  "CubeOpen substantive panel nodes changed."
);

const allCppFiles = walk(cppRoot);
const targetCopies = allCppFiles
  .filter((path) => extname(path).toLowerCase() === ".tva")
  .filter((path) => targetDefinitions.some((definition) => basename(path).toLowerCase() === definition.filename.toLowerCase()))
  .map(fileRecord);
const cubZSources = allCppFiles
  .filter((path) => ["cubz.cpp", "cubz (2).cpp"].includes(basename(path).toLowerCase()))
  .map(fileRecord);
const relatedCubXActorAssets = walk(cubXActorRoot)
  .filter((path) => extname(path).toLowerCase() === ".tva")
  .map(fileRecord)
  .map((record) => ({
    ...record,
    aliasesTarget: assets.some((asset) => asset.sha256 === record.sha256),
    classification: "distinct alternate CubXActor TVA; not a byte alias of CubeRot/CubeOpen"
  }));

const enhancedSourcePath = join(cppRoot, "CubZ (2).cpp");
const baselineSourcePath = join(cppRoot, "CubZ.cpp");
const playbackEvidence = sourceEvidence(enhancedSourcePath, [
  { pattern: 'CubeOpenActor->LoadTVA("media\\\\CubeOpen.tva", true, false);', fact: "Loads the dedicated open actor." },
  { pattern: 'CubXRotationActor->LoadTVA("media\\\\CubeRot.tva", true, false);', fact: "Loads the dedicated selection-rotation actor." },
  { pattern: "CubXRotationActor->SetAnimationLoop(false);", fact: "Selection rotations are non-looping." },
  { pattern: "CubXRotationActor->SetAnimationID(iCubeIndex);", fact: "Nonzero cube selection uses the numeric animation range matching the click index." },
  { pattern: "CubXRotationActor->PlayAnimation(1);", fact: "Selection rotation plays forward at speed +1." },
  { pattern: "if(CubXRotationActor->IsAnimationFinished())", fact: "Opening begins only after the selection rotation reports finished." },
  { pattern: "CubeOpenActor->PlayAnimation(1);", fact: "Open plays the open clip forward at speed +1." },
  { pattern: "CubeOpenActor->PlayAnimation(-1);", fact: "Close replays the same open clip backward at speed -1." },
  { pattern: "CubXRotationActor->SetAnimationID(iSelectedCube+1);", fact: "BackRotate selects iSelectedCube+1 before reverse playback." },
  { pattern: "CubXRotationActor->PlayAnimation(-1);", fact: "BackRotate plays the selected rotation range backward." },
  { pattern: "for(j=0;j<8;j++)", fact: "The internal menu defines eight opened levels." },
  { pattern: "for(i=0;i<4;i++)", fact: "Each opened level owns four internal menu buttons." },
  { pattern: "clCubXMenuMgr->iNiveauActif = iSelectedCube + 1;", fact: "Finished opening activates menu level selectedCube+1." }
]);

const baselineEvidence = sourceEvidence(baselineSourcePath, [
  { pattern: "if( fKey <= 0.5 )", fact: "The byte-duplicated baseline branch stops reverse rotation near frame 0.5." },
  { pattern: "else if(bIsOpening)", fact: "The baseline branch declares an opening state but leaves its body empty." }
]);
const enhancedEvidence = sourceEvidence(enhancedSourcePath, [
  { pattern: "if( fKey <= 3 )", fact: "The enhanced branch uses a frame-3 reverse stop threshold." },
  { pattern: "if(CubeOpenActor->IsAnimationFinished())", fact: "The enhanced branch completes opening and switches menu level." },
  { pattern: "CubeOpenActor->ResetTime();", fact: "The enhanced branch resets actor time after opening." }
]);

const output = {
  schema: "graphysx.cubz-tva-audit/v1",
  generated: new Date().toISOString(),
  decoder: {
    format: "TrueVision3D 6.5 TVA chunk stream",
    exactLayouts: {
      fileHeader: "16 bytes; raw magic/word/chunk-count/chunk-start retained",
      MHI2: "464-byte records; three exact link integers + 256-byte node name retained",
      MANS: "280-byte animation-range records; 256-byte name + source/start/end/fps/raw/count fields",
      MANI: "24-byte counted header; position [frame,x,y,z], rotation [frame,x,y,z,w], scale [frame,x,y,z] float32 arrays"
    },
    rotationInterpretation: "The four stored rotation floats are emitted in source order and labeled x,y,z,w because every decoded key is unit length within 1e-6 and identity keys are [0,0,0,~1].",
    coordinateBoundary: "No TV3D-to-Three.js handedness conversion, Euler conversion, interpolation mode, or procedural easing is asserted here.",
    geometryBoundary: "MAG3/MHI2/MAMD geometry and skin payloads are inventoried but not converted by this animation-only pass."
  },
  evidenceBoundary: {
    decodedExact: [
      "TVA hashes, byte counts, chunk order/offsets/sizes and post-AEND hashes",
      "MHI2 raw hierarchy links and node names",
      "MANS animation names, source IDs, frame bounds and 30 fps timing",
      "MANI position, quaternion and scale float32 key arrays",
      "Only GlobalCube changes materially in CubeRot; only Right08, Top08 and Left08 change materially in CubeOpen"
    ],
    derivedFromDecodedValues: [
      "Clip duration in seconds is (endFrame-startFrame)/fps.",
      "Substantive-track classification compares decoded component spans and sign-invariant quaternion distance against disclosed tolerances.",
      "MHI2 parent/first-child/next-sibling labels are a structural interpretation of exact integers whose values form a consistent tree."
    ],
    sourceBackedPlaybackSemantics: [
      "Cube selection 0 opens immediately; selections 1-7 play CubeRot ranges forward, then swap to CubeOpen.",
      "Open plays CubeOpen forward; Close plays it backward; BackRotate then plays CubeRot backward.",
      "There are eight internal menu levels with four buttons per opened level."
    ],
    notRecovered: [
      "No archive field maps cube indices to modern category labels or actions.",
      "TV3D runtime interpolation/easing and invalid-animation-ID behavior are not available in the surviving headers.",
      "A web coordinate/handedness conversion is intentionally not inferred.",
      "The related CubXGet/CubXRot/CubXOpen files are a distinct implementation family, not copies of these targets."
    ]
  },
  assets,
  copyAudit: {
    targetedFilenameCopies: targetCopies,
    targetedHashGroups: groupRecordsByHash(targetCopies),
    cubZSourceCopies: cubZSources,
    cubZSourceHashGroups: groupRecordsByHash(cubZSources),
    conclusion: "The two target TVA files have no same-name byte copies under Yanik C++ BCKUP. Five CubZ.cpp files are byte-identical baseline copies; CubZ (2).cpp is the one distinct enhanced implementation."
  },
  relatedCubXActorAssets,
  playbackSource: {
    canonicalEnhancedSource: relativeToRoot(enhancedSourcePath),
    canonicalBaselineSource: relativeToRoot(baselineSourcePath),
    playbackEvidence,
    branchDifferences: { baselineEvidence, enhancedEvidence },
    binaryConfirmedPitfall: {
      title: "BackRotate animation ID is shifted and can exceed the decoded range table",
      decodedValidAnimationIds: [0, cubeRot.ranges.length - 1],
      forwardSelectionIds: [1, 7],
      reverseExpression: "iSelectedCube+1",
      resultingReverseIds: [2, 8],
      assessment: "For selections 1-6 the reverse ID is a different range than the forward ID; selection 7 requests ID 8 although CubeRot contains only IDs 0-7. This is a source/binary inconsistency, not behavior to reproduce as intended design."
    },
    combinedInterpretation: "Because CubeRot materially animates only GlobalCube while CubeOpen materially animates only the three named Box08 panels, the source actor swap strongly indicates that seven rotations bring a selected cell to one canonical Box08 opening pose. The exact index-to-Box name mapping is not directly encoded and remains an inference."
  },
  recoveryRecommendation: {
    feasible: true,
    decodedRuntimeDataReady: true,
    safeNextUse: "Drive the existing CubZ state flow from the exact source frame/quaternion keys after an explicitly tested TV3D-to-web coordinate mapping.",
    doNotClaimYet: "Do not label the animation TVA-exact in the main UI until handedness, interpolation, actor-swap continuity and the BackRotate range defect have visual regression coverage."
  }
};

writeFileSync(outputPath, `${JSON.stringify(output)}\n`);
console.log(`wrote ${relativeToRoot(outputPath)}`);
console.log(`CubeRot: ${cubeRot.ranges.length} ranges, ${cubeRot.structure.maniTrackCount} tracks, ${cubeRot.decodedSubstantiveTracks.length} substantive`);
console.log(`CubeOpen: ${cubeOpen.ranges.length} range, ${cubeOpen.structure.maniTrackCount} tracks, ${cubeOpen.decodedSubstantiveTracks.length} substantive`);
