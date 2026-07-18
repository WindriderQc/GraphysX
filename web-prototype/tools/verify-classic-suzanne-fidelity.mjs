import { createHash } from "node:crypto";
import { readFileSync } from "node:fs";
import { dirname, join, resolve } from "node:path";
import { fileURLToPath } from "node:url";

const here = dirname(fileURLToPath(import.meta.url));
const projectRoot = resolve(here, "..");
const repositoryRoot = resolve(projectRoot, "..");
const stockRoom = resolve(process.env.GRAPHYSX_STOCKROOM ?? "E:\\Media\\Datalake\\Tech\\StockRoom");
const assertions = [];

function assert(condition, message) {
  assertions.push({ pass: Boolean(condition), message });
  if (!condition) throw new Error(message);
}

function sha256(path) {
  return createHash("sha256").update(readFileSync(path)).digest("hex");
}

function countSymbols(rows) {
  const counts = {};
  for (const symbol of rows.join("")) counts[symbol] = (counts[symbol] ?? 0) + 1;
  return counts;
}

const expectedLevels = [
  {
    file: "Level1_base.ASCII",
    width: 20,
    height: 20,
    symbols: { T: 106, R: 20, "@": 1, F: 1, f: 1, H: 1, h: 1 },
    floor: "\\Texture\\Alien\\Alien01_B_diff.bmp",
    normal: "\\Texture\\Alien\\Alien01_B_normal.bmp",
    sky: "ClearBlue",
    humans: 0,
    score: 29815.1465
  },
  {
    file: "Level2_base.ASCII",
    width: 20,
    height: 20,
    symbols: { Z: 140, R: 20, "@": 1, F: 1, f: 1, H: 1, h: 1 },
    floor: "Checkerboard.png",
    normal: "",
    sky: "LostValley",
    humans: 10,
    score: 122428.984
  },
  {
    file: "Level3_base.ASCII",
    width: 20,
    height: 19,
    symbols: { M: 157, r: 20, "$": 1, F: 1, f: 1, H: 1, h: 1 },
    floor: "\\Texture\\Alien\\Alien02_diff.bmp",
    normal: "\\Texture\\Alien\\Alien02_normal.bmp",
    sky: "NightSky",
    humans: 0,
    score: 158507.313
  }
];

const levelList = readFileSync(join(stockRoom, "levelList.xml"), "utf8");
const levelBlocks = [...levelList.matchAll(/<Level>([\s\S]*?)<\/Level>/g)].map((match) => match[1]);
assert(levelBlocks.length === 3, "levelList.xml contains exactly the three classic levels");

for (const [index, expected] of expectedLevels.entries()) {
  const rows = readFileSync(join(stockRoom, expected.file), "utf8").replace(/^\uFEFF/, "").trimEnd().split(/\r?\n/);
  const counts = countSymbols(rows);
  assert(rows.length === expected.height && rows.every((row) => row.length === expected.width), `${expected.file} preserves its exact grid dimensions`);
  for (const [symbol, count] of Object.entries(expected.symbols)) {
    assert(counts[symbol] === count, `${expected.file} preserves ${count} ${JSON.stringify(symbol)} symbols`);
  }
  const block = levelBlocks[index];
  const field = (tag) => block.match(new RegExp(`<${tag}>([\\s\\S]*?)<\\/${tag}>`))?.[1] ?? "";
  assert(field("FloorTex") === expected.floor, `${expected.file} keeps its archived floor binding`);
  assert(field("FloorNormals") === expected.normal, `${expected.file} keeps its archived normal-map binding`);
  assert(field("SkyDay") === expected.sky, `${expected.file} keeps its archived sky assignment`);
  assert(Number(field("iNumHuman")) === expected.humans, `${expected.file} keeps its archived human count`);
  assert(Number(field("nbrTour")) === 3, `${expected.file} keeps its three-lap setting`);
  assert(Number(field("ScoreBest")) === expected.score, `${expected.file} keeps its archived best score`);
}

const ballzScreen = readFileSync(join(repositoryRoot, "GraphysX", "ArduinoGUI", "ArduinoGUI", "ScreenSys", "BallZScreen.cs"), "utf8");
for (const sourceFact of [
  "m_scene.Cam_setPos(0, 10, -10, 10, 5, 10)",
  "m_scene.Cam_alignWith(center, pos)",
  "ringSys.setBillboardSize(0.5f)",
  "ringSys.addRings(0.45f)",
  "stLight.position = new Vec3D(8, 20, 8)",
  "stLight.position.x = 32",
  "stLight.position.z = 32",
  "Scale = new Vec3D(0.3f, 32, 32)",
  "zk.setThrottle(true)",
  "zk.setThrottle(false)"
]) {
  assert(ballzScreen.includes(sourceFact), `BallZScreen.cs preserves ${sourceFact}`);
}

const styleSource = readFileSync(join(projectRoot, "src", "classic-level-style.ts"), "utf8");
assert(
  /"stockroom-level3"[\s\S]*?top: "\/assets\/textures\/archive\/twoway\.jpg",\s+sides: "\/assets\/textures\/classic\/Alien02_diff\.bmp"/.test(styleSource),
  "Level 3 uses screenshot-backed yellow arrow tops and purple Alien02 side faces"
);
assert((styleSource.match(/sourceUpdate: "Cam_alignWith\(map-center, player\)"/g) ?? []).length === 4, "Classic camera evidence type plus all three records preserve Cam_alignWith semantics");

const suzanne = JSON.parse(readFileSync(join(projectRoot, "src", "legacy", "suzanne1-ascii-scene.json"), "utf8"));
assert(suzanne.grid.width === 40 && suzanne.grid.height === 40, "Suzanne 1 preserves its exact 40x40 grid");
assert(suzanne.walls.length === 208 && suzanne.chains.length === 45, "Suzanne 1 preserves all walls and chains");
assert(suzanne.rings.length === 15 && suzanne.pistons.length === 3 && suzanne.effects.length === 2, "Suzanne 1 preserves rings, pistons and effects");
for (const source of Object.values(suzanne.provenance.sources)) {
  const relative = source.path.replace(/^StockRoom\//, "").replaceAll("/", "\\");
  assert(sha256(join(stockRoom, relative)) === source.sha256, `${source.path} still matches its audited SHA-256`);
}

const environmentSource = readFileSync(join(projectRoot, "src", "suzanne1-ascii-environment.ts"), "utf8");
assert(environmentSource.includes("material.color.set(0xffffff)"), "Suzanne loaded textures are no longer multiplied by fallback tints");

console.log(`Classic/Suzanne fidelity verification passed ${assertions.length}/${assertions.length} assertions.`);
