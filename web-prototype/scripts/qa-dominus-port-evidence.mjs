import fs from "node:fs";
import path from "node:path";
import { chromium } from "playwright";

const url = process.env.GRAPHYSX_URL ?? "http://127.0.0.1:4185/dominus-port-evidence-preview.html";
const outputDir = path.resolve("output/playwright/dominus-port-evidence");
fs.mkdirSync(outputDir, { recursive: true });
const browser = await chromium.launch({ headless: true, args: ["--use-gl=angle", "--use-angle=swiftshader"] });
const page = await browser.newPage({ viewport: { width: 1365, height: 768 } });
const errors = [];
page.on("console", (message) => {
  if (message.type() === "error") errors.push({ type: "console", text: message.text() });
});
page.on("pageerror", (error) => errors.push({ type: "page", text: String(error) }));
const result = { url, assertions: [], states: {}, errors };

function assert(condition, message) {
  result.assertions.push({ pass: Boolean(condition), message });
  if (!condition) throw new Error(message);
}

const debug = (method, ...args) => page.evaluate(({ method, args }) => window.__DOMINUS_PORT_EVIDENCE_DEBUG__[method](...args), { method, args });
const state = () => page.evaluate(() => window.__DOMINUS_PORT_EVIDENCE_DEBUG__.getState());
const screenshot = async (name) => {
  await page.waitForTimeout(180);
  await page.screenshot({ path: path.join(outputDir, `${name}.png`) });
};

try {
  await page.goto(url, { waitUntil: "domcontentloaded" });
  await page.waitForFunction(() => window.__DOMINUS_PORT_EVIDENCE_DEBUG__?.getState().ready === true, null, { timeout: 30_000 });
  let current = await state();
  result.states.initial = current;
  assert(current.id === "dominus-port-evidence" && current.recoveryStatus === "PARTIAL", "Environment exposes an explicit PARTIAL recovery state");
  assert(current.classification === "exact ObjectLibrary port-subset evidence view", "Environment identifies the rendered evidence as an ObjectLibrary subset");
  assert(current.sourceDocumentSha256 === "4BF51E84F92084E8BE2B91214391BFA8839B29BCEB4C942187DD73B2B04AF432", "Structured state exposes the exact source-document hash");
  assert(current.sourceRecordCount === 61 && current.dominusRecordCount === 41 && current.portPlacementCount === 28, "Structured state exposes the exact 61/41/28 scope");
  assert(current.decodedPlacementCount === 27 && current.unsupportedPlacementCount === 1, "Structured state exposes the exact 27 decoded / 1 unsupported boundary");
  assert(current.sourceVertexCount === 23594 && current.sourceTriangleCount === 13860 && current.sourceMaterialGroupCount === 126, "Structured state exposes exact port-subset geometry totals");
  assert(current.placements.length === 28 && current.placements[0].id === "port_const1" && current.placements.at(-1).id === "port_windmill", "All 28 rows preserve their source order");
  assert(current.placements.every((placement) => placement.sourceScale.every((value) => value === 0.01) && placement.sourceRotation.every((value) => value === 0)), "Every row preserves exact 0.01 scale and zero rotation");
  assert(current.placements.every((placement) => placement.displayPosition[2] === -placement.sourcePosition[2]), "Web display mirrors source Z exactly and discloses it in state");
  assert(current.conclusion.includes("not an authored village/port layout"), "Structured state rejects a false authored-village claim");
  assert(current.absentEvidence.some((statement) => statement.includes("camera")), "Structured state exposes absent camera/world evidence");
  await screenshot("00-exact-objectlibrary-grid");

  for (const placement of current.placements) {
    assert(await debug("select", placement.id, false), `${placement.id} is selectable without changing source geometry`);
    current = await state();
    assert(current.selectedId === placement.id && current.selectedIndex === placement.index, `${placement.id} selection preserves source order`);
    assert(current.selectedDecoded === (placement.status === "recovered-text-x"), `${placement.id} selection preserves its decoder boundary`);
  }

  assert(await debug("select", "port_maindocks", true), "port_maindocks can be focused");
  current = await state();
  assert(current.viewMode === "selected-asset" && current.selectedId === "port_maindocks", "Focus enters selected-asset camera mode");
  const mainDocks = current.placements[current.selectedIndex];
  assert(mainDocks.vertices === 6384 && mainDocks.triangles === 3840 && mainDocks.materialGroups === 15, "Main docks exact source counts remain intact");
  await screenshot("01-main-docks-exact-focus");

  assert(await debug("select", "port_lighthouse", true), "port_lighthouse can be focused");
  current = await state();
  assert(current.selectedSourceBounds?.size[1] === 585.659608, "Lighthouse exact source height remains intact");
  await screenshot("02-lighthouse-exact-focus");

  assert(await debug("select", "port_crateshed", true), "port_crateshed evidence boundary can be focused");
  current = await state();
  assert(!current.selectedDecoded && current.unsupportedPlacementCount === 1, "Crate shed remains the only unsupported binary-X record");
  await screenshot("03-crateshed-binary-boundary");

  const orbitBefore = current.orbitAngleRadians;
  await debug("orbit", 0.31);
  assert(Math.abs((await state()).orbitAngleRadians - orbitBefore) > 0.3, "Deterministic camera orbit changes the selected-record view");
  await debug("zoom", 1.25);
  assert(Math.abs((await state()).zoom - 1.25) < 0.001, "Deterministic camera zoom changes the selected-record view");
  await page.evaluate(() => window.advanceTime(500));
  assert((await state()).selectedId === "port_crateshed", "Deterministic time advance does not mutate evidence selection");
  assert(!(await debug("select", "invented-port-asset", true)), "Unknown asset selection is rejected");
  assert((await state()).selectedId === "port_crateshed", "Rejected selection leaves source state unchanged");

  await debug("overview");
  assert((await state()).viewMode === "source-grid", "Overview restores the exact source-grid camera");
  await debug("reset");
  current = await state();
  assert(current.viewMode === "source-grid" && current.selectedId === "port_const1" && current.zoom === 1, "Reset restores the first source row and overview camera");
  assert(errors.length === 0, "Dominus port evidence browser has no console or page errors");
  result.states.final = current;
} finally {
  if (!result.states.final) result.states.final = await state().catch(() => null);
  fs.writeFileSync(path.join(outputDir, "results.json"), JSON.stringify(result, null, 2));
  await browser.close();
}

console.log(JSON.stringify({ assertions: result.assertions.length, errors: errors.length, outputDir }, null, 2));
