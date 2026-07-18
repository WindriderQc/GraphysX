#!/usr/bin/env node

// Deterministic contract for the ObjectLibrary.xml catalog recovery. This does
// not judge appearance; it prevents the archive record from quietly turning
// into a new composition or from receiving proxy geometry for absent assets.

import assert from "node:assert/strict";
import { createHash } from "node:crypto";
import { existsSync, readFileSync, statSync } from "node:fs";
import { dirname, extname, join, resolve } from "node:path";
import { fileURLToPath } from "node:url";

const here = dirname(fileURLToPath(import.meta.url));
const project = resolve(here, "..");
const workspace = resolve(project, "..");
const normalizedPath = join(project, "src", "legacy", "xml-scene-documents.json");
const catalogPath = join(project, "src", "legacy", "object-library-catalog.json");
const publicRoot = join(project, "public");
const sha256 = (data) => createHash("sha256").update(data).digest("hex").toUpperCase();

const normalized = JSON.parse(readFileSync(normalizedPath, "utf8"));
const catalog = JSON.parse(readFileSync(catalogPath, "utf8"));
const source = normalized.documents.find((document) => document.id === "object-library");
assert(source, "normalized ObjectLibrary source is required");

assert.equal(source.objectCount, 61);
assert.equal(catalog.sourceDocument.id, source.id);
assert.equal(catalog.sourceDocument.source, source.source);
assert.equal(catalog.sourceDocument.sha256, source.sha256);
assert.equal(catalog.sourceDocument.objectCount, 61);
assert.equal(catalog.objects.length, 61);
assert.equal(catalog.sourceDocument.classification, "editor object-library grid, not a composed world");

const authoredFields = [
  "index", "type", "action", "name", "enabled", "mass", "meshControlled",
  "newtonMaterial", "position", "rotation", "scale", "pathToMesh", "textureName"
];
for (let index = 0; index < source.objects.length; index += 1) {
  const original = source.objects[index];
  const recovered = catalog.objects[index];
  for (const field of authoredFields) {
    assert.deepEqual(recovered[field], original[field], `object ${index} changed authored field ${field}`);
  }
  assert.deepEqual(recovered.resolution, original.resolution, `object ${index} changed normalized resolution`);
  assert.deepEqual(recovered.archiveResolution, original.resolution, `object ${index} changed archive resolution`);
}

assert.deepEqual(catalog.familyCounts, {
  primitive: 5,
  pipe: 1,
  building: 5,
  technology: 1,
  aircraft: 1,
  nature: 16,
  camp: 4,
  port: 28
});
assert.deepEqual(catalog.recoveryCounts, {
  "recovered-procedural": 5,
  "missing-file": 11,
  "recovered-tvm": 2,
  "missing-duplicate-source": 2,
  "recovered-text-x": 40,
  "unsupported-binary-x": 1
});

const objectStatusCount = (predicate) => catalog.objects.filter((object) => predicate(object.catalogResolution.status)).length;
assert.equal(objectStatusCount((status) => status.startsWith("recovered-")), 47);
assert.equal(objectStatusCount((status) => status.startsWith("missing-")), 13);
assert.equal(objectStatusCount((status) => status.startsWith("unsupported-")), 1);
assert.equal(Object.keys(catalog.assets.x).length, 40);
assert.deepEqual(Object.keys(catalog.assets.tvm), ["Prisme", "Airplane"]);

for (const object of catalog.objects) {
  const resolution = object.catalogResolution;
  if (resolution.status === "recovered-text-x") assert(catalog.assets.x[resolution.assetId], `${object.name}: X asset missing`);
  if (resolution.status === "recovered-tvm") assert(catalog.assets.tvm[resolution.assetId], `${object.name}: TVM asset missing`);
  if (resolution.status.startsWith("missing-") || resolution.status.startsWith("unsupported-")) {
    assert.equal(resolution.assetId, undefined, `${object.name}: unresolved record received geometry`);
  }
}

let xVertices = 0;
let xTriangles = 0;
let xMeshes = 0;
let xGroups = 0;
for (const [assetId, asset] of Object.entries(catalog.assets.x)) {
  const sourcePath = join(workspace, ...asset.source.split("/"));
  assert(existsSync(sourcePath), `${assetId}: X source missing`);
  const sourceBytes = readFileSync(sourcePath);
  assert.equal(sourceBytes.length, asset.bytes, `${assetId}: source byte count changed`);
  assert.equal(sha256(sourceBytes), asset.sha256, `${assetId}: source hash changed`);
  assert.equal(asset.meshCount, asset.meshes.length, `${assetId}: mesh count mismatch`);
  assert.equal(asset.vertexCount, asset.meshes.reduce((sum, mesh) => sum + mesh.vertexCount, 0));
  assert.equal(asset.triangleCount, asset.meshes.reduce((sum, mesh) => sum + mesh.triangleCount, 0));
  assert.equal(asset.materialGroupCount, asset.meshes.reduce((sum, mesh) => sum + mesh.groups.length, 0));
  assert.equal(asset.normals, "derived at runtime from exact transformed vertices and triangulated faces");
  for (const mesh of asset.meshes) {
    assert.equal(mesh.positions.length, mesh.vertexCount * 3, `${assetId}/${mesh.name}: position count`);
    assert(mesh.uvs === null || mesh.uvs.length === mesh.vertexCount * 2, `${assetId}/${mesh.name}: UV count`);
    assert.equal(mesh.indices.length, mesh.triangleCount * 3, `${assetId}/${mesh.name}: index count`);
    assert(mesh.indices.every((value) => Number.isInteger(value) && value >= 0 && value < mesh.vertexCount), `${assetId}/${mesh.name}: index range`);
    assert.equal(mesh.groups.reduce((sum, group) => sum + group.count, 0), mesh.indices.length, `${assetId}/${mesh.name}: group coverage`);
    for (const group of mesh.groups) {
      assert.equal(group.start % 3, 0, `${assetId}/${mesh.name}: group start alignment`);
      assert.equal(group.count % 3, 0, `${assetId}/${mesh.name}: group count alignment`);
      assert(group.materialIndex >= 0 && group.materialIndex < mesh.materials.length, `${assetId}/${mesh.name}: material index`);
    }
    xMeshes += 1;
    xGroups += mesh.groups.length;
  }
  xVertices += asset.vertexCount;
  xTriangles += asset.triangleCount;
}

for (const [assetId, asset] of Object.entries(catalog.assets.tvm)) {
  const sourcePath = join(workspace, ...asset.source.split("/"));
  assert(existsSync(sourcePath), `${assetId}: TVM source missing`);
  const sourceBytes = readFileSync(sourcePath);
  assert.equal(sourceBytes.length, asset.bytes, `${assetId}: source byte count changed`);
  assert.equal(sha256(sourceBytes), asset.sha256, `${assetId}: source hash changed`);
  assert.equal(asset.positions.length, asset.vertexCount * 3, `${assetId}: position count`);
  assert.equal(asset.normals.length, asset.vertexCount * 3, `${assetId}: normal count`);
  assert.equal(asset.uvs.length, asset.vertexCount * 2, `${assetId}: UV count`);
  assert.equal(asset.indices.length, asset.triangleCount * 3, `${assetId}: index count`);
  assert(asset.indices.every((value) => Number.isInteger(value) && value >= 0 && value < asset.vertexCount), `${assetId}: index range`);
  assert.equal(asset.groups.reduce((sum, group) => sum + group.triangleCount, 0), asset.triangleCount, `${assetId}: group coverage`);
  for (const group of asset.groups) {
    assert(group.materialIndex >= 0 && group.materialIndex < asset.materials.length, `${assetId}: material group index`);
  }
}
assert.deepEqual(
  Object.fromEntries(Object.entries(catalog.assets.tvm).map(([id, asset]) => [id, [asset.vertexCount, asset.triangleCount, asset.groups.length]])),
  { Prisme: [148, 169, 1], Airplane: [5367, 9202, 8] }
);

assert.equal(catalog.textureRecords.length, 71);
assert.equal(catalog.textureRecords.filter((record) => record.conversion === "lossless-pixel TGA to PNG").length, 21);
assert.equal(catalog.textureRecords.filter((record) => record.runtimeColorKey === "magenta").length, 11);
for (const record of catalog.textureRecords) {
  const sourcePath = join(workspace, ...record.source.split("/"));
  const outputPath = join(publicRoot, ...record.url.replace(/^\//, "").split("/"));
  assert(existsSync(sourcePath), `${record.source}: texture source missing`);
  assert(existsSync(outputPath), `${record.url}: texture output missing`);
  const sourceBytes = readFileSync(sourcePath);
  const outputBytes = readFileSync(outputPath);
  assert.equal(sourceBytes.length, record.sourceBytes, `${record.source}: texture source byte count`);
  assert.equal(outputBytes.length, record.outputBytes, `${record.url}: texture output byte count`);
  assert.equal(sha256(sourceBytes), record.sourceSha256, `${record.source}: texture source hash`);
  assert.equal(sha256(outputBytes), record.outputSha256, `${record.url}: texture output hash`);
  if (extname(sourcePath).toLowerCase() !== ".tga") assert.deepEqual(outputBytes, sourceBytes, `${record.url}: copied texture not byte-identical`);
  assert(record.exactMagentaPixelRatio >= 0 && record.exactMagentaPixelRatio <= 1, `${record.source}: invalid magenta ratio`);
  assert.equal(record.runtimeColorKey === "magenta", record.exactMagentaPixelRatio >= 0.05, `${record.source}: color-key threshold`);
}

assert(catalog.evidenceBoundary.absent.some((statement) => statement.includes("not a village")));
assert(catalog.evidenceBoundary.inspectionOnly.some((statement) => statement.includes("Camera, lights")));
assert(!JSON.stringify(catalog.objects).toLowerCase().includes("proxygeometry"));

console.log("ObjectLibrary catalog verification passed");
console.log(JSON.stringify({
  objects: catalog.objects.length,
  recovered: 47,
  missing: 13,
  unsupported: 1,
  xAssets: Object.keys(catalog.assets.x).length,
  xMeshes,
  xVertices,
  xTriangles,
  xMaterialGroups: xGroups,
  tvmAssets: Object.keys(catalog.assets.tvm).length,
  textures: catalog.textureRecords.length
}, null, 2));
