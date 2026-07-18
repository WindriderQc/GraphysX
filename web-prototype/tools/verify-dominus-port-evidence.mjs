import { createHash } from "node:crypto";
import { existsSync, readFileSync } from "node:fs";
import { dirname, join, resolve } from "node:path";
import { fileURLToPath } from "node:url";

const here = dirname(fileURLToPath(import.meta.url));
const prototype = resolve(here, "..");
const workspace = resolve(prototype, "..");
const sourcePath = join(workspace, "Archive", "bckup", "BallZ2015.bckup", "Media", "ObjectLibrary.xml");
const auditPath = join(prototype, "src", "legacy", "dominus-family-audit.json");
const manifestPath = join(prototype, "src", "legacy", "dominus-asset-gallery.json");
const source = readFileSync(sourcePath);
const sourceText = source.toString("utf8");
const audit = JSON.parse(readFileSync(auditPath, "utf8"));
const manifest = JSON.parse(readFileSync(manifestPath, "utf8"));
const assertions = [];

function assert(condition, message) {
  assertions.push({ pass: Boolean(condition), message });
  if (!condition) throw new Error(message);
}

function sha256(bytes) {
  return createHash("sha256").update(bytes).digest("hex").toUpperCase();
}

function tag(block, name) {
  const match = block.match(new RegExp(`<${name}>([\\s\\S]*?)<\\/${name}>`, "i"));
  return match ? match[1].trim() : "";
}

function tuple(block, name) {
  const nested = tag(block, name);
  return ["x", "y", "z"].map((axis) => Number(tag(nested, axis)));
}

const records = [...sourceText.matchAll(/<Object3D>([\s\S]*?)<\/Object3D>/gi)].map((match, index) => ({
  index,
  type: Number(tag(match[1], "Type")),
  name: tag(match[1], "Name"),
  pathToMesh: tag(match[1], "PathToMesh"),
  position: tuple(match[1], "Pos"),
  rotation: tag(match[1], "Rot") ? tuple(match[1], "Rot") : [0, 0, 0],
  scale: tuple(match[1], "Scale"),
  enabled: tag(match[1], "bEnable") === "true"
}));
const portRecords = records.filter((record) => record.name.toLowerCase().startsWith("port_"));
const portAssets = manifest.assets.filter((asset) => asset.family === "port");
const assetsById = new Map(portAssets.map((asset) => [asset.id, asset]));
const auditedPlacements = audit.objectLibraryExclusion.portPlacements;

assert(source.length === 27486, "Canonical ObjectLibrary.xml is exactly 27,486 bytes");
assert(sha256(source) === "4BF51E84F92084E8BE2B91214391BFA8839B29BCEB4C942187DD73B2B04AF432", "Canonical ObjectLibrary.xml hash is exact");
assert(records.length === 61, "ObjectLibrary.xml contains exactly 61 serialized records");
assert(portRecords.length === 28, "ObjectLibrary.xml contains exactly 28 port-prefixed records");
assert(audit.objectLibraryExclusion.portRows === 28 && auditedPlacements.length === 28, "Forensic audit retains all 28 port rows");
assert(audit.references.objectLibraryLogicalCopyCount === 1, "Three physical ObjectLibrary copies deduplicate to one logical document");
assert(audit.references.objectLibraryCopyHashes.every((copy) => copy.sha256 === sha256(source)), "Every known physical ObjectLibrary copy has the canonical hash");
assert(audit.references.sourceCodeHits.length === 0, "No archived source loader callsite references a Dominus port asset");
assert(audit.references.authoredSceneDocumentsExcludingObjectLibrary.length === 0, "No authored Dominus scene document exists outside ObjectLibrary");
assert(Object.values(audit.objectLibraryExclusion.authoredHostFieldsPresent).every((present) => present === false), "ObjectLibrary has no camera, navigation, spawn, water or terrain fields");

for (const [index, record] of portRecords.entries()) {
  const placement = auditedPlacements[index];
  const id = record.name.toLowerCase();
  assert(placement.name === record.name, `${record.name} audit order matches source record order`);
  assert(JSON.stringify(placement.position) === JSON.stringify(record.position), `${record.name} source position is exact`);
  assert(JSON.stringify(placement.rotation) === JSON.stringify(record.rotation), `${record.name} source rotation is exact`);
  assert(JSON.stringify(placement.scale) === JSON.stringify(record.scale), `${record.name} source scale is exact`);
  assert(record.type === 13 && record.enabled, `${record.name} remains an enabled PHYSICXMESH catalog row`);
  assert(record.position[1] === 0 && record.rotation.every((value) => value === 0) && record.scale.every((value) => value === 0.01), `${record.name} matches the uniform unrotated thumbnail-grid signature`);
  const asset = assetsById.get(id);
  assert(Boolean(asset), `${record.name} has one audited source-asset record`);
  if (!asset) continue;
  const sourceAssetPath = join(workspace, ...asset.source.split("/"));
  assert(existsSync(sourceAssetPath), `${record.name} canonical source mesh exists`);
  assert(sha256(readFileSync(sourceAssetPath)) === asset.sha256, `${record.name} canonical source mesh hash matches its manifest`);
  if (asset.status === "recovered-text-x") {
    const payloadPath = join(prototype, "public", ...asset.payloadUrl.replace(/^\//, "").split("/"));
    assert(existsSync(payloadPath), `${record.name} decoded payload exists`);
    assert(sha256(readFileSync(payloadPath)) === asset.payloadSha256, `${record.name} decoded payload hash matches its manifest`);
    const payload = JSON.parse(readFileSync(payloadPath, "utf8"));
    assert(payload.sha256 === asset.sha256, `${record.name} decoded payload is bound to the exact source mesh hash`);
  } else {
    assert(id === "port_crateshed" && asset.payloadUrl === null, "port_crateshed is the sole explicit unsupported binary-X boundary");
  }
}

assert(portAssets.length === 28, "Dominus gallery manifest has exactly 28 port-family assets");
assert(portAssets.filter((asset) => asset.status === "recovered-text-x").length === 27, "Exactly 27 port assets have decoded text-X geometry");
assert(portAssets.filter((asset) => asset.status === "unsupported-binary-x").length === 1, "Exactly one port asset remains unsupported binary X");
assert(portAssets.reduce((sum, asset) => sum + asset.vertexCount, 0) === 23594, "Port subset totals exactly 23,594 source vertices");
assert(portAssets.reduce((sum, asset) => sum + asset.triangleCount, 0) === 13860, "Port subset totals exactly 13,860 source triangles");
assert(portAssets.reduce((sum, asset) => sum + asset.materialGroupCount, 0) === 126, "Port subset totals exactly 126 material groups");
assert(audit.objectLibraryExclusion.allPortRowsHaveZeroRotation, "Audit proves every port thumbnail row has zero rotation");
assert(audit.objectLibraryExclusion.allPortRowsUseScale001, "Audit proves every port thumbnail row uses 0.01 uniform scale");
assert(audit.objectLibraryExclusion.allPortRowsSitAtY0OnFiveUnitXZGrid, "Audit proves every port thumbnail sits at Y=0 on the five-unit X/Z grid");
assert(audit.absentEvidence.some((statement) => statement.includes("No inter-asset placement")), "Audit explicitly records missing authored inter-asset placement");
assert(audit.objectLibraryExclusion.conclusion.includes("not an authored village/port layout"), "Audit conclusion rejects interpreting the catalog as a village layout");

const failed = assertions.filter((assertion) => !assertion.pass);
console.log(JSON.stringify({ assertions: assertions.length, failed: failed.length, sourceRecords: records.length, portRows: portRecords.length, decoded: 27, unsupported: 1 }, null, 2));
