import { createHash } from "node:crypto";
import { existsSync, mkdirSync, readFileSync, statSync, writeFileSync } from "node:fs";
import { dirname, join, resolve } from "node:path";
import { fileURLToPath } from "node:url";

const here = dirname(fileURLToPath(import.meta.url));
const projectRoot = resolve(here, "..");
const graphysxRoot = resolve(projectRoot, "..");
const stockRoom = resolve(process.env.GRAPHYSX_STOCKROOM ?? "E:\\Media\\Datalake\\Tech\\StockRoom");
const repoStockRoom = resolve(
  process.env.GRAPHYSX_REPO_STOCKROOM ?? "E:\\Media\\Datalake\\Tech\\Repos\\GraphysX\\StockRoom"
);
const laterRepoRoot = resolve(process.env.GRAPHYSX_LATER_REPO ?? "E:\\Media\\Datalake\\Tech\\Repos\\GraphysX\\GraphysX");
const outputPath = join(projectRoot, "src", "legacy", "suzanne2-ascii-scene.json");

const sourceFiles = {
  ascii: "Suzanne2.ASCII",
  xml: "Suzanne2.xml",
  suzanne1Ascii: "Suzanne1.ASCII",
  floor: "GrassSample.jpg",
  hashWall: "objet39.jpg",
  grassWall: "Grass.jpg",
  podiumWall: join("Texture", "Podium.JPG"),
  chain: "3D_Spheres.jpg",
  ring: "ZRing.png",
  ringNormal: "ball_Normal.png",
  effectBlock: "Zack.jpg",
  particle: "particle.dds",
  airplane: join("Airplane", "Airplane.x"),
  airplaneFuselage: join("Airplane", "FUS.BMP"),
  airplaneWheel: join("Airplane", "WHEEL.BMP"),
  airplaneWing: join("Airplane", "WING.BMP"),
  airplaneRed: join("Airplane", "RED.BMP"),
  airplaneHorizontalTail: join("Airplane", "HTAIL.BMP"),
  airplaneVerticalTail: join("Airplane", "VTAIL.BMP"),
  bonedGate: "BonedGate.x",
  playerCage: "SuperCage.x",
  playerInside: "FireArrow800.jpg",
  finishBoard: "Checkerboard.png"
};

for (const [label, relativePath] of Object.entries(sourceFiles)) {
  const path = join(stockRoom, relativePath);
  if (!existsSync(path)) throw new Error(`Suzanne 2 converter is missing ${label}: ${path}`);
}

function sha256(path) {
  return createHash("sha256").update(readFileSync(path)).digest("hex");
}

function sourceRecord(root, relativePath) {
  const path = join(root, relativePath);
  return {
    path: `${root === stockRoom ? "StockRoom" : "Repos/GraphysX/StockRoom"}/${relativePath.replaceAll("\\", "/")}`,
    sha256: sha256(path),
    bytes: statSync(path).size
  };
}

function codeSourceRecord(path, label) {
  if (!existsSync(path)) throw new Error(`Missing source evidence ${label}: ${path}`);
  return { path: label, sha256: sha256(path), bytes: statSync(path).size };
}

function round(value) {
  return Math.round(value * 100000) / 100000;
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
    const attr = (name, fallback = "") => attributes.match(new RegExp(`${name}="([^"]*)"`, "i"))?.[1] ?? fallback;
    objects.push({
      name: attr("Name"),
      type: Number(attr("Type", "-1")),
      typeName: Number(attr("Type", "-1")) === 5 ? "PHYSICCUSTOM" : Number(attr("Type", "-1")) === 1 ? "BILLBOARD" : "unknown",
      geometry: Number(attr("Geom", "-1")),
      geometryName: Number(attr("Geom", "-1")) === 6 ? "XMESH" : Number(attr("Geom", "-1")) === 5 ? "NON_PRIMITIVE" : "unknown",
      enabled: attr("Enabled", "true").toLowerCase() === "true",
      mass: Number(attr("masse", "0")),
      meshControlled: attr("MeshControlled", "false").toLowerCase() === "true",
      newtonMaterial: Number(attr("NewtonMat", "0")),
      position: vectorFromAttributes(body.match(/<Pos\b([^>]*)\/>/i)?.[1] ?? ""),
      scale: vectorFromAttributes(body.match(/<Scale\b([^>]*)\/>/i)?.[1] ?? ""),
      mesh: childText(body, "PathToMesh").replaceAll("\\\\", "/"),
      texture: childText(body, "TextureName")
    });
  }
  return objects;
}

const IDENTITY = [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1];

function multiplyMatrix(a, b) {
  const result = new Array(16).fill(0);
  for (let row = 0; row < 4; row += 1) {
    for (let column = 0; column < 4; column += 1) {
      for (let k = 0; k < 4; k += 1) result[row * 4 + column] += a[row * 4 + k] * b[k * 4 + column];
    }
  }
  return result;
}

function transformPoint(matrix, x, y, z) {
  return [
    x * matrix[0] + y * matrix[4] + z * matrix[8] + matrix[12],
    x * matrix[1] + y * matrix[5] + z * matrix[9] + matrix[13],
    x * matrix[2] + y * matrix[6] + z * matrix[10] + matrix[14]
  ].map(round);
}

function tokenizeX(text) {
  const clean = text.replace(/\/\/[^\n]*/g, "").replace(/#[^\n]*/g, "");
  return clean.match(/"[^"]*"|[{}]|[^\s{}]+/g) ?? [];
}

function parseNumbers(tokens, state, count) {
  state.queue ??= [];
  const values = [];
  while (values.length < count) {
    if (state.queue.length > 0) {
      values.push(state.queue.shift());
      continue;
    }
    if (state.index >= tokens.length) break;
    const token = tokens[state.index];
    const parsed = token.split(/[;,]+/).filter(Boolean).map(Number).filter(Number.isFinite);
    if (parsed.length === 0) {
      if (token === "{" || token === "}") break;
      state.index += 1;
      continue;
    }
    state.index += 1;
    state.queue.push(...parsed);
  }
  return values;
}

function seekOpenBrace(tokens, state) {
  while (state.index < tokens.length && tokens[state.index] !== "{") state.index += 1;
  if (tokens[state.index] !== "{") throw new Error("DirectX text block has no opening brace.");
  state.index += 1;
  state.queue = [];
}

function consumeBlockRemainder(tokens, state) {
  let depth = 1;
  while (depth > 0 && state.index < tokens.length) {
    const token = tokens[state.index++];
    if (token === "{") depth += 1;
    else if (token === "}") depth -= 1;
  }
  state.queue = [];
}

function parseMaterial(tokens, state) {
  let name = "material";
  if (tokens[state.index] !== "{") name = tokens[state.index++];
  seekOpenBrace(tokens, state);
  const color = parseNumbers(tokens, state, 4);
  const specularPower = parseNumbers(tokens, state, 1)[0] ?? 0;
  const specular = parseNumbers(tokens, state, 3);
  const emissive = parseNumbers(tokens, state, 3);
  let texture = null;
  let depth = 1;
  while (depth > 0 && state.index < tokens.length) {
    const token = tokens[state.index];
    if (token === "TextureFilename") {
      state.index += 1;
      seekOpenBrace(tokens, state);
      const value = tokens[state.index];
      if (value?.startsWith('"')) texture = value.replaceAll('"', "").replace(/;$/, "");
      consumeBlockRemainder(tokens, state);
    } else if (token === "{") {
      state.index += 1;
      depth += 1;
    } else if (token === "}") {
      state.index += 1;
      depth -= 1;
    } else {
      state.index += 1;
    }
  }
  return { name, color: color.map(round), specularPower: round(specularPower), specular: specular.map(round), emissive: emissive.map(round), texture };
}

function parseMaterialList(tokens, state) {
  seekOpenBrace(tokens, state);
  const materialCount = parseNumbers(tokens, state, 1)[0] ?? 0;
  const faceMaterialCount = parseNumbers(tokens, state, 1)[0] ?? 0;
  const faceMaterialIndices = parseNumbers(tokens, state, faceMaterialCount);
  const materials = [];
  let depth = 1;
  while (depth > 0 && state.index < tokens.length) {
    const token = tokens[state.index];
    if (token === "Material") {
      state.index += 1;
      materials.push(parseMaterial(tokens, state));
    } else if (token === "{") {
      state.index += 1;
      depth += 1;
    } else if (token === "}") {
      state.index += 1;
      depth -= 1;
    } else {
      state.index += 1;
    }
  }
  while (materials.length < materialCount) materials.push({ name: `material-${materials.length}`, color: [0.7, 0.7, 0.7, 1], specularPower: 0, specular: [0, 0, 0], emissive: [0, 0, 0], texture: null });
  return { faceMaterialIndices, materials };
}

function parseMesh(tokens, state, worldMatrix, defaultName) {
  let name = defaultName;
  if (tokens[state.index] !== "{") name = tokens[state.index++];
  seekOpenBrace(tokens, state);
  const vertexCount = parseNumbers(tokens, state, 1)[0];
  const rawPositions = parseNumbers(tokens, state, vertexCount * 3);
  const faceCount = parseNumbers(tokens, state, 1)[0];
  const faces = [];
  for (let face = 0; face < faceCount; face += 1) {
    const count = parseNumbers(tokens, state, 1)[0];
    const indices = parseNumbers(tokens, state, count);
    if (indices.some((index) => !(index >= 0 && index < vertexCount))) {
      throw new Error(`${name}: out-of-range DirectX face index.`);
    }
    faces.push(indices);
  }

  let uvs = null;
  let faceMaterialIndices = new Array(faceCount).fill(0);
  let materials = [];
  let depth = 1;
  while (depth > 0 && state.index < tokens.length) {
    const token = tokens[state.index];
    if (token === "MeshTextureCoords") {
      state.index += 1;
      seekOpenBrace(tokens, state);
      const uvCount = parseNumbers(tokens, state, 1)[0];
      uvs = parseNumbers(tokens, state, uvCount * 2).map(round);
      consumeBlockRemainder(tokens, state);
    } else if (token === "MeshMaterialList") {
      state.index += 1;
      const parsed = parseMaterialList(tokens, state);
      faceMaterialIndices = parsed.faceMaterialIndices;
      materials = parsed.materials;
    } else if (token === "{") {
      state.index += 1;
      depth += 1;
    } else if (token === "}") {
      state.index += 1;
      depth -= 1;
    } else {
      state.index += 1;
    }
  }

  const positions = [];
  for (let index = 0; index < vertexCount; index += 1) {
    positions.push(...transformPoint(worldMatrix, rawPositions[index * 3], rawPositions[index * 3 + 1], rawPositions[index * 3 + 2]));
  }
  if (uvs && uvs.length !== vertexCount * 2) throw new Error(`${name}: UV count differs from vertex count.`);
  if (faceMaterialIndices.length !== faceCount) throw new Error(`${name}: material face count differs from geometry face count.`);

  const indicesByMaterial = new Map();
  faces.forEach((face, faceIndex) => {
    const materialIndex = faceMaterialIndices[faceIndex] ?? 0;
    if (!indicesByMaterial.has(materialIndex)) indicesByMaterial.set(materialIndex, []);
    const output = indicesByMaterial.get(materialIndex);
    for (let index = 1; index < face.length - 1; index += 1) output.push(face[0], face[index + 1], face[index]);
  });
  const indices = [];
  const groups = [];
  for (const [materialIndex, materialIndices] of [...indicesByMaterial.entries()].sort((a, b) => a[0] - b[0])) {
    groups.push({ start: indices.length, count: materialIndices.length, materialIndex });
    for (const index of materialIndices) indices.push(index);
  }
  if (materials.length === 0) materials.push({ name: "default", color: [0.7, 0.7, 0.7, 1], specularPower: 0, specular: [0, 0, 0], emissive: [0, 0, 0], texture: null });
  return { name, positions, uvs, indices, groups, materials, vertexCount, faceCount, triangleCount: indices.length / 3 };
}

function parseFrame(tokens, state, parentMatrix, meshes) {
  let name = "unnamed";
  if (tokens[state.index] !== "{") name = tokens[state.index++];
  seekOpenBrace(tokens, state);
  let local = IDENTITY;
  let depth = 1;
  while (depth > 0 && state.index < tokens.length) {
    const token = tokens[state.index];
    if (token === "FrameTransformMatrix") {
      state.index += 1;
      seekOpenBrace(tokens, state);
      local = parseNumbers(tokens, state, 16);
      consumeBlockRemainder(tokens, state);
    } else if (token === "Frame") {
      state.index += 1;
      parseFrame(tokens, state, multiplyMatrix(local, parentMatrix), meshes);
    } else if (token === "Mesh") {
      state.index += 1;
      meshes.push(parseMesh(tokens, state, multiplyMatrix(local, parentMatrix), name));
    } else if (token === "{") {
      state.index += 1;
      depth += 1;
    } else if (token === "}") {
      state.index += 1;
      depth -= 1;
    } else {
      state.index += 1;
    }
  }
}

function meshBounds(meshes) {
  const min = [Infinity, Infinity, Infinity];
  const max = [-Infinity, -Infinity, -Infinity];
  for (const mesh of meshes) {
    for (let index = 0; index < mesh.positions.length; index += 3) {
      for (let axis = 0; axis < 3; axis += 1) {
        min[axis] = Math.min(min[axis], mesh.positions[index + axis]);
        max[axis] = Math.max(max[axis], mesh.positions[index + axis]);
      }
    }
  }
  return { min: min.map(round), max: max.map(round) };
}

function parseXAsset(relativePath, id) {
  const path = join(stockRoom, relativePath);
  const text = readFileSync(path, "latin1");
  if (!text.startsWith("xof") || !text.includes("txt")) throw new Error(`${relativePath} is not a text DirectX mesh.`);
  const tokens = tokenizeX(text.slice(16));
  const state = { index: 0, queue: [] };
  const meshes = [];
  while (state.index < tokens.length) {
    if (tokens[state.index] === "Frame") {
      state.index += 1;
      parseFrame(tokens, state, IDENTITY, meshes);
    } else if (tokens[state.index] === "Mesh") {
      state.index += 1;
      meshes.push(parseMesh(tokens, state, IDENTITY, id));
    } else {
      state.index += 1;
    }
  }
  const ticksPerSecond = Number(text.match(/AnimTicksPerSecond\s*\{\s*(\d+)/)?.[1] ?? 0);
  const animationTargets = [...text.matchAll(/Animation\s*\{\s*\{([^}]+)\}/g)].map((match) => match[1].trim());
  return {
    id,
    source: sourceRecord(stockRoom, relativePath),
    bounds: meshBounds(meshes),
    meshCount: meshes.length,
    vertexCount: meshes.reduce((sum, mesh) => sum + mesh.vertexCount, 0),
    faceCount: meshes.reduce((sum, mesh) => sum + mesh.faceCount, 0),
    triangleCount: meshes.reduce((sum, mesh) => sum + mesh.triangleCount, 0),
    animation: {
      present: animationTargets.length > 0,
      setNames: [...text.matchAll(/AnimationSet\s+([^\s{]+)/g)].map((match) => match[1]),
      targets: animationTargets,
      tracks: (text.match(/AnimationKey/g) ?? []).length,
      keysPerTrack: animationTargets.length > 0 ? 200 : 0,
      ticksPerSecond,
      lastAuthoredTick: animationTargets.length > 0 ? 199 : 0
    },
    meshes
  };
}

function positionFor(x, z, y = 0.5) {
  return [x + 0.5, y, z + 0.5];
}

function parseAscii(root, relativePath) {
  const rows = readFileSync(join(root, relativePath), "utf8").replace(/^\uFEFF/, "").trimEnd().split(/\r?\n/);
  const width = rows[0]?.length ?? 0;
  if (width !== 40 || rows.length !== 40 || rows.some((row) => row.length !== width)) {
    throw new Error(`${relativePath} must be an archived 40x40 grid.`);
  }
  const supported = new Set(".#Zz@CcFfHhRs0369".split(""));
  const symbolCounts = {};
  const walls = [];
  const chains = [];
  const rings = [];
  const effects = [];
  const pistons = [];
  const cubxAnchors = [];
  const lapMarkers = [];
  const gates = { finishStart: null, finishEnd: null, halfStart: null, halfEnd: null };
  let playerStart = null;
  const rotations = { "0": [0, 270, 90], "3": [0, 0, 90], "6": [0, 90, 90], "9": [0, 180, 90] };
  for (let z = 0; z < rows.length; z += 1) {
    for (let x = 0; x < rows[z].length; x += 1) {
      const symbol = rows[z][x];
      if (!supported.has(symbol)) throw new Error(`Unsupported ASCII symbol ${JSON.stringify(symbol)} at ${x},${z}.`);
      symbolCounts[symbol] = (symbolCounts[symbol] ?? 0) + 1;
      const cell = { symbol, x, z, position: positionFor(x, z) };
      if ("#Zz".includes(symbol)) {
        walls.push({
          ...cell,
          kind: "dynamic-wall-cube",
          texture: symbol === "#" ? "objet39.jpg" : symbol === "Z" ? "Grass.jpg" : "Podium.jpg",
          scale: [1, 1, 1],
          physicsMaterial: "WALL"
        });
      } else if (symbol === "@") {
        playerStart = [x, 0.5, z];
      } else if (symbol === "C") {
        cubxAnchors.push({ ...cell, kind: "cubx-actor-anchor", actorPosition: [x + 0.5, 5.5, z + 0.5] });
      } else if (symbol === "c") {
        chains.push({
          ...cell,
          kind: "three-body-chain",
          basePosition: positionFor(x, z, 0.75),
          linkPositions: [positionFor(x, z, 0.5), positionFor(x, z, 0.25)],
          baseScale: [0.25, 0.25, 0.25],
          linkScales: [[0.25 / 1.75, 0.25, 0.25], [0.25 / 1.5, 0.25, 0.25]],
          jointAngularLimitRadians: round(Math.PI / 3),
          linkMass: 0.5,
          texture: "3D_Spheres.jpg"
        });
      } else if ("FfHh".includes(symbol)) {
        const marker = {
          ...cell,
          kind: "lap-gate-post",
          gate: symbol.toLowerCase() === "f" ? "finish" : "halfway",
          endpoint: symbol === symbol.toUpperCase() ? "start" : "end",
          color: symbol.toLowerCase() === "f" ? "red" : "blue",
          scale: [0.2, 2, 0.2],
          physicsMaterial: "WALL"
        };
        lapMarkers.push(marker);
        if (symbol === "F") gates.finishStart = cell.position;
        if (symbol === "f") gates.finishEnd = cell.position;
        if (symbol === "H") gates.halfStart = cell.position;
        if (symbol === "h") gates.halfEnd = cell.position;
      } else if (symbol === "R") {
        rings.push({
          ...cell,
          kind: "ring-sphere",
          position: positionFor(x, z, 0.45),
          radius: 0.5,
          texture: "ZRing.png",
          normalTexture: "ball_Normal.png",
          pickupDistance: 0.6,
          rotatesDegreesPerElapsedMillisecond: 0.1
        });
      } else if (symbol === "s") {
        effects.push({
          ...cell,
          kind: "dynamic-effect-wall-and-particles",
          texture: "Zack.jpg",
          scale: [1, 1, 1],
          physicsMaterial: "WALL",
          particleTexture: "particle.dds",
          emitters: [
            { color: [1, 0, 1, 1], particleCapacity: 64, power: 100, paramA: 5, paramB: 2 },
            { color: [0, 0.1, 1, 1], particleCapacity: 64, power: 100, paramA: 5, paramB: 2 }
          ]
        });
      } else if ("0369".includes(symbol)) {
        pistons.push({
          ...cell,
          kind: "triggered-sliding-piston",
          rotationDegrees: rotations[symbol],
          barScale: [5, 0.1, 0.1],
          plateScale: [0.25, 0.95, 0.95],
          triggerScale: [0.75, 0.95, 0.75],
          pushForceLocal: [50, 0, 0],
          pullForceLocal: [-1, 0, 0],
          linearLimits: [-0.5, 0.5],
          plateMass: 0.1,
          triggerVisible: false
        });
      }
    }
  }
  if (!playerStart || Object.values(gates).some((value) => value === null)) throw new Error(`${relativePath} lacks player or lap gates.`);
  return { width, height: rows.length, rows, symbolCounts, playerStart, gates, walls, chains, rings, effects, pistons, cubxAnchors, lapMarkers };
}

const ascii = parseAscii(stockRoom, sourceFiles.ascii);
const suzanne1 = parseAscii(stockRoom, sourceFiles.suzanne1Ascii);
const xmlObjects = parseXmlObjects(readFileSync(join(stockRoom, sourceFiles.xml), "utf8"));

const canonicalAscii = sourceRecord(stockRoom, sourceFiles.ascii);
const repoAscii = sourceRecord(repoStockRoom, sourceFiles.ascii);
const canonicalXml = sourceRecord(stockRoom, sourceFiles.xml);
const repoXml = sourceRecord(repoStockRoom, sourceFiles.xml);
if (canonicalAscii.sha256 !== repoAscii.sha256 || canonicalXml.sha256 !== repoXml.sha256) {
  throw new Error("Suzanne 2 StockRoom and repository copies are not byte-identical.");
}

const expectedCounts = { ".": 1257, "#": 192, Z: 65, z: 56, R: 15, c: 3, C: 2, s: 2, "0": 1, "6": 1, "9": 1, "@": 1, F: 1, f: 1, H: 1, h: 1 };
for (const [symbol, count] of Object.entries(expectedCounts)) {
  if (ascii.symbolCounts[symbol] !== count) throw new Error(`Suzanne2.ASCII ${symbol} count is ${ascii.symbolCounts[symbol]}, expected ${count}.`);
}

const meshAssets = {
  airplane: parseXAsset(sourceFiles.airplane, "suzanne2-xml-airplane"),
  bonedGate: parseXAsset(sourceFiles.bonedGate, "suzanne2-xml-boned-gate"),
  playerCage: parseXAsset(sourceFiles.playerCage, "suzanne2-zombie-killer-cage")
};

const finishStart = ascii.gates.finishStart;
const finishEnd = ascii.gates.finishEnd;
const finishLength = round(Math.hypot(finishEnd[0] - finishStart[0], finishEnd[1] - finishStart[1], finishEnd[2] - finishStart[2]));

const output = {
  schema: "graphysx.suzanne2-ascii-scene.v1",
  id: "suzanne2-ascii-arena",
  title: "Suzanne 2 — isolated ASCII environment",
  status: "authored-layout-and-source-runtime-recovered",
  evidenceBoundary: {
    statement: "No Suzanne2 screenshot survives in the audited archive. The candidate therefore follows the active March 2017 GamePlayScreen + GraphysX_1 BuildASCIIScene branch and does not borrow Suzanne1.png presentation inferences.",
    screenshotFound: false,
    copiedFacts: "Only byte-identical Suzanne2.ASCII/XML copies and code-backed runtime semantics are treated as authoritative.",
    excludedFromLevelIdentity: [
      "Voie Lactee, the AirplaneLP spline flyer, ten unseeded humans, test cone, and fire are GamePlayScreen host-globals shared by levels, not Suzanne2.ASCII/XML authored content.",
      "The later July SceneEditor bindings are semantic drift, not the active loader paired with Suzanne2.",
      "RingsFile Suzanne1.obj, iRingColor AQUA, fRingSize 0.2, and iNumHuman 300 are constructor fields that the active gameplay path ignores."
    ]
  },
  provenance: {
    copies: { canonicalAscii, repoAscii, canonicalXml, repoXml },
    assets: Object.fromEntries(Object.entries(sourceFiles).filter(([key]) => !["ascii", "xml", "suzanne1Ascii", "airplane", "bonedGate", "playerCage"].includes(key)).map(([key, path]) => [key, sourceRecord(stockRoom, path)])),
    code: {
      asciiRuntime: codeSourceRecord(join(graphysxRoot, "GraphysX_1", "Scene.cpp"), "GraphysX_1/Scene.cpp"),
      gameplayHost: codeSourceRecord(join(graphysxRoot, "Scene3D", "GamePlayScreen.cpp"), "Scene3D/GamePlayScreen.cpp"),
      levelDeclaration: codeSourceRecord(join(graphysxRoot, "Scene3D", "BallZLevel.h"), "Scene3D/BallZLevel.h"),
      laterEditorDrift: codeSourceRecord(join(laterRepoRoot, "SceneEditor.cpp"), "Repos/GraphysX/GraphysX/SceneEditor.cpp")
    }
  },
  coordinateSystem: {
    units: "TV3D world units",
    axes: "+X follows ASCII columns, +Y is up, +Z follows ASCII rows",
    cellCenter: "[column + 0.5, 0.5, row + 0.5]",
    playerException: "The loader stores @ at [column, 0.5, row] without half-cell centering."
  },
  grid: { width: ascii.width, height: ascii.height, rows: ascii.rows, symbolCounts: ascii.symbolCounts },
  sceneDefaults: {
    floor: { center: [20, -0.05, 20], size: [40, 0.1, 40], texture: "GrassSample.jpg", addedBy: "BallZLevel bAddFloor=true" },
    camera: { position: [20, 10, 20], lookAt: "player", note: "The per-frame fixed map-center camera supersedes the on-entry [-10,7.5,-5] pose." },
    player: {
      implementation: "ZombieKiller",
      position: ascii.playerStart,
      physicsRadius: 0.3,
      cageMesh: "../StockRoom/SuperCage.x",
      cageScale: [0.3, 0.3, 0.3],
      insideShape: "sphere",
      insideRadius: 0.225,
      insideRotationDegrees: [-90, 0, 0],
      insideTexture: "FireArrow800.jpg"
    },
    gates: ascii.gates,
    finishBoard: {
      position: [round((finishStart[0] + finishEnd[0]) / 2), round((finishStart[1] + finishEnd[1]) / 2 + 1), round((finishStart[2] + finishEnd[2]) / 2)],
      scale: [0.25, 0.75, finishLength],
      texture: "Checkerboard.png",
      looksAt: finishEnd,
      text: { value: "Finish", font: "Arial", size: 42, color: "blue", position: [finishEnd[0], finishEnd[1] + 1 - 0.3, finishEnd[2] - 1.5], rotationDegrees: [0, 90, 0] }
    },
    ringRuntime: { authoredInventory: ascii.rings.length, victoryThresholdActuallyImplemented: 2, pickupDistance: 0.6, radius: 0.5, colorArgumentActuallyUsed: "WHITE", configuredButIgnored: { ringsFile: "../StockRoom/Suzanne1.obj", color: "AQUA", size: 0.2 } },
    lapRuntime: { rule: "The halfway bounding zone must be crossed before the finish bounding zone can increment the lap display.", targetOrVictoryCondition: null, note: "No lap target gates progression in the active gameplay function." },
    controls: { mouseDown: "enable ZombieKiller throttle and click scene", mouseUp: "disable throttle", space: "request jump", b: "shoot bullet toward 3D mouse", c: "toggle free camera" }
  },
  counts: {
    wallCubesFromHashZz: ascii.walls.length,
    wallCubesBySymbol: { hash: ascii.symbolCounts["#"], grassZ: ascii.symbolCounts.Z, podiumz: ascii.symbolCounts.z },
    effectWallCubes: ascii.effects.length,
    totalAsciiCollisionCubes: ascii.walls.length + ascii.effects.length,
    rings: ascii.rings.length,
    chainAssemblies: ascii.chains.length,
    chainRigidBodies: ascii.chains.length * 3,
    pistonAssemblies: ascii.pistons.length,
    pistonBodiesIncludingHiddenTriggers: ascii.pistons.length * 3,
    lapGatePosts: ascii.lapMarkers.length,
    gateSegments: 2,
    effectCells: ascii.effects.length,
    particleEmitters: ascii.effects.length * 2,
    particleCapacity: ascii.effects.reduce((sum, effect) => sum + effect.emitters.reduce((subtotal, emitter) => subtotal + emitter.particleCapacity, 0), 0),
    cubxActorAnchors: ascii.cubxAnchors.length,
    xmlObjects: xmlObjects.length
  },
  walls: ascii.walls,
  chains: ascii.chains,
  rings: ascii.rings,
  effects: ascii.effects,
  pistons: ascii.pistons,
  lapMarkers: ascii.lapMarkers,
  cubxAnchors: ascii.cubxAnchors,
  xmlObjects,
  meshAssets,
  comparisonToSuzanne1: {
    sameDimensions: true,
    suzanne1Counts: { walls: suzanne1.walls.length, chains: suzanne1.chains.length, rings: suzanne1.rings.length, pistons: suzanne1.pistons.length, effects: suzanne1.effects.length, cubxAnchors: suzanne1.cubxAnchors.length },
    suzanne2Counts: { walls: ascii.walls.length, chains: ascii.chains.length, rings: ascii.rings.length, pistons: ascii.pistons.length, effects: ascii.effects.length, cubxAnchors: ascii.cubxAnchors.length },
    differences: [
      "Suzanne2 has 313 #/Z/z wall cells versus Suzanne1's 208 (+105).",
      "Suzanne2 has 3 chain cells versus Suzanne1's 45 (-42).",
      "Fourteen of fifteen ring cells are identical; the z=2 ring moves from x=31 to x=33.",
      "Suzanne2 player start is [10,0.5,6]; Suzanne1 player start is [5,0.5,36].",
      "Suzanne2 finish is the vertical x=14 segment from z=3.5 to z=9.5; Suzanne1's effective finish is the horizontal z=34.5 segment from x=10.5 to x=1.5.",
      "Suzanne2 halfway is the vertical x=20.5 segment from z=25.5 to z=33.5; Suzanne1's repeated H markers make its active loader retain only the last H assignment.",
      "Suzanne2 places C anchors at both bottom corners; Suzanne1 places them at opposite corners.",
      "The three piston symbols/positions are identical."
    ]
  },
  laterEditorSemanticDrift: {
    status: "documented, excluded from default candidate",
    changes: [
      "# changes to Alien03 diffuse; Z changes to Wood03 diffuse+normal; z gains Podium normal.",
      "Chains change from scale 0.25/mass 0.5/3D_Spheres.jpg to scale 0.30/mass 0.25/COKE.jpg.",
      "C actor creation is commented out.",
      "Piston dimensions, rotations, forces, masses, and trigger sizes differ substantially."
    ]
  },
  hostGlobals: {
    includedInIsolatedCandidate: false,
    objects: ["Voie Lactee", "AirplaneLP.TVM following Spline.xml", "10 unseeded random human spheres", "test cone at [5,5,5]", "fire at [5,0.1,5]"],
    rationale: "These are instantiated by GamePlayScreen for every level and are not encoded by Suzanne2.ASCII or Suzanne2.xml."
  },
  unresolved: [
    "No Suzanne2 screenshot was found, so exact historic camera framing, sky, exposure, and composite appearance cannot be image-matched.",
    "C cells call createCubX and place actors at y=5.5, but the exact actor animation/menu state is outside the ASCII/XML evidence; the preview exposes anchors only.",
    "BonedGate.x contains a 200-tick, 24-ticks-per-second animation for Armature, Armature_Bone, and GateBlock, but the active XML path loads it as a mesh and does not establish animation playback.",
    "The source particle system preserves capacities/colors/settings but not a deterministic seed; preview particle locations are explicitly deterministic visualization, not recovered trajectories.",
    "The active source advances after two ring pickups even though fifteen rings are authored. Both facts are preserved instead of silently inventing a corrected win rule."
  ]
};

mkdirSync(dirname(outputPath), { recursive: true });
writeFileSync(outputPath, `${JSON.stringify(output, null, 2)}\n`);
console.log(`Wrote ${outputPath}`);
console.log(`Suzanne 2: ${ascii.width}x${ascii.height}, ${ascii.walls.length} #/Z/z walls + ${ascii.effects.length} effect walls, ${ascii.rings.length} rings, ${ascii.chains.length} chains, ${ascii.pistons.length} pistons`);
for (const asset of Object.values(meshAssets)) console.log(`${asset.id}: ${asset.meshCount} mesh(es), ${asset.vertexCount} vertices, ${asset.triangleCount} triangles`);
