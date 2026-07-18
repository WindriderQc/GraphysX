// Static verification for the generated slide/track-family audit.
// Run after `node tools/convert-ballz-slide-track-family.mjs`.

import { createHash } from "node:crypto";
import { readFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { fileURLToPath } from "node:url";

const here = dirname(fileURLToPath(import.meta.url));
const data = JSON.parse(readFileSync(join(here, "..", "src", "legacy", "ballz-slide-track-family.json"), "utf8"));
const asset = (id) => {
  const found = data.assets.find((candidate) => candidate.id === id);
  if (!found) throw new Error(`Missing audited asset ${id}.`);
  return found;
};
const assert = (condition, message) => {
  if (!condition) throw new Error(message);
};
const sha256 = (value) => createHash("sha256").update(value).digest("hex").toUpperCase();

assert(data.assets.length === 10, "Expected ten audited slide/track assets or revisions.");
assert(data.assets.every((entry) => entry.copiesByteIdentical), "A same-ID archive copy changed bytes.");
assert(data.assets.reduce((sum, entry) => sum + entry.knownCopies.length, 0) === 32, "Expected 32 verified surviving copies.");

const activeSlide = asset("slide1-atmel-active");
const stockroomSlide = asset("slide1-stockroom-revision");
assert(activeSlide.sha256 === "3180052AC51E9C5DFDB25302E66F3C976B771987B7BC292A61988EADD2E75602", "Active Slide1 hash changed.");
assert(activeSlide.vertexCount === 566 && activeSlide.triangleCount === 552, "Active Slide1 geometry counts changed.");
assert(activeSlide.geometry && activeSlide.geometry.positions.length === 566 * 3, "Selected Slide1 geometry is not exported.");
assert(stockroomSlide.sha256 === "B1B5066CE91B8A27E4BBB2CD11BFBC2D00B36CAB695A248AD5EDC9F48E95839C", "StockRoom Slide1 hash changed.");
assert(activeSlide.sha256 !== stockroomSlide.sha256, "Distinct Slide1 revisions were incorrectly merged.");

assert(asset("slide-long1").sha256 === "5748B62F50511527A5E0F3DB3C4EF2D1E836D05A49247CA0E75C4AD20CDD5E61", "SlideLong1 no longer aliases BallZ 2011 Level1.");
assert(asset("pipe1").sha256 === "36C0B46C8DAA424A252B7DE0D46A74E65B8807122A7DCB21F44B8EC25DE7284A", "Pipe1 hash changed.");
assert(asset("pipe1").role === "flightx-component-not-distinct-ballz-scene", "Pipe1 classification changed.");

const bump = asset("slide-bump");
const gridBump = asset("slide-bump-gridtex");
assert(JSON.stringify(bump.bounds) === JSON.stringify(gridBump.bounds), "Slide Bump variants no longer share bounds.");
assert(bump.triangleCount === gridBump.triangleCount && bump.vertexCount !== gridBump.vertexCount, "Slide Bump remap evidence changed.");
assert(gridBump.uv.uniquePairCount > bump.uv.uniquePairCount, "GridTex should preserve a richer UV mapping.");

const assembly = data.sourceBehavior.selectedAssembly;
assert(JSON.stringify(assembly.slide.position) === JSON.stringify([0, -5000, 0]), "Slide1 source transform changed.");
assert(JSON.stringify(assembly.ball.position) === JSON.stringify([-50, -4750, -250]), "Ball source spawn changed.");
assert(JSON.stringify(assembly.camera.offset) === JSON.stringify([0, 350, 300]), "Source chase offset changed.");
assert(assembly.controls.status === "partial-source-implementation", "Incomplete source controls must remain disclosed.");
assert(assembly.rings.status === "source-exact-placeholder-not-gameplay-fidelity", "Placeholder rings must not become a claimed objective.");

const normalization = data.selection.displayNormalization;
const sourcePoint = assembly.ball.position;
const displayPoint = sourcePoint.map((value, axis) => (value - normalization.anchor[axis]) * normalization.displayScale);
const restoredPoint = displayPoint.map((value, axis) => value / normalization.displayScale + normalization.anchor[axis]);
const roundTripError = Math.max(...restoredPoint.map((value, axis) => Math.abs(value - sourcePoint[axis])));
assert(roundTripError < 1e-9, "Slide1 display normalization is not reversible.");

const gallery = data.remainingGallery;
const expectedGalleryIds = ["slide1a-legacy-active", "level-slides", "level-steps", "slide-bump", "slide-bump-gridtex", "ballz-track1"];
assert(gallery.schema === "graphysx.ballz-track-gallery/v1", "Remaining-gallery schema changed.");
assert(JSON.stringify(gallery.assetIds) === JSON.stringify(expectedGalleryIds), "Remaining-gallery asset list changed.");
assert(gallery.excludedAliases.map((entry) => entry.id).join(",") === "slide-long1,pipe1", "Known aliases leaked into the remaining gallery.");
assert(gallery.excludedRevision.id === "slide1-stockroom-revision", "Same-name StockRoom Slide1 revision boundary changed.");
assert(gallery.statusRecommendations.every((entry) => entry.status === "PARTIAL"), "Implemented gallery visits should remain evidence-bounded PARTIAL recommendations.");

for (const id of expectedGalleryIds) {
  const entry = asset(id);
  assert(entry.geometry, `${id} exact geometry is not exported.`);
  assert(entry.geometry.positions.length === entry.vertexCount * 3, `${id} position count changed.`);
  assert(entry.geometry.normals.length === entry.vertexCount * 3, `${id} normal count changed.`);
  assert(entry.geometry.uvs.length === entry.vertexCount * 2, `${id} UV count changed.`);
  assert(entry.geometry.indices.length === entry.triangleCount * 3, `${id} index count changed.`);
  assert(entry.geometry.materialIndexByTriangle.length === entry.triangleCount, `${id} material assignment count changed.`);
  const galleryEntry = gallery.assets.find((candidate) => candidate.id === id);
  assert(galleryEntry, `${id} gallery evidence record is missing.`);
  assert(galleryEntry.normalization.preservesAspectAndOrientation, `${id} display normalization stopped preserving shape/orientation.`);
  const point = galleryEntry.normalization.sourceWorldBounds.min;
  const display = point.map((value, axis) => (value - galleryEntry.normalization.anchor[axis]) * galleryEntry.normalization.displayScale);
  const restored = display.map((value, axis) => value / galleryEntry.normalization.displayScale + galleryEntry.normalization.anchor[axis]);
  assert(Math.max(...restored.map((value, axis) => Math.abs(value - point[axis]))) < 1e-9, `${id} gallery normalization is not reversible.`);
}

const slides = asset("level-slides");
assert(slides.embeddedMaterialRecords.length === 3 && slides.embeddedMaterialRecords.every((record) => !record.textureName), "Level.Slides exact three untextured material records changed.");
const steps = asset("level-steps");
assert(JSON.stringify(steps.embeddedMaterialRecords.map((record) => record.textureName)) === JSON.stringify(["grass.jpg", "concrete.png", "wood.jpg"]), "Level.Steps material-to-texture bindings changed.");
assert(asset("slide-bump").embeddedMaterialRecords[0].rgbaSlots.flat().every((value) => value === 0), "SlideBump zeroed material evidence changed.");
assert(asset("slide-bump-gridtex").embeddedMaterialRecords[0].textureName === "EarthGri.bmp", "SlideBumpGridTex EarthGri binding changed.");
assert(gallery.assets.find((entry) => entry.id === "slide1a-legacy-active").host.position[1] === -5000, "Slide1A exact host position changed.");
assert(JSON.stringify(gallery.assets.find((entry) => entry.id === "ballz-track1").host.ballSpawn) === JSON.stringify([-20, 310, 225]), "BallZTrack1 source ball spawn changed.");

for (const texture of Object.values(gallery.textureCatalog)) {
  const localPath = join(here, "..", "public", texture.browserPath.replace(/^\/assets\//, "assets/"));
  assert(sha256(readFileSync(localPath)) === texture.sha256, `${texture.embeddedName} browser copy changed bytes.`);
}

console.log("slide/track family verification passed");
console.log(`10 assets/revisions; 32 byte-verified copies; 6 exact-geometry remaining visits; selected ${activeSlide.vertexCount}v/${activeSlide.triangleCount}t`);
