#!/usr/bin/env node

// Evidence-only forensic audit for the Dominus Art asset family. This does not
// convert or arrange a village: no authored inter-asset composition survived.

import assert from "node:assert/strict";
import { createHash } from "node:crypto";
import { existsSync, readFileSync, readdirSync, statSync, writeFileSync } from "node:fs";
import { basename, dirname, extname, join, relative, resolve } from "node:path";
import { fileURLToPath } from "node:url";

const here = dirname(fileURLToPath(import.meta.url));
const project = resolve(here, "..");
const workspace = resolve(project, "..");
const canonicalRoot = join(workspace, "Yanik C++ BCKUP", "Media", "Models", "Dominus Art");
const archiveMirrorRoot = "E:\\Media\\Datalake\\Codes\\Yanik C++ BCKUP\\Media\\Models\\Dominus Art";
const fullArchiveRoot = "E:\\Media";
const objectLibraryPaths = [
  join(workspace, "Archive", "bckup", "BallZ2015.bckup", "Media", "ObjectLibrary.xml"),
  "E:\\Media\\Datalake\\Codes\\BallZ2015\\Archive\\bckup\\BallZ2015.bckup\\Media\\ObjectLibrary.xml",
  "E:\\Media\\Datalake\\dump\\BallZ2015\\Archive\\bckup\\BallZ2015.bckup\\Media\\ObjectLibrary.xml"
];
const output = join(project, "src", "legacy", "dominus-family-audit.json");
const includeFullArchiveSweep = process.argv.includes("--full-archive");

const hash = (data) => createHash("sha256").update(data).digest("hex").toUpperCase();
const normalize = (path) => path.replaceAll("\\", "/");
const label = (path) => path.toLowerCase().startsWith(workspace.toLowerCase())
  ? normalize(relative(workspace, path))
  : normalize(path);
const unique = (values) => [...new Set(values)];

function category(name) {
  if (/^port_/i.test(name)) return "port";
  if (/^tree_/i.test(name)) return "tree";
  if (/^bush_/i.test(name)) return "bush";
  if (/^grass_/i.test(name)) return "grass";
  if (/^camp1_/i.test(name)) return "camp";
  if (/^(?:doman|ishad|renzok|scale_renzok)/i.test(name)) return "character";
  return "weapon";
}

function walk(root) {
  const files = [];
  let directoryCount = 0;
  const visit = (directory) => {
    let entries;
    try {
      entries = readdirSync(directory, { withFileTypes: true });
    } catch {
      return;
    }
    directoryCount += 1;
    for (const entry of entries) {
      if ([".git", "node_modules", "$RECYCLE.BIN", "System Volume Information", "dist"].includes(entry.name)) continue;
      const path = join(directory, entry.name);
      if (entry.isDirectory()) visit(path);
      else if (entry.isFile()) files.push(path);
    }
  };
  if (existsSync(root)) visit(root);
  return { root, directoryCount, files };
}

function childText(body, tag) {
  return body.match(new RegExp(`<${tag}\\b[^>]*>([\\s\\S]*?)<\\/${tag}>`, "i"))?.[1].trim() ?? "";
}

function vector(body, tag, fallback) {
  const match = body.match(new RegExp(`<${tag}\\b([^>]*?)(?:\\/\\s*>|>([\\s\\S]*?)<\\/${tag}>)`, "i"));
  if (!match) return [...fallback];
  const attributes = Object.fromEntries([...match[1].matchAll(/([xyz])\s*=\s*"([^"]*)"/gi)].map((entry) => [entry[1].toLowerCase(), entry[2]]));
  const inner = match[2] ?? "";
  return ["x", "y", "z"].map((axis, index) => {
    const value = Number(attributes[axis] ?? childText(inner, axis));
    return Number.isFinite(value) ? value : fallback[index];
  });
}

function parseObjectLibrary(path) {
  const data = readFileSync(path);
  const xml = data.toString("utf8").replace(/^\uFEFF/, "");
  const objects = [];
  for (const match of xml.matchAll(/<(Object3D|Obj3D)\b[^>]*>([\s\S]*?)<\/\1>/gi)) {
    const body = match[2];
    objects.push({
      index: objects.length,
      name: childText(body, "Name"),
      pathToMesh: childText(body, "PathToMesh"),
      position: vector(body, "Pos", [0, 0, 0]),
      rotation: vector(body, "Rot", [0, 0, 0]),
      scale: vector(body, "Scale", [1, 1, 1])
    });
  }
  return {
    path,
    bytes: data.length,
    sha256: hash(data),
    xml,
    objects
  };
}

const canonicalFiles = readdirSync(canonicalRoot).sort((left, right) => left.localeCompare(right));
const extensionCounts = {};
for (const name of canonicalFiles) {
  const extension = extname(name).toLowerCase() || "(none)";
  extensionCounts[extension] = (extensionCounts[extension] ?? 0) + 1;
}

const meshNames = canonicalFiles.filter((name) => /\.x$/i.test(name));
const meshNameByLower = new Map(meshNames.map((name) => [name.toLowerCase(), name]));
const meshes = meshNames.map((name) => {
  const path = join(canonicalRoot, name);
  const data = readFileSync(path);
  const signature = data.subarray(0, 16).toString("latin1");
  const textFormat = signature.includes("txt");
  const text = textFormat ? data.toString("latin1") : "";
  const textureReferences = textFormat
    ? unique([...text.matchAll(/TextureFilename\s*\{\s*"([^"]+)"/gi)]
        .map((match) => match[1].split(/[\\/]/).at(-1)))
    : [];
  const templateFrameTransforms = textFormat ? (text.match(/\btemplate\s+FrameTransformMatrix\b/gi) ?? []).length : 0;
  const allFrameTransforms = textFormat ? (text.match(/\bFrameTransformMatrix\b/gi) ?? []).length : 0;
  return {
    name,
    category: category(name),
    source: label(path),
    bytes: data.length,
    sha256: hash(data),
    format: textFormat ? signature.trim() : signature.trim(),
    structurallyDecoded: textFormat,
    localFrameTransformRecords: Math.max(0, allFrameTransforms - templateFrameTransforms),
    textureReferences
  };
});

const meshHashGroups = new Map();
for (const mesh of meshes) {
  const group = meshHashGroups.get(mesh.sha256) ?? [];
  group.push(mesh.name);
  meshHashGroups.set(mesh.sha256, group);
}
const meshDuplicateGroups = [...meshHashGroups.values()].filter((names) => names.length > 1);
const meshCategories = {};
for (const mesh of meshes) meshCategories[mesh.category] = (meshCategories[mesh.category] ?? 0) + 1;
const availableFileNames = new Set(canonicalFiles.map((name) => name.toLowerCase()));
const referencedTextureNames = unique(meshes.flatMap((mesh) => mesh.textureReferences.map((name) => name.toLowerCase())));
const missingReferencedTextureNames = referencedTextureNames.filter((name) => !availableFileNames.has(name));
const multiTextureMeshes = meshes.filter((mesh) => mesh.textureReferences.length > 1);

const datNames = canonicalFiles.filter((name) => /\.dat$/i.test(name));
const datEntries = datNames.map((name) => {
  const data = readFileSync(join(canonicalRoot, name));
  const pairedName = meshNameByLower.get(name.replace(/\.dat$/i, ".x").toLowerCase()) ?? null;
  return {
    name,
    bytes: data.length,
    sha256: hash(data),
    pairedMesh: pairedName,
    prefixBytes: [data[0], data[1]],
    trailingFloatCount: data.length === 86 ? 21 : null,
    printableStringRuns: data.toString("latin1").match(/[ -~]{4,}/g) ?? []
  };
});
const datHashGroups = new Map();
for (const entry of datEntries) {
  const group = datHashGroups.get(entry.sha256) ?? [];
  group.push(entry.name);
  datHashGroups.set(entry.sha256, group);
}

const mirrorFiles = existsSync(archiveMirrorRoot)
  ? readdirSync(archiveMirrorRoot).sort((left, right) => left.localeCompare(right))
  : [];
const mirrorDifferences = [];
for (const name of unique([...canonicalFiles, ...mirrorFiles])) {
  const canonicalPath = join(canonicalRoot, name);
  const mirrorPath = join(archiveMirrorRoot, name);
  if (!existsSync(canonicalPath) || !existsSync(mirrorPath)) {
    mirrorDifferences.push({ name, reason: "missing" });
    continue;
  }
  const canonicalHash = hash(readFileSync(canonicalPath));
  const mirrorHash = hash(readFileSync(mirrorPath));
  if (canonicalHash !== mirrorHash) mirrorDifferences.push({ name, reason: "hash", canonicalHash, mirrorHash });
}

const objectLibraries = objectLibraryPaths.filter(existsSync).map(parseObjectLibrary);
const objectLibrary = objectLibraries[0];
const dominusRows = objectLibrary.objects.flatMap((object) => {
  const file = object.pathToMesh.split(/[\\/]+/).filter(Boolean).at(-1)?.toLowerCase() ?? "";
  const canonicalName = meshNameByLower.get(file);
  if (!canonicalName) return [];
  return [{ ...object, canonicalMesh: canonicalName, category: category(canonicalName) }];
});
const portRows = dominusRows.filter((object) => object.category === "port");
const zeroVector = (values) => values.every((value) => value === 0);
const unitHundredth = (values) => values.every((value) => value === 0.01);
const onFiveUnitGrid = (value) => Math.abs(value / 5 - Math.round(value / 5)) < 1e-8;

const sourceExtensions = new Set([".cpp", ".h", ".hpp", ".c", ".cc", ".cs", ".xml", ".ini", ".txt", ".vcproj", ".vcxproj", ".sln"]);
const sourceCodeExtensions = new Set([".cpp", ".h", ".hpp", ".c", ".cc", ".cs"]);
const portNeedles = meshes.filter((mesh) => mesh.category === "port").map((mesh) => mesh.name.replace(/\.x$/i, "").toLowerCase());
const workspaceSweep = walk(workspace);
const archiveSweep = includeFullArchiveSweep ? walk(fullArchiveRoot) : { root: fullArchiveRoot, directoryCount: null, files: [] };
const copyCandidates = [...workspaceSweep.files, ...archiveSweep.files]
  .filter((path) => meshNameByLower.has(basename(path).toLowerCase()));
const copyRecords = copyCandidates.map((path) => {
  const data = readFileSync(path);
  const canonicalName = meshNameByLower.get(basename(path).toLowerCase());
  const canonical = meshes.find((mesh) => mesh.name === canonicalName);
  const sha256 = hash(data);
  return { path: label(path), canonicalMesh: canonicalName, bytes: data.length, sha256, byteIdenticalToCanonical: sha256 === canonical.sha256 };
});

const referenceCandidates = [...workspaceSweep.files, ...archiveSweep.files].filter((path) => {
  const normalized = normalize(path).toLowerCase();
  return sourceExtensions.has(extname(path).toLowerCase())
    && !normalized.includes("/web-prototype/")
    && !normalized.includes("/gnet/deps/")
    && !normalized.includes("/newton.bckup/");
});
const referenceFiles = [];
for (const path of referenceCandidates) {
  let text;
  try {
    if (statSync(path).size > 8_000_000) continue;
    text = readFileSync(path, "utf8").toLowerCase();
  } catch {
    continue;
  }
  const matchedNames = portNeedles.filter((name) => text.includes(name));
  if (!matchedNames.length && !text.includes("dominus art")) continue;
  referenceFiles.push({
    path: label(path),
    extension: extname(path).toLowerCase(),
    kind: basename(path).toLowerCase() === "objectlibrary.xml"
      ? "object-library-grid"
      : sourceCodeExtensions.has(extname(path).toLowerCase()) ? "source-code" : "other-document",
    matchedPortNames: matchedNames
  });
}

const objectLibraryHashes = unique(objectLibraries.map((entry) => entry.sha256));
const nonCanonicalCopyRoots = unique(copyRecords.map((record) => dirname(record.path)));
const sourceCodeHits = referenceFiles.filter((entry) => entry.kind === "source-code");
const authoredDocumentHits = referenceFiles.filter((entry) => entry.kind === "other-document");
const catalogDocumentHits = referenceFiles.filter((entry) => entry.kind === "object-library-grid");

assert.equal(meshes.length, 65);
assert.deepEqual(meshCategories, { bush: 4, camp: 4, character: 8, grass: 5, port: 28, tree: 10, weapon: 6 });
assert.equal(meshes.filter((mesh) => mesh.format.includes("txt")).length, 63);
assert.deepEqual(meshes.filter((mesh) => mesh.format.includes("bin")).map((mesh) => mesh.name).sort(), ["port_crateshed.X", "renzokscale.X"]);
assert.equal(meshHashGroups.size, 65);
assert.equal(meshDuplicateGroups.length, 0);
assert.equal(meshes.filter((mesh) => mesh.textureReferences.length > 0).length, 63);
assert.equal(multiTextureMeshes.length, 24);
assert.equal(referencedTextureNames.length, 83);
assert.equal(missingReferencedTextureNames.length, 0);
assert.equal(datEntries.length, 30);
assert.ok(datEntries.every((entry) => entry.bytes === 86 && entry.printableStringRuns.length === 0));
assert.equal(datHashGroups.size, 11);
assert.equal(datEntries.filter((entry) => entry.pairedMesh).length, 28);
assert.deepEqual(datEntries.filter((entry) => !entry.pairedMesh).map((entry) => entry.name).sort(), ["tree_dead1.dat", "tree_green1.dat"]);
assert.equal(canonicalFiles.length, 187);
assert.equal(mirrorFiles.length, 187);
assert.equal(mirrorDifferences.length, 0);
assert.equal(objectLibrary.objects.length, 61);
assert.equal(dominusRows.length, 41);
assert.equal(portRows.length, 28);
assert.ok(portRows.every((row) => zeroVector(row.rotation) && unitHundredth(row.scale) && row.position[1] === 0));
assert.ok(portRows.every((row) => onFiveUnitGrid(row.position[0]) && onFiveUnitGrid(row.position[2])));
assert.equal(objectLibraryHashes.length, 1);
assert.equal(sourceCodeHits.length, 0);
assert.equal(authoredDocumentHits.length, 0);
if (includeFullArchiveSweep) {
  assert.equal(copyRecords.length, 130);
  assert.ok(copyRecords.every((record) => record.byteIdenticalToCanonical));
  assert.equal(catalogDocumentHits.length, 3);
}

const report = {
  generated: new Date().toISOString(),
  id: "dominus-family-forensic/v1",
  verdict: {
    authoredCompositionRecoverable: false,
    classification: "source asset family only",
    censusRecommendation: "Keep Dominus Village / Port at PIPELINE. Do not present a curated port layout as recovered.",
    conciseReason: "The archive preserves 65 unique asset meshes and their local model transforms/material references, but no host loader, scene document outside ObjectLibrary, inter-asset transforms, camera, spawn, navigation, water/terrain, or interaction rules."
  },
  scope: {
    canonicalRoot: label(canonicalRoot),
    archiveMirrorRoot: normalize(archiveMirrorRoot),
    fullArchiveSweepRequested: includeFullArchiveSweep,
    fullArchiveRoot: normalize(fullArchiveRoot),
    workspaceDirectoriesScanned: workspaceSweep.directoryCount,
    workspaceFilesScanned: workspaceSweep.files.length,
    fullArchiveDirectoriesScanned: archiveSweep.directoryCount,
    fullArchiveFilesScanned: archiveSweep.files.length,
    sourceExtensionsSearched: [...sourceExtensions].sort()
  },
  inventory: {
    physicalFilesPerRoot: canonicalFiles.length,
    extensionCounts,
    meshFileCount: meshes.length,
    logicalUniqueMeshCount: meshHashGroups.size,
    meshCategories,
    textXCount: meshes.filter((mesh) => mesh.format.includes("txt")).length,
    binaryXCount: meshes.filter((mesh) => mesh.format.includes("bin")).length,
    binaryXNames: meshes.filter((mesh) => mesh.format.includes("bin")).map((mesh) => mesh.name),
    exactDuplicateGroupsWithinRoot: meshDuplicateGroups,
    textureReferenceSummary: {
      textMeshesWithTextureReferences: meshes.filter((mesh) => mesh.textureReferences.length > 0).length,
      textMeshesWithMultipleTextureReferences: multiTextureMeshes.length,
      uniqueReferencedTextureFileNames: referencedTextureNames.length,
      missingReferencedTextureFileNames: missingReferencedTextureNames,
      evidenceBoundary: "Texture references are decoded only from the 63 text X files. The two binary X files require a binary DirectX decoder."
    },
    meshes
  },
  duplicateCopies: {
    canonicalAndMirrorFileCounts: [canonicalFiles.length, mirrorFiles.length],
    canonicalAndMirrorDifferences: mirrorDifferences,
    canonicalAndMirrorByteIdenticalFileCount: canonicalFiles.length - mirrorDifferences.length,
    meshCopyRecords: copyRecords,
    meshCopyDirectoryRoots: nonCanonicalCopyRoots,
    conclusion: "Two byte-identical physical archive roots contain the same 65 logical meshes; no third mesh copy was found in the full E:/Media basename sweep."
  },
  datCompanions: {
    count: datEntries.length,
    fixedByteLength: 86,
    pairedMeshCount: datEntries.filter((entry) => entry.pairedMesh).length,
    orphanNames: datEntries.filter((entry) => !entry.pairedMesh).map((entry) => entry.name),
    uniquePayloadCount: datHashGroups.size,
    exactDuplicatePayloadGroups: [...datHashGroups.values()].filter((names) => names.length > 1),
    records: datEntries,
    evidenceBoundary: "Each file is one opaque 86-byte asset-local sidecar (two prefix bytes and 21 float-width values) with no printable path/name/camera text. Repeated default payloads and one-file-per-mesh naming do not evidence a multi-object scene."
  },
  references: {
    referenceFiles,
    sourceCodeHits,
    authoredSceneDocumentsExcludingObjectLibrary: authoredDocumentHits,
    objectLibraryPhysicalCopies: catalogDocumentHits,
    objectLibraryCopyHashes: objectLibraries.map((entry) => ({ path: label(entry.path), bytes: entry.bytes, sha256: entry.sha256 })),
    objectLibraryLogicalCopyCount: objectLibraryHashes.length
  },
  objectLibraryExclusion: {
    source: label(objectLibrary.path),
    totalRecords: objectLibrary.objects.length,
    dominusBasenameMatches: dominusRows.length,
    portRows: portRows.length,
    nonDominusRecords: objectLibrary.objects.length - dominusRows.length,
    allPortRowsHaveZeroRotation: portRows.every((row) => zeroVector(row.rotation)),
    allPortRowsUseScale001: portRows.every((row) => unitHundredth(row.scale)),
    allPortRowsSitAtY0OnFiveUnitXZGrid: portRows.every((row) => row.position[1] === 0 && onFiveUnitGrid(row.position[0]) && onFiveUnitGrid(row.position[2])),
    authoredHostFieldsPresent: {
      camera: /<(?:Camera|Viewpoint)\b/i.test(objectLibrary.xml),
      navigation: /<(?:Navigation|Waypoint|NavMesh)\b/i.test(objectLibrary.xml),
      spawn: /<(?:Spawn|StartPosition)\b/i.test(objectLibrary.xml),
      waterOrTerrain: /<(?:Water|Terrain|Landscape)\b/i.test(objectLibrary.xml)
    },
    portPlacements: portRows.map((row) => ({ name: row.name, position: row.position, rotation: row.rotation, scale: row.scale })),
    conclusion: "ObjectLibrary is a mixed 61-entry editor/catalog display. Its 28 port props are unrotated, uniformly scaled thumbnails placed on a regular five-unit grid among unrelated objects; this is not an authored village/port layout."
  },
  recoverableWithoutInvention: [
    "The 65 logical asset meshes as individual props, including each text X file's local frame transforms and embedded texture references.",
    "An explicitly labeled asset gallery/catalog, with binary X files handled by a capable decoder and all material groups preserved."
  ],
  absentEvidence: [
    "No historical source loader references the Dominus/port assets.",
    "No authored scene document outside the excluded ObjectLibrary catalog grid was found.",
    "No inter-asset placement, camera, spawn, navigation, terrain/water, collision, lighting, sky, or gameplay configuration survived.",
    "The existing reconstructed port/ring composition is therefore a modern curated prototype, not a recovered scene."
  ]
};

writeFileSync(output, JSON.stringify(report, null, 2));
console.log(`wrote ${output}`);
console.log(`${meshes.length} X meshes / ${meshHashGroups.size} unique hashes; ${meshCategories.port} port assets`);
console.log(`${datEntries.length} DAT sidecars / ${datHashGroups.size} unique payloads; ${datEntries.filter((entry) => entry.pairedMesh).length} paired`);
console.log(`${canonicalFiles.length}/${mirrorFiles.length} canonical/mirror files; ${mirrorDifferences.length} differences`);
console.log(`ObjectLibrary: ${dominusRows.length} matching assets, ${portRows.length} port grid rows; excluded from composition evidence`);
console.log(`host source hits ${sourceCodeHits.length}; authored document hits ${authoredDocumentHits.length}; composition recoverable: no`);
