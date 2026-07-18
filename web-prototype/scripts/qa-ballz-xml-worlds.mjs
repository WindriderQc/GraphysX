#!/usr/bin/env node

import assert from "node:assert/strict";
import { mkdirSync, writeFileSync } from "node:fs";
import { fileURLToPath } from "node:url";
import { chromium } from "playwright";

const url = process.env.BALLZ_XML_WORLDS_URL ?? "http://127.0.0.1:6001/ballz-xml-worlds-preview.html";
const output = new URL("../output/playwright/ballz-xml-worlds/", import.meta.url);
mkdirSync(fileURLToPath(output), { recursive: true });

const browser = await chromium.launch({ headless: true });
const page = await browser.newPage({ viewport: { width: 1280, height: 720 }, deviceScaleFactor: 1 });
const errors = [];
page.on("pageerror", (error) => errors.push({ type: "pageerror", message: error.message }));
page.on("console", (message) => {
  if (message.type() === "error") errors.push({ type: "console", message: message.text() });
});

try {
  await page.goto(url, { waitUntil: "networkidle" });
  await page.waitForFunction(() => window.__BALLZ_XML_WORLDS_DEBUG__?.state().ready === true);

  let state = await page.evaluate(() => window.__BALLZ_XML_WORLDS_DEBUG__.state());
  assert.equal(state.sceneId, "myworld");
  assert.equal(state.objectCount, 4);
  assert.equal(state.renderedObjectCount, 2);
  assert.equal(state.unresolvedObjectCount, 2);
  assert.equal(state.assembledDiscoverableWorld, false);
  assert.deepEqual(state.exactTvmAssets.map((asset) => [asset.id, asset.vertices, asset.triangles]), [
    ["airplane-high", 10261, 11168]
  ]);
  await page.screenshot({ path: fileURLToPath(new URL("myworld-overview.png", output)) });

  assert.equal(await page.evaluate(() => window.__BALLZ_XML_WORLDS_DEBUG__.focus(1)), true);
  state = await page.evaluate(() => window.__BALLZ_XML_WORLDS_DEBUG__.state());
  assert.equal(state.focusedObjectIndex, 1);
  assert.equal(state.objects[1].name, "Cylinder");
  await page.screenshot({ path: fileURLToPath(new URL("myworld-cylinder-exact-scale.png", output)) });
  const orbitBefore = state.orbitAngleRadians;
  await page.evaluate(() => window.__BALLZ_XML_WORLDS_DEBUG__.orbit(0.6));
  state = await page.evaluate(() => window.__BALLZ_XML_WORLDS_DEBUG__.state());
  assert.ok(Math.abs(state.orbitAngleRadians - orbitBefore - 0.6) < 0.00001);
  assert.equal(await page.evaluate(() => window.__BALLZ_XML_WORLDS_DEBUG__.setObjectVisible(1, false)), true);
  assert.equal((await page.evaluate(() => window.__BALLZ_XML_WORLDS_DEBUG__.state())).objects[1].visible, false);
  await page.evaluate(() => window.__BALLZ_XML_WORLDS_DEBUG__.reset());

  await page.click('[data-scene="testworld"]');
  state = await page.evaluate(() => window.__BALLZ_XML_WORLDS_DEBUG__.state());
  assert.equal(state.sceneId, "testworld");
  assert.equal(state.objectCount, 8);
  assert.equal(state.renderedObjectCount, 5);
  assert.equal(state.unresolvedObjectCount, 3);
  assert.deepEqual(state.exactTvmAssets.map((asset) => [asset.id, asset.vertices, asset.triangles]), [
    ["airplane-lp", 5367, 9202]
  ]);
  assert.deepEqual(state.objects.filter((object) => object.rendered).map((object) => object.index), [0, 1, 2, 3, 4]);
  assert.deepEqual(state.objects.filter((object) => !object.rendered).map((object) => object.resolution), [
    "missing-file", "invalid-duplicate-target", "invalid-duplicate-target"
  ]);
  await page.screenshot({ path: fileURLToPath(new URL("testworld-overview.png", output)) });

  assert.equal(await page.evaluate(() => window.__BALLZ_XML_WORLDS_DEBUG__.focus(4)), true);
  state = await page.evaluate(() => window.__BALLZ_XML_WORLDS_DEBUG__.state());
  assert.equal(state.focusedObjectIndex, 4);
  assert.equal(state.objects[4].name, "Airplane");
  await page.screenshot({ path: fileURLToPath(new URL("testworld-airplane-focus.png", output)) });

  const textState = JSON.parse(await page.evaluate(() => window.render_game_to_text()));
  assert.equal(textState.mode, "ballz-xml-worlds-preview");
  assert.equal(textState.environment.sceneId, "testworld");
  assert.ok(textState.coordinateSystem.includes("+Z"));
  assert.deepEqual(errors, []);

  writeFileSync(fileURLToPath(new URL("results.json", output)), JSON.stringify({ passed: 28, errors, finalState: state }, null, 2));
  console.log("BallZ XML browser QA passed (28 assertions, 0 errors)");
  console.log(`Artifacts: ${fileURLToPath(output)}`);
} finally {
  await browser.close();
}
