// Static verification for the generated CubZ TVA animation audit.
// Run after `node tools/convert-cubz-tva.mjs`.

import { createHash } from "node:crypto";
import { readFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { fileURLToPath } from "node:url";

const here = dirname(fileURLToPath(import.meta.url));
const webRoot = join(here, "..");
const root = join(webRoot, "..");
const data = JSON.parse(readFileSync(join(webRoot, "src", "legacy", "cubz-tva-animations.json"), "utf8"));
const sha256 = (bytes) => createHash("sha256").update(bytes).digest("hex").toUpperCase();
const assert = (condition, message) => {
  if (!condition) throw new Error(message);
};
const asset = (id) => {
  const found = data.assets.find((candidate) => candidate.id === id);
  if (!found) throw new Error(`Missing CubZ TVA asset ${id}.`);
  return found;
};
const verifyRawFile = (record) => {
  const bytes = readFileSync(join(root, ...record.source.split("/")));
  assert(bytes.length === record.bytes, `${record.filename} byte count changed.`);
  assert(sha256(bytes) === record.sha256, `${record.filename} SHA-256 changed.`);
};
const verifyQuaternionKeys = (keys, label) => {
  for (let index = 0; index < keys.length; index += 1) {
    if (index > 0) assert(keys[index][0] >= keys[index - 1][0], `${label} frame keys are not monotonic.`);
    const norm = Math.hypot(...keys[index].slice(1));
    assert(Math.abs(norm - 1) < 0.000001, `${label} contains a non-unit quaternion.`);
  }
};

assert(data.schema === "graphysx.cubz-tva-audit/v1", "Unexpected CubZ TVA audit schema.");
assert(data.assets.length === 2, "Expected exactly CubeRot and CubeOpen target assets.");
assert(data.recoveryRecommendation.feasible === true, "The decoded TVA recovery should remain feasible.");
assert(data.decoder.coordinateBoundary.includes("No TV3D-to-Three.js"), "Coordinate uncertainty must remain disclosed.");

const rotation = asset("cube-rotation");
const opening = asset("cube-open");
verifyRawFile(rotation);
verifyRawFile(opening);

assert(rotation.sha256 === "8ADBA22A2103A6EDAC9B173474B1A4599DB04B21C7DF4418412EA35903AE7B4B", "CubeRot identity changed.");
assert(rotation.bytes === 700768, "CubeRot size changed.");
assert(rotation.structure.animationRangeCount === 8, "CubeRot range count changed.");
assert(rotation.structure.hierarchyNodeCount === 70, "CubeRot hierarchy count changed.");
assert(rotation.structure.maniTrackCount === 46, "CubeRot MANI count changed.");
assert(rotation.structure.postAendBytes === 980, "CubeRot post-AEND tail changed.");
assert(rotation.structure.chunkTagsInOrder.filter((tag) => tag === "MAMD").length === 44, "CubeRot MAMD count changed.");
assert(rotation.structure.chunkTagsInOrder.filter((tag) => tag === "MANI").length === 46, "CubeRot chunk directory changed.");
assert(rotation.ranges.map((range) => range.name).join(",") === "Animation0,Animation1,Animation2,Animation3,Animation4,Animation5,Animation6,Animation7", "CubeRot range names changed.");
assert(JSON.stringify(rotation.ranges.map((range) => [range.startFrame, range.endFrame])) === JSON.stringify([[0,220],[0,28],[30,58],[60,88],[90,118],[120,148],[150,178],[180,208]]), "CubeRot frame ranges changed.");
assert(rotation.ranges.every((range) => range.framesPerSecond === 30), "CubeRot fps changed.");
assert(rotation.ranges.slice(1).every((range) => Math.abs(range.durationSeconds - 28 / 30) < 1e-12), "CubeRot selection durations changed.");
assert(rotation.decodedSubstantiveTracks.length === 1, "CubeRot must retain one substantive transform track.");
assert(rotation.decodedSubstantiveTracks[0].nodeId === 1 && rotation.decodedSubstantiveTracks[0].nodeName === "GlobalCube", "CubeRot animated node changed.");
assert(rotation.decodedSubstantiveTracks[0].rotationKeys.length === 211, "CubeRot GlobalCube key count changed.");
verifyQuaternionKeys(rotation.decodedSubstantiveTracks[0].rotationKeys, "CubeRot GlobalCube");
assert(rotation.rangeBoundaryRotations.every((range) => range.tracks.every((track) => track.exactStartKey && track.exactEndKey)), "A CubeRot range boundary lacks an exact decoded key.");
assert(rotation.trackSummaries.filter((track) => track.substantive.rotation).map((track) => track.nodeName).join(",") === "GlobalCube", "CubeRot substantive rotation classification changed.");
assert(rotation.trackSummaries.every((track) => !track.substantive.position && !track.substantive.scale), "CubeRot unexpectedly gained material position/scale motion.");

assert(opening.sha256 === "CCB1ED3BB77919509E2B501663FB34BFD7E64C34E95F01E003E6D553160CA27B", "CubeOpen identity changed.");
assert(opening.bytes === 222564, "CubeOpen size changed.");
assert(opening.structure.animationRangeCount === 1, "CubeOpen range count changed.");
assert(opening.structure.hierarchyNodeCount === 69, "CubeOpen hierarchy count changed.");
assert(opening.structure.maniTrackCount === 45, "CubeOpen MANI count changed.");
assert(opening.structure.postAendBytes === 980, "CubeOpen post-AEND tail changed.");
assert(opening.ranges[0].name === "" && opening.ranges[0].startFrame === 0 && opening.ranges[0].endFrame === 50, "CubeOpen range changed.");
assert(opening.ranges[0].framesPerSecond === 30 && Math.abs(opening.ranges[0].durationSeconds - 50 / 30) < 1e-12, "CubeOpen timing changed.");
assert(JSON.stringify(opening.decodedSubstantiveTracks.map((track) => track.nodeName).sort()) === JSON.stringify(["Left08", "Right08", "Top08"].sort()), "CubeOpen animated panel set changed.");
assert(opening.decodedSubstantiveTracks.every((track) => track.rotationKeys.length === 51), "CubeOpen panel key counts changed.");
for (const track of opening.decodedSubstantiveTracks) verifyQuaternionKeys(track.rotationKeys, `CubeOpen ${track.nodeName}`);
assert(opening.trackSummaries.every((track) => !track.substantive.position && !track.substantive.scale), "CubeOpen unexpectedly gained material position/scale motion.");
assert(opening.trackSummaries.filter((track) => track.substantive.rotation).length === 3, "CubeOpen substantive rotation count changed.");

assert(data.copyAudit.targetedFilenameCopies.length === 2, "Target TVA copy inventory changed.");
assert(data.copyAudit.targetedHashGroups.length === 2 && data.copyAudit.targetedHashGroups.every((group) => group.count === 1), "Distinct target TVA files were incorrectly de-duplicated.");
assert(data.copyAudit.cubZSourceCopies.length === 6, "CubZ.cpp source inventory changed.");
assert(data.copyAudit.cubZSourceHashGroups.some((group) => group.count === 5 && group.sha256 === "6EC6CC8A5F2BC80AE5BD9E16AFDB46FA2E87AE93341BF88D3A67CA30A55C56BA"), "Five baseline CubZ.cpp copies are no longer byte-identical.");
assert(data.copyAudit.cubZSourceHashGroups.some((group) => group.count === 1 && group.sha256 === "1337AE604176B7B31E664F74B9EB02F926DC1DA9F018E9A6D6BC618288CA28F6"), "Enhanced CubZ (2).cpp identity changed.");
assert(data.relatedCubXActorAssets.length === 16 && data.relatedCubXActorAssets.every((record) => !record.aliasesTarget), "A related CubXActor TVA was incorrectly merged with the targets.");

const pitfall = data.playbackSource.binaryConfirmedPitfall;
assert(JSON.stringify(pitfall.decodedValidAnimationIds) === JSON.stringify([0, 7]), "Decoded CubeRot valid ID range changed.");
assert(pitfall.reverseExpression === "iSelectedCube+1" && JSON.stringify(pitfall.resultingReverseIds) === JSON.stringify([2, 8]), "BackRotate mismatch disclosure changed.");
assert(data.playbackSource.playbackEvidence.length === 13, "Playback call-chain evidence changed.");

console.log("CubZ TVA verification passed");
console.log("CubeRot 8 ranges / 211 GlobalCube quaternion keys; CubeOpen 3 x 51 panel quaternion keys");
console.log("6 CubZ.cpp copies collapsed to 2 unique revisions; 16 alternate CubXActor TVAs remain distinct");

