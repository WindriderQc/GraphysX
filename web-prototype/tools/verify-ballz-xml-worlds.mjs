#!/usr/bin/env node

import assert from "node:assert/strict";
import { createHash } from "node:crypto";
import { readFileSync } from "node:fs";
import { dirname, join, resolve } from "node:path";
import { fileURLToPath } from "node:url";

const here = dirname(fileURLToPath(import.meta.url));
const project = resolve(here, "..");
const workspace = resolve(project, "..");
const media = join(workspace, "Archive", "bckup", "BallZ2015.bckup", "Media");
const manifest = JSON.parse(readFileSync(join(project, "src", "legacy", "ballz-xml-worlds.json"), "utf8"));
const hash = (path) => createHash("sha256").update(readFileSync(path)).digest("hex").toUpperCase();
const scenes = new Map(manifest.scenes.map((scene) => [scene.id, scene]));

assert.equal(manifest.format, "graphysx-ballz-xml-worlds/v1");
assert.deepEqual(manifest.classification, {
  sceneCount: 2,
  assembledDiscoverableWorldCount: 0,
  distinctSerializedCompositionCount: 2,
  reason: manifest.classification.reason
});
assert.equal(scenes.size, 2);

const myworld = scenes.get("myworld");
assert.ok(myworld);
assert.equal(myworld.sha256, hash(join(media, "MyWorld.xml")));
assert.equal(myworld.objectCount, 4);
assert.equal(myworld.renderedObjectCount, 2);
assert.equal(myworld.actions[14], "DUPLICATE");
assert.deepEqual(myworld.resolutionCounts, {
  "exact-file": 1,
  "exact-procedural-primitive": 1,
  "invalid-duplicate-target": 2
});
assert.deepEqual(myworld.objects.slice(0, 2).map((object) => [object.name, object.action, object.position, object.scale]), [
  ["Airplane", "PHYSICCUSTOM", [0, 10, 0], [1, 1, 1]],
  ["Cylinder", "PHYSICSCYLINDER", [0.5, -0.005, 0], [0.05, 0.1, 1]]
]);
assert.ok(myworld.objects.slice(2).every((object) =>
  object.action === "DUPLICATE"
  && object.resolution.status === "invalid-duplicate-target"
  && object.resolution.coincidentFile
));

const testworld = scenes.get("testworld");
assert.ok(testworld);
assert.equal(testworld.sha256, hash(join(media, "TestWorld.xml")));
assert.equal(testworld.objectCount, 8);
assert.equal(testworld.renderedObjectCount, 5);
assert.equal(testworld.actions[13], "DUPLICATE");
assert.deepEqual(testworld.objects.slice(0, 5).map((object) => [object.name, object.action, object.position]), [
  ["CubeTestPhys", "PHYSICCUBE", [5, 0, 0]],
  ["SphereTestPhys", "PHYSICSPHERE", [10, 0, 0]],
  ["CylinderTestPhys", "PHYSICSCYLINDER", [15, 0, 0]],
  ["ConeTestPhys", "PHYSICCONE", [20, 0, 0]],
  ["Airplane", "PHYSICCUSTOM", [25, 0, 0]]
]);
assert.equal(testworld.objects[5].resolution.status, "missing-file");
assert.equal(testworld.objects[5].resolution.requestedPath.toLowerCase(), "c:\\\\media\\\\nature\\\\bush1.x");
assert.ok(testworld.objects.slice(6).every((object) => object.action === "DUPLICATE" && object.resolution.targetName === null));

const expectedAssets = {
  "airplane-high": { vertices: 10261, triangles: 11168, groups: 48, format: "MGR4-172", file: "Airplane.tvm" },
  "airplane-lp": { vertices: 5367, triangles: 9202, groups: 8, format: "MGRP-416", file: "AirplaneLP.TVM" }
};
for (const [id, expected] of Object.entries(expectedAssets)) {
  const asset = manifest.tvmAssets[id];
  assert.ok(asset, `missing ${id}`);
  assert.equal(asset.vertexCount, expected.vertices);
  assert.equal(asset.triangleCount, expected.triangles);
  assert.equal(asset.groups.length, expected.groups);
  assert.equal(asset.materials.length, expected.groups);
  assert.equal(asset.materialFormat, expected.format);
  assert.equal(asset.positions.length, asset.vertexCount * 3);
  assert.equal(asset.normals.length, asset.vertexCount * 3);
  assert.equal(asset.uvs.length, asset.vertexCount * 2);
  assert.equal(asset.indices.length, asset.triangleCount * 3);
  assert.equal(asset.materialIndexByTriangle.length, asset.triangleCount);
  assert.equal(asset.sha256, hash(join(media, "Airplane", expected.file)));
}
assert.deepEqual(
  manifest.tvmAssets["airplane-lp"].materials.filter((material) => material.textureName).map((material) => material.textureName),
  ["FUS.BMP", "HTAIL.BMP", "RED.BMP", "VTAIL.BMP", "WHEEL.BMP", "WING.BMP"]
);

assert.equal(manifest.exactCopies.length, 8);
assert.ok(manifest.exactCopies.every((copy) => copy.byteIdentical && copy.archive.sha256 === copy.stockRoom.sha256));
for (const name of ["FUS.BMP", "HTAIL.BMP", "RED.BMP", "VTAIL.BMP", "WHEEL.BMP", "WING.BMP"]) {
  assert.equal(hash(join(project, "public", "assets", "ballz-xml-worlds", "airplane", name)), hash(join(media, "Airplane", name)));
}
assert.equal(hash(join(project, "public", "assets", "ballz-xml-worlds", "yellowtwoway.jpg")), hash(join(media, "yellowtwoway.jpg")));

assert.ok(manifest.parserRule.includes("own Actions header"));
assert.ok(manifest.hostEvidence.callsites.some((entry) => entry.includes("No surviving literal TestWorld.xml callsite")));
assert.ok(manifest.evidenceBoundary.unresolved.some((entry) => entry.includes("not substituted")));

console.log("BallZ XML worlds verification passed (36 invariant groups)");
console.log("- MyWorld.xml: 2/4 exact renderable records; malformed duplicates remain unresolved");
console.log("- TestWorld.xml: 5/8 exact renderable records; bush source/duplicates remain unresolved");
console.log("- exact TVM geometry: 15,628 vertices / 20,370 triangles across two airplane revisions");
console.log("- 8 archive/StockRoom copy pairs and 7 browser texture outputs are byte-identical");

