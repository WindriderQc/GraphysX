import fs from "node:fs/promises";
import path from "node:path";
import { chromium } from "playwright";

const baseUrl = process.argv[2] ?? process.env.GRAPHYSX_URL ?? "http://127.0.0.1:4192";
const url = new URL("/ballz-track-gallery-preview.html", baseUrl).href;
const outputDir = path.resolve("output/playwright/ballz-track-gallery");
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
  ({ method, args }) => window.__BALLZ_TRACK_GALLERY_HARNESS__[method](...args),
  { method, args }
);
const state = () => page.evaluate(() => window.__BALLZ_TRACK_GALLERY_HARNESS__.state());
const textState = () => page.evaluate(() => JSON.parse(window.render_game_to_text()));
const screenshot = async (name) => {
  await page.waitForTimeout(60);
  await page.screenshot({ path: path.join(outputDir, `${name}.png`) });
};
const distance = (left, right) => Math.hypot(...left.map((value, index) => value - right[index]));

const expected = [
  { id: "slide1a-legacy-active", vertices: 2271, triangles: 2849, groups: 1, components: 53, boundary: 1587, nonManifold: 0, uv: 1601, host: "source-hosted-static-level", computedNormals: false },
  { id: "level-slides", vertices: 792, triangles: 1524, groups: 3, components: 15, boundary: 0, nonManifold: 0, uv: 1, host: "no-located-host", computedNormals: true },
  { id: "level-steps", vertices: 200, triangles: 120, groups: 3, components: 30, boundary: 240, nonManifold: 0, uv: 4, host: "no-located-host", computedNormals: true },
  { id: "slide-bump", vertices: 699, triangles: 1456, groups: 1, components: 1, boundary: 44, nonManifold: 61, uv: 4, host: "no-located-host", computedNormals: true },
  { id: "slide-bump-gridtex", vertices: 685, triangles: 1456, groups: 1, components: 1, boundary: 8, nonManifold: 61, uv: 528, host: "no-located-host", computedNormals: true },
  { id: "ballz-track1", vertices: 385, triangles: 447, groups: 1, components: 15, boundary: 293, nonManifold: 0, uv: 288, host: "source-hosted-static-scene", computedNormals: false }
];

try {
  await page.goto(url, { waitUntil: "domcontentloaded" });
  await page.waitForFunction(() => Boolean(window.__BALLZ_TRACK_GALLERY_HARNESS__ && window.render_game_to_text));
  await page.waitForFunction(() => window.__BALLZ_TRACK_GALLERY_HARNESS__.state().assetsReady);

  let current = await state();
  assert(current.mode === "evidence-bounded-non-gameplay-gallery" && !current.playable, "Gallery starts as an explicitly non-playable evidence visit");
  assert(current.textureLoadErrors.length === 0, "All four exact gallery textures load successfully");
  assert(current.exclusions.map((entry) => entry.id).join(",") === "slide-long1,pipe1", "SlideLong1 and pipe1 aliases stay excluded");
  const rendered = await textState();
  assert(rendered.diagnostic === "ballz-track-gallery" && rendered.state.asset.connectedComponents === 53, "render_game_to_text exposes concise visible gallery state");

  const statesById = {};
  for (let index = 0; index < expected.length; index += 1) {
    const item = expected[index];
    current = await harness("select", item.id);
    statesById[item.id] = current;
    assert(current.selectedAssetId === item.id && !current.playable, `${item.id} selects as non-playable`);
    assert(current.asset.vertexCount === item.vertices && current.asset.triangleCount === item.triangles, `${item.id} exact vertex/triangle totals match`);
    assert(current.asset.exactGroupCount === item.groups, `${item.id} exact material-group count matches`);
    assert(current.asset.topology.connectedComponents.length === item.components, `${item.id} exact connected-component count matches`);
    assert(current.asset.topology.boundaryEdgeCount === item.boundary && current.asset.topology.nonManifoldEdgeCount === item.nonManifold, `${item.id} exact topology diagnostics match`);
    assert(current.asset.uv.uniquePairCount === item.uv, `${item.id} exact UV cardinality matches`);
    assert((current.asset.displayNormalAdapter === "computed-for-inspection") === item.computedNormals, `${item.id} source-normal/display-adapter boundary matches`);
    assert(current.hostEvidence.status === item.host, `${item.id} host evidence status remains bounded`);
    assert(current.statusRecommendation.status === "PARTIAL", `${item.id} recommends PARTIAL rather than false restoration`);
    assert(current.normalization.preservesAspectAndOrientation && current.normalization.status === "inferred-reversible-display-only", `${item.id} discloses reversible display normalization`);

    const sourcePoint = current.normalization.sourceWorldBounds.min;
    const displayPoint = await harness("sourceWorldToDisplay", item.id, sourcePoint);
    const restoredPoint = await harness("displayToSourceWorld", item.id, displayPoint);
    assert(distance(sourcePoint, restoredPoint) < 0.000001, `${item.id} display conversion round-trips to source coordinates`);

    await harness("camera", "overview");
    await harness("materials", "source-evidence");
    await harness("edges", true);
    await harness("bounds", false);
    await screenshot(`${String(index).padStart(2, "0")}-${item.id}`);
  }
  report.states.assets = statesById;

  const slide1a = statesById["slide1a-legacy-active"];
  assert(JSON.stringify(slide1a.hostEvidence.position) === JSON.stringify([0, -5000, 0]), "Slide1A preserves exact older host position (0,-5000,0)");
  assert(slide1a.hostEvidence.materialMode === "host-overrides-all-groups-with-StdMat", "Slide1A preserves the exact host StdMat override");

  const slides = statesById["level-slides"];
  assert(slides.material.embeddedRecords.length === 3 && slides.material.resolvedTextures.length === 0, "Level.Slides preserves three embedded color groups without invented textures");

  const steps = statesById["level-steps"];
  assert(JSON.stringify(steps.material.resolvedTextures.map((binding) => binding.textureName)) === JSON.stringify(["grass.jpg", "concrete.png", "wood.jpg"]), "Level.Steps preserves exact group-to-texture bindings");
  assert(steps.material.resolvedTextures.every((binding) => binding.sha256 && binding.browserPath), "Level.Steps exact texture bindings are hash-backed browser assets");

  const bump = statesById["slide-bump"];
  const gridBump = statesById["slide-bump-gridtex"];
  assert(bump.material.neutralFallback, "SlideBump exposes its zeroed material through a disclosed neutral fallback");
  assert(!gridBump.material.neutralFallback && gridBump.material.resolvedTextures[0].textureName === "EarthGri.bmp", "GridTex preserves its exact EarthGri material binding");
  assert(bump.asset.sha256 !== gridBump.asset.sha256 && bump.asset.vertexCount !== gridBump.asset.vertexCount, "Bump revisions remain distinct binaries/geometry");
  assert(JSON.stringify(bump.asset.exactBounds) === JSON.stringify(gridBump.asset.exactBounds) && bump.asset.triangleCount === gridBump.asset.triangleCount, "Bump revisions retain identical bounds/triangle evidence without deduplication");

  const track = statesById["ballz-track1"];
  assert(JSON.stringify(track.hostEvidence.ballSpawn) === JSON.stringify([-20, 310, 225]), "BallZTrack1 preserves exact source ball spawn evidence");
  assert(JSON.stringify(track.hostEvidence.chaseOffset) === JSON.stringify([25, 100, 30]), "BallZTrack1 preserves exact source chase offset evidence");
  assert(track.hostEvidence.gameplayBoundary.includes("UpdateScene is empty"), "BallZTrack1 keeps empty update/input and placeholder-ring boundary visible");

  current = await harness("select", "level-slides");
  current = await harness("materials", "diagnostic-groups");
  assert(current.material.mode === "diagnostic-groups", "Material-group diagnostic mode is selectable");
  current = await harness("edges", false);
  current = await harness("bounds", true);
  assert(!current.diagnostics.edgesVisible && current.diagnostics.boundsVisible, "Edge and bounds diagnostics toggle independently");
  await harness("camera", "top");
  let text = await textState();
  assert(text.camera.profile === "top", "Top camera profile is reflected in structured state");
  await screenshot("06-level-slides-diagnostic-top");

  await harness("camera", "overview");
  await harness("orbit", false);
  const beforeCamera = (await textState()).camera.position;
  await harness("orbit", true);
  await page.evaluate(() => window.advanceTime(1000));
  text = await textState();
  assert(distance(beforeCamera, text.camera.position) > 1, "advanceTime deterministically advances auto-orbit camera state");

  const invalidRejected = await page.evaluate(() => {
    try {
      window.__BALLZ_TRACK_GALLERY_HARNESS__.select("slide-long1");
      return false;
    } catch {
      return true;
    }
  });
  assert(invalidRejected, "Harness rejects excluded aliases instead of duplicating them");
  assert(browserProblems.length === 0, "BallZ track gallery has no browser console or page errors");
} finally {
  report.states.final = await state().catch(() => null);
  await fs.writeFile(path.join(outputDir, "results.json"), JSON.stringify(report, null, 2));
  await browser.close();
}

console.log(JSON.stringify({ assertions: report.assertions.length, browserProblems: browserProblems.length, outputDir }, null, 2));
