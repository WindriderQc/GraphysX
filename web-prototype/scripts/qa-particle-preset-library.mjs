import { mkdir, writeFile } from "node:fs/promises";
import path from "node:path";
import { chromium } from "playwright";

const url = process.env.PARTICLE_PRESET_LIBRARY_URL ?? "http://127.0.0.1:4173/particle-preset-library-preview.html";
const outputDir = path.resolve("output/playwright/particle-preset-library");
const assertions = [];
const errors = [];
const failedResponses = [];

function assert(condition, message) {
  assertions.push({ ok: Boolean(condition), message });
  if (!condition) throw new Error(message);
}

await mkdir(outputDir, { recursive: true });
const browser = await chromium.launch({ headless: true });
const page = await browser.newPage({ viewport: { width: 1440, height: 900 }, deviceScaleFactor: 1 });

page.on("console", (message) => {
  if (message.type() === "error") errors.push({ type: "console", text: message.text() });
});
page.on("pageerror", (error) => errors.push({ type: "page", text: error.message }));
page.on("response", (response) => {
  if (response.status() >= 400) failedResponses.push({ status: response.status(), url: response.url() });
});

const state = () => page.evaluate(() => window.__PARTICLE_PRESET_LIBRARY__.state());

async function playPreset(id, milliseconds = 250) {
  await page.evaluate(({ id, milliseconds }) => {
    window.__PARTICLE_PRESET_LIBRARY__.setPaused(true);
    window.__PARTICLE_PRESET_LIBRARY__.setAutoReplay(false);
    window.__PARTICLE_PRESET_LIBRARY__.select(id);
    window.__PARTICLE_PRESET_LIBRARY__.restart();
    window.__PARTICLE_PRESET_LIBRARY__.setPaused(false);
    window.advanceTime(milliseconds);
  }, { id, milliseconds });
  return state();
}

try {
  await page.goto(url, { waitUntil: "networkidle" });
  await page.waitForFunction(() => window.__PARTICLE_PRESET_LIBRARY__?.state().ready === true);

  const initial = await state();
  const expectedCounts = {
    compiledFilenames: 19,
    uniqueCompiledBinaries: 17,
    readablePresets: 16,
    readableCompiledPresets: 14,
    readableSourceOnlyPresets: 2,
    opaqueCompiledPresets: 3,
    emitters: 29,
    attractors: 1,
    uniqueTextureBindings: 6,
    missingTextureBindings: 1
  };
  assert(initial.classification === "engine-fx-feature-not-scene", "Library is classified as an Engine/FX feature, not a scene");
  assert(JSON.stringify(initial.library.counts) === JSON.stringify(expectedCounts), "Archive-wide preset, emitter, attractor and texture counts are exact");
  assert(initial.library.entries.length === 19, "All 16 readable and 3 opaque presets are selectable");
  assert(initial.library.exactAliasGroups.length === 2, "Two exact compiled alias groups are deduplicated");
  assert(initial.library.opaque.length === 3, "Three binary-only presets remain explicitly opaque");

  const explosionAliases = initial.library.exactAliasGroups.find((group) => group.aliases.includes("explosion_01.TVP"));
  const explosion2Aliases = initial.library.exactAliasGroups.find((group) => group.aliases.includes("explosion_02.TVP"));
  assert(explosionAliases?.aliases.join(",") === "explosion1.TVP,explosion_01.TVP", "Explosion1's compiled alias pair is retained without a duplicate simulation");
  assert(explosion2Aliases?.aliases.join(",") === "explosion2.TVP,explosion_02.TVP", "Explosion2's compiled alias pair is retained without a duplicate simulation");

  const readableEntries = initial.library.entries.filter((entry) => entry.playable);
  for (const entry of readableEntries) {
    const preset = await playPreset(entry.id, 300);
    const expectedCapacity = preset.selected.emitterConfigs.reduce((sum, emitter) => sum + emitter.maxParticles, 0);
    assert(preset.selected.readable, `${entry.id} is backed by readable archived configuration`);
    assert(preset.counts.activeEmitters === preset.selected.emitterConfigs.length, `${entry.id} creates exactly one batch per archived emitter`);
    assert(preset.counts.emitterCapacity === expectedCapacity, `${entry.id} preserves the sum of archived emitter capacities`);
    assert(preset.counts.activeParticles > 0, `${entry.id} produces particles under deterministic playback`);
    await page.waitForTimeout(50);
    await page.screenshot({ path: path.join(outputDir, `readable-${entry.id}-300ms.png`) });
    if (entry.id === "firetrail_01") {
      await page.locator(".panel").screenshot({ path: path.join(outputDir, "firetrail-01-panel-layer.png") });
    }
  }

  const missingTexturePreset = await playPreset("expl1", 500);
  assert(missingTexturePreset.selected.availability === "readable-source-only", "expl1 remains explicitly source-only");
  assert(missingTexturePreset.selected.textureBindings.some((texture) => texture.status === "missing" && texture.name === "clumpy_blurry.dds"), "expl1 exposes its missing clumpy_blurry.dds binding");
  assert(missingTexturePreset.emitters.every((emitter) => emitter.textureStatus === "missing-binding-diagnostic-fallback"), "Missing archive texture uses a diagnostic fallback, never an invented replacement");
  await page.waitForTimeout(50);
  await page.screenshot({ path: path.join(outputDir, "expl1-missing-texture-500ms.png") });

  const explosionOne = await playPreset("explosion_01", 700);
  assert(explosionOne.selected.compiled?.aliases.join(",") === "explosion1.TVP,explosion_01.TVP", "explosion_01 selection reports both compiled aliases");
  assert(explosionOne.counts.spawnedParticles === 82 && explosionOne.counts.activeParticles === 82, "Explosion1 has all exact 82 one-shot particles alive at deterministic 700 ms");
  await page.waitForTimeout(50);
  await page.screenshot({ path: path.join(outputDir, "explosion-01-700ms.png") });

  const deterministicSample = JSON.stringify(explosionOne.emitters.map((emitter) => emitter.sampleSourcePositions));
  const repeatedExplosion = await playPreset("explosion_01", 700);
  assert(JSON.stringify(repeatedExplosion.emitters.map((emitter) => emitter.sampleSourcePositions)) === deterministicSample, "Restart reproduces the same seeded particle trajectories");

  await playPreset("smartbomb_01", 240);
  await page.waitForTimeout(50);
  await page.screenshot({ path: path.join(outputDir, "smartbomb-01-240ms.png") });

  for (const entry of initial.library.entries.filter((candidate) => !candidate.playable)) {
    const opaque = await playPreset(entry.id, 300);
    assert(!opaque.selected.readable && opaque.selected.availability === "opaque-compiled-only", `${entry.id} remains binary-only evidence`);
    assert(opaque.selected.reason?.length > 0, `${entry.id} explains why it cannot be revived exactly`);
    assert(opaque.selected.emitterConfigs.length === 0 && opaque.selected.attractorConfigs.length === 0, `${entry.id} does not invent emitter or attractor configuration`);
    assert(opaque.counts.activeEmitters === 0 && opaque.counts.emitterCapacity === 0 && opaque.counts.activeParticles === 0, `${entry.id} remains unsimulated`);
  }
  await playPreset("explosion3", 300);
  await page.waitForTimeout(50);
  await page.screenshot();
  await page.waitForTimeout(50);
  await page.screenshot({ path: path.join(outputDir, "explosion3-opaque-evidence.png") });

  await page.evaluate(() => window.__PARTICLE_PRESET_LIBRARY__.select("explosion_01"));
  await page.keyboard.press("ArrowRight");
  assert((await state()).selected.id === "explosion_02", "Right arrow selects the next preset");
  await page.keyboard.press("ArrowLeft");
  assert((await state()).selected.id === "explosion_01", "Left arrow selects the previous preset");
  await page.keyboard.press("Space");
  assert((await state()).paused, "Space pauses playback");
  const pausedTime = (await state()).totalElapsedSeconds;
  await page.evaluate(() => window.advanceTime(1000));
  assert(Math.abs((await state()).totalElapsedSeconds - pausedTime) < 1e-9, "Paused playback ignores deterministic time");
  await page.keyboard.press("Space");
  await page.keyboard.press("KeyA");
  assert((await state()).autoReplay, "A enables preview-only automatic replay");
  await page.evaluate(() => window.advanceTime(400));
  await page.keyboard.press("KeyR");
  assert((await state()).totalElapsedSeconds === 0 && (await state()).cycle === 1, "R restarts the selected preset at cycle one");

  assert(failedResponses.length === 0, "Preview and all present exact DDS textures load without failed responses");
  assert(errors.length === 0, "Preview emits no console or page errors");

  const finalState = await state();
  await writeFile(path.join(outputDir, "results.json"), JSON.stringify({ url, assertions, errors, failedResponses, finalState }, null, 2));
  console.log(`Particle preset library QA passed ${assertions.length}/${assertions.length} assertions.`);
} catch (error) {
  const message = error instanceof Error ? error.stack ?? error.message : String(error);
  await writeFile(path.join(outputDir, "results.json"), JSON.stringify({ url, assertions, errors, failedResponses, failure: message }, null, 2));
  console.error(message);
  process.exitCode = 1;
} finally {
  await browser.close();
}
