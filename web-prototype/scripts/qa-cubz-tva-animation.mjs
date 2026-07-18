import fs from "node:fs/promises";
import path from "node:path";
import { chromium } from "playwright";

const baseUrl = process.argv[2] ?? process.env.GRAPHYSX_URL ?? "http://127.0.0.1:4189";
const url = new URL("/cubz-tva-animation-preview.html", baseUrl).href;
const outputDir = path.resolve("output/playwright/cubz-tva-animation");
await fs.rm(outputDir, { recursive: true, force: true });
await fs.mkdir(outputDir, { recursive: true });

const browser = await chromium.launch({ headless: true });
const page = await browser.newPage({ viewport: { width: 1280, height: 720 }, deviceScaleFactor: 1 });
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
  ({ method, args }) => window.__CUBZ_TVA_ANIMATION_HARNESS__[method](...args),
  { method, args }
);
const state = () => page.evaluate(() => window.__CUBZ_TVA_ANIMATION_HARNESS__.state());
const textState = () => page.evaluate(() => JSON.parse(window.render_game_to_text()));
const screenshot = (name) => page.screenshot({ path: path.join(outputDir, `${name}.png`) });
const distance = (left, right) => Math.hypot(...left.map((value, index) => value - right[index]));
const quaternionNorm = (value) => Math.hypot(...value);

try {
  await page.goto(url, { waitUntil: "domcontentloaded" });
  await page.waitForFunction(() => Boolean(window.__CUBZ_TVA_ANIMATION_HARNESS__ && window.render_game_to_text));
  await page.keyboard.press("Space");
  await harness("reset");

  let current = await state();
  report.states.initial = current;
  assert(current.mode === "rotation" && current.selection === 1 && current.direction === "forward", "Harness starts on CubeRot Animation1 forward");
  assert(current.sample.fidelity.interpolation.status === "deterministic-inference-engine-routine-not-recovered", "Interpolation remains explicitly marked as inference");
  assert(current.sample.fidelity.coordinateSpace.status === "raw-tv3d-source-order-no-web-conversion", "No hidden TV3D-to-web coordinate conversion is applied");
  assert(current.sample.fidelity.reverseRepair.archivedExpression === "iSelectedCube+1", "Archived reverse defect remains disclosed");
  assert((await textState()).sample.kind === "cube-rotation", "render_game_to_text mirrors the harness sample");

  const rotationEndpoints = {};
  for (let selection = 1; selection <= 7; selection += 1) {
    await harness("select", selection);
    await harness("direction", "forward");
    const forwardStart = await harness("seek", 0);
    const duration = forwardStart.sample.cursor.durationSeconds;
    const forwardEnd = await harness("seek", duration);
    assert(forwardStart.sample.animationId === selection && forwardStart.sample.cursor.sourceFrame === forwardStart.sample.cursor.startFrame, `Animation${selection} forward begins at its decoded start frame`);
    assert(forwardEnd.sample.cursor.sourceFrame === forwardEnd.sample.cursor.endFrame && forwardEnd.sample.cursor.finished, `Animation${selection} forward reaches its decoded end frame`);
    assert(forwardStart.sample.globalCube.exactStoredKey && forwardEnd.sample.globalCube.exactStoredKey, `Animation${selection} endpoints are exact MANI keys`);
    assert(Math.abs(quaternionNorm(forwardEnd.sample.globalCube.quaternion) - 1) < 0.000001, `Animation${selection} endpoint quaternion remains unit length`);

    await harness("direction", "reverse");
    const reverseStart = await harness("seek", 0);
    const reverseEnd = await harness("seek", duration);
    assert(reverseStart.sample.animationId === selection && reverseStart.sample.reverseBehavior.animationId === selection, `Animation${selection} reverse reuses the forward animation ID`);
    assert(reverseStart.sample.cursor.sourceFrame === forwardEnd.sample.cursor.sourceFrame && distance(reverseStart.sample.globalCube.quaternion, forwardEnd.sample.globalCube.quaternion) < 1e-12, `Animation${selection} reverse begins at the exact forward endpoint`);
    assert(reverseEnd.sample.cursor.sourceFrame === forwardStart.sample.cursor.sourceFrame && distance(reverseEnd.sample.globalCube.quaternion, forwardStart.sample.globalCube.quaternion) < 1e-12, `Animation${selection} reverse finishes at the exact forward start`);
    assert(reverseStart.sample.reverseBehavior.archivedWouldRequest === selection + 1, `Animation${selection} reports the archived shifted reverse request`);
    if (selection === 7) assert(!reverseStart.sample.reverseBehavior.archivedRequestIsValid, "Selection 7 reports archived ID 8 as invalid");

    const forwardElapsed = duration * 0.37;
    await harness("direction", "forward");
    const forwardMiddle = await harness("seek", forwardElapsed);
    await harness("direction", "reverse");
    const reverseMiddle = await harness("seek", duration - forwardElapsed);
    assert(Math.abs(forwardMiddle.sample.cursor.sourceFrame - reverseMiddle.sample.cursor.sourceFrame) < 1e-9, `Animation${selection} forward/reverse complementary time reaches the same source frame`);
    assert(distance(forwardMiddle.sample.globalCube.quaternion, reverseMiddle.sample.globalCube.quaternion) < 1e-9, `Animation${selection} forward/reverse complementary sample is quaternion-identical`);

    await harness("direction", "forward");
    const fractional = await harness("seek", 0.5 / 30);
    assert(!fractional.sample.globalCube.exactStoredKey && fractional.sample.globalCube.interpolation === "shortest-arc-slerp", `Animation${selection} fractional frame uses disclosed shortest-arc SLERP`);
    assert(Math.abs(quaternionNorm(fractional.sample.globalCube.quaternion) - 1) < 0.000001, `Animation${selection} interpolated quaternion remains normalized`);
    rotationEndpoints[selection] = {
      start: forwardStart.sample.globalCube.quaternion,
      end: forwardEnd.sample.globalCube.quaternion
    };
  }
  report.states.rotationEndpoints = rotationEndpoints;

  await harness("select", 4);
  await harness("direction", "reverse");
  current = await harness("seek", 0.41);
  report.states.rotationDiagnostic = current;
  await screenshot("00-animation4-repaired-reverse");

  await harness("open");
  await harness("direction", "forward");
  const openStart = await harness("seek", 0);
  const openDuration = openStart.sample.cursor.durationSeconds;
  const openEnd = await harness("seek", openDuration);
  assert(openStart.sample.kind === "cube-open" && openDuration === 50 / 30, "CubeOpen uses its exact 0..50 @30fps duration");
  assert(JSON.stringify(Object.keys(openStart.sample.panels).sort()) === JSON.stringify(["Left08", "Right08", "Top08"].sort()), "CubeOpen exposes exactly the three decoded Box08 panel tracks");
  for (const panelName of ["Right08", "Top08", "Left08"]) {
    assert(openStart.sample.panels[panelName].exactStoredKey && openEnd.sample.panels[panelName].exactStoredKey, `${panelName} open endpoints are exact MANI keys`);
    assert(Math.abs(quaternionNorm(openEnd.sample.panels[panelName].quaternion) - 1) < 0.000001, `${panelName} endpoint quaternion remains normalized`);
  }

  await harness("direction", "reverse");
  const closeStart = await harness("seek", 0);
  const closeEnd = await harness("seek", openDuration);
  for (const panelName of ["Right08", "Top08", "Left08"]) {
    assert(distance(closeStart.sample.panels[panelName].quaternion, openEnd.sample.panels[panelName].quaternion) < 1e-12, `${panelName} close starts at the exact open endpoint`);
    assert(distance(closeEnd.sample.panels[panelName].quaternion, openStart.sample.panels[panelName].quaternion) < 1e-12, `${panelName} close finishes at the exact open start`);
  }

  await harness("direction", "forward");
  const openFractional = await harness("seek", 0.5 / 30);
  assert(Object.values(openFractional.sample.panels).every((panel) => !panel.exactStoredKey && panel.interpolation === "shortest-arc-slerp"), "All three Box08 panels use SLERP at a fractional source frame");
  assert(Object.values(openFractional.sample.panels).every((panel) => Math.abs(quaternionNorm(panel.quaternion) - 1) < 0.000001), "All fractional Box08 quaternions remain normalized");

  await harness("seek", openDuration * 0.58);
  report.states.openDiagnostic = await state();
  await screenshot("01-box08-open-panels");

  await harness("select", 2);
  await harness("direction", "forward");
  await harness("reset");
  await harness("running", true);
  await page.evaluate(() => window.advanceTime(1000 / 30));
  current = await state();
  assert(Math.abs(current.sample.cursor.sourceFrame - 31) < 1e-9, "Deterministic advanceTime moves Animation2 by exactly one 30fps source frame");

  const invalidSelectionRejected = await page.evaluate(() => {
    try {
      window.__CUBZ_TVA_ANIMATION_HARNESS__.select(8);
      return false;
    } catch {
      return true;
    }
  });
  assert(invalidSelectionRejected, "Harness rejects selection 8 instead of touching the archived invalid reverse ID");
  assert(browserProblems.length === 0, "CubZ TVA diagnostic has no browser console or page errors");
} finally {
  report.states.final = await state().catch(() => null);
  await fs.writeFile(path.join(outputDir, "results.json"), JSON.stringify(report, null, 2));
  await browser.close();
}

console.log(JSON.stringify({ assertions: report.assertions.length, browserProblems: browserProblems.length, outputDir }, null, 2));
