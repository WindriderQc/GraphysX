import fs from "node:fs/promises";
import path from "node:path";
import { chromium } from "playwright";

const baseUrl = process.argv[2] ?? process.env.GRAPHYSX_URL ?? "http://127.0.0.1:4194";
const url = new URL("/stockroom-xml-artifact-preview.html", baseUrl).href;
const outputDir = path.resolve("output/playwright/stockroom-xml-artifacts");
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
  ({ method, args }) => window.__STOCKROOM_XML_ARTIFACT_HARNESS__[method](...args),
  { method, args }
);
const state = () => page.evaluate(() => window.__STOCKROOM_XML_ARTIFACT_HARNESS__.state());
const textState = () => page.evaluate(() => JSON.parse(window.render_game_to_text()));
const screenshot = async (name) => {
  await page.waitForTimeout(80);
  await page.screenshot({ path: path.join(outputDir, `${name}.png`) });
};
const distance = (left, right) => Math.hypot(...left.map((value, index) => value - right[index]));

try {
  await page.goto(url, { waitUntil: "domcontentloaded" });
  await page.waitForFunction(() => Boolean(window.__STOCKROOM_XML_ARTIFACT_HARNESS__ && window.render_game_to_text));
  await page.waitForFunction(() => window.__STOCKROOM_XML_ARTIFACT_HARNESS__.state().ready);

  let current = await state();
  report.states.baseSource = current;
  assert(current.selectedArtifactId === "base-scene", "BaseScene is the default artifact");
  assert(current.version === "V1_1" && current.root === "Scene3D", "BaseScene v1.1 schema is exposed");
  assert(current.sourceSha256 === "1F56E50E0FE48B6F7BF34024FA11C010755C3F59FA6FB2F70377BA0B8574CCA8", "BaseScene source hash matches");
  assert(current.objectCount === 18 && current.renderedObjectCount === 18, "All 18 serialized BaseScene records render");
  assert(current.uniqueObjectSignatureCount === 1, "BaseScene contains one unique serialized object signature");
  assert(current.distinctSerializedTransformCount === 1 && current.exactOverlapGroupSizes[0] === 18, "All BaseScene objects remain exactly coincident");
  assert(current.objects.every((object) => object.name === "CubeTestPhys" && object.action === "PHYSICCUBE"), "BaseScene object names/actions match");
  assert(current.objects.every((object) => JSON.stringify(object.position) === "[0,0,0]" && JSON.stringify(object.scale) === "[10,10,10]"), "BaseScene transforms remain exact");
  assert(current.objects.every((object) => object.sourceRotation === null), "Absent BaseScene Rot fields remain explicit");
  assert(current.inspectionAdapters.identityRotationForAbsentSourceFieldCount === 18, "Identity rotation adapter is disclosed for all 18 BaseScene records");
  assert(current.physics.zeroMassRecordCount === 18 && !current.physics.simulated, "BaseScene zero-mass physics metadata is preserved without invented simulation");
  assert(current.copyEquivalence.byteIdentical && current.copyEquivalence.semanticEquivalent, "Both BaseScene copies are byte-identical");
  assert(!current.material.candidateTextureEnabled && current.material.candidateTextureReady, "Unproven BaseScene texture candidate is ready but disabled by default");
  assert(current.distinctAssembledScene === false, "BaseScene is explicitly not classified as an assembled scene");
  assert(Object.entries(current.serializedSubsystems).filter(([name]) => ["camera", "lights", "sky", "gravity", "physicsTimestep", "interactions", "gameplayObjective"].includes(name)).every(([, present]) => present === false), "BaseScene does not invent absent world subsystems");
  let text = await textState();
  assert(text.diagnostic === "stockroom-xml-serializer-artifacts", "render_game_to_text identifies the artifact preview");
  assert(text.state.visibleObjects.length === 18, "render_game_to_text exposes all currently visible source records");
  await harness("orbit", false);
  await harness("resetCamera");
  await screenshot("00-base-scene-exact-overlap-neutral");

  current = await harness("candidateTexture", true);
  assert(current.material.candidateTextureEnabled, "Exact same-folder twoway.jpg candidate can be explicitly enabled");
  assert(current.material.candidateStatus.includes("runtime-binding-unproven"), "Candidate binding remains visibly unproven");
  assert(current.material.candidateSha256 === "4998527ACE4E853954E77EC5B937024CB2D2E23064FBBDE4A039224E978A2B21", "Candidate texture hash matches exact source art");
  await screenshot("01-base-scene-opt-in-texture-candidate");

  current = await harness("select", "test1");
  report.states.test1 = current;
  assert(current.version === "V1_2", "test1 exposes v1.2 schema");
  assert(current.sourceSha256 === "BB8396EBA946E01948BA1072C2D683A505719CBC43AA4BE535784CA31FE0672F", "Canonical test1 source hash matches");
  assert(current.objectCount === 1 && current.renderedObjectCount === 1, "test1 renders its one serialized cube");
  assert(current.objects[0].name === "cubBasic" && current.objects[0].meshControlled, "test1 name and MeshControlled flag match");
  assert(current.objects[0].massAttributeName === "Masse" && current.objects[0].newtonMaterial === 1, "test1 preserves attribute case and Newton material ID");
  assert(JSON.stringify(current.objects[0].position) === "[0,0,0]" && JSON.stringify(current.objects[0].sourceRotation) === "[0,0,0]" && JSON.stringify(current.objects[0].scale) === "[1,1,1]", "test1 exact transform matches");
  assert(current.objects[0].textureName === "" && !current.material.candidateTextureEnabled, "test1 keeps its empty texture and does not inherit BaseScene candidate art");
  assert(current.mapSize === 0 && current.ringPositionCount === 0, "test1 map/ring metadata matches");
  assert(!current.copyEquivalence.byteIdentical && current.copyEquivalence.semanticEquivalent, "test1 raw newline variants remain semantically equivalent");
  assert(current.sourceCopies[0].sha256 !== current.sourceCopies[1].sha256 && current.sourceCopies[0].whitespaceSemanticSha256 === current.sourceCopies[1].whitespaceSemanticSha256, "test1 reports both distinct raw hashes and one semantic hash");
  assert(current.distinctAssembledScene === false && current.classification.includes("schema-smoke"), "test1 remains a schema smoke artifact, not a world");
  await harness("orbit", false);
  await harness("resetCamera");
  await screenshot("02-test1-exact-single-cube");

  const before = (await textState()).camera.position;
  await harness("orbit", true);
  await page.evaluate(() => window.advanceTime(1000));
  text = await textState();
  assert(distance(before, text.camera.position) > 0.5, "advanceTime deterministically advances the inspection orbit");
  assert(text.state.selectedArtifactId === "test1" && text.state.objectCount === 1, "Structured state remains synchronized after deterministic stepping");
  assert(browserProblems.length === 0, "Standalone StockRoom XML preview has no console or page errors");
} finally {
  report.states.final = await state().catch(() => null);
  await fs.writeFile(path.join(outputDir, "results.json"), JSON.stringify(report, null, 2));
  await browser.close();
}

console.log(JSON.stringify({
  assertions: report.assertions.length,
  browserProblems: browserProblems.length,
  outputDir
}, null, 2));
