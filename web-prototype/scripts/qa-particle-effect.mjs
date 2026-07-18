import { mkdir, writeFile } from "node:fs/promises";
import path from "node:path";
import { chromium } from "playwright";

const url = process.env.PARTICLE_EFFECT_URL ?? "http://127.0.0.1:4173/particle-effect-preview.html";
const outputDir = path.resolve("output/playwright/particle-effect-explosion1");
const assertions = [];
const errors = [];
const failedResponses = [];

function assert(condition, message) {
  assertions.push({ ok: Boolean(condition), message });
  if (!condition) throw new Error(message);
}

await mkdir(outputDir, { recursive: true });
const browser = await chromium.launch({ headless: true });
const page = await browser.newPage({ viewport: { width: 1280, height: 720 }, deviceScaleFactor: 1 });

page.on("console", (message) => {
  if (message.type() === "error") errors.push({ type: "console", text: message.text() });
});
page.on("pageerror", (error) => errors.push({ type: "page", text: error.message }));
page.on("response", (response) => {
  if (response.status() >= 400) failedResponses.push({ status: response.status(), url: response.url() });
});

const state = () => page.evaluate(() => window.__PARTICLE_EFFECT_ARCHIVE__.state());

try {
  await page.goto(url, { waitUntil: "networkidle" });
  await page.waitForFunction(() => window.__PARTICLE_EFFECT_ARCHIVE__?.state().ready === true);

  const initial = await state();
  assert(initial.classification === "reusable-effect-preset-not-a-scene", "Effect is classified as a reusable preset, not a scene");
  assert(initial.counts.emitters === 3 && initial.counts.maximumParticles === 82, "Exact Explosion1 emitter/capacity totals are exposed");
  assert(initial.emitters.map((emitter) => emitter.maxParticles).join(",") === "10,64,8", "Exact per-emitter capacities are exposed");
  assert(initial.emitters.map((emitter) => emitter.generationSpeedMilliseconds).join(",") === "10,10,10", "Exact generation intervals are exposed");
  assert(initial.emitters.map((emitter) => emitter.lifetimeSeconds).join(",") === "2,3,4", "Exact particle lifetimes are exposed");

  await page.evaluate(() => {
    window.__PARTICLE_EFFECT_ARCHIVE__.setPaused(true);
    window.__PARTICLE_EFFECT_ARCHIVE__.setAutoReplay(false);
    window.__PARTICLE_EFFECT_ARCHIVE__.restart();
    window.__PARTICLE_EFFECT_ARCHIVE__.setPaused(false);
    window.advanceTime(700);
  });
  const firstRun = await state();
  assert(firstRun.counts.spawnedParticles === 82 && firstRun.counts.activeParticles === 82, "All 82 one-shot particles are alive after deterministic 700 ms");
  const deterministicSample = JSON.stringify(firstRun.emitters.map((emitter) => emitter.sampleSourcePositions));
  await page.screenshot({ path: path.join(outputDir, "explosion1-700ms.png"), fullPage: true });

  await page.evaluate(() => {
    window.__PARTICLE_EFFECT_ARCHIVE__.restart();
    window.advanceTime(700);
  });
  const repeatedRun = await state();
  assert(JSON.stringify(repeatedRun.emitters.map((emitter) => emitter.sampleSourcePositions)) === deterministicSample, "Restart reproduces identical seeded source-space trajectories");

  await page.keyboard.press("Space");
  const paused = await state();
  assert(paused.paused, "Space pauses the isolated effect");
  await page.evaluate(() => window.advanceTime(1000));
  const pausedAfterStep = await state();
  assert(Math.abs(pausedAfterStep.totalElapsedSeconds - paused.totalElapsedSeconds) < 1e-9, "Paused effect ignores deterministic time");
  await page.keyboard.press("Space");
  await page.evaluate(() => window.advanceTime(4500));
  const finished = await state();
  assert(finished.finished && finished.counts.activeParticles === 0, "Source one-shot reaches an empty finished state");

  await page.keyboard.press("KeyA");
  await page.keyboard.press("KeyR");
  const replayRestart = await state();
  assert(replayRestart.autoReplay && replayRestart.cycle === 1 && replayRestart.totalElapsedSeconds === 0, "A enables preview replay and R restarts cycle one");
  await page.evaluate(() => window.advanceTime(4600));
  const replayed = await state();
  assert(replayed.cycle === 2 && replayed.counts.activeParticles > 0, "Preview-only auto replay starts a second visible cycle");

  assert(failedResponses.length === 0, "Preview and byte-identical DDS assets load without failed responses");
  assert(errors.length === 0, "Preview emits no console/page errors");

  await writeFile(path.join(outputDir, "results.json"), JSON.stringify({ url, assertions, errors, failedResponses, finalState: replayed }, null, 2));
  console.log(`Particle effect QA passed ${assertions.length}/${assertions.length} assertions.`);
} catch (error) {
  const message = error instanceof Error ? error.stack ?? error.message : String(error);
  await writeFile(path.join(outputDir, "results.json"), JSON.stringify({ url, assertions, errors, failedResponses, failure: message }, null, 2));
  console.error(message);
  process.exitCode = 1;
} finally {
  await browser.close();
}
