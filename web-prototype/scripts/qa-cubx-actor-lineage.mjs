import fs from "node:fs/promises";
import path from "node:path";
import { chromium } from "playwright";

const baseUrl = process.argv[2] ?? process.env.GRAPHYSX_URL ?? "http://127.0.0.1:4191";
const url = new URL("/cubx-actor-lineage-preview.html", baseUrl).href;
const outputDir = path.resolve("output/playwright/cubx-actor-lineage");
await fs.rm(outputDir, { recursive: true, force: true });
await fs.mkdir(outputDir, { recursive: true });

const browser = await chromium.launch({ headless: true });
const page = await browser.newPage({ viewport: { width: 1440, height: 900 }, deviceScaleFactor: 1 });
const browserProblems = [];
page.on("console", (message) => {
  if (message.type() === "error") browserProblems.push(`console: ${message.text()}`);
});
page.on("pageerror", (error) => browserProblems.push(`pageerror: ${error.message}`));

const report = { url, assertions: [], states: {}, browserProblems };
const assert = (condition, message) => {
  report.assertions.push({ pass: Boolean(condition), message });
  if (!condition) throw new Error(message);
};
const harness = (method, ...args) => page.evaluate(
  ({ method, args }) => window.__CUBX_ACTOR_LINEAGE_HARNESS__[method](...args),
  { method, args }
);
const state = () => page.evaluate(() => window.__CUBX_ACTOR_LINEAGE_HARNESS__.state());
const textState = () => page.evaluate(() => JSON.parse(window.render_game_to_text()));
const screenshot = async (name) => {
  await page.waitForTimeout(40);
  await page.screenshot({ path: path.join(outputDir, `${name}.png`) });
};

try {
  await page.goto(url, { waitUntil: "domcontentloaded" });
  await page.waitForFunction(() => Boolean(window.__CUBX_ACTOR_LINEAGE_HARNESS__ && window.render_game_to_text));

  let current = await harness("setClip", "closed", 1);
  report.states.closed = current;
  assert(current.isolatedFromCubz, "Inspector is explicitly isolated from the unresolved CubZ mapping");
  assert(current.clip.filename === "CubXMesh.tvm" && current.clip.assetId === null, "Closed view uses the exact CubXMesh.tvm companion");
  assert(current.geometry.vertices === 2264 && current.geometry.triangles === 2750, "Closed companion exposes exact 2264-vertex / 2750-triangle totals");
  assert(current.geometry.buttonMeshes === 8 && current.geometry.buttonVertices === 192 && current.geometry.buttonTriangles === 96, "All eight exact TVM click proxies are present");
  assert((await textState()).diagnostic === "cubx-actor-lineage", "render_game_to_text exposes the isolated lineage state");

  const getMotionEnds = [30, 50, 30, 50, 50, 50, 50];
  const getFiles = [];
  for (let pair = 1; pair <= 7; pair += 1) {
    current = await harness("setClip", "get", pair);
    getFiles.push(current.clip.filename);
    assert(current.clip.assetId === `get-${pair + 1}` && current.clip.filename === `CubXGet${pair + 1}.tva`, `Get pair ${pair} selects exact CubXGet${pair + 1}.tva`);
    assert(current.clip.startFrame === 0 && current.clip.endFrame === 100 && current.clip.framesPerSecond === 30, `Get pair ${pair} preserves 0..100 @ 30fps`);
    assert(current.clip.motionCompletedAtFrame === getMotionEnds[pair - 1] && current.clip.terminalHoldFrames === 100 - getMotionEnds[pair - 1], `Get pair ${pair} preserves its terminal hold`);
    assert(current.geometry.vertices === 2264 && current.geometry.triangles === 2750 && current.geometry.meshChunks === 23, `Get pair ${pair} displays all exact decoded geometry chunks`);
  }
  report.states.getFiles = getFiles;

  await harness("setClip", "get", 1);
  current = await harness("setFrame", 29.5);
  assert(!current.clip.exactStoredRotationKey && !current.clip.terminalHoldActive, "Fractional Get frame is disclosed as inferred SLERP before the hold");
  current = await harness("setFrame", 30);
  assert(current.clip.exactStoredRotationKey && current.clip.terminalHoldActive, "Get2 frame 30 is an exact terminal-hold key");
  await harness("setFrame", 15);
  await screenshot("00-get2-motion-frame15");

  await harness("setFrame", 0);
  await harness("playing", true);
  await page.evaluate(() => window.advanceTime(1000));
  current = await state();
  assert(Math.abs(current.clip.sourceFrame - 30) < 1e-9 && current.clip.playing, "Deterministic advanceTime moves Get2 by exactly 30 frames in one second");
  assert(current.clip.terminalHoldActive, "Deterministic playback enters the archived Get2 terminal hold");
  await page.evaluate(() => window.advanceTime(3000));
  current = await state();
  assert(current.clip.sourceFrame === 100 && !current.clip.playing, "Playback clamps at source frame 100 and stops after the full hold");

  const rotEnds = [20, 30, 20, 30, 30, 30, 30];
  for (let pair = 1; pair <= 7; pair += 1) {
    current = await harness("setClip", "rot", pair);
    assert(current.clip.assetId === `rot-${pair}` && current.clip.filename === `CubXRot${pair}.tva`, `Rot pair ${pair} selects exact CubXRot${pair}.tva`);
    assert(current.clip.endFrame === rotEnds[pair - 1] && current.clip.terminalHoldFrames === 0, `Rot pair ${pair} preserves its exact range without a terminal hold`);
    assert(current.geometry.vertices === 2148 && current.geometry.triangles === 2688 && current.geometry.meshChunks === 20, `Rot pair ${pair} displays all exact decoded geometry chunks`);
  }
  await harness("setClip", "rot", 4);
  await harness("setFrame", 15);
  await screenshot("01-rot4-frame15-host-unused");

  current = await harness("setClip", "open-full", 1);
  assert(current.clip.filename === "CubXOpen.tva" && current.clip.endFrame === 100, "Full open selects exact CubXOpen.tva 0..100 range");
  assert(current.clip.motionCompletedAtFrame === 50 && current.clip.terminalHoldFrames === 50, "Full open preserves its 50-frame terminal hold");
  assert(current.geometry.vertices === 2264 && current.geometry.triangles === 2750 && current.geometry.meshChunks === 23, "Full open displays exact decoded geometry totals");
  current = await harness("setFrame", 50);
  assert(current.clip.exactStoredRotationKey && current.clip.terminalHoldActive, "Full-open frame 50 is the exact start of its terminal hold");
  await harness("setFrame", 72);
  await screenshot("02-open-full-terminal-hold");

  current = await harness("setClip", "open-solo", 1);
  assert(current.clip.filename === "CubeOpensolo.tva" && current.clip.endFrame === 50 && current.clip.terminalHoldFrames === 0, "Solo open selects exact CubeOpensolo.tva 0..50 range");
  assert(current.geometry.vertices === 140 && current.geometry.triangles === 74 && current.geometry.meshChunks === 4, "Solo open displays its four exact panel geometry chunks");

  await harness("setClip", "closed", 1);
  current = await harness("selectClick", 8);
  report.states.click8 = current;
  assert(current.clickInspection.clickIndex === 8 && current.clickInspection.exactSpatialBoxLabel === "Box01", "Click 8 retains its exact Box01 spatial label");
  assert(!current.clickInspection.actorSlotInitialized && current.clickInspection.actorFilename === null && !current.clickInspection.renderedByHostLoop, "Click 8 exposes the uninitialized slot-7 / render-loop defect instead of inventing an actor");
  assert(current.clip.filename === "CubXMesh.tvm", "Inspecting broken click 8 does not swap in an invented actor");
  await screenshot("03-click8-uninitialized-defect");

  const expectedSpatialOrder = ["Box05", "Box04", "Box03", "Box02", "Box08", "Box06", "Box07"];
  for (let click = 1; click <= 7; click += 1) {
    current = await harness("selectClick", click);
    assert(current.clickInspection.exactSpatialBoxLabel === expectedSpatialOrder[click - 1], `Click ${click} reports its exact spatial ${expectedSpatialOrder[click - 1]} label`);
    assert(current.clickInspection.actorArrayIndex === click - 1 && current.clickInspection.actorFilename === `CubXGet${click + 1}.tva`, `Click ${click} reports exact host slot ${click - 1} -> CubXGet${click + 1}`);
  }

  current = await harness("buttons", false);
  assert(!current.geometry.buttonsVisible, "Click proxies can be hidden without changing source geometry");
  current = await harness("buttons", true);
  assert(current.geometry.buttonsVisible, "Click proxies can be restored");
  current = await harness("diagnostics", false);
  assert(!current.geometry.diagnosticColors, "Source-gray material mode is available");
  current = await harness("diagnostics", true);
  assert(current.geometry.diagnosticColors, "Diagnostic component colors are available and disclosed");

  const invalidPairRejected = await page.evaluate(() => {
    try {
      window.__CUBX_ACTOR_LINEAGE_HARNESS__.setClip("get", 8);
      return false;
    } catch {
      return true;
    }
  });
  const invalidClickRejected = await page.evaluate(() => {
    try {
      window.__CUBX_ACTOR_LINEAGE_HARNESS__.selectClick(9);
      return false;
    } catch {
      return true;
    }
  });
  assert(invalidPairRejected && invalidClickRejected, "Harness rejects invalid actor pairs and click indices");
  assert(browserProblems.length === 0, "CubXActor lineage inspector has no browser console or page errors");
} finally {
  report.states.final = await state().catch(() => null);
  await fs.writeFile(path.join(outputDir, "results.json"), JSON.stringify(report, null, 2));
  await browser.close();
}

console.log(JSON.stringify({ assertions: report.assertions.length, browserProblems: browserProblems.length, outputDir }, null, 2));

