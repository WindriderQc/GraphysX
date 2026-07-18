// Static verifier for src/legacy/cubx-actor-lineage.json.
// Usage: node tools/verify-cubx-actor-lineage.mjs

import { createHash } from "node:crypto";
import { readFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { fileURLToPath } from "node:url";

const here = dirname(fileURLToPath(import.meta.url));
const webRoot = join(here, "..");
const root = join(webRoot, "..");
const data = JSON.parse(readFileSync(join(webRoot, "src", "legacy", "cubx-actor-lineage.json"), "utf8"));
const inspectionGeometry = JSON.parse(readFileSync(join(webRoot, "src", "legacy", "cubx-actor-inspection-geometry.json"), "utf8"));
const sha256 = (value) => createHash("sha256").update(value).digest("hex").toUpperCase();

function assert(condition, message) {
  if (!condition) throw new Error(message);
}

function asset(id) {
  const found = data.assets.find((candidate) => candidate.id === id);
  assert(found, `Missing CubXActor asset ${id}.`);
  return found;
}

assert(data.schema === "graphysx.cubx-actor-lineage-audit/v1", "Unexpected CubXActor audit schema.");
assert(inspectionGeometry.schema === "graphysx.cubx-actor-inspection-geometry/v1", "Unexpected CubXActor inspection geometry schema.");
assert(data.assetCensus.totalTvaAssets === 16, "CubXActor TVA census changed.");
assert(data.assetCensus.uniqueTvaHashes === 16, "CubXActor TVA assets must remain byte-distinct.");
assert(data.assetCensus.hashGroups.length === 16 && data.assetCensus.hashGroups.every((group) => group.count === 1), "CubXActor TVA hash grouping changed.");
for (const record of data.assetCensus.tvaRecords) {
  const raw = readFileSync(join(root, ...record.path.split("/")));
  assert(raw.length === record.bytes, `${record.path} byte count drifted.`);
  assert(sha256(raw) === record.sha256, `${record.path} hash drifted.`);
}

const expectedHashes = {
  "get-2": "FFC2AF719DDF3818056B5443F9F0590AB9BFBAEBE25F91361212E2E8DF51D05E",
  "get-3": "FE2E1BBF3D8B2E49D54C44B614FA5606AB469B7AB31C19877BC44D148E069C2E",
  "get-4": "11983C3214E86ED33EC7372FF94FBF8BBDF029B3DCD87E492D328829A21D4D8A",
  "get-5": "F79F63368F47CBE18EF8732D8A8FA29C5F9544D97132651347C1D901CD733B4B",
  "get-6": "E5DEB98EEBC6D740D0AB8125586D50192B4F650A7188BD9AED86A6D1D9747E0F",
  "get-7": "595CEC05FE1E561396354BB1D9AC887A0146C94DFA7C0A11E60E6407903391A9",
  "get-8": "ED98FEB2743C8F564D07985FA7202E7189CE3FA049D7A879C4D3E67D8C684887",
  "rot-1": "BDF3036C37308FDFA38AE215162E92B74E8AA1FC5E9259B8D454BFF14EAE7DC9",
  "rot-2": "614E2731D6BF38700DC5A65B1153FDE6D0E68E44B08853670291AAC94328AA45",
  "rot-3": "58592DC0531E8DFC9A59ADF4D08C1E5DD0A73E75C82F11D4355DD0DD300EBBBA",
  "rot-4": "7C979641646CF0F0690491655DE307469CFBF19D91AACFF844D7D3A4E6A2A522",
  "rot-5": "5DF1A76C1705C6A49B2BB4CF2450F7AB04C547AF9BB8118118A1FD04403725A6",
  "rot-6": "ABCE2475353BC823E97FF2EC9082557D20475E568E290D668B9FFDF849ECCC0D",
  "rot-7": "52EB4AF86DB31022169B356C009D5B73BCD371D493772BE059ABB080B8257C0E",
  "open-full": "3D86D878B0132EA92A4D8DE3D5C88B22AE9F459BE93FB9491ABC31D428657060",
  "open-solo": "A00A952BD41223035C4DD57B017CA8626526C09700D3CA9966A5EFFC4B7AF99A"
};
for (const [id, hash] of Object.entries(expectedHashes)) assert(asset(id).sha256 === hash, `${id} target hash changed.`);

const getAssets = data.assets.filter((candidate) => candidate.family === "host-loaded-get");
assert(getAssets.length === 7, "Expected seven Get actors.");
for (const [index, get] of getAssets.entries()) {
  assert(get.filename === `CubXGet${index + 2}.tva`, `Get actor ${index} filename changed.`);
  assert(get.structure.hierarchyNodeCount === 29 && get.structure.geometryChunkCount === 23 && get.structure.animationTrackCount === 25, `${get.filename} structure changed.`);
  assert(get.geometryTotals.vertices === 2264 && get.geometryTotals.triangles === 2750, `${get.filename} geometry totals changed.`);
  assert(JSON.stringify(get.geometryTotals.uniqueMaterialIndices) === "[0]", `${get.filename} embedded material assignment changed.`);
  assert(get.ranges.length === 1 && get.ranges[0].startFrame === 0 && get.ranges[0].endFrame === 100 && get.ranges[0].framesPerSecond === 30, `${get.filename} range changed.`);
  assert(get.decodedSubstantiveTracks.length === 1 && get.decodedSubstantiveTracks[0].nodeName === "Group01", `${get.filename} substantive node changed.`);
  const expectedMotionEnd = index === 0 || index === 2 ? 30 : 50;
  assert(get.timing.motionCompletedAtFrame === expectedMotionEnd && get.timing.terminalHoldFrames === 100 - expectedMotionEnd, `${get.filename} terminal hold changed.`);
}

const rotationEndFrames = [20, 30, 20, 30, 30, 30, 30];
const rotAssets = data.assets.filter((candidate) => candidate.family === "unreferenced-rotation-prototype");
assert(rotAssets.length === 7, "Expected seven Rot prototypes.");
for (const [index, rotation] of rotAssets.entries()) {
  assert(rotation.filename === `CubXRot${index + 1}.tva`, `Rot actor ${index} filename changed.`);
  assert(rotation.structure.hierarchyNodeCount === 22 && rotation.structure.geometryChunkCount === 20 && rotation.structure.animationTrackCount === 22, `${rotation.filename} structure changed.`);
  assert(rotation.geometryTotals.vertices === 2148 && rotation.geometryTotals.triangles === 2688, `${rotation.filename} geometry totals changed.`);
  assert(rotation.ranges[0].endFrame === rotationEndFrames[index] && rotation.ranges[0].framesPerSecond === 30, `${rotation.filename} timing changed.`);
  assert(rotation.timing.motionCompletedAtFrame === rotationEndFrames[index] && rotation.timing.terminalHoldFrames === 0, `${rotation.filename} must have no terminal hold.`);
  assert(rotation.decodedSubstantiveTracks.length === 1 && rotation.decodedSubstantiveTracks[0].nodeName === "Group01", `${rotation.filename} substantive node changed.`);
}

const fullOpen = asset("open-full");
assert(fullOpen.structure.hierarchyNodeCount === 28 && fullOpen.structure.geometryChunkCount === 23 && fullOpen.structure.animationTrackCount === 24, "CubXOpen structure changed.");
assert(fullOpen.geometryTotals.vertices === 2264 && fullOpen.geometryTotals.triangles === 2750, "CubXOpen geometry totals changed.");
assert(fullOpen.ranges[0].endFrame === 100 && fullOpen.timing.motionCompletedAtFrame === 50 && fullOpen.timing.terminalHoldFrames === 50, "CubXOpen timing changed.");
assert(JSON.stringify(fullOpen.decodedSubstantiveTracks.map((track) => track.nodeName).sort()) === JSON.stringify(["Left", "Right", "Top"]), "CubXOpen panel set changed.");

const soloOpen = asset("open-solo");
assert(soloOpen.structure.hierarchyNodeCount === 8 && soloOpen.structure.geometryChunkCount === 4 && soloOpen.structure.animationTrackCount === 5, "CubeOpensolo structure changed.");
assert(soloOpen.geometryTotals.vertices === 140 && soloOpen.geometryTotals.triangles === 74, "CubeOpensolo geometry totals changed.");
assert(soloOpen.ranges[0].endFrame === 50 && soloOpen.timing.motionCompletedAtFrame === 50 && soloOpen.timing.terminalHoldFrames === 0, "CubeOpensolo timing changed.");
assert(JSON.stringify(soloOpen.decodedSubstantiveTracks.map((track) => track.nodeName).sort()) === JSON.stringify(["Left08", "Right08", "Top08"]), "CubeOpensolo panel set changed.");

assert(data.companionClickGeometry.closedMesh.vertexCount === 2264 && data.companionClickGeometry.closedMesh.triangleCount === 2750, "CubXMesh companion geometry changed.");
assert(data.companionClickGeometry.clickMeshes.length === 8, "CubX click mesh census changed.");
assert(data.companionClickGeometry.clickMeshes.every((mesh) => mesh.vertexCount === 24 && mesh.triangleCount === 12), "CubX click mesh geometry changed.");
const expectedSpatialOrder = ["Box05", "Box04", "Box03", "Box02", "Box08", "Box06", "Box07", "Box01"];
assert(JSON.stringify(data.clickFlow.map((flow) => flow.exactSpatialBoxLabel)) === JSON.stringify(expectedSpatialOrder), "Click-to-spatial-BoxNN mapping changed.");
for (let click = 1; click <= 7; click += 1) {
  const flow = data.clickFlow[click - 1];
  assert(flow.sourceSelectedCube === click && flow.actorArrayIndex === click - 1, `Click ${click} source indexing changed.`);
  assert(flow.actorSlotInitialized && flow.actorFilename === `CubXGet${click + 1}.tva` && flow.actorFilenameBoxLabel === `Box${String(click + 1).padStart(2, "0")}`, `Click ${click} actor flow changed.`);
  assert(flow.centerDistance < 1, `Click ${click} spatial match drifted.`);
}
const click8 = data.clickFlow[7];
assert(click8.sourceSelectedCube === 8 && click8.actorArrayIndex === 7 && !click8.actorSlotInitialized && click8.actorFilename === null && !click8.renderedByHostLoop, "Click 8 source defect changed.");

assert(data.rotationPrototypePairs.length === 7, "Rotation/Get pair census changed.");
assert(data.rotationPrototypePairs.every((pair) => pair.terminalQuaternionDistance < 0.00004), "Rotation/Get terminal relationship drifted.");
assert(data.openingLineage.fullActorMissingClosedBoxLabel === "Box05", "Full opening actor replacement cell changed.");
assert(JSON.stringify(data.openingLineage.soloAssemblyLabels) === JSON.stringify(["Box08", "Right08", "Top08", "Left08"]), "Solo opening labels changed.");
assert(data.openingLineage.panelPairs.every((pair) => pair.panelGeometryPayloadIdentical), "Opening panel geometry lineage changed.");
assert(data.openingLineage.panelPairs.every((pair) => pair.maximumDifferenceFirst51Keys <= 0.00000021), "Opening panel key equivalence drifted.");

assert(JSON.stringify(data.materialAndHostTransform.actorPosition) === JSON.stringify([-400, 500, 0]), "CLCubX actor position changed.");
assert(data.materialAndHostTransform.constructorSizeArgument === 4 && data.materialAndHostTransform.scaleStatus === "not-applied-source-commented-out", "CLCubX scale evidence changed.");
assert(JSON.stringify(data.materialAndHostTransform.materialOverride.diffuse) === JSON.stringify([0.8, 0.8, 0.8, 1]), "CLCubX StdMat evidence changed.");
assert(data.hostSourceAudit.implementationSources.length === 4 && data.hostSourceAudit.implementationHashGroups.length === 1, "CLCubX source de-duplication changed.");
assert(data.hostSourceAudit.sceneSources.length === 4 && data.hostSourceAudit.sceneHashGroups.length === 1, "CLCubXScene source de-duplication changed.");
assert(data.hostSourceAudit.hostReferenceAudit.formattedGetLoader.length === 4, "CubXGet host-reference census changed.");
assert(data.hostSourceAudit.hostReferenceAudit.fullOpenLoader.length === 4, "CubXOpen host-reference census changed.");
assert(data.hostSourceAudit.hostReferenceAudit.numberedRotAssetLiterals.length === 0, "CubXRot prototypes must remain source-unreferenced.");
assert(data.hostSourceAudit.hostReferenceAudit.soloOpenAssetLiterals.length === 0, "CubeOpensolo must remain source-unreferenced.");
assert(data.hostSourceAudit.defects.length === 5, "CLCubX source-defect inventory changed.");
assert(!data.assetCensus.assetPathEvidence.hardCodedDirectoryExistsInArchive && data.assetCensus.assetPathEvidence.status === "source-path-mismatch", "CLCubX asset path defect changed.");
assert(data.mappingAssessment.resolvesMissingCubzClickToBoxMapping === false, "CubXActor must not be promoted as resolving the CubZ BoxNN mapping.");

assert(inspectionGeometry.actorGeometryPayloads.length === 8, "Deduplicated actor geometry payload census changed.");
const actorPayloadsByHash = new Map(inspectionGeometry.actorGeometryPayloads.map((payload) => [payload.geometryPayloadSha256, payload]));
for (const decodedAsset of data.assets) {
  for (const geometry of decodedAsset.geometry) {
    assert(actorPayloadsByHash.has(geometry.geometryPayloadSha256), `${decodedAsset.filename} geometry payload ${geometry.geometryPayloadSha256} is missing.`);
  }
}
for (const payload of inspectionGeometry.actorGeometryPayloads) {
  assert(payload.positions.length === payload.vertexCount * 3, `${payload.geometryPayloadSha256} position count changed.`);
  assert(payload.normals.length === payload.vertexCount * 3, `${payload.geometryPayloadSha256} normal count changed.`);
  assert(payload.uvs.length === payload.vertexCount * 2, `${payload.geometryPayloadSha256} UV count changed.`);
  assert(payload.indices.length === payload.triangleCount * 3, `${payload.geometryPayloadSha256} index count changed.`);
  assert(payload.materialIndices.length === payload.triangleCount, `${payload.geometryPayloadSha256} material count changed.`);
}
const companionEntries = Object.entries(inspectionGeometry.companionGeometryAssets);
assert(companionEntries.length === 9, "Inspection companion geometry census changed.");
for (const [filename, geometry] of companionEntries) {
  const summary = filename === "CubXMesh.tvm"
    ? data.companionClickGeometry.closedMesh
    : data.companionClickGeometry.clickMeshes.find((candidate) => candidate.filename === filename);
  assert(summary && summary.sha256 === geometry.sourceSha256, `${filename} inspection geometry source hash changed.`);
  assert(geometry.positions.length === geometry.vertexCount * 3 && geometry.indices.length === geometry.triangleCount * 3, `${filename} inspection array count changed.`);
}

console.log("CubXActor lineage verification passed");
console.log("16/16 byte-distinct TVA assets match exact source hashes");
console.log("click spatial order: Box05, Box04, Box03, Box02, Box08, Box06, Box07, Box01");
console.log("host flow: clicks 1..7 -> Get2..8; click 8 -> uninitialized slot 7");
console.log("mapping verdict: BoxNN semantic identity remains unresolved");
