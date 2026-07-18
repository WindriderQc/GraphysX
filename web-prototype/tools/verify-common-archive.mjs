import assert from "node:assert/strict";
import { createHash } from "node:crypto";
import { readFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { fileURLToPath } from "node:url";

const here = dirname(fileURLToPath(import.meta.url));
const project = join(here, "..");
const workspace = join(project, "..");
const catalog = JSON.parse(readFileSync(join(project, "src", "legacy", "common-scenes.json"), "utf8"));

const room = catalog.scenes.find((scene) => scene.id === "common-room");
const sky = catalog.scenes.find((scene) => scene.id === "common-sky-space");
assert(room, "common/room.tvm conversion is missing");
assert(sky, "common/sky.tvm conversion is missing");

assert.deepEqual(
  [room.vertexCount, room.triangleCount, room.groups.length, room.materials.length],
  [180, 250, 1, 1],
  "Room 1 geometry/material counts changed"
);
assert.deepEqual(
  [sky.vertexCount, sky.triangleCount, sky.groups.length, sky.materials.length],
  [726, 1200, 6, 6],
  "sky.tvm geometry/material counts changed"
);
assert.deepEqual([...new Set(sky.materialIndexByTriangle)], [0, 1, 2, 3, 4, 5]);
assert.match(sky.classification, /not an evidenced standalone scene/);
assert.equal(catalog.recoveryAssessment.exact.length, 3);
assert.equal(catalog.recoveryAssessment.inference.length, 2);

function sha256(path) {
  return createHash("sha256").update(readFileSync(path)).digest("hex").toUpperCase();
}

for (const file of ["tv3dlogo_d.dds", "tv3dlogo_n.dds", "mid_day.dds", "top_pole.dds", "bottom_pole.dds"]) {
  const source = sha256(join(workspace, "common", file));
  const publicCopy = sha256(join(project, "public", "assets", "textures", "common", file));
  assert.equal(publicCopy, source, `${file} public copy differs from the archived source`);
}

console.log("Common archive verification passed:");
console.log("- room.tvm: 180 vertices, 250 triangles, one exact diffuse/normal material");
console.log("- sky.tvm: 726 vertices, 1,200 triangles, six exact skydome materials");
console.log("- five public DDS dependencies are byte-identical to common/");
console.log("- sky.tvm remains classified as a component, not an invented standalone scene");
