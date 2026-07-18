import { createHash } from "node:crypto";
import { copyFile, mkdir, readFile, readdir, stat, writeFile } from "node:fs/promises";
import path from "node:path";

const projectRoot = process.cwd();
const workspaceRoot = path.resolve(projectRoot, "..");
const sourceDir = path.join(workspaceRoot, "Yanik C++ BCKUP", "Media", "PartSys", "resources");
const outputPath = path.join(projectRoot, "src", "legacy", "particle-preset-library.json");
const publicDir = path.join(projectRoot, "public", "assets", "particles", "archive");

const sha256 = (buffer) => createHash("sha256").update(buffer).digest("hex").toUpperCase();
const numeric = (value) => Number(value);
const bool = (value) => String(value).toLowerCase() === "true";
const vector = (value) => String(value).split("/").map(numeric);
const color = (value) => String(value).split(",").map(numeric);

function attributes(source) {
  const result = {};
  const expression = /([\w-]+)="([^"]*)"/g;
  for (const match of source.matchAll(expression)) result[match[1]] = match[2];
  return result;
}

function emitterFromXml(openTag, body) {
  const value = attributes(openTag);
  const keyframes = [...body.matchAll(/<keyframe\s+([^>]*?)\s*\/>/g)].map((match) => {
    const key = attributes(match[1]);
    return {
      id: numeric(key.id),
      timeSeconds: numeric(key.key),
      size: vector(key.size),
      color: color(key.color)
    };
  });
  const textureFile = path.win32.basename(value.texture.replaceAll("/", "\\"));
  return {
    id: numeric(value.id),
    type: value.type,
    maxParticles: numeric(value["max-particles"]),
    defaultSize: numeric(value.size),
    location: vector(value.location),
    direction: vector(value.direction),
    directional: bool(value.directional),
    randomDirectionFactor: vector(value.randomdirectionfactor),
    power: numeric(value.power),
    lifetimeSeconds: numeric(value.lifetime),
    shape: numeric(value.shape),
    textureBinding: value.texture,
    textureFile,
    defaultColor: color(value.color),
    generationSpeedMilliseconds: numeric(value.speed),
    gravityEnabled: bool(value.usegravity),
    gravity: vector(value.gravity),
    particleChange: numeric(value.alphachange),
    blending: numeric(value.alphablending),
    alphaTest: bool(value.alphatest),
    alphaReference: numeric(value.alpharef),
    depthWrite: bool(value.alphadepthwrite),
    boxSize: vector(value.boxsize),
    sphereRadius: numeric(value.sphereradius),
    looping: bool(value.looping),
    keyframes
  };
}

function parseReadablePreset(xml) {
  const emitters = [];
  const emitterExpression = /<emitter\s+([^>]*?)>([\s\S]*?)<\/emitter>/g;
  for (const match of xml.matchAll(emitterExpression)) emitters.push(emitterFromXml(match[1], match[2]));
  const selfClosingEmitterExpression = /<emitter\s+([^>]*?)\s*\/>/g;
  for (const match of xml.matchAll(selfClosingEmitterExpression)) emitters.push(emitterFromXml(match[1], ""));
  emitters.sort((left, right) => left.id - right.id);

  const attractors = [...xml.matchAll(/<attractor\s+([^>]*?)\s*\/>/g)].map((match) => {
    const value = attributes(match[1]);
    return {
      id: numeric(value.id),
      directional: bool(value.directional),
      location: vector(value.location),
      attenuation: vector(value.attenuation),
      fieldDirection: vector(value.fielddirection),
      repulsionConstant: numeric(value.repulsionconst),
      velocityDependency: numeric(value.velocitydepend),
      radius: numeric(value.radius)
    };
  });
  return { emitters, attractors };
}

function categoryFor(id) {
  if (id.startsWith("expl")) return "explosion";
  if (id.startsWith("fire")) return "fire";
  if (id.includes("trail")) return "trail";
  if (id.startsWith("shot") || id.startsWith("gatling")) return "weapon";
  if (id.startsWith("hit") || id.startsWith("impact")) return "impact";
  if (id.startsWith("smartbomb")) return "area-effect";
  if (id.startsWith("lightning")) return "energy";
  return "effect";
}

const names = await readdir(sourceDir);
const tvpNames = names.filter((name) => name.toLowerCase().endsWith(".tvp")).sort();
const tvpjNames = names.filter((name) => name.toLowerCase().endsWith(".tvpj")).sort();
const binaryGroups = new Map();

for (const name of tvpNames) {
  const filePath = path.join(sourceDir, name);
  const buffer = await readFile(filePath);
  const hash = sha256(buffer);
  const group = binaryGroups.get(hash) ?? { sha256: hash, bytes: buffer.byteLength, names: [] };
  group.names.push(name);
  binaryGroups.set(hash, group);
}

const textureFiles = ["Glow2.dds", "smokey.dds", "circle_soft4.dds", "star1.dds", "ring_blur.dds", "clumpy_blurry.dds"];
const textures = [];
await mkdir(publicDir, { recursive: true });
for (const name of textureFiles) {
  const sourcePath = path.join(sourceDir, name);
  try {
    const buffer = await readFile(sourcePath);
    const targetPath = path.join(publicDir, name);
    await copyFile(sourcePath, targetPath);
    textures.push({
      name,
      status: "present",
      bytes: buffer.byteLength,
      sha256: sha256(buffer),
      source: path.relative(workspaceRoot, sourcePath).replaceAll("\\", "/"),
      publicUrl: `/assets/particles/archive/${name}`
    });
  } catch (error) {
    if (error?.code !== "ENOENT") throw error;
    textures.push({ name, status: "missing", bytes: 0, sha256: null, source: null, publicUrl: null });
  }
}

const readable = [];
const usedBinaryHashes = new Set();
for (const name of tvpjNames) {
  const sourcePath = path.join(sourceDir, name);
  const xmlBuffer = await readFile(sourcePath);
  const xml = xmlBuffer.toString("utf8");
  const id = path.basename(name, path.extname(name)).toLowerCase();
  const parsed = parseReadablePreset(xml);
  const directBinaryName = tvpNames.find((candidate) => candidate.toLowerCase() === `${id}.tvp`);
  let compiled = null;
  if (directBinaryName) {
    const binaryBuffer = await readFile(path.join(sourceDir, directBinaryName));
    const hash = sha256(binaryBuffer);
    const group = binaryGroups.get(hash);
    usedBinaryHashes.add(hash);
    compiled = { sha256: hash, bytes: group.bytes, aliases: [...group.names].sort() };
  }

  const textureBindings = [...new Set(parsed.emitters.map((emitter) => emitter.textureFile))].map((textureName) => {
    const record = textures.find((texture) => texture.name.toLowerCase() === textureName.toLowerCase());
    return record ?? { name: textureName, status: "missing", bytes: 0, sha256: null, source: null, publicUrl: null };
  });
  readable.push({
    id,
    label: id.replaceAll("_", " ").replace(/\b\w/g, (letter) => letter.toUpperCase()),
    category: categoryFor(id),
    availability: compiled ? "readable-compiled" : "readable-source-only",
    runtimeEvidence: compiled?.aliases.some((alias) => alias.toLowerCase() === "explosion1.tvp")
      ? "located-graphysx-callsite"
      : "no-graphysx-callsite-located",
    source: {
      path: path.relative(workspaceRoot, sourcePath).replaceAll("\\", "/"),
      sha256: sha256(xmlBuffer),
      bytes: (await stat(sourcePath)).size
    },
    compiled,
    textureBindings,
    ...parsed
  });
}

const opaque = [...binaryGroups.values()]
  .filter((group) => !usedBinaryHashes.has(group.sha256))
  .map((group) => ({
    id: path.basename(group.names[0], path.extname(group.names[0])).toLowerCase(),
    label: path.basename(group.names[0], path.extname(group.names[0])).replaceAll("_", " ").replace(/\b\w/g, (letter) => letter.toUpperCase()),
    category: categoryFor(group.names[0].toLowerCase()),
    availability: "opaque-compiled-only",
    runtimeEvidence: "no-graphysx-callsite-located",
    compiled: { sha256: group.sha256, bytes: group.bytes, aliases: [...group.names].sort() },
    reason: "Compiled TVP survives without a readable TVPJ companion; emitter fields are intentionally not guessed."
  }))
  .sort((left, right) => left.id.localeCompare(right.id));

const output = {
  schema: "graphysx.particle-preset-library/v1",
  title: "GraphysX archived TV3D particle preset library",
  classification: "engine-fx-feature-not-scene",
  sourceDirectory: "Yanik C++ BCKUP/Media/PartSys/resources",
  enumEvidence: "Archive/GNet/Deps/Include/TV3D65/{CTVParticleSystem.h,tv_types.h}",
  counts: {
    compiledFilenames: tvpNames.length,
    uniqueCompiledBinaries: binaryGroups.size,
    readablePresets: readable.length,
    readableCompiledPresets: readable.filter((preset) => preset.availability === "readable-compiled").length,
    readableSourceOnlyPresets: readable.filter((preset) => preset.availability === "readable-source-only").length,
    opaqueCompiledPresets: opaque.length,
    emitters: readable.reduce((sum, preset) => sum + preset.emitters.length, 0),
    attractors: readable.reduce((sum, preset) => sum + preset.attractors.length, 0),
    uniqueTextureBindings: textures.length,
    missingTextureBindings: textures.filter((texture) => texture.status === "missing").length
  },
  exactAliasGroups: [...binaryGroups.values()]
    .filter((group) => group.names.length > 1)
    .map((group) => ({ sha256: group.sha256, bytes: group.bytes, aliases: [...group.names].sort() })),
  textures,
  readable,
  opaque,
  fidelityBoundary: {
    exact: "TVPJ emitter/attractor fields, compiled/readable hashes, exact aliases and surviving DDS bytes.",
    adapted: "Seeded component-random direction, fixed-step integration, batched point sprites, inspection normalization and approximate Three.js blend mapping.",
    unavailable: "TV3D private RNG/integrator/blend-state recipes and emitter/particle interpolation implementation."
  }
};

await mkdir(path.dirname(outputPath), { recursive: true });
await writeFile(outputPath, `${JSON.stringify(output, null, 2)}\n`);
console.log(`Wrote ${path.relative(projectRoot, outputPath)} with ${readable.length} readable presets, ${output.counts.emitters} emitters and ${opaque.length} opaque binaries.`);
