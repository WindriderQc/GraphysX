// duplicate test — full converter, new filename
import { readFileSync, writeFileSync, mkdirSync } from "node:fs";
import { dirname, join } from "node:path";
import { fileURLToPath } from "node:url";

const here = dirname(fileURLToPath(import.meta.url));
const MEDIA = join(here, "..", "..", "Archive", "bckup", "BallZ2015.bckup", "Media");
const OUT = join(here, "..", "src", "legacy", "suzanne1-level.json");

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
        uvs[u * 2 + 1] = round(1 - rawUv[u * 2 + 1]); // DirectX V -> GL V
      }
    } else if (t === "Material") {
      state.i++;
      if (tokens[state.i] !== "{") state.i++;
      while (tokens[state.i] !== "{") state.i++;
      state.i++;
      depth++;
      state.q = [];
      color = parseNumbers(tokens, state, 4).slice(0, 3);
    } else if (t === "TextureFilename") {
      state.i++;
      while (tokens[state.i] !== "{") state.i++;
      state.i++;
      depth++;
      const tok = tokens[state.i];
      if (tok && tok.startsWith('"')) {
        texture = tok.replace(/"/g, "").replace(/;$/, "");
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

  meshes.push({ name, positions, indices, color, texture, uvs });
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

function parseObjPath(path) {
  const text = readFileSync(path, "latin1");
  const pts = [];
  for (const line of text.split("\n")) {
    const m = line.match(/^v\s+(-?[\d.eE+-]+)\s+(-?[\d.eE+-]+)\s+(-?[\d.eE+-]+)/);
    if (m) pts.push([Number(m[1]), Number(m[2]), Number(m[3])]);
  }
  return pts;
}

const sources = [
  { file: "Suzanne1.blend.x", role: "level", physics: "static" },
  { file: "Suzanne1.PistonStand.x", role: "piston-stand", physics: "static" },
  { file: "Suzanne1.PistonTrigger.x", role: "piston-trigger", physics: "static" },
  { file: "Suzanne1.FinishLine.x", role: "finish-line", physics: "none" },
  { file: "Suzanne1.DoorGate.x", role: "door-gate", physics: "static" },
  { file: "Suzanne1.Piston.x", role: "piston", physics: "kinematic-piston" },
  { file: "Suzanne1.Rotator.x", role: "rotator", physics: "kinematic-rotator" },
  { file: "Suzanne1.RotatorCube.x", role: "rotator-cube", physics: "kinematic-rotator" }
];

const objects = [];
for (const src of sources) {
  const meshes = parseXFile(join(MEDIA, src.file));
  const b = bounds(meshes);
  const tris = meshes.reduce((s, m) => s + m.indices.length / 3, 0);
  const vtx = meshes.reduce((s, m) => s + m.positions.length / 3, 0);
  console.log(
    src.file + ": " + meshes.length + " mesh(es), " + vtx + " verts, " + tris + " tris, bounds " +
      JSON.stringify(b.min) + " .. " + JSON.stringify(b.max)
  );
  objects.push({ source: src.file, role: src.role, physics: src.physics, bounds: b, meshes });
}

const ringPath = parseObjPath(join(MEDIA, "Suzanne1.obj")).map((p) => p.map(round));
console.log("Suzanne1.obj ring path: " + ringPath.length + " points");

mkdirSync(dirname(OUT), { recursive: true });
writeFileSync(
  OUT,
  JSON.stringify({
    generated: new Date().toISOString(),
    source: "Archive/bckup/BallZ2015.bckup/Media",
    objects,
    ringPath
  })
);
console.log("wrote " + OUT);
