// Converts additional legacy DirectX .x meshes and the airplane Spline.xml
// into src/legacy/asset-catalog.json for the web prototype.
//
// Usage: node tools/convert-assets.mjs
//
// TVM meshes (CubX actor, alphabet, BallZTrack1) are TV3D-proprietary binary
// and are NOT handled here — they need a TVM decoder or a Blender re-export.

import { readFileSync, writeFileSync, mkdirSync } from "node:fs";
import { dirname, join } from "node:path";
import { fileURLToPath } from "node:url";

const here = dirname(fileURLToPath(import.meta.url));
const MEDIA = join(here, "..", "..", "Archive", "bckup", "BallZ2015.bckup", "Media");
const OUT = join(here, "..", "src", "legacy", "asset-catalog.json");

const IDENTITY = [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1];

function mulMatrix(a, b) {
  const r = new Array(16).fill(0);
  for (let i = 0; i < 4; i++)
    for (let j = 0; j < 4; j++) {
      let s = 0;
      for (let k = 0; k < 4; k++) s += a[i * 4 + k] * b[k * 4 + j];
      r[i * 4 + j] = s;
    }
  return r;
}

function transformPoint(m, x, y, z) {
  return [
    x * m[0] + y * m[4] + z * m[8] + m[12],
    x * m[1] + y * m[5] + z * m[9] + m[13],
    x * m[2] + y * m[6] + z * m[10] + m[14]
  ];
}

function tokenize(text) {
  const clean = text.replace(/\/\/[^\n]*/g, "").replace(/#[^\n]*/g, "");
  return clean.match(/"[^"]*"|[{}]|[^\s{}]+/g) ?? [];
}

function parseNumbers(tokens, state, count) {
  state.q ??= [];
  const nums = [];
  while (nums.length < count) {
    if (state.q.length > 0) {
      nums.push(state.q.shift());
      continue;
    }
    if (state.i >= tokens.length) break;
    const t = tokens[state.i];
    const parts = t.split(/[;,]+/).filter((p) => p.length > 0);
    const vals = parts.map(Number).filter((v) => !Number.isNaN(v));
    if (vals.length === 0) {
      if (t === "{" || t === "}") break;
      state.i++;
      continue;
    }
    state.i++;
    state.q.push(...vals);
  }
  return nums;
}

function parseMesh(tokens, state, worldMatrix, meshes, name) {
  while (tokens[state.i] !== "{") state.i++;
  state.i++;
  state.q = [];

  const vertexCount = parseNumbers(tokens, state, 1)[0];
  const verts = parseNumbers(tokens, state, vertexCount * 3);
  const faceCount = parseNumbers(tokens, state, 1)[0];

  const indices = [];
  for (let f = 0; f < faceCount; f++) {
    const n = parseNumbers(tokens, state, 1)[0];
    const idx = parseNumbers(tokens, state, n);
    for (let k = 1; k < n - 1; k++) indices.push(idx[0], idx[k + 1], idx[k]);
  }

  for (const idx of indices) {
    if (!(idx >= 0 && idx < vertexCount)) {
      throw new Error(name + ": face index " + idx + " out of range (" + vertexCount + " verts)");
    }
  }

  let color = null;
  let texture = null;
  let depth = 1;
  while (depth > 0 && state.i < tokens.length) {
    const t = tokens[state.i];
    if (t === "{") {
      depth++;
      state.i++;
    } else if (t === "}") {
      depth--;
      state.i++;
    } else if (t === "Material") {
      state.i++;
      if (tokens[state.i] !== "{") state.i++;
      while (tokens[state.i] !== "{") state.i++;
      state.i++;
      depth++;
      state.q = [];
      const rgba = parseNumbers(tokens, state, 4);
      if (!color) color = rgba.slice(0, 3);
    } else if (t === "TextureFilename") {
      state.i++;
      while (tokens[state.i] !== "{") state.i++;
      state.i++;
      depth++;
      const tok = tokens[state.i];
      if (tok && tok.startsWith('"')) {
        if (!texture) texture = tok.replace(/"/g, "").replace(/;$/, "");
        state.i++;
      }
    } else {
      state.i++;
    }
  }

  const positions = new Array(vertexCount * 3);
  for (let v = 0; v < vertexCount; v++) {
    const p = transformPoint(worldMatrix, verts[v * 3], verts[v * 3 + 1], verts[v * 3 + 2]);
    positions[v * 3] = round(p[0]);
    positions[v * 3 + 1] = round(p[1]);
    positions[v * 3 + 2] = round(p[2]);
  }

  meshes.push({ name, positions, indices, color, texture });
}

function parseFrame(tokens, state, parentMatrix, meshes) {
  let name = "unnamed";
  if (tokens[state.i] !== "{") name = tokens[state.i++];
  state.i++;

  let local = IDENTITY;
  let depth = 1;
  while (depth > 0 && state.i < tokens.length) {
    const t = tokens[state.i];
    if (t === "FrameTransformMatrix") {
      state.i++;
      while (tokens[state.i] !== "{") state.i++;
      state.i++;
      state.q = [];
      local = parseNumbers(tokens, state, 16);
      while (tokens[state.i] !== "}") state.i++;
      state.i++;
    } else if (t === "Frame") {
      state.i++;
      parseFrame(tokens, state, mulMatrix(local, parentMatrix), meshes);
    } else if (t === "Mesh") {
      state.i++;
      let meshName = name;
      if (tokens[state.i] !== "{") meshName = tokens[state.i];
      parseMesh(tokens, state, mulMatrix(local, parentMatrix), meshes, meshName);
    } else if (t === "{") {
      depth++;
      state.i++;
    } else if (t === "}") {
      depth--;
      state.i++;
    } else {
      state.i++;
    }
  }
}

function round(v) {
  return Math.round(v * 10000) / 10000;
}

function parseXFile(path) {
  const text = readFileSync(path, "latin1");
  if (!text.startsWith("xof") || !text.includes("txt")) {
    throw new Error(path + ": not a text-format .x file");
  }
  const tokens = tokenize(text.slice(16));
  const state = { i: 0, q: [] };
  const meshes = [];
  while (state.i < tokens.length) {
    if (tokens[state.i] === "Frame") {
      state.i++;
      parseFrame(tokens, state, IDENTITY, meshes);
    } else {
      state.i++;
    }
  }
  return meshes;
}

function bounds(meshes) {
  const min = [Infinity, Infinity, Infinity];
  const max = [-Infinity, -Infinity, -Infinity];
  for (const m of meshes) {
    for (let i = 0; i < m.positions.length; i += 3) {
      for (let a = 0; a < 3; a++) {
        min[a] = Math.min(min[a], m.positions[i + a]);
        max[a] = Math.max(max[a], m.positions[i + a]);
      }
    }
  }
  return { min: min.map(round), max: max.map(round) };
}

function parseSplineXml(path) {
  const text = readFileSync(path, "latin1");
  const points = [];
  const regex = /<point\s+x="(-?[\d.eE+-]+)"\s+y="(-?[\d.eE+-]+)"\s+z="(-?[\d.eE+-]+)"/g;
  let match;
  while ((match = regex.exec(text)) !== null) {
    points.push([round(Number(match[1])), round(Number(match[2])), round(Number(match[3]))]);
  }
  return points;
}

const sources = [
  { file: "Gate1.x", id: "gate1", label: "Gate 1 (generic race gate)" },
  { file: "Airplane/Airplane.x", id: "airplane", label: "Airplane (Suzanne1 spline flyer)" },
  { file: "testSpline.x", id: "test-spline", label: "Spline test object" }
];

const assets = [];
for (const src of sources) {
  try {
    const meshes = parseXFile(join(MEDIA, src.file));
    const b = bounds(meshes);
    const tris = meshes.reduce((s, m) => s + m.indices.length / 3, 0);
    console.log(src.file + ": " + meshes.length + " mesh(es), " + tris + " tris, bounds " + JSON.stringify(b.min) + " .. " + JSON.stringify(b.max));
    assets.push({ id: src.id, label: src.label, source: src.file, bounds: b, meshes });
  } catch (error) {
    console.log("SKIP " + src.file + ": " + error.message);
  }
}

const airplaneSpline = parseSplineXml(join(MEDIA, "Spline.xml"));
console.log("Spline.xml: " + airplaneSpline.length + " points");

mkdirSync(dirname(OUT), { recursive: true });
writeFileSync(
  OUT,
  JSON.stringify({
    generated: new Date().toISOString(),
    source: "Archive/bckup/BallZ2015.bckup/Media",
    note: "TVM assets (CubX, Alphabet, BallZTrack1) still need a TVM decoder or Blender re-export.",
    assets,
    airplaneSpline
  })
);
console.log("wrote " + OUT);
