import fs from "node:fs";
import path from "node:path";
import { chromium } from "playwright";

const url = process.env.GRAPHYSX_URL ?? "http://127.0.0.1:4176/";
const requestedCase = process.argv[2] ?? "all";
const classicRaceIds = ["green-grid-run", "rotator-cube-works", "piston-gateworks"];
const allowedCases = new Set(["all", ...classicRaceIds, "suzanne1-classic"]);
if (!allowedCases.has(requestedCase)) throw new Error(`Unknown fidelity case: ${requestedCase}`);
const outputDir = path.resolve("output/playwright/classic-suzanne-fidelity");
fs.mkdirSync(outputDir, { recursive: true });

const browser = await chromium.launch({
  headless: true,
  args: ["--use-gl=angle", "--use-angle=swiftshader"]
});
const errors = [];

const assertions = [];
const states = {};
const assert = (condition, message) => {
  assertions.push({ pass: Boolean(condition), message });
  if (!condition) throw new Error(message);
};
const debug = (page, method, argument) =>
  page.evaluate(({ method, argument }) => window.__GRAPHYSX_DEBUG__[method](argument), { method, argument });
const textState = (page) => page.evaluate(() => JSON.parse(window.render_game_to_text()));
const openPage = async () => {
  const page = await browser.newPage({ viewport: { width: 1440, height: 960 } });
  page.on("console", (message) => {
    if (message.type() === "error") errors.push({ type: "console", text: message.text() });
  });
  page.on("pageerror", (error) => errors.push({ type: "page", text: String(error) }));
  await page.goto(url, { waitUntil: "domcontentloaded" });
  await page.waitForFunction(() => window.__GRAPHYSX_DEBUG__ && window.render_game_to_text);
  return page;
};

try {
  const requestedClassicRaces = requestedCase === "all" ? classicRaceIds : classicRaceIds.filter((raceId) => raceId === requestedCase);
  for (const raceId of requestedClassicRaces) {
    const racePage = await openPage();
    assert(await debug(racePage, "selectRace", raceId), `${raceId} remains reachable`);
    assert(await debug(racePage, "startRace"), `${raceId} starts`);
    await racePage.waitForFunction(() => window.__GRAPHYSX_DEBUG__.state() === "gameplay");
    await racePage.waitForTimeout(700);
    const snapshot = await debug(racePage, "snapshot");
    assert(snapshot.ringsTotal === 20, `${raceId} keeps all 20 checkpoints`);
    assert(snapshot.lapsTotal === 3, `${raceId} keeps all three laps`);
    assert(snapshot.playerRadius === 0.3, `${raceId} keeps the source BallZ radius`);
    states[raceId] = await textState(racePage);
    await racePage.screenshot({ path: path.join(outputDir, `${raceId}.png`) });
    await racePage.close();
  }

  if (requestedCase === "all" || requestedCase === "suzanne1-classic") {
    const suzannePage = await openPage();
    assert(await debug(suzannePage, "selectRace", "suzanne1-classic"), "Suzanne 1 remains reachable");
    await suzannePage.waitForFunction(() => window.__GRAPHYSX_DEBUG__.snapshot().loadState.ready);
    assert(await debug(suzannePage, "startRace"), "Suzanne 1 starts");
    await suzannePage.waitForFunction(() => window.__GRAPHYSX_DEBUG__.state() === "gameplay");
    await suzannePage.waitForTimeout(900);
    states.suzanne = await textState(suzannePage);
    assert(states.suzanne.suzanne.loadStatus === "ready", "Suzanne exact textures finish loading");
    assert(states.suzanne.suzanne.counts.walls === 208, "Suzanne keeps all 208 walls");
    assert(states.suzanne.suzanne.counts.chains === 45, "Suzanne keeps all 45 chains");
    assert(states.suzanne.suzanne.counts.ringsTotal === 15, "Suzanne keeps all 15 ZRing spheres");
    await suzannePage.screenshot({ path: path.join(outputDir, "suzanne1-classic.png") });
    await suzannePage.close();
  }

  assert(errors.length === 0, "Classic/Suzanne browser pass has no console or page errors");
} finally {
  const resultsName = requestedCase === "all" ? "results.json" : `results-${requestedCase}.json`;
  fs.writeFileSync(path.join(outputDir, resultsName), JSON.stringify({ url, requestedCase, assertions, states, errors }, null, 2));
  await browser.close();
}

console.log(`Classic/Suzanne browser QA passed ${assertions.length}/${assertions.length} assertions with ${errors.length} errors.`);
