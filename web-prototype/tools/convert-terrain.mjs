// CarX terrain — CarScene.cpp:19 CLLand("CarHeightmap.bmp","grass.jpg", 2, 2,
// pos(-256,-400,-256)) => 512x512 world. Sampled to a 96x96 heightfield here.
// Usage: node tools/convert-terrain.mjs
import { writeFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { fileURLToPath } from "node:url";
import { execFileSync } from "node:child_process";

const here = dirname(fileURLToPath(import.meta.url));
// NOTE: CarHeightmap.bmp is genuinely ALL BLACK — the 2008 CarScene drove on a
// flat plain. Heightmap.bmp is the real sculpted terrain from the same folder.
const SRC = join(here, "..", "..", "Yanik C++ BCKUP", "Media", "textures n else", "Heightmaps", "Heightmap.bmp");
const OUT = join(here, "..", "src", "legacy", "terrain-carx.json");

const N = 96;
let pgmBuffer;
try {
  pgmBuffer = execFileSync("convert", [SRC, "-resize", `${N}x${N}!`, "-colorspace", "Gray", "-compress", "none", "pgm:-"]);
} catch (error) {
  // the 2005-era BMP header understates the file size; ImageMagick still
  // decodes it fully and writes the PGM before complaining
  if (error.stdout && error.stdout.length > 100) {
    pgmBuffer = error.stdout;
  } else {
    throw error;
  }
}
const pgm = pgmBuffer.toString("latin1");
const values = pgm.split(/\s+/).filter((t) => /^\d+$/.test(t)).map(Number);
// P2 header tokens: width height maxval, then N*N samples
const samples = values.slice(3);
if (samples.length !== N * N) {
  throw new Error("unexpected sample count " + samples.length);
}
const maxVal = values[2] || 255;
const heights = samples.map((v) => Math.round((v / maxVal) * 1000) / 1000);
writeFileSync(OUT, JSON.stringify({
  generated: new Date().toISOString(),
  source: "Heightmaps/CarHeightmap.bmp via CarScene.cpp CLLand(2,2,-400)",
  size: N,
  heights
}));
const nonZero = heights.filter((h) => h > 0.02).length;
console.log("terrain " + N + "x" + N + ", non-flat samples: " + nonZero + " (" + Math.round((nonZero / heights.length) * 100) + "%)");
console.log("wrote " + OUT);
