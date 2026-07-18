import fs from "node:fs";
import path from "node:path";
import { chromium } from "playwright";

const url = process.env.GRAPHYSX_URL ?? "http://127.0.0.1:4184/dominus-asset-gallery-preview.html";
const outputDir = path.resolve("output/playwright/dominus-asset-gallery");
fs.mkdirSync(outputDir, { recursive: true });

const browser = await chromium.launch({ headless: true, args: ["--use-gl=angle", "--use-angle=swiftshader"] });
const page = await browser.newPage({ viewport: { width: 1280, height: 720 } });
const errors = [];
page.on("console", (message) => {
  if (message.type() === "error") errors.push({ type: "console", text: message.text() });
});
page.on("pageerror", (error) => errors.push({ type: "page", text: String(error) }));

const result = { url, assertions: [], states: {}, errors };
const assert = (condition, message) => {
  result.assertions.push({ pass: Boolean(condition), message });
  if (!condition) throw new Error(message);
};
const debug = (method, argument) => page.evaluate(({ method, argument }) => window.__DOMINUS_GALLERY_DEBUG__[method](argument), { method, argument });
const state = () => page.evaluate(() => window.__DOMINUS_GALLERY_DEBUG__.getState());
const screenshot = async (name) => {
  await page.waitForTimeout(160);
  await page.screenshot({ path: path.join(outputDir, `${name}.png`) });
};

try {
  await page.goto(url, { waitUntil: "domcontentloaded" });
  await page.waitForFunction(() => window.__DOMINUS_GALLERY_DEBUG__?.getState().ready === true, null, { timeout: 30_000 });
  let current = await state();
  assert(current.assetCount === 65 && current.recoveredCount === 63 && current.unsupportedCount === 2, "Gallery exposes 65 unique assets with the exact 63/2 recovery boundary");
  assert(JSON.stringify(current.familyCounts) === JSON.stringify({ bush: 4, camp: 4, character: 8, grass: 5, port: 28, weapon: 6, tree: 10 }), "All seven proven family counts are exact");
  assert(current.classification.includes("no authored composition survives"), "Gallery states that no authored composition survives");
  assert(current.evidence.absent.some((statement) => statement.includes("not a village")), "Gallery explicitly rejects a village/world interpretation");
  assert(current.coordinateSystem.includes("no inter-asset coordinate system"), "Structured state keeps local coordinates separate from composition");
  await screenshot("00-bush-local-asset");

  const inventory = current.assets;
  for (const asset of inventory) {
    assert(await debug("select", asset.id), `${asset.id} is selectable`);
    current = await state();
    assert(current.ready && current.loadError === null, `${asset.id} loads without error`);
    if (asset.status === "recovered-text-x") {
      assert(current.renderedGeometry && !current.inspectionMarkerOnly, `${asset.id} renders its decoded source geometry`);
      assert(current.sourceVertexCount === asset.vertices && current.sourceTriangleCount === asset.triangles && current.sourceMaterialGroupCount === asset.materialGroups, `${asset.id} browser counts match its manifest`);
    } else {
      assert(!current.renderedGeometry && current.inspectionMarkerOnly, `${asset.id} receives an inspection label and no proxy geometry`);
    }
  }

  const expectedFamilies = { bush: 4, camp: 4, character: 8, grass: 5, port: 28, weapon: 6, tree: 10 };
  for (const [family, count] of Object.entries(expectedFamilies)) {
    assert(await debug("setFamily", family), `${family} filter is accepted`);
    current = await state();
    assert(current.matchingCount === count && current.selectedFamily === family, `${family} filter exposes exactly ${count} assets`);
  }
  await debug("setFamily", "all");
  assert(await debug("setStatus", "unsupported"), "Binary-only status filter is accepted");
  current = await state();
  assert(current.matchingCount === 2 && current.inspectionMarkerOnly && !current.renderedGeometry, "Binary filter contains exactly two label-only records");
  assert(await debug("select", "renzokscale"), "renzokscale binary record is selectable");
  assert((await state()).inspectionMarkerOnly, "renzokscale remains label-only");
  assert(await debug("select", "port_crateshed"), "port_crateshed binary record is selectable");
  assert((await state()).inspectionMarkerOnly, "port_crateshed remains label-only");
  await screenshot("01-binary-x-boundary");

  assert(await debug("setFamily", "bush"), "A family with no unsupported entries remains selectable");
  current = await state();
  assert(current.familyFilter === "bush" && current.statusFilter === "all" && current.matchingCount === 4 && current.selectedFamily === "bush", "Impossible family/status combinations fall back to the selected family instead of showing stale geometry");
  assert(await debug("setStatus", "unsupported"), "Unsupported can be selected again from a recovered-only family");
  current = await state();
  assert(current.familyFilter === "all" && current.statusFilter === "unsupported" && current.matchingCount === 2, "Impossible status/family combinations fall back to all families instead of showing stale geometry");

  await debug("setStatus", "recovered");
  assert(await debug("select", "port_maindocks"), "port_maindocks exact text-X payload is selectable");
  current = await state();
  assert(current.sourceMeshCount === 1 && current.sourceVertexCount === 6384 && current.sourceTriangleCount === 3840 && current.sourceMaterialGroupCount === 15, "port_maindocks exact geometry/material counts are preserved");
  await screenshot("02-port-maindocks-local-asset");

  assert(await debug("select", "doman_m"), "doman_m character is selectable");
  current = await state();
  assert(current.selectedFamily === "character" && current.renderedGeometry, "Character family renders as a local asset, not a placed actor");
  await screenshot("03-character-local-asset");

  assert(await debug("select", "tree_green02"), "tree_green02 is selectable");
  current = await state();
  assert(current.selectedFamily === "tree" && current.textureReferences.length === 2, "Tree uses both decoded source texture groups");
  await screenshot("04-tree-color-keyed-local-asset");

  const orbitBefore = current.orbitAngleRadians;
  await debug("orbit", 0.3);
  assert(Math.abs((await state()).orbitAngleRadians - orbitBefore) > 0.29, "Deterministic orbit API changes the inspection camera");
  await debug("zoom", 1.25);
  assert(Math.abs((await state()).zoom - 1.25) < 0.001, "Deterministic zoom API changes the inspection camera");
  await page.evaluate(() => window.advanceTime(500));
  assert((await state()).selectedId === "tree_green02", "Deterministic time advance does not mutate asset selection");

  await debug("reset");
  current = await state();
  assert(current.familyFilter === "all" && current.statusFilter === "all" && current.selectedId === "bush_01" && current.matchingCount === 65, "Reset restores the complete gallery");
  assert(errors.length === 0, "Gallery browser console and page errors remain clean");
} finally {
  result.states.final = await state().catch(() => null);
  fs.writeFileSync(path.join(outputDir, "results.json"), JSON.stringify(result, null, 2));
  await browser.close();
}

console.log(JSON.stringify({ assertions: result.assertions.length, errors: errors.length, outputDir }, null, 2));
