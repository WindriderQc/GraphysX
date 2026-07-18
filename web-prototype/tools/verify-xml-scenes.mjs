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

const audit = JSON.parse(readFileSync(join(project, "src", "legacy", "xml-scene-documents.json"), "utf8"));
const selected = JSON.parse(readFileSync(join(project, "src", "legacy", "xml-myworld-copy-scene.json"), "utf8"));
const documents = new Map(audit.documents.map((document) => [document.id, document]));

const hash = (path) => createHash("sha256").update(readFileSync(path)).digest("hex").toUpperCase();
const count = (id, expected) => {
  const document = documents.get(id);
  assert.ok(document, `missing audited document: ${id}`);
  assert.equal(document.objectCount, expected, `${id}: unexpected object count`);
  assert.equal(document.objects.length, expected, `${id}: object array/count mismatch`);
  return document;
};

assert.equal(audit.parser.id, "graphysx-xml-scene/v1");
assert.deepEqual(audit.parser.supportedRoots, ["CScene3D", "Scene3D"]);
assert.equal(audit.selection.selectedId, "myworld-copy");
assert.equal(documents.size, 6);

const myworld = count("myworld", 4);
assert.equal(myworld.classification, "broken authored editor save");
assert.equal(myworld.resolutionCounts["invalid-duplicate-target"], 2);

const myworldCopy = count("myworld-copy", 7);
assert.equal(myworldCopy.classification, "authored scene-builder test composition");
assert.deepEqual(myworldCopy.resolutionCounts, {
  "procedural-primitive": 4,
  "missing-file": 1,
  "exact-file": 2
});
assert.deepEqual(
  myworldCopy.objects.slice(0, 4).map((object) => [object.action, object.position]),
  [
    ["PHYSICCUBE", [5, 0, 0]],
    ["PHYSICSPHERE", [10, 0, 0]],
    ["PHYSICSCYLINDER", [15, 0, 0]],
    ["PHYSICCONE", [20, 0, 0]]
  ]
);
assert.deepEqual(myworldCopy.objects.find((object) => object.name === "ArcheChinois")?.position, [0, 0, 50]);
assert.equal(myworldCopy.objects.find((object) => object.name === "ArcheChinois")?.resolution.status, "missing-file");
assert.deepEqual(myworldCopy.objects.find((object) => object.name === "Airplane")?.position, [33.5003052, 6.75325632, 16.09642]);
assert.equal(myworldCopy.objects.find((object) => object.name === "Level2")?.resolution.status, "exact-file");

const testworld = count("testworld", 8);
assert.equal(testworld.classification, "scene-loader coverage test");
assert.equal(testworld.resolutionCounts["invalid-duplicate-target"], 2);

const objectLibrary = count("object-library", 61);
assert.equal(objectLibrary.classification, "editor object-library grid, not a composed world");
assert.deepEqual(objectLibrary.resolutionCounts, {
  "procedural-primitive": 5,
  "missing-file": 52,
  "exact-file": 2,
  "duplicate-target": 2
});

const baseScene = count("base-scene", 18);
assert.equal(baseScene.classification, "serializer/template artifact");
assert.ok(baseScene.objects.every((object) =>
  object.action === "PHYSICCUBE"
  && JSON.stringify(object.position) === "[0,0,0]"
  && JSON.stringify(object.scale) === "[10,10,10]"
));

const test1 = count("test1", 1);
assert.equal(test1.classification, "minimal serializer smoke document");
assert.equal(test1.objects[0].action, "PHYSICCUBE");

assert.equal(selected.format, "graphysx-xml-scene/v1");
assert.equal(selected.sourceDocument.sha256, myworldCopy.sha256);
assert.deepEqual(Object.keys(selected.exactTvmAssets), ["Airplane", "Level2"]);
assert.deepEqual(
  {
    vertices: selected.exactTvmAssets.Airplane.vertexCount,
    triangles: selected.exactTvmAssets.Airplane.triangleCount,
    materials: selected.exactTvmAssets.Airplane.materials.length,
    groups: selected.exactTvmAssets.Airplane.groups.length
  },
  { vertices: 5367, triangles: 9202, materials: 8, groups: 8 }
);
assert.deepEqual(
  {
    vertices: selected.exactTvmAssets.Level2.vertexCount,
    triangles: selected.exactTvmAssets.Level2.triangleCount,
    materials: selected.exactTvmAssets.Level2.materials.length,
    groups: selected.exactTvmAssets.Level2.groups.length
  },
  { vertices: 448, triangles: 692, materials: 2, groups: 2 }
);

for (const asset of Object.values(selected.exactTvmAssets)) {
  assert.equal(asset.indices.length, asset.triangleCount * 3);
  assert.equal(asset.positions.length, asset.vertexCount * 3);
  assert.equal(asset.normals.length, asset.vertexCount * 3);
  assert.equal(asset.uvs.length, asset.vertexCount * 2);
  assert.equal(asset.materialIndexByTriangle.length, asset.triangleCount);
  assert.equal(hash(join(workspace, asset.source)), asset.sha256);
}

const texturePairs = [
  ...["FUS.BMP", "HTAIL.BMP", "RED.BMP", "VTAIL.BMP", "WHEEL.BMP", "WING.BMP"].map((file) => [
    join(media, "Airplane", file),
    join(project, "public", "assets", "xml-scenes", "airplane", file)
  ]),
  [join(media, "Level2.png"), join(project, "public", "assets", "xml-scenes", "Level2.png")],
  [join(media, "twoway.jpg"), join(project, "public", "assets", "xml-scenes", "twoway.jpg")]
];
for (const [source, copy] of texturePairs) {
  assert.equal(hash(copy), hash(source), `copied texture differs from archive source: ${copy}`);
}

console.log("XML scene verification passed");
console.log("- 6 documents and 99 serialized objects audited");
console.log("- MyWorld - Copie: 6/7 objects recoverable (4 procedural + 2 exact TVM)");
console.log("- exact TVM geometry/material counts and 8 byte-identical textures verified");
console.log("- ObjectLibrary grid and BaseScene/test1 artifact boundaries verified");
