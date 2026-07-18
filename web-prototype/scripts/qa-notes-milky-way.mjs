import { mkdir } from "node:fs/promises";
import { chromium } from "playwright";

const baseUrl = process.env.GRAPHYSX_QA_URL ?? "http://127.0.0.1:4182";
const outputDirectory = "output/playwright/notes-milky-way-audit";
await mkdir(outputDirectory, { recursive: true });

function assert(condition, message) {
  if (!condition) throw new Error(message);
}

function near(actual, expected, tolerance = 0.001) {
  return Math.abs(actual - expected) <= tolerance;
}

async function state(page) {
  return page.evaluate(() => JSON.parse(window.render_game_to_text()));
}

async function waitReady(page) {
  await page.waitForFunction(() => {
    const snapshot = JSON.parse(window.render_game_to_text());
    return snapshot.environment.loadStatus === "ready";
  });
}

const browser = await chromium.launch({ headless: true });
const page = await browser.newPage({ viewport: { width: 1280, height: 720 }, deviceScaleFactor: 1 });
const failures = [];
page.on("console", (message) => {
  if (message.type() === "error") failures.push(`console: ${message.text()}`);
});
page.on("pageerror", (error) => failures.push(`page: ${error.message}`));

await page.goto(`${baseUrl}/notes-manager-preview.html`, { waitUntil: "networkidle" });
await waitReady(page);
let snapshot = await state(page);
assert(snapshot.environment.activeNotes === 0, "Notes Manager must start with zero active notes.");
assert(snapshot.environment.capacity === 50, "Notes Manager capacity must remain exactly 50.");

const addPoint = await page.evaluate(() => window.__NOTES_MANAGER_DEBUG__.addButtonScreenPoint());
await page.mouse.click(addPoint[0], addPoint[1]);
snapshot = await state(page);
assert(snapshot.environment.activeNotes === 1, "Clicking the actual marble add cube must enable one note.");
assert(
  JSON.stringify(snapshot.environment.activeNotePositions[0]) === JSON.stringify([0, 50, 0]),
  "The first note must use the exact archived position [0,50,0]."
);

await page.keyboard.press("a");
snapshot = await state(page);
assert(snapshot.environment.activeNotes === 2, "The preview A shortcut must execute the same add behavior.");
assert(
  JSON.stringify(snapshot.environment.activeNotePositions[1]) === JSON.stringify([0, 50, 20]),
  "The second note must use the exact archived position [0,50,20]."
);

await page.evaluate(() => window.__NOTES_MANAGER_DEBUG__.setActiveNotes(50));
await page.keyboard.press("a");
snapshot = await state(page);
assert(snapshot.environment.activeNotes === 50, "Adding beyond capacity must leave the exact count at 50.");
assert(snapshot.environment.nextNoteIndex === null, "A full block must expose no next note index.");

await page.keyboard.press("r");
snapshot = await state(page);
assert(snapshot.environment.activeNotes === 0, "R must reset the isolated preview.");
await page.evaluate(() => window.__NOTES_MANAGER_DEBUG__.setActiveNotes(12));
await page.screenshot({ path: `${outputDirectory}/notes-manager-12-notes.png` });

await page.goto(`${baseUrl}/milky-way-preview.html`, { waitUntil: "networkidle" });
await waitReady(page);
snapshot = await state(page);
assert(snapshot.environment.profile === "graphysx2017", "The exact later profile must be the default.");
assert(snapshot.environment.exactAssetBinding === true, "The later profile must report exact asset binding.");
assert(snapshot.environment.generatedStars === 0, "Voie Lactée must not invent a star field.");
assert(snapshot.environment.planets.length === 5, "The vignette must contain its five authored meshes.");
assert(
  JSON.stringify(snapshot.environment.planets.map((planet) => [planet.id, planet.radius, planet.position])) ===
    JSON.stringify([
      ["earth", 6, [-10, 15, 10]],
      ["clouds", 6.1, [-10, 15, 10]],
      ["moon", 2, [-8, 14, 12]],
      ["mars", 12, [-40, 15, 10]],
      ["venus", 4, [-70, 15, 10]]
    ]),
  "The later planet positions/radii must match CreateVoieLactee exactly."
);
await page.evaluate(() => {
  window.__MILKY_WAY_DEBUG__.reset();
  window.advanceTime(1000);
});
snapshot = await state(page);
assert(snapshot.environment.earthRotationDegrees === 0, "The later static profile must not invent rotation.");
await page.screenshot({ path: `${outputDirectory}/milky-way-graphysx2017.png` });

await page.goto(`${baseUrl}/milky-way-preview.html?profile=ballz2015`, { waitUntil: "networkidle" });
await waitReady(page);
await page.evaluate(() => {
  window.__MILKY_WAY_DEBUG__.reset();
  window.advanceTime(1000);
});
snapshot = await state(page);
assert(snapshot.environment.profile === "ballz2015", "The legacy query must select the BallZ-2015 profile.");
assert(snapshot.environment.exactAssetBinding === false, "The legacy profile must disclose substituted maps.");
assert(near(snapshot.environment.earthRotationDegrees, 1.2), "Legacy Earth must rotate 0.02 degrees per 60 Hz update.");
assert(near(snapshot.environment.cloudRotationDegrees, 3.6), "Legacy clouds must rotate 0.06 degrees per 60 Hz update.");
assert(near(snapshot.environment.moonOrbitDegrees, -3), "Legacy moon orbit must use -0.003 degrees per elapsed millisecond.");
await page.screenshot({ path: `${outputDirectory}/milky-way-ballz2015.png` });

assert(failures.length === 0, `Browser errors were captured:\n${failures.join("\n")}`);
await browser.close();
console.log("Notes Manager / Voie Lactée QA passed: exact 3D note flow, exact later planets, disclosed partial legacy motion.");

