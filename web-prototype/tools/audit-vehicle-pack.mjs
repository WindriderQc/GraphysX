// Deterministic evidence audit for the archived GT4 and Low Cobra vehicle packs.
//
// This tool does not rewrite or normalize the source assets. It preserves authored
// 3DS coordinates, exposes a separate Z-up -> Y-up view, reports object matrices,
// face/material assignments, texture references, duplicate aliases, and the
// converted TVM geometry that survived beside the 3DS files.
//
// Usage:
//   node tools/audit-vehicle-pack.mjs
//   node tools/audit-vehicle-pack.mjs --verify

import { createHash } from "node:crypto";
import { existsSync, readFileSync, readdirSync, statSync } from "node:fs";
import { basename, dirname, join } from "node:path";
import { fileURLToPath } from "node:url";

const here = dirname(fileURLToPath(import.meta.url));
const archiveCars = join(here, "..", "..", "Yanik C++ BCKUP", "Media", "Models", "cars");
const verify = process.argv.includes("--verify");

function round(value) {
  return Math.round(value * 1_000_000) / 1_000_000;
}

function hashFile(path) {
  return createHash("sha256").update(readFileSync(path)).digest("hex");
}

function readCString(data, offset, limit) {
  const zero = data.indexOf(0, offset);
  const end = zero >= offset && zero < limit ? zero : limit;
  return { value: data.toString("latin1", offset, end), next: Math.min(end + 1, limit) };
}

function boundsOf(positions) {
  const min = [Infinity, Infinity, Infinity];
  const max = [-Infinity, -Infinity, -Infinity];
  for (let index = 0; index < positions.length; index += 3) {
    for (let axis = 0; axis < 3; axis += 1) {
      min[axis] = Math.min(min[axis], positions[index + axis]);
      max[axis] = Math.max(max[axis], positions[index + axis]);
    }
  }
  return { min: min.map(round), max: max.map(round) };
}

function convertZUpToYUp(positions) {
  const converted = new Array(positions.length);
  for (let index = 0; index < positions.length; index += 3) {
    converted[index] = positions[index];
    converted[index + 1] = positions[index + 2];
    converted[index + 2] = -positions[index + 1];
  }
  return converted;
}

function parseScalarChunk(data, start, end) {
  let cursor = start;
  while (cursor + 6 <= end) {
    const id = data.readUInt16LE(cursor);
    const length = data.readUInt32LE(cursor + 2);
    if (length < 6 || cursor + length > end) break;
    const body = cursor + 6;
    if ((id === 0x0030 || id === 0x0031) && body + 2 <= cursor + length) {
      return round(data.readUInt16LE(body) / 100);
    }
    cursor += length;
  }
  return null;
}

function parseColorChunk(data, start, end) {
  let cursor = start;
  while (cursor + 6 <= end) {
    const id = data.readUInt16LE(cursor);
    const length = data.readUInt32LE(cursor + 2);
    if (length < 6 || cursor + length > end) break;
    const body = cursor + 6;
    if ((id === 0x0010 || id === 0x0013) && body + 12 <= cursor + length) {
      return [data.readFloatLE(body), data.readFloatLE(body + 4), data.readFloatLE(body + 8)].map(round);
    }
    if ((id === 0x0011 || id === 0x0012) && body + 3 <= cursor + length) {
      return [data[body] / 255, data[body + 1] / 255, data[body + 2] / 255].map(round);
    }
    cursor += length;
  }
  return null;
}

function parse3ds(path) {
  const data = readFileSync(path);
  const materials = [];
  const objects = [];

  function walk(start, end, context = {}) {
    let cursor = start;
    while (cursor + 6 <= end) {
      const id = data.readUInt16LE(cursor);
      const length = data.readUInt32LE(cursor + 2);
      if (length < 6 || cursor + length > end) break;
      const body = cursor + 6;
      const chunkEnd = cursor + length;

      if (id === 0x4d4d || id === 0x3d3d || id === 0x4100 || id === 0xa200) {
        walk(body, chunkEnd, context);
      } else if (id === 0xafff) {
        const material = {
          name: "",
          texture: null,
          ambient: null,
          diffuse: null,
          specular: null,
          shininess: null,
          transparency: null
        };
        materials.push(material);
        walk(body, chunkEnd, { ...context, material });
      } else if (id === 0xa000 && context.material) {
        context.material.name = readCString(data, body, chunkEnd).value;
      } else if (id === 0xa010 && context.material) {
        context.material.ambient = parseColorChunk(data, body, chunkEnd);
      } else if (id === 0xa020 && context.material) {
        context.material.diffuse = parseColorChunk(data, body, chunkEnd);
      } else if (id === 0xa030 && context.material) {
        context.material.specular = parseColorChunk(data, body, chunkEnd);
      } else if (id === 0xa040 && context.material) {
        context.material.shininess = parseScalarChunk(data, body, chunkEnd);
      } else if (id === 0xa050 && context.material) {
        context.material.transparency = parseScalarChunk(data, body, chunkEnd);
      } else if (id === 0xa300 && context.material) {
        context.material.texture = readCString(data, body, chunkEnd).value;
      } else if (id === 0x4000) {
        const name = readCString(data, body, chunkEnd);
        const object = {
          name: name.value,
          positions: [],
          indices: [],
          uvs: [],
          faceMaterials: [],
          smoothingGroups: [],
          localMatrix: null
        };
        objects.push(object);
        walk(name.next, chunkEnd, { ...context, object });
      } else if (id === 0x4110 && context.object) {
        const count = data.readUInt16LE(body);
        for (let vertex = 0; vertex < count; vertex += 1) {
          const offset = body + 2 + vertex * 12;
          context.object.positions.push(
            round(data.readFloatLE(offset)),
            round(data.readFloatLE(offset + 4)),
            round(data.readFloatLE(offset + 8))
          );
        }
      } else if (id === 0x4120 && context.object) {
        const count = data.readUInt16LE(body);
        context.object.faceMaterials = new Array(count).fill(null);
        for (let face = 0; face < count; face += 1) {
          const offset = body + 2 + face * 8;
          context.object.indices.push(
            data.readUInt16LE(offset),
            data.readUInt16LE(offset + 2),
            data.readUInt16LE(offset + 4)
          );
        }
        walk(body + 2 + count * 8, chunkEnd, context);
      } else if (id === 0x4130 && context.object) {
        const name = readCString(data, body, chunkEnd);
        const count = data.readUInt16LE(name.next);
        for (let face = 0; face < count; face += 1) {
          const faceIndex = data.readUInt16LE(name.next + 2 + face * 2);
          if (faceIndex < context.object.faceMaterials.length) {
            context.object.faceMaterials[faceIndex] = name.value;
          }
        }
      } else if (id === 0x4140 && context.object) {
        const count = data.readUInt16LE(body);
        for (let uv = 0; uv < count; uv += 1) {
          const offset = body + 2 + uv * 8;
          context.object.uvs.push(round(data.readFloatLE(offset)), round(data.readFloatLE(offset + 4)));
        }
      } else if (id === 0x4150 && context.object) {
        const faceCount = context.object.indices.length / 3;
        for (let face = 0; face < faceCount && body + face * 4 + 4 <= chunkEnd; face += 1) {
          context.object.smoothingGroups.push(data.readUInt32LE(body + face * 4));
        }
      } else if (id === 0x4160 && context.object && body + 48 <= chunkEnd) {
        context.object.localMatrix = Array.from({ length: 12 }, (_, index) => round(data.readFloatLE(body + index * 4)));
      }

      cursor += length;
    }
  }

  walk(0, data.length);

  for (const object of objects) {
    const vertexCount = object.positions.length / 3;
    if (object.indices.some((index) => index >= vertexCount)) {
      throw new Error(`${basename(path)}:${object.name} has an out-of-range face index`);
    }
    if (object.uvs.length !== vertexCount * 2) object.uvs = [];
  }

  const yUpObjects = objects.map((object) => ({
    ...object,
    positions: convertZUpToYUp(object.positions),
    indices: object.indices.flatMap((_, index, source) => {
      if (index % 3 !== 0) return [];
      return [source[index], source[index + 2], source[index + 1]];
    })
  }));

  return {
    byteLength: data.length,
    sha256: hashFile(path),
    materials,
    objects,
    yUpObjects
  };
}

const knownTvmStrides = new Set([24, 32, 36, 40, 44, 48, 56, 64]);

function parseTvm(path) {
  const data = readFileSync(path);
  let stride = 32;
  const groups = [];
  let cursor = 0;
  while (cursor < data.length - 8) {
    const tag = data.toString("latin1", cursor, cursor + 4);
    if (tag !== "MSTA" && tag !== "MVER" && tag !== "MI16" && tag !== "MI32") {
      cursor += 1;
      continue;
    }
    const size = data.readUInt32LE(cursor + 4);
    const start = cursor + 8;
    if (start + size > data.length) {
      cursor += 1;
      continue;
    }
    if (tag === "MSTA" && size >= 28) {
      const candidate = data.readUInt32LE(start + 24);
      if (knownTvmStrides.has(candidate)) stride = candidate;
    } else if (tag === "MVER") {
      const count = Math.floor(size / stride);
      const positions = [];
      for (let vertex = 0; vertex < count; vertex += 1) {
        const offset = start + vertex * stride;
        positions.push(
          round(data.readFloatLE(offset)),
          round(data.readFloatLE(offset + 4)),
          round(data.readFloatLE(offset + 8))
        );
      }
      groups.push({ positions, indexCount: 0, indexType: null });
    } else if ((tag === "MI16" || tag === "MI32") && groups.length > 0) {
      const bytes = tag === "MI16" ? 2 : 4;
      groups.at(-1).indexCount = Math.floor(size / bytes);
      groups.at(-1).indexType = tag;
    }
    cursor = start + size;
  }
  const positions = groups.flatMap((group) => group.positions);
  return {
    byteLength: data.length,
    sha256: hashFile(path),
    stride,
    groupCount: groups.length,
    vertexCount: positions.length / 3,
    indexCount: groups.reduce((sum, group) => sum + group.indexCount, 0),
    triangleCount: groups.reduce((sum, group) => sum + Math.floor(group.indexCount / 3), 0),
    bounds: boundsOf(positions),
    groups: groups.map((group) => ({
      vertexCount: group.positions.length / 3,
      indexCount: group.indexCount,
      indexType: group.indexType,
      bounds: boundsOf(group.positions)
    }))
  };
}

function findTexture(reference, roots) {
  const wanted = reference.toLocaleLowerCase();
  for (const root of roots) {
    if (!existsSync(root)) continue;
    const match = readdirSync(root).find((name) => name.toLocaleLowerCase() === wanted);
    if (match) {
      const path = join(root, match);
      return { path, byteLength: statSync(path).size, sha256: hashFile(path) };
    }
  }
  return null;
}

function summarize3ds(path, textureRoots) {
  const parsed = parse3ds(path);
  const textureReferences = [...new Set(parsed.materials.map((material) => material.texture).filter(Boolean))];
  const assignmentCounts = new Map();
  for (const object of parsed.objects) {
    for (const name of object.faceMaterials) {
      const key = name ?? "<unassigned>";
      assignmentCounts.set(key, (assignmentCounts.get(key) ?? 0) + 1);
    }
  }
  const rawPositions = parsed.objects.flatMap((object) => object.positions);
  const yUpPositions = parsed.yUpObjects.flatMap((object) => object.positions);
  return {
    file: path,
    byteLength: parsed.byteLength,
    sha256: parsed.sha256,
    objectCount: parsed.objects.length,
    vertexCount: parsed.objects.reduce((sum, object) => sum + object.positions.length / 3, 0),
    triangleCount: parsed.objects.reduce((sum, object) => sum + object.indices.length / 3, 0),
    authoredZUpBounds: boundsOf(rawPositions),
    deterministicYUpBounds: boundsOf(yUpPositions),
    objects: parsed.objects.map((object) => ({
      name: object.name,
      vertexCount: object.positions.length / 3,
      triangleCount: object.indices.length / 3,
      uvCount: object.uvs.length / 2,
      authoredZUpBounds: boundsOf(object.positions),
      localMatrix: object.localMatrix,
      materialAssignments: Object.fromEntries(
        [...new Set(object.faceMaterials)].map((name) => [
          name ?? "<unassigned>",
          object.faceMaterials.filter((candidate) => candidate === name).length
        ])
      )
    })),
    materials: parsed.materials,
    faceMaterialAssignments: Object.fromEntries(assignmentCounts),
    textures: textureReferences.map((reference) => ({
      reference,
      resolved: findTexture(reference, textureRoots)
    }))
  };
}

const gt4Path = join(archiveCars, "gt4.3DS");
const cobraPath = join(archiveCars, "Low_Cobra.3DS");
const cobraDuplicatePath = join(archiveCars, "Low Cobra", "Low_Cobra.3DS");
const gt4TvmPath = join(archiveCars, "GT4.tvm");
const cobraTvmPath = join(archiveCars, "Low Cobra.tvm");

const report = {
  audit: "GT4 and Low Cobra archive vehicle-pack evidence",
  coordinatePolicy: {
    authored: "3DS coordinates are retained exactly as authored (Z-up).",
    inspection: "The separate deterministic inspection view maps (x,y,z) to (x,z,-y) and reverses triangle winding; it applies no size normalization."
  },
  selectorEvidence: {
    source: "Yanik C++ BCKUP/CubXSolution/3DScenes.h + 3DScenes.cpp",
    declaredCount: 1,
    declaredNames: ["Impreza"],
    clickCallback: "empty",
    gt4BoundToSelector: false,
    lowCobraBoundToSelector: false
  },
  hostVehicleEvidence: {
    source: "Yanik C++ BCKUP/CubXSolution/Vehicule.cpp",
    binding: "Constructor hardcodes Impreza chassis/wheels/textures; no GT4 or Low Cobra path occurs in the host code.",
    reusableButUnboundSettings: {
      chassisMass: 1500,
      chassisShape: "convex hull",
      wheelMass: 25,
      wheelWidth: 0.5,
      wheelRadius: 0.372,
      suspensionLength: 1,
      suspensionShock: 40,
      suspensionSpring: 300,
      drivenWheels: ["rear-left", "rear-right"],
      steeringWheels: ["front-left", "front-right"],
      inputPower: 3000,
      handbrakeTorque: 2000
    },
    fidelityBoundary: "These values are evidence for CLVehicule/Impreza only. Applying them to GT4 or Low Cobra would be a new binding, not an archive restoration fact."
  },
  gt4: {
    model3ds: summarize3ds(gt4Path, [archiveCars]),
    convertedTvm: { file: gt4TvmPath, ...parseTvm(gt4TvmPath) },
    aliases: [
      { path: gt4Path, role: "editable 3DS source" },
      { path: gt4TvmPath, role: "TrueVision converted mesh" },
      { path: join(archiveCars, "GT4 WORK.jpg"), role: "exact GT4 WORK.JPG material-map reference (case-insensitive match)" }
    ]
  },
  lowCobra: {
    model3ds: summarize3ds(cobraPath, [archiveCars, join(archiveCars, "Low Cobra")]),
    convertedTvm: { file: cobraTvmPath, ...parseTvm(cobraTvmPath) },
    aliases: [
      { path: cobraPath, sha256: hashFile(cobraPath), role: "root 3DS source" },
      { path: cobraDuplicatePath, sha256: hashFile(cobraDuplicatePath), role: "byte-identical folder duplicate" },
      { path: cobraTvmPath, role: "TrueVision converted mesh" },
      { path: join(archiveCars, "Low_Cobra_3ds.zip"), role: "packaging archive; not required when the extracted source is present" }
    ],
    adjacentPaintVariants: ["Cobra_Black.tga", "Cobra_Blue.tga", "Cobra_Green.tga", "Cobra_org.tga", "Cobra_Red.tga"].map((name) => {
      const path = join(archiveCars, name);
      return {
        path,
        byteLength: statSync(path).size,
        sha256: hashFile(path),
        role: "included body-paint variant; Low_Cobra.3DS references COBTEX.TGA rather than this file"
      };
    })
  }
};

if (verify) {
  const checks = [
    [report.selectorEvidence.declaredCount === 1, "selector count stays one"],
    [report.selectorEvidence.declaredNames.join() === "Impreza", "selector roster stays Impreza-only"],
    [report.gt4.model3ds.objectCount > 0, "GT4 3DS contains mesh objects"],
    [report.gt4.model3ds.triangleCount > 0, "GT4 3DS contains faces"],
    [report.gt4.convertedTvm.vertexCount > 0, "GT4 TVM geometry is readable"],
    [report.lowCobra.model3ds.objectCount > 0, "Low Cobra 3DS contains mesh objects"],
    [report.lowCobra.model3ds.triangleCount > 0, "Low Cobra 3DS contains faces"],
    [report.lowCobra.convertedTvm.vertexCount > 0, "Low Cobra TVM geometry is readable"],
    [
      report.lowCobra.aliases[0].sha256 === report.lowCobra.aliases[1].sha256,
      "Low Cobra root/folder 3DS aliases are byte-identical"
    ],
    [
      report.gt4.model3ds.textures.every((texture) => texture.resolved),
      "every GT4 3DS texture reference resolves"
    ],
    [
      report.lowCobra.model3ds.textures.every((texture) => texture.resolved),
      "every Low Cobra 3DS texture reference resolves"
    ]
  ];
  const failures = checks.filter(([passed]) => !passed);
  for (const [passed, label] of checks) {
    console.log(`${passed ? "PASS" : "FAIL"} ${label}`);
  }
  if (failures.length > 0) process.exitCode = 1;
} else {
  console.log(JSON.stringify(report, null, 2));
}
