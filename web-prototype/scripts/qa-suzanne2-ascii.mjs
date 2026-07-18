import fs from "node:fs/promises";
import path from "node:path";
import { chromium } from "playwright";

const baseUrl = process.argv[2] ?? process.env.GRAPHYSX_URL ?? "http://127.0.0.1:4187";
const url = new URL("/suzanne2-ascii-preview.html", baseUrl).href;
const outputDir = path.resolve("output/playwright/suzanne2-ascii");
await fs.rm(outputDir, { recursive: true, force: true });
await fs.mkdir(outputDir, { recursive: true });

const browser = await chromium.launch({
  headless: true,
  args: ["--use-gl=angle", "--use-angle=swiftshader"]
});
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
const state = () => page.evaluate(() => window.__SUZANNE2_ASCII_DEBUG__.state());
const debug = (method, ...args) => page.evaluate(({ method, args }) => window.__SUZANNE2_ASCII_DEBUG__[method](...args), { method, args });
const screenshot = (name) => page.screenshot({ path: path.join(outputDir, `${name}.png`) });

try {
  await page.goto(url, { waitUntil: "domcontentloaded" });
  await page.waitForFunction(() => Boolean(window.__SUZANNE2_ASCII_DEBUG__ && window.render_game_to_text));
  await page.waitForFunction(() => window.__SUZANNE2_ASCII_DEBUG__.state().loadStatus === "ready", null, { timeout: 30000 });

  let current = await state();
  report.states.initial = current;
  assert(current.grid.width === 40 && current.grid.height === 40, "Suzanne2 remains the exact 40x40 authored grid");
  assert(current.counts.wallCubesFromHashZz === 313, "# / Z / z author exactly 313 wall cubes");
  assert(current.counts.effectWallCubes === 2 && current.counts.totalAsciiCollisionCubes === 315, "effect walls bring the collision-cube total to 315");
  assert(current.counts.rings === 15 && current.counts.ringsVisible === 15, "all fifteen authored rings render initially");
  assert(current.counts.chainAssemblies === 3 && current.counts.chainRigidBodies === 9, "three source chains preserve nine rigid parts");
  assert(current.counts.pistonAssemblies === 3 && current.counts.pistonBodiesIncludingHiddenTriggers === 9, "three pistons preserve bar, plate, and trigger semantics");
  assert(current.counts.effectCells === 2 && current.counts.particleEmitters === 4 && current.counts.particleCapacity === 256, "both effect cells preserve four 64-particle emitters");
  assert(current.counts.lapGatePosts === 4 && current.counts.gateSegments === 2, "finish and halfway gates preserve four posts and two segments");
  assert(current.counts.cubxActorAnchors === 2 && current.counts.cubxAnchorsVisible === 0, "two CubX anchors remain explicit but hidden by default");
  assert(current.counts.xmlObjects === 3 && current.counts.xmlMeshesVisible === 3, "all three Suzanne2.xml objects render");
  assert(JSON.stringify(current.player.position) === JSON.stringify([10, 0.5, 6]), "the @ spawn preserves its non-centered [10,0.5,6] loader position");
  assert(current.player.implementation === "ZombieKiller" && current.player.physicsRadius === 0.3 && current.player.insideRadius === 0.225, "active-source SuperCage / FireArrow player dimensions survive");
  assert(JSON.stringify(current.gates.finishStart) === JSON.stringify([14.5, 0.5, 3.5]) && JSON.stringify(current.gates.finishEnd) === JSON.stringify([14.5, 0.5, 9.5]), "finish endpoints match the ASCII F/f cells");
  assert(JSON.stringify(current.gates.halfStart) === JSON.stringify([20.5, 0.5, 25.5]) && JSON.stringify(current.gates.halfEnd) === JSON.stringify([20.5, 0.5, 33.5]), "halfway endpoints match the ASCII H/h cells");
  assert(current.rules.authoredRingInventory === 15 && current.rules.implementedVictoryThreshold === 2, "QA preserves the authored 15 versus implemented 2 progression conflict");
  assert(current.rules.lapVictoryTarget === null, "no invented lap victory target is attached to Suzanne2");
  assert(current.hostGlobalsIncluded === false && current.screenshotEvidence === false, "isolated candidate excludes host globals and labels the missing screenshot boundary");
  await screenshot("00-source-layout-overview");

  const rotationBefore = await debug("ringRotation", 0);
  await page.evaluate(() => window.advanceTime(1000));
  const rotationAfter = await debug("ringRotation", 0);
  const rotationDelta = rotationAfter - rotationBefore;
  assert(Math.abs(rotationDelta - Math.PI * 100 / 180) < 0.08, "deterministic one-second advance applies the source 100-degrees-per-second ring rotation");

  await debug("setRingVisible", 0, false);
  await debug("setPistonActivation", 1, 1);
  await debug("setCubxVisible", true);
  await debug("setXmlVisible", false);
  await debug("setPlayerVisible", false);
  current = await state();
  report.states.mutated = current;
  assert(current.counts.ringsVisible === 14, "ring pickup visibility API removes one authored ring without mutating inventory");
  assert(JSON.stringify(current.activePistons) === JSON.stringify([1]) && Math.abs((await debug("pistonOffset", 1)) - 0.5) < 0.0001, "piston API reaches the exact +0.5 sliding limit");
  assert(current.counts.cubxAnchorsVisible === 2, "CubX evidence anchors can be inspected independently");
  assert(current.counts.xmlMeshesVisible === 0 && current.player.visible === false, "XML and player layers are independently isolatable");
  await screenshot("01-layer-and-piston-diagnostics");

  await debug("reset");
  await debug("setCameraMode", "source2017");
  const rendered = JSON.parse(await page.evaluate(() => window.render_game_to_text()));
  report.states.sourceCamera = rendered;
  assert(rendered.camera.mode === "source2017" && JSON.stringify(rendered.camera.position) === JSON.stringify([20, 10, 20]), "implemented source camera pose is directly inspectable");
  current = await state();
  assert(current.counts.ringsVisible === 15 && current.counts.xmlMeshesVisible === 3 && current.player.visible === true, "reset restores every source layer");
  await screenshot("02-implemented-source-camera");

  assert(browserProblems.length === 0, "Suzanne2 preview has no browser console or page errors");
} finally {
  report.states.final = await state().catch(() => null);
  await fs.writeFile(path.join(outputDir, "results.json"), JSON.stringify(report, null, 2));
  await browser.close();
}

console.log(JSON.stringify({ assertions: report.assertions.length, browserProblems: browserProblems.length, outputDir }, null, 2));
