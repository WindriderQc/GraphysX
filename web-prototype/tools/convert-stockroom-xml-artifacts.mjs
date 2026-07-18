import { createHash } from "node:crypto";
import { existsSync } from "node:fs";
import { copyFile, mkdir, readFile, writeFile } from "node:fs/promises";
import { dirname, resolve } from "node:path";

const root = resolve(import.meta.dirname, "../..");
const prototypeRoot = resolve(root, "web-prototype");
const stockRoom = "E:/Media/Datalake/Tech/StockRoom";
const repositoryStockRoom = "E:/Media/Datalake/Tech/Repos/GraphysX/StockRoom";

const outputPath = resolve(prototypeRoot, "src/legacy/stockroom-xml-artifacts.json");
const textureOutputPath = resolve(prototypeRoot, "public/assets/stockroom-xml/twoway.jpg");

const sourceGroups = [
  {
    id: "base-scene",
    label: "BaseScene.xml",
    canonicalPath: `${stockRoom}/BaseScene.xml`,
    copies: [`${stockRoom}/BaseScene.xml`, `${repositoryStockRoom}/BaseScene.xml`],
    classification: "serializer-default-duplication-artifact",
    assessment: "Eighteen byte-serialized PHYSICCUBE records have the same name, transform, scale, material metadata, and origin. They overlap exactly and do not evidence an assembled world."
  },
  {
    id: "test1",
    label: "test1.xml",
    canonicalPath: `${stockRoom}/test1.xml`,
    copies: [`${stockRoom}/test1.xml`, `${repositoryStockRoom}/test1.xml`],
    classification: "minimal-v1.2-schema-smoke-document",
    assessment: "One default-sized PHYSICCUBE with no texture or surrounding composition proves the v1.2 serializer shape, not an authored discovery world."
  }
];

const hostEvidenceSources = [
  {
    id: "obj3d-lineage",
    path: "E:/Media/Datalake/Tech/Repos/Obj3D.h",
    relevance: "Only located Obj3D source lineage; confirms XML attributes, vector fields, default PHYSICCUBE metadata, and the literal default texture token TwoWay."
  },
  {
    id: "primitive-host",
    path: resolve(root, "GraphysX_1/MeshPrimitive.cpp"),
    relevance: "Later native continuation; CreateBox consumes serialized scale as box dimensions and applies position/rotation directly."
  },
  {
    id: "physics-host",
    path: resolve(root, "GraphysX_1/PhysGeom.cpp"),
    relevance: "Later native continuation; a physics primitive with mass 0 remains static and receives no gravity/transform callbacks."
  },
  {
    id: "texture-host",
    path: resolve(root, "GraphysX_1/TextureCache.cpp"),
    relevance: "Later native continuation expects a filename, so the serialized extensionless TwoWay token cannot be claimed as a proven twoway.jpg runtime binding."
  }
];

function sha256(bufferOrText) {
  return createHash("sha256").update(bufferOrText).digest("hex").toUpperCase();
}

function normalizeSlashes(path) {
  return path.replaceAll("\\", "/");
}

function stableWhitespaceXml(text) {
  return text.replace(/\s+/g, " ").trim();
}

function decodeXml(text) {
  return text
    .replaceAll("&lt;", "<")
    .replaceAll("&gt;", ">")
    .replaceAll("&quot;", "\"")
    .replaceAll("&apos;", "'")
    .replaceAll("&amp;", "&");
}

function attributes(text) {
  const result = {};
  for (const match of text.matchAll(/([A-Za-z_][\w:.-]*)="([^"]*)"/g)) {
    result[match[1]] = decodeXml(match[2]);
  }
  return result;
}

function childText(body, name) {
  const match = body.match(new RegExp(`<${name}\\b[^>]*>([\\s\\S]*?)<\\/${name}>`, "i"));
  return match ? decodeXml(match[1]).trim() : null;
}

function childVector(body, name) {
  const match = body.match(new RegExp(`<${name}\\b([^>]*)\\/?\\s*>`, "i"));
  if (!match) return null;
  const value = attributes(match[1]);
  return [Number(value.x ?? 0), Number(value.y ?? 0), Number(value.z ?? 0)];
}

function parseHeader(xml) {
  const match = xml.match(/<ActionsHeader\b[^>]*>([\s\S]*?)<\/ActionsHeader>/i);
  const raw = match ? decodeXml(match[1]) : "";
  const tokens = [...raw.matchAll(/(\d+)\s*=\s*([A-Z][A-Z0-9_]*)/gi)].map((entry) => ({
    id: Number(entry[1]),
    name: entry[2].toUpperCase()
  }));
  const versionAt = tokens.findIndex((entry) => /^V\d+_\d+$/.test(entry.name));
  const actions = Object.fromEntries(tokens.slice(0, versionAt < 0 ? tokens.length : versionAt).map((entry) => [entry.id, entry.name]));
  const version = versionAt >= 0 ? tokens[versionAt].name : "UNKNOWN";
  const physicsMaterials = Object.fromEntries(tokens.slice(versionAt + 1).map((entry) => [entry.id, entry.name]));
  return {
    rawSha256: sha256(raw),
    normalized: raw.replace(/\s+/g, " ").trim(),
    version,
    actions,
    physicsMaterials
  };
}

function objectSignature(object) {
  return sha256(JSON.stringify({
    type: object.type,
    action: object.action,
    name: object.name,
    enabled: object.enabled,
    mass: object.mass,
    meshControlled: object.meshControlled,
    newtonMaterial: object.newtonMaterial,
    position: object.position,
    rotation: object.rotation,
    scale: object.scale,
    pathToMesh: object.pathToMesh,
    textureName: object.textureName
  }));
}

function parseDocument(xml, group) {
  const rootMatch = xml.match(/<([A-Za-z_][\w.-]*)\b[^>]*>/);
  const header = parseHeader(xml);
  const objects = [];
  const objectPattern = /<Obj3D\b([^>]*)>([\s\S]*?)<\/Obj3D>/gi;
  for (const match of xml.matchAll(objectPattern)) {
    const rawAttributes = attributes(match[1]);
    const body = match[2];
    const type = Number(rawAttributes.Type ?? -1);
    const massAttributeName = Object.hasOwn(rawAttributes, "Masse") ? "Masse" : Object.hasOwn(rawAttributes, "masse") ? "masse" : null;
    const position = childVector(body, "Pos");
    const rotation = childVector(body, "Rot");
    const scale = childVector(body, "Scale");
    const object = {
      index: objects.length,
      tag: "Obj3D",
      rawAttributes,
      type,
      action: header.actions[type] ?? "UNRESOLVED_TYPE",
      name: rawAttributes.Name ?? "",
      enabled: (rawAttributes.Enabled ?? "true").toLowerCase() === "true",
      mass: Number(rawAttributes[massAttributeName] ?? 0),
      massAttributeName,
      meshControlled: (rawAttributes.MeshControlled ?? "false").toLowerCase() === "true",
      newtonMaterial: Number(rawAttributes.NewtonMat ?? 0),
      position,
      rotation,
      scale,
      pathToMesh: childText(body, "PathToMesh") ?? "",
      textureName: childText(body, "TextureName") ?? "",
      absentSerializedFields: [
        ...(position ? [] : ["Pos"]),
        ...(rotation ? [] : ["Rot"]),
        ...(scale ? [] : ["Scale"])
      ]
    };
    object.signatureSha256 = objectSignature(object);
    objects.push(object);
  }

  const mapSizeText = childText(xml, "mapSize");
  const hasRingList = /<ringPosList\b/i.test(xml);
  const ringMatches = hasRingList ? [...xml.matchAll(/<Vec3D\b([^>]*)\/?\s*>/gi)] : [];
  const uniqueSignatures = [...new Set(objects.map((object) => object.signatureSha256))];
  const transformKeys = objects.map((object) => JSON.stringify({
    position: object.position,
    rotation: object.rotation,
    scale: object.scale
  }));
  const transformGroups = Object.values(transformKeys.reduce((result, key) => {
    result[key] = (result[key] ?? 0) + 1;
    return result;
  }, {}));

  return {
    id: group.id,
    label: group.label,
    root: rootMatch?.[1] ?? null,
    schemaGeneration: header.version === "V1_1" ? "Scene3D/Obj3D v1.1" : "Scene3D/Obj3D v1.2",
    classification: group.classification,
    assessment: group.assessment,
    header,
    filepathASCII: childText(xml, "filepathASCII") ?? "",
    mapSize: mapSizeText === null ? null : Number(mapSizeText),
    ringPositionCount: ringMatches.length,
    ringPositionListSerialized: hasRingList,
    objectCount: objects.length,
    uniqueObjectSignatureCount: uniqueSignatures.length,
    distinctSerializedTransformCount: new Set(transformKeys).size,
    exactOverlapGroupSizes: transformGroups.filter((count) => count > 1).sort((left, right) => right - left),
    objects,
    unresolvedRecords: [
      ...(objects.some((object) => object.rotation === null) ? [{
        kind: "source-field-absent",
        field: "Obj3D/Rot",
        count: objects.filter((object) => object.rotation === null).length,
        policy: "Inspection renderer applies identity rotation but does not claim it was serialized."
      }] : []),
      ...(group.id === "base-scene" ? [{
        kind: "runtime-texture-binding-unproven",
        field: "Obj3D/TextureName",
        serializedValue: "TwoWay",
        candidate: "twoway.jpg",
        policy: "The same-folder file is preserved as an opt-in candidate only; source mode does not silently bind it."
      }] : [])
    ],
    serializedSubsystems: {
      objects: true,
      mapSize: mapSizeText !== null,
      ringPositions: hasRingList,
      camera: false,
      lights: false,
      sky: false,
      gravity: false,
      physicsTimestep: false,
      interactions: false,
      gameplayObjective: false
    }
  };
}

async function sourceCopy(path) {
  if (!existsSync(path)) throw new Error(`Required StockRoom source is missing: ${path}`);
  const bytes = await readFile(path);
  const text = bytes.toString("utf8");
  return {
    source: normalizeSlashes(path),
    bytes: bytes.byteLength,
    sha256: sha256(bytes),
    whitespaceSemanticSha256: sha256(stableWhitespaceXml(text))
  };
}

const documents = [];
for (const group of sourceGroups) {
  const copyRecords = await Promise.all(group.copies.map(sourceCopy));
  const canonicalBytes = await readFile(group.canonicalPath);
  const document = parseDocument(canonicalBytes.toString("utf8"), group);
  document.canonicalSource = normalizeSlashes(group.canonicalPath);
  document.bytes = canonicalBytes.byteLength;
  document.sha256 = sha256(canonicalBytes);
  document.semanticSha256 = sha256(stableWhitespaceXml(canonicalBytes.toString("utf8")));
  document.copies = copyRecords;
  document.copiesByteIdentical = new Set(copyRecords.map((copy) => copy.sha256)).size === 1;
  document.copiesSemanticallyEquivalent = new Set(copyRecords.map((copy) => copy.whitespaceSemanticSha256)).size === 1;
  documents.push(document);
}

const texturePath = `${stockRoom}/twoway.jpg`;
const textureBytes = await readFile(texturePath);
const textureRecord = {
  serializedToken: "TwoWay",
  status: "same-folder-case-insensitive-stem-candidate-runtime-binding-unproven",
  source: texturePath,
  bytes: textureBytes.byteLength,
  sha256: sha256(textureBytes),
  browserPath: "/assets/stockroom-xml/twoway.jpg",
  policy: "Disabled by default. This is exact nearby source art, but the surviving v1.1 host code that would prove extension resolution was not located."
};

const hostEvidence = [];
for (const source of hostEvidenceSources) {
  if (!existsSync(source.path)) throw new Error(`Required host evidence is missing: ${source.path}`);
  const bytes = await readFile(source.path);
  hostEvidence.push({
    ...source,
    path: normalizeSlashes(source.path),
    bytes: bytes.byteLength,
    sha256: sha256(bytes)
  });
}

const data = {
  schema: "graphysx.stockroom-xml-artifacts/v1",
  converter: {
    id: "convert-stockroom-xml-artifacts",
    version: 1,
    deterministic: true,
    sourceSearchScope: [stockRoom, repositoryStockRoom, "E:/Media/Datalake/Tech/Repos/Obj3D.h", "workspace GraphysX_1 host continuation"]
  },
  classification: {
    distinctAssembledScenes: 0,
    serializerArtifacts: 2,
    conclusion: "BaseScene.xml and test1.xml are distinct schema artifacts, not distinct assembled worlds."
  },
  textureCandidate: textureRecord,
  hostEvidence,
  documents,
  fidelityBoundary: {
    exact: [
      "Raw source byte counts and SHA-256 hashes for all four located copies.",
      "Per-document action/version headers, attributes with original case, vectors, paths, texture tokens, object order, and overlap counts.",
      "Procedural cube dimensions, transforms, and static mass metadata as serialized."
    ],
    unresolved: [
      "BaseScene.xml omits Rot for all 18 objects; identity rotation is an inspection adapter, not serialized evidence.",
      "The v1.1 runtime binding from extensionless TwoWay to twoway.jpg is not proven by surviving host code.",
      "No authored camera, lighting, sky, gravity, physics timestep, interactions, or gameplay objective survives in either document."
    ],
    forbiddenInference: [
      "Do not spread BaseScene cubes apart to fabricate a level.",
      "Do not add a floor, player, objective, or dynamic physics simulation to either artifact.",
      "Do not count these serializer artifacts as restored discovery worlds."
    ]
  }
};

await mkdir(dirname(outputPath), { recursive: true });
await writeFile(outputPath, `${JSON.stringify(data, null, 2)}\n`, "utf8");
await mkdir(dirname(textureOutputPath), { recursive: true });
await copyFile(texturePath, textureOutputPath);

console.log(JSON.stringify({
  outputPath: normalizeSlashes(outputPath),
  textureOutputPath: normalizeSlashes(textureOutputPath),
  documents: documents.map((document) => ({
    id: document.id,
    version: document.header.version,
    objects: document.objectCount,
    rawCopyHashes: document.copies.map((copy) => copy.sha256),
    copiesByteIdentical: document.copiesByteIdentical,
    copiesSemanticallyEquivalent: document.copiesSemanticallyEquivalent
  }))
}, null, 2));
