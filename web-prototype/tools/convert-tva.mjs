// .tva decoder — TrueVision3D actor ANIMATIONS, reverse-engineered 2026-07-17.
//
// Chunked like TVM but with `"Dd` header. Relevant chunks:
//   MHI2 — node hierarchy (names in order)
//   MANI — one TRS track: header uint32[6] = [?, ?, nPos, nRot, nScale, ?]
//          then nPos × (t,x,y,z) float4, nRot × (t,qx,qy,qz,qw) float5,
//          nScale × (t,sx,sy,sz) float4
//
// Usage: node tools/convert-tva.mjs
// Writes src/legacy/cubx-anim.json — the REAL 2008 CubX choreography.

import { readFileSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { fileURLToPath } from "node:url";

const here = dirname(fileURLToPath(import.meta.url));
const CUBX = join(here, "..", "..", "Yanik C++ BCKUP", "Media", "CubXActor");
const OUT = join(here, "..", "src", "legacy", "cubx-anim.json");

function round(v) {
  return Math.round(v * 1000) / 1000;
}

function parseTva(path) {
  const data = readFileSync(path);
  const nodeNames = [];
  const tracks = [];
  let i = 16;
  while (i < data.length - 8) {
    const tag = data.toString("latin1", i, i + 4);
    const printable = /^[ -~]{4}$/.test(tag);
    if (!printable) {
      i += 1;
      continue;
    }
    const size = data.readUInt32LE(i + 4);
    if (size <= 0 || i + 8 + size > data.length + 8) {
      i += 1;
      continue;
    }
    const start = i + 8;

    if (tag === "MHI2") {
      // node names appear in hierarchy order as ASCII runs
      const chunk = data.toString("latin1", start, start + size);
      const matches = chunk.match(/[ -~]{3,}/g) ?? [];
      for (const name of matches) {
        if (/^[A-Za-z_][\w:]*$/.test(name)) {
          nodeNames.push(name);
        }
      }
    } else if (tag === "MANI") {
      const nPos = data.readUInt32LE(start + 8);
      const nRot = data.readUInt32LE(start + 12);
      const nScale = data.readUInt32LE(start + 16);
      const expected = 24 + nPos * 16 + nRot * 20 + nScale * 16;
      if (expected === size) {
        let offset = start + 24;
        const pos = [];
        for (let k = 0; k < nPos; k++) {
          pos.push([
            round(data.readFloatLE(offset)),
            round(data.readFloatLE(offset + 4)),
            round(data.readFloatLE(offset + 8)),
            round(data.readFloatLE(offset + 12))
          ]);
          offset += 16;
        }
        const rot = [];
        for (let k = 0; k < nRot; k++) {
          rot.push([
            round(data.readFloatLE(offset)),
            round(data.readFloatLE(offset + 4)),
            round(data.readFloatLE(offset + 8)),
            round(data.readFloatLE(offset + 12)),
            round(data.readFloatLE(offset + 16))
          ]);
          offset += 20;
        }
        const scale = [];
        for (let k = 0; k < nScale; k++) {
          scale.push([
            round(data.readFloatLE(offset)),
            round(data.readFloatLE(offset + 4)),
            round(data.readFloatLE(offset + 8)),
            round(data.readFloatLE(offset + 12))
          ]);
          offset += 16;
        }
        tracks.push({ header: [data.readUInt32LE(start), data.readUInt32LE(start + 4), data.readUInt32LE(start + 20)], pos, rot, scale });
      } else {
        tracks.push({ header: [size], pos: [], rot: [], scale: [], unparsed: true });
      }
    }
    i = start + size;
  }
  return { nodeNames, tracks };
}

const animations = {};
const sources = [
  ["open", "CubXOpen.tva"],
  ["rot1", "CubXRot1.tva"],
  ["get2", "CubXGet2.tva"],
  ["screensaver", "CubXScreensaver.tva"],
  ["openSolo", "CubeOpensolo.tva"]
];

for (const [key, file] of sources) {
  try {
    const { nodeNames, tracks } = parseTva(join(CUBX, file));
    const named = tracks.map((track, index) => ({
      node: nodeNames[index] ?? "track" + index,
      ...track
    }));
    animations[key] = { source: file, nodeNames, tracks: named };
    const parsed = tracks.filter((t) => !t.unparsed).length;
    console.log(key + ": " + nodeNames.length + " nodes, " + tracks.length + " tracks (" + parsed + " parsed)");
    const sample = named.find((t) => t.pos.length > 1);
    if (sample) {
      console.log("  sample " + sample.node + " pos[0]=" + JSON.stringify(sample.pos[0]) + " pos[last]=" + JSON.stringify(sample.pos[sample.pos.length - 1]));
    }
  } catch (error) {
    console.log("SKIP " + file + ": " + error.message);
  }
}

writeFileSync(OUT, JSON.stringify({ generated: new Date().toISOString(), note: "TVA TRS tracks — the real 2008 CubX choreography", animations }));
console.log("wrote " + OUT);
