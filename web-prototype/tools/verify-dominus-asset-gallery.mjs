#!/usr/bin/env node

import assert from "node:assert/strict";
import { createHash } from "node:crypto";
import { existsSync, readFileSync } from "node:fs";
import { dirname, extname, join, resolve } from "node:path";
import { fileURLToPath } from "node:url";

const here = dirname(fileURLToPath(import.meta.url));
const project = resolve(here, "..");
const workspace = resolve(project, "..");
const publicRoot = join(project, "public");
const audit = JSON.parse(readFileSync(join(project, "src", "legacy", "dominus-family-audit.json"), "utf8"));
const manifest = JSON.parse(readFileSync(join(project, "src", "legacy", "dominus-asset-gallery.json"), "utf8"));
const sha256 = (data) => createHash("sha256").update(data).digest("hex").toUpperCase();

assert.equal(manifest.format, "graphysx-dominus-asset-gallery/v1");
assert.equal(manifest.classification, "source asset family only; no authored composition survives");
assert.equal(manifest.assets.length, 65);
assert.equal(new Set(manifest.assets.map((asset) => asset.id)).size, 65);
assert.equal(new Set(manifest.assets.map((asset) => asset.sha256)).size, 65);
assert.deepEqual(manifest.familyCounts, { bush: 4, camp: 4, character: 8, grass: 5, port: 28, weapon: 6, tree: 10 });
assert.deepEqual(manifest.familyCounts, audit.inventory.meshCategories);
assert.deepEqual(manifest.recoveryCounts, { "recovered-text-x": 63, "unsupported-binary-x": 2 });
assert.deepEqual(manifest.assets.filter((asset) => asset.status === "unsupported-binary-x").map((asset) => asset.name), ["port_crateshed.X", "renzokscale.X"]);

let vertices = 0;
let triangles = 0;
let materialGroups = 0;
let meshParts = 0;
for (let index = 0; index < manifest.assets.length; index += 1) {
  const asset = manifest.assets[index];
  const audited = audit.inventory.meshes[index];
  assert.equal(asset.index, index);
  assert.equal(asset.name, audited.name);
  assert.equal(asset.family, audited.category);
  assert.equal(asset.source, audited.source);
  assert.equal(asset.bytes, audited.bytes);
  assert.equal(asset.sha256, audited.sha256);
  assert.deepEqual(asset.textureReferences, audited.textureReferences);
  const sourcePath = join(workspace, ...asset.source.split("/"));
  const sourceBytes = readFileSync(sourcePath);
  assert.equal(sourceBytes.length, asset.bytes, `${asset.id}: source byte count`);
  assert.equal(sha256(sourceBytes), asset.sha256, `${asset.id}: source hash`);
  if (asset.status === "unsupported-binary-x") {
    assert.equal(asset.payloadUrl, null, `${asset.id}: binary received payload`);
    assert.equal(asset.vertexCount, 0, `${asset.id}: binary received vertices`);
    assert.equal(asset.triangleCount, 0, `${asset.id}: binary received triangles`);
    assert.equal(asset.materialGroupCount, 0, `${asset.id}: binary received material groups`);
    continue;
  }
  assert.equal(asset.status, "recovered-text-x");
  const payloadPath = join(publicRoot, ...asset.payloadUrl.replace(/^\//, "").split("/"));
  assert(existsSync(payloadPath), `${asset.id}: payload missing`);
  const payloadBytes = readFileSync(payloadPath);
  assert.equal(payloadBytes.length, asset.payloadBytes, `${asset.id}: payload byte count`);
  assert.equal(sha256(payloadBytes), asset.payloadSha256, `${asset.id}: payload hash`);
  const payload = JSON.parse(payloadBytes);
  assert.equal(payload.source, asset.source);
  assert.equal(payload.sha256, asset.sha256);
  assert.deepEqual(payload.bounds, asset.bounds);
  assert.equal(payload.meshCount, asset.meshCount);
  assert.equal(payload.vertexCount, asset.vertexCount);
  assert.equal(payload.triangleCount, asset.triangleCount);
  assert.equal(payload.materialGroupCount, asset.materialGroupCount);
  assert.equal(payload.meshes.length, asset.meshCount);
  assert.equal(payload.normals, "derived at runtime from exact transformed vertices and triangulated faces");
  for (const mesh of payload.meshes) {
    assert.equal(mesh.positions.length, mesh.vertexCount * 3, `${asset.id}/${mesh.name}: position count`);
    assert(mesh.uvs === null || mesh.uvs.length === mesh.vertexCount * 2, `${asset.id}/${mesh.name}: UV count`);
    assert.equal(mesh.indices.length, mesh.triangleCount * 3, `${asset.id}/${mesh.name}: index count`);
    assert(mesh.indices.every((value) => Number.isInteger(value) && value >= 0 && value < mesh.vertexCount), `${asset.id}/${mesh.name}: index range`);
    assert.equal(mesh.groups.reduce((sum, group) => sum + group.count, 0), mesh.indices.length, `${asset.id}/${mesh.name}: group coverage`);
    for (const group of mesh.groups) {
      assert.equal(group.start % 3, 0, `${asset.id}/${mesh.name}: group start alignment`);
      assert.equal(group.count % 3, 0, `${asset.id}/${mesh.name}: group count alignment`);
      assert(group.materialIndex >= 0 && group.materialIndex < mesh.materials.length, `${asset.id}/${mesh.name}: material index`);
    }
    const referenced = mesh.materials.filter((material) => material.textureName);
    assert(referenced.every((material) => material.textureUrl), `${asset.id}/${mesh.name}: unresolved material texture`);
    meshParts += 1;
  }
  vertices += payload.vertexCount;
  triangles += payload.triangleCount;
  materialGroups += payload.materialGroupCount;
}
assert.equal(vertices, 40_836);
assert.equal(triangles, 26_317);
assert.equal(materialGroups, 308);

assert.equal(manifest.textureRecords.length, 83);
assert.equal(manifest.textureRecords.filter((record) => record.conversion === "lossless-pixel TGA to PNG").length, 31);
assert.equal(manifest.textureRecords.filter((record) => record.runtimeColorKey === "magenta").length, 21);
for (const record of manifest.textureRecords) {
  const sourcePath = join(workspace, ...record.source.split("/"));
  const outputPath = join(publicRoot, ...record.url.replace(/^\//, "").split("/"));
  const sourceBytes = readFileSync(sourcePath);
  const outputBytes = readFileSync(outputPath);
  assert.equal(sourceBytes.length, record.sourceBytes, `${record.source}: source byte count`);
  assert.equal(outputBytes.length, record.outputBytes, `${record.url}: output byte count`);
  assert.equal(sha256(sourceBytes), record.sourceSha256, `${record.source}: source hash`);
  assert.equal(sha256(outputBytes), record.outputSha256, `${record.url}: output hash`);
  if (extname(sourcePath).toLowerCase() !== ".tga") assert.deepEqual(outputBytes, sourceBytes, `${record.url}: copied texture changed`);
  assert.equal(record.runtimeColorKey === "magenta", record.exactMagentaPixelRatio >= 0.05, `${record.url}: magenta threshold`);
}

assert(manifest.evidenceBoundary.absent.some((statement) => statement.includes("No authored scene composition")));
assert(manifest.evidenceBoundary.absent.some((statement) => statement.includes("not a village")));
assert(!Object.hasOwn(manifest, "layout"));
assert(!Object.hasOwn(manifest, "camera"));
assert(!Object.hasOwn(manifest, "spawn"));

console.log("Dominus asset gallery verification passed");
console.log(JSON.stringify({
  assets: manifest.assets.length,
  recoveredTextX: manifest.recoveryCounts["recovered-text-x"],
  unsupportedBinaryX: manifest.recoveryCounts["unsupported-binary-x"],
  meshParts,
  vertices,
  triangles,
  materialGroups,
  textures: manifest.textureRecords.length,
  magentaColorKeys: manifest.textureRecords.filter((record) => record.runtimeColorKey === "magenta").length
}, null, 2));
