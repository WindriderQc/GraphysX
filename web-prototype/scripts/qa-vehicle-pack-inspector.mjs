import { mkdir, writeFile } from "node:fs/promises";
import { chromium } from "playwright";

const baseUrl = process.argv[2] ?? "http://127.0.0.1:5181/vehicle-pack-inspector.html";
const outputDir = "output/playwright/vehicle-pack-inspector";
await mkdir(outputDir, { recursive: true });

const browser = await chromium.launch({ headless: true });
const page = await browser.newPage({ viewport: { width: 1440, height: 900 }, deviceScaleFactor: 1 });
const browserErrors = [];
const assertions = [];

page.on("console", (message) => {
  if (message.type() === "error") browserErrors.push(`console: ${message.text()}`);
});
page.on("pageerror", (error) => browserErrors.push(`pageerror: ${error.message}`));

function assert(condition, message) {
  assertions.push({ passed: Boolean(condition), message });
  console.log(`${condition ? "PASS" : "FAIL"} ${message}`);
}

async function state() {
  return page.evaluate(() => JSON.parse(window.render_game_to_text()));
}

async function waitReady(selected) {
  await page.waitForFunction((wanted) => {
    const current = JSON.parse(window.render_game_to_text());
    return current.selected === wanted && current.loadStatus === "ready";
  }, selected);
  return state();
}

await page.goto(baseUrl, { waitUntil: "networkidle" });
let current = await waitReady("gt4");
assert(current.mode === "vehicle-pack-evidence-inspector", "isolated inspector reports its own mode");
assert(current.evidence.objects === 14, "GT4 exposes all 14 authored 3DS objects");
assert(current.evidence.vertices === 10740, "GT4 exposes the audited 10,740 authored vertices");
assert(current.evidence.triangles === 8345, "GT4 exposes the audited 8,345 faces");
assert(current.evidence.materials === 2, "GT4 exposes both authored material slots");
assert(current.evidence.textureReferences.join() === "GT4 WORK.JPG", "GT4 retains its exact material-map reference");
assert(current.evidence.selectorBinding === false, "GT4 makes no invented selector binding");
assert(current.evidence.physicsBinding === false, "GT4 makes no invented driving-physics binding");
assert(current.presentation.viewTransform.includes("no size normalization"), "inspection transform discloses that source scale is preserved");
assert(current.loaderEvidence.objectCount === 14, "runtime 3DS parser loads all 14 GT4 objects");
assert(current.loaderEvidence.triangleCount === 8345, "runtime 3DS parser loads all 8,345 GT4 faces");
assert(current.loaderEvidence.compatibilityOverrides.length === 1, "GT4's black textured diffuse slot has one disclosed inspection override");
await page.screenshot({ path: `${outputDir}/gt4.png`, fullPage: true });

await page.click("#select-cobra");
current = await waitReady("low-cobra");
assert(current.evidence.objects === 10, "Low Cobra exposes all 10 authored 3DS objects");
assert(current.evidence.vertices === 6961, "Low Cobra exposes the audited 6,961 authored vertices");
assert(current.evidence.triangles === 3266, "Low Cobra exposes the audited 3,266 faces");
assert(current.evidence.materials === 7, "Low Cobra exposes all seven authored material slots");
assert(current.evidence.textureReferences.length === 7, "Low Cobra retains all seven exact texture references");
assert(current.evidence.textureReferences.includes("COBTEX.TGA"), "Low Cobra uses the referenced CobTex body map, not an invented paint binding");
assert(current.evidence.selectorBinding === false, "Low Cobra makes no invented selector binding");
assert(current.evidence.physicsBinding === false, "Low Cobra makes no invented driving-physics binding");
assert(current.loaderEvidence.objectCount === 10, "runtime 3DS parser loads all 10 Low Cobra objects");
assert(current.loaderEvidence.triangleCount === 3266, "runtime 3DS parser loads all 3,266 Low Cobra faces");
assert(current.loaderEvidence.textureReferences.length === 7, "runtime loader resolves all seven Cobra material maps");
assert(current.loaderEvidence.compatibilityOverrides.length === 0, "Low Cobra needs no material compatibility override");
await page.screenshot({ path: `${outputDir}/low-cobra.png`, fullPage: true });

const canvas = page.locator("#vehicle-canvas");
const box = await canvas.boundingBox();
if (!box) throw new Error("vehicle canvas has no bounding box");
await page.mouse.move(box.x + box.width * 0.6, box.y + box.height * 0.55);
await page.mouse.down();
await page.mouse.move(box.x + box.width * 0.72, box.y + box.height * 0.62, { steps: 5 });
await page.mouse.up();
await page.mouse.wheel(0, 280);
const moved = await state();
assert(moved.presentation.yaw !== -0.55, "drag changes the inspection yaw");
assert(moved.presentation.zoom > 1, "wheel input changes the fitted-camera zoom");
await page.keyboard.press("r");
const reset = await state();
assert(reset.presentation.yaw === -0.55, "R resets inspection yaw deterministically");
assert(reset.presentation.zoom === 1, "R resets inspection zoom deterministically");

await page.keyboard.press("1");
current = await waitReady("gt4");
assert(current.selected === "gt4", "keyboard 1 returns to GT4");
await page.keyboard.press("2");
current = await waitReady("low-cobra");
assert(current.selected === "low-cobra", "keyboard 2 returns to Low Cobra");

assert(browserErrors.length === 0, "standalone inspector produces no browser/page errors");

const result = {
  url: baseUrl,
  assertions,
  passed: assertions.filter((entry) => entry.passed).length,
  failed: assertions.filter((entry) => !entry.passed).length,
  browserErrors,
  finalState: current
};
await writeFile(`${outputDir}/results.json`, JSON.stringify(result, null, 2));
await browser.close();

if (result.failed > 0 || browserErrors.length > 0) process.exitCode = 1;
