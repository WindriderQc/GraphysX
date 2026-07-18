import fs from "node:fs";
import path from "node:path";
import { chromium } from "playwright";

const url = process.env.GRAPHYSX_URL ?? "http://127.0.0.1:4183/object-library-catalog-preview.html";
const outputDir = path.resolve("output/playwright/object-library-catalog");
fs.mkdirSync(outputDir, { recursive: true });

const browser = await chromium.launch({
  headless: true,
  args: ["--use-gl=angle", "--use-angle=swiftshader"]
});
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
const debug = (method, argument) => page.evaluate(({ method, argument }) => window.__OBJECT_LIBRARY_DEBUG__[method](argument), { method, argument });
const state = () => page.evaluate(() => window.__OBJECT_LIBRARY_DEBUG__.getState());
const screenshot = async (name) => {
  await page.waitForTimeout(180);
  await page.screenshot({ path: path.join(outputDir, `${name}.png`) });
};

try {
  await page.goto(url, { waitUntil: "domcontentloaded" });
  await page.waitForFunction(() => window.__OBJECT_LIBRARY_DEBUG__?.getState().ready === true, null, { timeout: 30_000 });

  let current = await state();
  assert(current.objectCount === 61, "Catalog retains all 61 ObjectLibrary records");
  assert(current.recoveredCount === 47 && current.missingCount === 13 && current.unsupportedCount === 1, "Recovery boundary is 47 recovered, 13 missing and 1 unsupported");
  assert(current.matchingCount === 61 && current.renderedGeometryCount === 47 && current.inspectionMarkerCount === 14, "Overview distinguishes archive geometry from evidence labels");
  assert(current.classification.includes("not a composed world"), "Catalog is not presented as a composed world");
  assert(current.physics.simulated === false, "Catalog physics metadata is not turned into invented simulation");
  await screenshot("00-all-records-authored-grid");

  assert(await debug("setFamily", "port"), "Port family filter is accepted");
  current = await state();
  assert(current.matchingCount === 28 && current.renderedGeometryCount === 27 && current.inspectionMarkerCount === 1, "Port filter shows 27 decoded X assets plus the binary crateshed label");
  await screenshot("01-port-catalog-grid");

  assert(await debug("setStatus", "unsupported"), "Unsupported filter is accepted");
  current = await state();
  assert(current.matchingCount === 1 && current.selectedIndex === 39 && current.selectedName === "port_crateshed", "Binary port_crateshed remains one explicit unsupported record");
  assert(current.objects[39].recoveredGeometry === false && current.objects[39].inspectionMarkerOnly === true, "Binary X receives a label but no proxy geometry");
  await screenshot("02-binary-x-boundary");

  await debug("setFamily", "all");
  await debug("setStatus", "missing");
  current = await state();
  assert(current.matchingCount === 13 && current.renderedGeometryCount === 0 && current.inspectionMarkerCount === 13, "Missing filter contains labels only for all 13 unresolved records");
  assert(current.objects.filter((object) => object.matchesFilter).every((object) => !object.recoveredGeometry && object.inspectionMarkerOnly), "No missing record receives invented geometry");
  await screenshot("03-missing-records-only");

  await debug("setStatus", "all");
  await debug("setFamily", "nature");
  current = await state();
  assert(current.matchingCount === 16 && current.renderedGeometryCount === 9 && current.inspectionMarkerCount === 7, "Nature separates nine recovered trees from seven absent bush/green records");
  await screenshot("04-nature-recovery-boundary");

  await debug("setFamily", "primitive");
  await debug("setStatus", "recovered");
  current = await state();
  assert(current.matchingCount === 5 && current.renderedGeometryCount === 5, "All five serialized procedural primitives are recovered");

  await debug("setFamily", "port");
  assert(await debug("select", 49), "Lighthouse record can be selected by exact index");
  current = await state();
  assert(current.selectedName === "port_lighthouse", "Selected index 49 is port_lighthouse");
  assert(JSON.stringify(current.objects[49].sourcePosition) === JSON.stringify([-45, 0, 15]), "Lighthouse exact serialized position is preserved");
  assert(JSON.stringify(current.objects[49].rotationDegrees) === JSON.stringify([0, 0, 0]), "Lighthouse exact serialized rotation is preserved");
  assert(JSON.stringify(current.objects[49].scale) === JSON.stringify([0.01, 0.01, 0.01]), "Lighthouse exact serialized scale is preserved");
  assert(current.objects[49].sourceAsset.endsWith("port_lighthouse.x"), "Lighthouse exposes its exact-basename recovered source asset");

  const orbitBefore = current.orbitAngleRadians;
  await debug("orbit", 0.25);
  assert(Math.abs((await state()).orbitAngleRadians - orbitBefore) > 0.24, "Deterministic orbit API changes the inspection camera");
  await debug("zoom", 1.2);
  assert(Math.abs((await state()).zoom - 1.2) < 0.001, "Deterministic zoom API changes the inspection camera");
  await page.evaluate(() => window.advanceTime(500));
  assert((await state()).selectedIndex === 49, "Deterministic time advance does not mutate authored catalog selection");

  await debug("reset");
  current = await state();
  assert(current.familyFilter === "all" && current.statusFilter === "all" && current.selectedIndex === 0 && current.matchingCount === 61, "Reset restores the complete catalog overview");
  assert(errors.length === 0, "Catalog browser console and page errors remain clean");
} finally {
  result.states.final = await state().catch(() => null);
  fs.writeFileSync(path.join(outputDir, "results.json"), JSON.stringify(result, null, 2));
  await browser.close();
}

console.log(JSON.stringify({ assertions: result.assertions.length, errors: errors.length, outputDir }, null, 2));
