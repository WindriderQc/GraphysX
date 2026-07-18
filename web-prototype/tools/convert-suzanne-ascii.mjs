import { createHash } from "node:crypto";
import { existsSync, mkdirSync, readFileSync, statSync, writeFileSync } from "node:fs";
import { dirname, join, resolve } from "node:path";
import { fileURLToPath } from "node:url";

const here = dirname(fileURLToPath(import.meta.url));
const projectRoot = resolve(here, "..");
const outputPath = join(projectRoot, "src", "legacy", "suzanne1-ascii-scene.json");
const defaultStockRoom = "E:\\Media\\Datalake\\Tech\\StockRoom";
const stockRoom = resolve(process.env.GRAPHYSX_STOCKROOM ?? defaultStockRoom);

const sourceFiles = {
  ascii: "Suzanne1.ASCII",
  compactXml: "Suzanne1.xml",
  backupXml: "Suzanne1bkp.xml",
  reference: "Suzanne1.png",
  ringPath: "Suzanne1.obj",
  grassSample: "GrassSample.jpg",
  grass: "Grass.jpg",
  wall: "objet39.jpg",
  chain: "3D_Spheres.jpg",
  podium: join("Texture", "Podium.JPG"),
  ring: "ZRing.png",
  magician: "Zack.jpg",
  particle: "particle.dds",
  concrete: "concrete.png",
  wood: "wood.jpg"
};

for (const [label, relativePath] of Object.entries(sourceFiles)) {
  const fullPath = join(stockRoom, relativePath);
  if (!existsSync(fullPath)) {
    throw new Error(`Suzanne converter is missing ${label}: ${fullPath}`);
  }
}

function sha256(path) {
  return createHash("sha256").update(readFileSync(path)).digest("hex");
}

function sourceRecord(relativePath) {
  const fullPath = join(stockRoom, relativePath);
  return {
    path: `StockRoom/${relativePath.replaceAll("\\", "/")}`,
    sha256: sha256(fullPath),
    bytes: statSync(fullPath).size
  };
}

function vectorFromAttributes(attributes) {
  const value = (name, fallback = 0) => {
    const match = attributes.match(new RegExp(`${name}="([^"]+)"`, "i"));
    return match ? Number(match[1]) : fallback;
  };
  return [value("x"), value("y"), value("z")];
}

function childText(body, tag) {
  return body.match(new RegExp(`<${tag}[^>]*>([\\s\\S]*?)<\\/${tag}>`, "i"))?.[1]?.trim() ?? "";
}

function parseXmlObjects(xml) {
  const objects = [];
  for (const match of xml.matchAll(/<Obj3D\b([^>]*)>([\s\S]*?)<\/Obj3D>/gi)) {
    const attributes = match[1];
    const body = match[2];
    const positionAttributes = body.match(/<Pos\b([^>]*)\/>/i)?.[1] ?? "";
    const scaleAttributes = body.match(/<Scale\b([^>]*)\/>/i)?.[1] ?? "";
    const attribute = (name, fallback = "") =>
      attributes.match(new RegExp(`${name}="([^"]*)"`, "i"))?.[1] ?? fallback;
    objects.push({
      name: attribute("Name"),
      type: Number(attribute("Type", "-1")),
      geometry: Number(attribute("Geom", "-1")),
      enabled: attribute("Enabled", "true").toLowerCase() === "true",
      mass: Number(attribute("masse", "0")),
      meshControlled: attribute("MeshControlled", "false").toLowerCase() === "true",
      newtonMaterial: Number(attribute("NewtonMat", "0")),
      position: vectorFromAttributes(positionAttributes),
      scale: vectorFromAttributes(scaleAttributes),
      mesh: childText(body, "PathToMesh").replaceAll("\\\\", "/"),
      texture: childText(body, "TextureName")
    });
  }
  return objects;
}

function readPngDimensions(path) {
  const bytes = readFileSync(path);
  const pngSignature = "89504e470d0a1a0a";
  if (bytes.subarray(0, 8).toString("hex") !== pngSignature) {
    throw new Error(`${path} is not a PNG file.`);
  }
  return [bytes.readUInt32BE(16), bytes.readUInt32BE(20)];
}

const asciiPath = join(stockRoom, sourceFiles.ascii);
const rows = readFileSync(asciiPath, "utf8").replace(/^\uFEFF/, "").trimEnd().split(/\r?\n/);
const width = rows[0]?.length ?? 0;
const height = rows.length;
if (width !== 40 || height !== 40 || rows.some((row) => row.length !== width)) {
  throw new Error(`Suzanne1.ASCII must be the archived 40x40 grid; received ${width}x${height}.`);
}

const supportedSymbols = new Set(".#Zz@CcFfHhRs0369".split(""));
const counts = {};
const objects = [];
const rings = [];
const walls = [];
const chains = [];
const effects = [];
const pistons = [];
const cubxAnchors = [];
const gates = { finishStart: null, finishEnd: null, halfStart: null, halfEnd: null };
let playerStart = null;

function positionFor(x, z, y = 0.5) {
  return [x + 0.5, y, z + 0.5];
}

function pistonRotation(symbol) {
  return {
    "0": [0, 270, 90],
    "3": [0, 0, 90],
    "6": [0, 90, 90],
    "9": [0, 180, 90]
  }[symbol];
}

for (let z = 0; z < rows.length; z += 1) {
  for (let x = 0; x < rows[z].length; x += 1) {
    const symbol = rows[z][x];
    if (!supportedSymbols.has(symbol)) {
      throw new Error(`Unsupported Suzanne ASCII symbol ${JSON.stringify(symbol)} at ${x},${z}.`);
    }
    counts[symbol] = (counts[symbol] ?? 0) + 1;
    const cell = { symbol, x, z, position: positionFor(x, z) };
    if ("#Zz".includes(symbol)) {
      const texture = symbol === "#" ? "objet39.jpg" : symbol === "Z" ? "Grass.jpg" : "Podium.JPG";
      const wall = { ...cell, kind: "wall", texture, scale: [1, 1, 1], physics: "dynamic-wall" };
      walls.push(wall);
      objects.push(wall);
    } else if (symbol === "@") {
      playerStart = [x, 0.5, z];
    } else if (symbol === "C") {
      const anchor = { ...cell, kind: "cubx-anchor", actorPosition: [x + 0.5, 5.5, z + 0.5] };
      cubxAnchors.push(anchor);
      objects.push(anchor);
    } else if (symbol === "c") {
      const chain = {
        ...cell,
        kind: "chain",
        basePosition: positionFor(x, z, 0.75),
        linkPositions: [positionFor(x, z, 0.5), positionFor(x, z, 0.25)],
        baseScale: [0.25, 0.25, 0.25],
        linkScales: [[0.25 / 1.75, 0.25, 0.25], [0.25 / 1.5, 0.25, 0.25]],
        jointAngularLimitRadians: Math.PI / 3,
        linkMass: 0.5,
        texture: "3D_Spheres.jpg"
      };
      chains.push(chain);
      objects.push(chain);
    } else if ("FfHh".includes(symbol)) {
      const gate = {
        ...cell,
        kind: "lap-marker",
        gate: symbol.toLowerCase() === "f" ? "finish" : "half",
        endpoint: symbol === symbol.toUpperCase() ? "start" : "end",
        color: symbol.toLowerCase() === "f" ? "red" : "blue",
        scale: [0.2, 2, 0.2],
        physics: "wall"
      };
      if (symbol === "F") gates.finishStart = cell.position;
      if (symbol === "f") gates.finishEnd = cell.position;
      if (symbol === "H") gates.halfStart = cell.position;
      if (symbol === "h") gates.halfEnd = cell.position;
      objects.push(gate);
    } else if (symbol === "R") {
      const ring = {
        ...cell,
        kind: "ring-sphere",
        position: positionFor(x, z, 0.45),
        radius: 0.5,
        texture: "ZRing.png",
        pickupDistance: 0.6,
        rotatesDegreesPerElapsedMillisecond: 0.1
      };
      rings.push(ring);
      objects.push(ring);
    } else if (symbol === "s") {
      const effect = {
        ...cell,
        kind: "magician-particle-cell",
        texture: "Zack.jpg",
        emitters: [
          { color: [1, 0, 1, 1], particles: 64 },
          { color: [0, 0.1, 1, 1], particles: 64 }
        ],
        particleTexture: "particle.dds"
      };
      effects.push(effect);
      objects.push(effect);
    } else if ("0369".includes(symbol)) {
      const piston = {
        ...cell,
        kind: "piston",
        rotationDegrees: pistonRotation(symbol),
        barScale: [5, 0.1, 0.1],
        plateScale: [0.25, 0.95, 0.95],
        triggerScale: [0.75, 0.95, 0.75],
        pushForceLocal: [50, 0, 0],
        pullForceLocal: [-1, 0, 0],
        linearLimits: [-0.5, 0.5],
        plateMass: 0.1,
        color: "darkgray"
      };
      pistons.push(piston);
      objects.push(piston);
    }
  }
}

if (!playerStart || Object.values(gates).some((value) => value === null)) {
  throw new Error("Suzanne1.ASCII is missing its player, finish, or halfway markers.");
}

const compactXmlPath = join(stockRoom, sourceFiles.compactXml);
const backupXmlPath = join(stockRoom, sourceFiles.backupXml);
const referencePath = join(stockRoom, sourceFiles.reference);
const referenceDimensions = readPngDimensions(referencePath);

const output = {
  schema: "graphysx.suzanne-ascii-scene.v1",
  id: "suzanne1-ascii-arena",
  title: "Suzanne 1 — ASCII arena",
  status: "authored-layout-recovered",
  provenance: {
    note: "This is the 40x40 ASCII arena shown by Suzanne1.png, not the separate 2015 Suzanne1.blend.x machinery study.",
    sources: Object.fromEntries(Object.entries(sourceFiles).map(([label, path]) => [label, sourceRecord(path)])),
    referenceScreenshot: {
      dimensions: referenceDimensions,
      visualFacts: [
        "grass floor",
        "wood-toned boundary walls",
        "large yellow/red Z sphere pickups",
        "red 3D_Spheres chain/block field",
        "red and blue lap-marker cylinders",
        "BallZ shell with internal controller",
        "legacy FPS/time/score/lap/speed/alpha HUD"
      ]
    }
  },
  coordinateSystem: {
    units: "TV3D world units",
    axes: "+X follows ASCII columns, +Y is up, +Z follows ASCII rows",
    cellCenter: "[column + 0.5, 0.5, row + 0.5]",
    playerException: "The archived loader stores @ at [column, 0.5, row]."
  },
  grid: { width, height, rows, symbolCounts: counts },
  presentationProfiles: {
    reference2016: {
      evidence: "Suzanne1.png visual target; floor/wall bindings and the larger pickup/chain presentation are image-backed inference because that exact branch's binding table is absent.",
      floorTexture: "GrassSample.jpg",
      hashWallTexture: "wood.jpg",
      ringRadius: 1.5,
      chainScaleMultiplier: 3,
      background: "#000000"
    },
    source2017: {
      evidence: "Exact Scene3D/BallZLevel.h and GraphysX_1/Scene.cpp bindings.",
      floorTexture: "concrete.png",
      hashWallTexture: "objet39.jpg",
      ringRadius: 0.5,
      chainScaleMultiplier: 1,
      background: "SkyX/LostValley branch-dependent"
    }
  },
  sceneDefaults: {
    floor: { center: [20, -0.05, 20], size: [40, 0.1, 40], archivedSubdivision: [16, 16] },
    player: { position: playerStart, visualRadius: 0.3, internalRadius: 0.225, internalTexture: "FireArrow800.jpg" },
    gates,
    laps: 3,
    rings: { total: rings.length, sphereRadius: 0.5, texture: "ZRing.png", particleTexture: "particle.dds" },
    cameraBranches: [
      { source: "Scene3D/GamePlayScreen.cpp", position: [20, 10, 20], lookAt: "player", mode: "fixed map-center overlook" },
      { source: "ArduinoGUI/BallZScreen.cs", position: [0, 10, -10], lookAt: [10, 5, 10], mode: "align map center with player" }
    ],
    lights: [
      { type: "point", position: [8, 20, 8], diffuseRgb255: [20, 20, 20], ambientRgb255: [10, 10, 10], range: 40 },
      { type: "point", position: [32, 20, 32], diffuseRgb255: [20, 20, 20], ambientRgb255: [10, 10, 10], range: 40 }
    ],
    hud: {
      screenshotBranch: ["FPS", "Time", "Score", "Lap", "Speed", "Alpha", "BumpAmount"],
      laterBranch: ["Time", "Score", "Lap", "Speed"]
    },
    controls: {
      mouseDown: "enable BallZ thrust",
      mouseUp: "disable BallZ thrust",
      space: "jump",
      b: "shoot bullet",
      c: "toggle free/chase camera"
    },
    gameplay: {
      lapRule: "Cross halfway segment before the finish segment; ghost samples every 100 frames.",
      ringRule: "Pickup proximity is player scale X / 0.5 (0.6 at the authored 0.3 player radius).",
      knownSourceBug: "One Scene3D branch advances after score == 2 even though the arena authors 15 rings; do not preserve this as intended design."
    }
  },
  xmlBranches: {
    compact2016: parseXmlObjects(readFileSync(compactXmlPath, "utf8")),
    backup2016: parseXmlObjects(readFileSync(backupXmlPath, "utf8"))
  },
  objects,
  walls,
  chains,
  rings,
  effects,
  pistons,
  cubxAnchors,
  unresolved: [
    "The exact pre-2017 texture binding code that produced Suzanne1.png is absent; grass and wood are preserved as screenshot-backed profile choices.",
    "The two C cells invoke createCubX, but that actor's exact runtime animation/menu state is not encoded by the ASCII file.",
    "The XML airplane can be placed exactly, but its authored interaction in this arena varies across source branches.",
    "The screenshot branch's random human count and seed are not recoverable from the image."
  ]
};

mkdirSync(dirname(outputPath), { recursive: true });
writeFileSync(outputPath, `${JSON.stringify(output, null, 2)}\n`);
console.log(`Wrote ${outputPath}`);
console.log(`Suzanne ASCII: ${width}x${height}, ${walls.length} walls, ${chains.length} chains, ${rings.length} rings, ${pistons.length} pistons`);
