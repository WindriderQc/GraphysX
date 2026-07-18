// TVM decoder — reverse-engineered from TrueVision3D 6.5 mesh files.
//
// Format: chunked binary. Header "!Dd", then chunks of [tag:4][size:uint32][payload].
//   MSTA — counts; uint32[6] is the vertex stride (observed 32)
//   MVER — vertex data: pos(3f) normal(3f) uv(2f) per vertex
//   MI16 / MI32 — triangle indices (uint16 / uint32)
//
// Usage: node tools/convert-tvm.mjs
// Writes src/legacy/tvm-catalog.json (+ world1-level.json for the World 1 set)

import { readFileSync, writeFileSync, mkdirSync } from "node:fs";
import { dirname, join } from "node:path";
import { fileURLToPath } from "node:url";

const here = dirname(fileURLToPath(import.meta.url));
const MEDIA = join(here, "..", "..", "Archive", "bckup", "BallZ2015.bckup", "Media");
const CUBX = join(here, "..", "..", "Yanik C++ BCKUP", "Media", "CubXActor");
const OUT_CATALOG = join(here, "..", "src", "legacy", "tvm-catalog.json");
const OUT_WORLD1 = join(here, "..", "src", "legacy", "world1-level.json");

const KNOWN_STRIDES = new Set([24, 32, 36, 40, 44, 48, 56, 64]);

function round(v) {
  return Math.round(v * 1000) / 1000;
}

function parseTvm(path) {
  const data = readFileSync(path);
  let stride = 32;
  const groups = [];
  let i = 0;
  while (i < data.length - 8) {
    const tag = data.toString("latin1", i, i + 4);
    if (tag === "MSTA" || tag === "MVER" || tag === "MI16" || tag === "MI32") {
      const size = data.readUInt32LE(i + 4);
      const start = i + 8;
      if (tag === "MSTA" && size >= 28) {
        const candidate = data.readUInt32LE(start + 24);
        if (KNOWN_STRIDES.has(candidate)) {
          stride = candidate;
        }
      } else if (tag === "MVER") {
        const count = Math.floor(size / stride);
        const positions = new Array(count * 3);
        const uvs = stride >= 32 ? new Array(count * 2) : null;
        for (let v = 0; v < count; v++) {
          positions[v * 3] = round(data.readFloatLE(start + v * stride));
          positions[v * 3 + 1] = round(data.readFloatLE(start + v * stride + 4));
          positions[v * 3 + 2] = round(data.readFloatLE(start + v * stride + 8));
          if (uvs) {
            uvs[v * 2] = round(data.readFloatLE(start + v * stride + 24));
            uvs[v * 2 + 1] = round(1 - data.readFloatLE(start + v * stride + 28)); // DX V -> GL V
          }
        }
        groups.push({ positions, uvs, indices: [] });
      } else if (tag === "MI16" || tag === "MI32") {
        const bytes = tag === "MI16" ? 2 : 4;
        const count = Math.floor(size / bytes);
        const indices = new Array(count);
        for (let n = 0; n < count; n++) {
          indices[n] = bytes === 2 ? data.readUInt16LE(start + n * 2) : data.readUInt32LE(start + n * 4);
        }
        if (groups.length > 0) {
          groups[groups.length - 1].indices = indices;
        }
      }
      i = start + size;
    } else {
      i += 1;
    }
  }

  // merge groups into one mesh with offset indices
  const positions = [];
  const uvs = [];
  const indices = [];
  for (const group of groups) {
    const offset = positions.length / 3;
    positions.push(...group.positions);
    if (group.uvs) {
      uvs.push(...group.uvs);
    }
    for (const index of group.indices) {
      indices.push(index + offset);
    }
  }

  const vertexCount = positions.length / 3;
  for (const index of indices) {
    if (!(index >= 0 && index < vertexCount)) {
      throw new Error(path + ": index out of range");
    }
  }
  return { positions, indices, uvs: uvs.length === vertexCount * 2 ? uvs : null };
}

function boundsOf(positions) {
  const min = [Infinity, Infinity, Infinity];
  const max = [-Infinity, -Infinity, -Infinity];
  for (let i = 0; i < positions.length; i += 3) {
    for (let axis = 0; axis < 3; axis++) {
      min[axis] = Math.min(min[axis], positions[i + axis]);
      max[axis] = Math.max(max[axis], positions[i + axis]);
    }
  }
  return { min: min.map(round), max: max.map(round) };
}

function recenter(mesh) {
  const b = boundsOf(mesh.positions);
  const center = [(b.min[0] + b.max[0]) / 2, b.min[1], (b.min[2] + b.max[2]) / 2];
  const positions = mesh.positions.map((value, index) => round(value - center[index % 3]));
  return { positions, indices: mesh.indices, uvs: mesh.uvs };
}

// ---------- curated catalog ----------

const toys = [
  { id: "cubx", file: join(CUBX, "CubXMesh.tvm"), label: "CubX Actor" },
  { id: "cubx-btn1", file: join(CUBX, "CubXBtn1.tvm"), label: "CubX Button 1" },
  { id: "cubx-btn2", file: join(CUBX, "CubXBtn2.tvm"), label: "CubX Button 2" },
  { id: "ring-tvm", file: join(MEDIA, "ring.tvm"), label: "BallZ Ring" },
  { id: "fleche", file: join(MEDIA, "fleche.TVM"), label: "Arrow (fleche)" },
  { id: "prisme", file: join(MEDIA, "Prisme.TVM"), label: "Prisme" },
  { id: "invert-sphere", file: join(MEDIA, "InvertSphere.TVM"), label: "Invert Sphere" },
  { id: "slide1", file: join(MEDIA, "Slide1.TVM"), label: "Slide 1" },
  { id: "ballz-track1", file: join(MEDIA, "BallZTrack1.tvm"), label: "BallZ Track 1" },
  { id: "corridor", file: join(MEDIA, "Corridor.TVM"), label: "Corridor" },
  { id: "finish-tvm", file: join(MEDIA, "Finish.TVM"), label: "Finish Plate" },
  { id: "90right", file: join(MEDIA, "90Right.TVM"), label: "90 Right Turn" },
  { id: "half-empty-ball", file: join(MEDIA, "HalfEmptyBall.3DS.TVM"), label: "Half Empty Ball" },
  { id: "pipe1", file: join(MEDIA, "pipe1.tvm"), label: "Pipe 1" }
];

const catalog = { generated: new Date().toISOString(), note: "Decoded from TrueVision3D TVM binaries", assets: [], alphabet: {} };

for (const toy of toys) {
  try {
    const mesh = recenter(parseTvm(toy.file));
    catalog.assets.push({ id: toy.id, label: toy.label, source: toy.file.split(/[\\/]/).slice(-1)[0], bounds: boundsOf(mesh.positions), ...mesh });
    console.log(toy.id + ": " + mesh.positions.length / 3 + " verts, " + mesh.indices.length / 3 + " tris");
  } catch (error) {
    console.log("SKIP " + toy.id + ": " + error.message);
  }
}

// full alphabet + digits, each glyph recentered to its own origin
const glyphs = "abcdefghijklmnopqrstuvwxyz".split("").map((g) => [g, join(MEDIA, "Alphabet", g + ".TVM")])
  .concat("0123456789".split("").map((g) => [g, join(MEDIA, "Alphabet", g + ".tvm")]));

let glyphCount = 0;
for (const [glyph, file] of glyphs) {
  try {
    catalog.alphabet[glyph] = recenter(parseTvm(file));
    glyphCount++;
  } catch (error) {
    console.log("SKIP glyph " + glyph + ": " + error.message);
  }
}
console.log("alphabet glyphs: " + glyphCount);

mkdirSync(dirname(OUT_CATALOG), { recursive: true });
writeFileSync(OUT_CATALOG, JSON.stringify(catalog));
console.log("wrote " + OUT_CATALOG);

// ---------- World 1 set (Level3 + core + elevator + finish + holes) ----------

const world1Parts = [
  { role: "terrain", file: "Level3.TVM" },
  { role: "core", file: "World1Core.TVM" },
  { role: "elevator", file: "World1Elevator1.TVM" },
  { role: "finish", file: "World1Finish.TVM" },
  { role: "hole-1", file: "World1LevelHole1.TVM" },
  { role: "hole-2", file: "World1LevelHole2.TVM" }
];

const world1 = { generated: new Date().toISOString(), note: "World 1 assembly, shared coordinate space", objects: [] };
for (const part of world1Parts) {
  try {
    const mesh = parseTvm(join(MEDIA, part.file));
    world1.objects.push({ role: part.role, source: part.file, bounds: boundsOf(mesh.positions), ...mesh });
    console.log("world1 " + part.role + ": " + mesh.positions.length / 3 + " verts");
  } catch (error) {
    console.log("SKIP world1 " + part.role + ": " + error.message);
  }
}
writeFileSync(OUT_WORLD1, JSON.stringify(world1));
console.log("wrote " + OUT_WORLD1);

// ---------- BallZ 2011: original ball trio + Map1 ----------

const BALL2011 = join(here, "..", "..", "Yanik C++ BCKUP", "BallZ 2011", "Release", "Media");

const ballParts = [
  { id: "ctrl", file: join(BALL2011, "Ball", "BallCtrl.tvm") },
  { id: "shell", file: join(BALL2011, "Ball", "BallShell.tvm") },
  { id: "fire", file: join(BALL2011, "Ball", "BallFire.TVM") }
];

const ball = {};
for (const part of ballParts) {
  try {
    const mesh = parseTvm(part.file);
    const b = boundsOf(mesh.positions);
    ball[part.id] = { ...mesh, radius: Math.round(((b.max[0] - b.min[0]) / 2) * 1000) / 1000 };
    console.log("ball " + part.id + ": " + mesh.positions.length / 3 + " verts, radius " + ball[part.id].radius);
  } catch (error) {
    console.log("SKIP ball " + part.id + ": " + error.message);
  }
}
const catalogWithBall = JSON.parse(readFileSync(OUT_CATALOG, "latin1"));
catalogWithBall.ball = ball;
writeFileSync(OUT_CATALOG, JSON.stringify(catalogWithBall));
console.log("catalog updated with 2011 ball trio");

const OUT_MAP1 = join(here, "..", "src", "legacy", "map1-level.json");
try {
  const map1 = parseTvm(join(BALL2011, "Map1.TVM"));
  writeFileSync(
    OUT_MAP1,
    JSON.stringify({
      generated: new Date().toISOString(),
      note: "BallZ 2011 Map1, decoded from TVM",
      objects: [{ role: "terrain", source: "BallZ 2011/Release/Media/Map1.TVM", bounds: boundsOf(map1.positions), ...map1 }]
    })
  );
  console.log("wrote " + OUT_MAP1 + " (" + map1.positions.length / 3 + " verts)");
} catch (error) {
  console.log("SKIP map1: " + error.message);
}

// ---------- Phase R2: the 2008/2011 slide world (Level0 = SlideLarge) ----------

const OUT_SLIDE = join(here, "..", "src", "legacy", "slide-level.json");
try {
  const slide = parseTvm(join(MEDIA, "SlideLarge.TVM"));
  writeFileSync(
    OUT_SLIDE,
    JSON.stringify({
      generated: new Date().toISOString(),
      note: "BallZ 2011 Level0 / SlideLarge — the giant slide",
      objects: [{ role: "terrain", source: "Media/SlideLarge.TVM", bounds: boundsOf(slide.positions), ...slide }]
    })
  );
  console.log("wrote " + OUT_SLIDE + " (" + slide.positions.length / 3 + " verts)");
} catch (error) {
  console.log("SKIP slide: " + error.message);
}

// ---------- BallZ 2011 Level1 — the 1135-unit mega-world ----------

const OUT_LEVEL1 = join(here, "..", "src", "legacy", "level1-2011-level.json");
try {
  const level1 = parseTvm(join(BALL2011, "Level1.TVM"));
  writeFileSync(
    OUT_LEVEL1,
    JSON.stringify({
      generated: new Date().toISOString(),
      note: "BallZ 2011 Level1 mega-world",
      objects: [{ role: "terrain", source: "BallZ 2011/Release/Media/Level1.TVM", bounds: boundsOf(level1.positions), ...level1 }]
    })
  );
  console.log("wrote " + OUT_LEVEL1 + " (" + level1.positions.length / 3 + " verts)");
} catch (error) {
  console.log("SKIP level1: " + error.message);
}
