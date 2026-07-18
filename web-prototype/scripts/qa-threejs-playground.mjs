import crypto from "node:crypto";
import fs from "node:fs";
import path from "node:path";
import { chromium } from "playwright";

const url = process.env.GRAPHYSX_URL ?? "http://127.0.0.1:4194/threejs-playground-preview.html";
const outputDir = path.resolve("output/playwright/threejs-playground");
fs.mkdirSync(outputDir, { recursive: true });

const expectedAssets = {
  "media/Airplane.glb": "2F9A5382E70BDBEF5D54024C6846A3A53389B889D73121159FB335352D311388",
  "textures/twoway.jpg": "4998527ACE4E853954E77EC5B937024CB2D2E23064FBBDE4A039224E978A2B21",
  "textures/earth/earth_uv_with_topo.jpg": "A9F0088972DEE0254610AF851C4D6838CA3F2CF79176987E0A5713E2C15EC042",
  "textures/sky/asteroids/asteroids_bk.jpg": "C93993EEA4760D30293B437B3D3191067291C6ADD2490DF06C69E99BF3C1203D",
  "textures/sky/asteroids/asteroids_dn.jpg": "14918B653F59E7D652AA71461EF1A4F9AC2037B7485CEC328CDFEEEE9ED87D1F",
  "textures/sky/asteroids/asteroids_ft.jpg": "4EB42FC7EB8C7EEE84FB63C174711634570FCE2C79EA1C484B35BC9F6452E3BC",
  "textures/sky/asteroids/asteroids_lf.jpg": "EB609931BA0B4C21D89447C03819B7CA06387EE505B8AB259B8212047AE6CE76",
  "textures/sky/asteroids/asteroids_rt.jpg": "A69148B4CFCECEC961C1319E3F262E2F46AB4F00DE5FBCF9805F3FB20EBCE582",
  "textures/sky/asteroids/asteroids_up.jpg": "87E1F43997F0A705A88A4BE1BE6C53301B8AFE0C10E7A8EAD83377E615FE9886"
};

const browser = await chromium.launch({ headless: true, args: ["--use-gl=angle", "--use-angle=swiftshader"] });
const page = await browser.newPage({ viewport: { width: 1365, height: 768 } });
const errors = [];
page.on("console", (message) => {
  if (message.type() === "error") errors.push({ type: "console", text: message.text() });
});
page.on("pageerror", (error) => errors.push({ type: "page", text: String(error) }));

const result = { url, assertions: [], states: {}, raycastNdc: null, errors };
function assert(condition, message) {
  result.assertions.push({ pass: Boolean(condition), message });
  if (!condition) throw new Error(message);
}
const debug = (method, ...args) => page.evaluate(({ method, args }) => window.__THREEJS_PLAYGROUND_DEBUG__[method](...args), { method, args });
const state = () => debug("getState");

try {
  for (const [relativePath, expectedHash] of Object.entries(expectedAssets)) {
    const bytes = fs.readFileSync(path.resolve("public/assets/threejs-playground", relativePath));
    const actualHash = crypto.createHash("sha256").update(bytes).digest("hex").toUpperCase();
    assert(actualHash === expectedHash, `${relativePath} remains byte-identical to the archive`);
  }

  await page.goto(url, { waitUntil: "domcontentloaded" });
  await page.waitForFunction(() => window.__THREEJS_PLAYGROUND_DEBUG__?.getState().ready === true, null, { timeout: 30_000 });
  await debug("reset");
  let current = await state();
  result.states.initial = current;
  assert(current.id === "threejs-playground" && current.loadStatus === "ready" && current.error === null, "Environment reaches ready state without a load error");
  assert(current.restorationStatus === "RESTORED", "Playground is complete at its authored composition and control scope");
  assert(current.source.assetCount === 9 && current.source.totalBytes === 3712102, "State reports the exact nine-asset, 3,712,102-byte source payload");
  assert(Object.keys(current.source.assets).length === 9, "State exposes all nine SHA-256 source records");
  assert(current.sky.ready && current.sky.name === "asteroids" && current.sky.faceCount === 6 && current.sky.size === 10000, "Authored six-face, 10000-unit asteroids sky is ready");
  assert(current.airplane.loaded && current.airplane.scale === 10 && current.airplane.orbitRadius === 5, "Archived Airplane.glb loads with scale and orbit radius intact");
  assert(current.morphSpheres.count === 3 && current.morphSpheres.shaderActive, "All three archived shader morph spheres are active");
  assert(JSON.stringify(current.morphSpheres.positions) === JSON.stringify([[0, 20, 20], [10, 20, 20], [20, 20, 20]]), "Morph positions preserve the authored position.set(x,y,y) result");
  assert(current.earthSpheres.count === 3 && current.earthSpheres.sharedEarthTexture, "Earth, Mars, and Moon retain the shared archived Earth texture");
  assert(current.terrain.vertexCount === 121 && current.terrain.raycastEnabled, "Deterministic red terrain preserves 121 vertices and raycast support");
  assert(current.animatedLight.active && current.animatedLight.sourcePeriodFrames === 240, "Archived animated spotlight formula is active");
  assert(current.presentationAdapters.length >= 7, "State explicitly discloses every modernization/presentation adapter");
  assert(current.gui.restored && current.gui.controllerCount === 16 && current.gui.folders.length === 8, "All 16 source dat.GUI controllers and eight named folders are restored");
  assert(current.gui.cube.speedX === 2000 && current.gui.cube.speedY === 1000 && current.gui.plane.width === 10 && current.gui.plane.height === 10, "GUI defaults match Cube and Plane source constructors");
  assert(await page.locator("[data-playground-param]").count() === 16, "Isolated preview exposes all 16 controls as visible native range inputs");
  await page.screenshot({ path: path.join(outputDir, "00-ready-composition.png") });

  await page.evaluate(() => window.advanceTime(1000));
  current = await state();
  result.states.afterOneSecond = current;
  assert(current.elapsedSeconds === 1 && current.frameCount60Hz === 61, "advanceTime deterministically advances exactly one second at 60 Hz");
  assert(Math.abs(current.earthSpheres.rotationYRadians - 1) < 0.000001, "Earth spheres retain the archived one-radian-per-second rotation formula");
  assert(Math.abs(current.airplane.orbitAngleRadians - 1.745329) < 0.00001, "Airplane retains the archived time/360 × pi × 0.2 orbit formula");
  assert(current.animatedLight.intensity > 126 && current.animatedLight.intensity < 127, "Spotlight intensity follows the archived sinusoidal formula");
  assert(current.fps.value === 1 && current.fps.band === "red" && await page.locator("#fps-progress").getAttribute("max") === "100", "Original FPS value, red threshold and 0-100 progress scale are live");

  await page.locator('[data-playground-param="plane.width"]').evaluate((input) => {
    input.value = "20";
    input.dispatchEvent(new Event("input", { bubbles: true }));
  });
  await page.locator('[data-playground-param="plane.height"]').evaluate((input) => {
    input.value = "12";
    input.dispatchEvent(new Event("input", { bubbles: true }));
  });
  assert(await debug("setParameter", "cube.speedX", 800), "Cube speedX accepts the exact archived 1-3600 GUI lane");
  assert(await debug("setParameter", "earth.speedY", 500), "Earth speedY accepts the exact archived 1-3600 GUI lane");
  assert(await debug("setParameter", "camera.x", 120), "Camera X accepts the exact archived -1000 to 1000 GUI lane");
  current = await state();
  assert(current.terrain.width === 20 && current.terrain.height === 12 && current.terrain.vertexCount === 273, "Plane GUI rebuilds the source width/height segmented terrain deterministically");
  assert(current.gui.cube.speedX === 800 && current.gui.planets[0].speedY === 500 && current.gui.camera.x === 120, "Cube, planet and camera GUI changes reach the live environment state");
  await page.screenshot({ path: path.join(outputDir, "01-gui-and-fps-restored.png") });
  await debug("reset");
  await page.evaluate(() => window.advanceTime(1000));

  for (let y = -0.95; y <= 0.95 && !result.raycastNdc; y += 0.05) {
    for (let x = -0.95; x <= 0.95; x += 0.05) {
      if (await debug("raycast", Number(x.toFixed(2)), Number(y.toFixed(2)))) {
        result.raycastNdc = [Number(x.toFixed(2)), Number(y.toFixed(2))];
        break;
      }
    }
  }
  current = await state();
  assert(Boolean(result.raycastNdc) && current.terrain.raycastHit && current.terrain.lastRaycastPoint !== null, "The presented terrain returns a real world-space raycast hit");
  await page.screenshot({ path: path.join(outputDir, "02-terrain-raycast-active.png") });

  const orbitBeforeKeyboard = current.camera.orbitAngleRadians;
  await page.keyboard.down("KeyA");
  await page.evaluate(() => window.advanceTime(500));
  await page.keyboard.up("KeyA");
  current = await state();
  assert(current.camera.orbitAngleRadians < orbitBeforeKeyboard - 0.35, "A-key deterministic input orbits the presentation camera");

  const canvas = page.locator("#viewport");
  const bounds = await canvas.boundingBox();
  assert(Boolean(bounds), "Preview exposes one interactive WebGL canvas");
  const dragStart = { x: bounds.x + bounds.width * 0.48, y: bounds.y + bounds.height * 0.46 };
  const orbitBeforeDrag = current.camera.orbitAngleRadians;
  await page.mouse.move(dragStart.x, dragStart.y);
  await page.mouse.down();
  await page.mouse.move(dragStart.x + 110, dragStart.y, { steps: 8 });
  await page.mouse.up();
  await page.waitForTimeout(250);
  current = await state();
  assert(Math.abs(current.camera.orbitAngleRadians - orbitBeforeDrag) > 0.15, "Mouse drag drives the restored OrbitControls");

  const zoomBeforeWheel = current.camera.zoom;
  await page.mouse.wheel(0, 380);
  await page.waitForTimeout(250);
  current = await state();
  assert(Math.abs(current.camera.zoom - zoomBeforeWheel) > 0.05, "Mouse wheel drives OrbitControls zoom and synchronized structured state");
  await page.screenshot({ path: path.join(outputDir, "03-orbit-and-zoom-controls.png") });

  const renderedTextState = JSON.parse(await page.evaluate(() => window.render_game_to_text()));
  current = await state();
  assert(renderedTextState.id === current.id && Math.abs(renderedTextState.camera.orbitAngleRadians - current.camera.orbitAngleRadians) < 0.02, "render_game_to_text matches the visible environment state while OrbitControls damping settles");
  assert(errors.length === 0, "Preview produces no console or page errors");

  await debug("reset");
  current = await state();
  assert(current.elapsedSeconds === 0 && current.camera.orbitAngleRadians === 0.72 && current.camera.zoom === 1 && current.terrain.vertexCount === 121 && current.gui.cube.speedX === 2000, "Reset restores deterministic time, presentation framing and all GUI defaults");
  result.states.final = current;
  await debug("dispose");
  assert((await state()).loadStatus === "disposed", "dispose releases the isolated environment and marks state disposed");
} finally {
  if (!result.states.final) result.states.final = await state().catch(() => null);
  fs.writeFileSync(path.join(outputDir, "results.json"), JSON.stringify(result, null, 2));
  await browser.close();
}

console.log(JSON.stringify({ assertions: result.assertions.length, raycastNdc: result.raycastNdc, errors: errors.length, outputDir }, null, 2));
