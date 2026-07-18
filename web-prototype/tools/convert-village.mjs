// Dominus Art village converter — the whole port town, with its real textures.
// Parses the text .x prop library + villager .3ds files, merges each file into
// one mesh, recenters on its base, and converts referenced textures (tga->png).
//
// Usage: node tools/convert-village.mjs
// Writes src/legacy/village-catalog.json + public/assets/textures/village/*

import { readFileSync, writeFileSync, mkdirSync, readdirSync, existsSync, copyFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { fileURLToPath } from "node:url";
import { execFileSync } from "node:child_process";

const here = dirname(fileURLToPath(import.meta.url));
const VILLAGE = join(here, "..", "..", "Yanik C++ BCKUP", "Media", "Models", "Dominus Art");
const ANIMALS = join(here, "..", "..", "Yanik C++ BCKUP", "Media", "textures n else", "Animals");
const MODELS = join(here, "..", "..", "Yanik C++ BCKUP", "Media", "Models");
const OUT = join(here, "..", "src", "legacy", "village-catalog.json");
const TEXOUT = join(here, "..", "public", "assets", "textures", "village");

function round(v) {
  return Math.round(v * 100) / 100;
}

// ---------- text .x parser (same engine as convert-legacy-x) ----------

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
      throw new Error(name + ": face index out of range");
    }
  }

  let texture = null;
  let uvs = null;
  let depth = 1;
  while (depth > 0 && state.i < tokens.length) {
    const t = tokens[state.i];
    if (t === "{") {
      depth++;
      state.i++;
    } else if (t === "}") {
      depth--;
      state.i++;
    } else if (t === "MeshTextureCoords") {
      state.i++;
      while (tokens[state.i] !== "{") state.i++;
      state.i++;
      depth++;
      state.q = [];
      const uvCount = parseNumbers(tokens, state, 1)[0];
      const rawUv = parseNumbers(tokens, state, uvCount * 2);
      uvs = new Array(uvCount * 2);
      for (let u = 0; u < uvCount; u++) {
        uvs[u * 2] = round(rawUv[u * 2]);
        uvs[u * 2 + 1] = round(1 - rawUv[u * 2 + 1]);
      }
    } else if (t === "TextureFilename") {
      state.i++;
      while (tokens[state.i] !== "{") state.i++;
      state.i++;
      depth++;
      const tok = tokens[state.i];
      if (tok && tok.startsWith('"')) {
        if (!texture) texture = tok.replace(/"/g, "").replace(/;$/, "").split(/[\\/]/).pop();
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
  meshes.push({ positions, indices, uvs, texture });
}

function parseFrame(tokens, state, parentMatrix, meshes) {
  if (tokens[state.i] !== "{") state.i++;
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
      parseMesh(tokens, state, mulMatrix(local, parentMatrix), meshes, "mesh");
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

function parseXFile(path) {
  const text = readFileSync(path, "latin1");
  if (!text.startsWith("xof") || !text.slice(0, 16).includes("txt")) {
    throw new Error("not text .x");
  }
  const tokens = tokenize(text.slice(16));
  const state = { i: 0, q: [] };
  const meshes = [];
  while (state.i < tokens.length) {
    const token = tokens[state.i];
    if (token === "template") {
      // 0302-era files declare templates whose bodies contain the word Mesh —
      // skip the whole block or the parser desyncs
      state.i++;
      while (state.i < tokens.length && tokens[state.i] !== "{") state.i++;
      let depth = 0;
      do {
        const t = tokens[state.i++];
        if (t === "{") depth++;
        else if (t === "}") depth--;
      } while (depth > 0 && state.i < tokens.length);
    } else if (token === "Frame") {
      state.i++;
      parseFrame(tokens, state, IDENTITY, meshes);
    } else if (token === "Mesh") {
      state.i++;
      parseMesh(tokens, state, IDENTITY, meshes, "mesh");
    } else {
      state.i++;
    }
  }
  return meshes;
}

// ---------- merge + recenter ----------

function mergeAndRecenter(meshes) {
  const positions = [];
  const indices = [];
  const uvs = [];
  let texture = null;
  let uvsOk = true;
  for (const mesh of meshes) {
    const offset = positions.length / 3;
    positions.push(...mesh.positions);
    for (const index of mesh.indices) indices.push(index + offset);
    if (mesh.uvs && mesh.uvs.length === (mesh.positions.length / 3) * 2) {
      uvs.push(...mesh.uvs);
    } else {
      uvsOk = false;
    }
    if (!texture && mesh.texture) texture = mesh.texture;
  }
  const min = [Infinity, Infinity, Infinity];
  const max = [-Infinity, -Infinity, -Infinity];
  for (let i = 0; i < positions.length; i += 3) {
    for (let a = 0; a < 3; a++) {
      min[a] = Math.min(min[a], positions[i + a]);
      max[a] = Math.max(max[a], positions[i + a]);
    }
  }
  const cx = (min[0] + max[0]) / 2;
  const cy = min[1];
  const cz = (min[2] + max[2]) / 2;
  for (let i = 0; i < positions.length; i += 3) {
    positions[i] = round(positions[i] - cx);
    positions[i + 1] = round(positions[i + 1] - cy);
    positions[i + 2] = round(positions[i + 2] - cz);
  }
  return {
    positions,
    indices,
    uvs: uvsOk && uvs.length === (positions.length / 3) * 2 ? uvs : null,
    texture,
    size: [round(max[0] - min[0]), round(max[1] - min[1]), round(max[2] - min[2])]
  };
}

// ---------- texture pipeline ----------

mkdirSync(TEXOUT, { recursive: true });
const villageFiles = readdirSync(VILLAGE);
const textureIndex = new Map();
for (const file of villageFiles) {
  textureIndex.set(file.toLowerCase(), file);
}

const convertedTextures = new Map();
function ensureTexture(name) {
  if (!name) return null;
  const key = name.toLowerCase();
  if (convertedTextures.has(key)) return convertedTextures.get(key);
  const actual = textureIndex.get(key);
  if (!actual) {
    convertedTextures.set(key, null);
    return null;
  }
  const base = actual.replace(/\.[^.]+$/, "").toLowerCase().replace(/[^a-z0-9_-]/g, "_");
  const isTga = /\.tga$/i.test(actual);
  const outName = base + (isTga ? ".png" : ".jpg");
  const outPath = join(TEXOUT, outName);
  try {
    if (!existsSync(outPath)) {
      if (isTga) {
        execFileSync("convert", [join(VILLAGE, actual), outPath]);
      } else {
        copyFileSync(join(VILLAGE, actual), outPath);
      }
    }
    convertedTextures.set(key, "/assets/textures/village/" + outName);
  } catch {
    convertedTextures.set(key, null);
  }
  return convertedTextures.get(key);
}

// ---------- convert the town ----------

const catalog = { generated: new Date().toISOString(), source: "Models/Dominus Art + Animals", assets: [] };
let converted = 0;
let skipped = 0;

const INCLUDE = /^(port_|tree_|bush_|grass_|camp1_)/i;
for (const file of villageFiles) {
  if (!/\.x$/i.test(file)) continue;
  if (!INCLUDE.test(file)) continue;
  const id = file.replace(/\.x$/i, "").toLowerCase().replace(/[^a-z0-9_-]/g, "_");
  try {
    const merged = mergeAndRecenter(parseXFile(join(VILLAGE, file)));
    merged.texture = ensureTexture(merged.texture);
    catalog.assets.push({ id, source: "Dominus Art/" + file, ...merged });
    converted++;
  } catch (error) {
    skipped++;
    console.log("SKIP " + file + ": " + error.message);
  }
}

// the dock fish
for (const fish of ["Fish1.x", "Fish2.x", "Fish3.x"]) {
  try {
    const merged = mergeAndRecenter(parseXFile(join(ANIMALS, fish)));
    merged.texture = null;
    catalog.assets.push({ id: fish.replace(".x", "").toLowerCase(), source: "Animals/" + fish, ...merged });
    converted++;
  } catch (error) {
    console.log("SKIP " + fish + ": " + error.message);
  }
}

// villagers from .3ds (Z-up -> Y-up)
function parse3dsPositions(path) {
  const data = readFileSync(path);
  const objects = [];
  const walk = (start, end) => {
    let i = start;
    while (i + 6 <= end) {
      const id = data.readUInt16LE(i);
      const length = data.readUInt32LE(i + 2);
      if (length < 6 || i + length > end + 1) break;
      const body = i + 6;
      if (id === 0x4d4d || id === 0x3d3d) walk(body, i + length);
      else if (id === 0x4000) {
        const zero = data.indexOf(0, body);
        walk(zero + 1, i + length);
      } else if (id === 0x4100) {
        objects.push({ positions: [], indices: [], uvs: null, texture: null });
        walk(body, i + length);
      } else if (id === 0x4110) {
        const count = data.readUInt16LE(body);
        const target = objects[objects.length - 1];
        for (let v = 0; v < count; v++) {
          const x = data.readFloatLE(body + 2 + v * 12);
          const y = data.readFloatLE(body + 2 + v * 12 + 4);
          const z = data.readFloatLE(body + 2 + v * 12 + 8);
          target.positions.push(round(x), round(z), round(-y));
        }
      } else if (id === 0x4120) {
        const count = data.readUInt16LE(body);
        const target = objects[objects.length - 1];
        for (let f = 0; f < count; f++) {
          target.indices.push(
            data.readUInt16LE(body + 2 + f * 8),
            data.readUInt16LE(body + 2 + f * 8 + 4),
            data.readUInt16LE(body + 2 + f * 8 + 2)
          );
        }
      }
      i += length;
    }
  };
  walk(0, data.length);
  return objects;
}

for (const villager of ["woman.3ds"]) {
  try {
    const merged = mergeAndRecenter(parse3dsPositions(join(MODELS, villager)));
    catalog.assets.push({ id: villager.replace(".3ds", ""), source: "Models/" + villager, ...merged });
    converted++;
  } catch (error) {
    console.log("SKIP " + villager + ": " + error.message);
  }
}

writeFileSync(OUT, JSON.stringify(catalog));
console.log("village assets: " + converted + " converted, " + skipped + " skipped");
console.log("textures: " + [...convertedTextures.values()].filter(Boolean).length + " recovered");
console.log("wrote " + OUT);
for (const asset of catalog.assets.slice(0, 8)) {
  console.log("  " + asset.id + " size " + JSON.stringify(asset.size) + " tex " + (asset.texture ?? "-"));
}
