// Cars pack decoder — .3ds (documented chunk format) + PisteOvale TVM.
//
// 3DS chunks: [id:uint16][length:uint32] — 0x4D4D main, 0x3D3D editor,
// 0x4000 named object (cstring name), 0x4100 trimesh, 0x4110 vertices,
// 0x4120 faces, 0x4140 UVs. 3DS is Z-up; converted here to Y-up (x, z, -y).
//
// Usage: node tools/convert-cars.mjs
// Writes src/legacy/cars-catalog.json

import { readFileSync, writeFileSync, mkdirSync } from "node:fs";
import { dirname, join } from "node:path";
import { fileURLToPath } from "node:url";

const here = dirname(fileURLToPath(import.meta.url));
const CARS = join(here, "..", "..", "Yanik C++ BCKUP", "Media", "Models", "cars");
const ATMEL = join(here, "..", "..", "AtmelCubx");
const OUT = join(here, "..", "src", "legacy", "cars-catalog.json");

function round(v) {
  return Math.round(v * 1000) / 1000;
}

// ---------- .3ds ----------

function parse3ds(path) {
  const data = readFileSync(path);
  const objects = [];
  const materials = [];

  const walk = (start, end, name, material = null) => {
    let i = start;
    while (i + 6 <= end) {
      const id = data.readUInt16LE(i);
      const length = data.readUInt32LE(i + 2);
      if (length < 6 || i + length > end + 1) {
        break;
      }
      const body = i + 6;
      if (id === 0x4d4d || id === 0x3d3d) {
        walk(body, i + length, name, material);
      } else if (id === 0xafff) {
        const nextMaterial = { name: "", texture: "" };
        materials.push(nextMaterial);
        walk(body, i + length, name, nextMaterial);
      } else if (id === 0xa000 && material) {
        const zero = data.indexOf(0, body);
        material.name = data.toString("latin1", body, zero);
      } else if (id === 0xa200 && material) {
        walk(body, i + length, name, material);
      } else if (id === 0xa300 && material) {
        const zero = data.indexOf(0, body);
        material.texture = data.toString("latin1", body, zero);
      } else if (id === 0x4000) {
        const zero = data.indexOf(0, body);
        walk(zero + 1, i + length, data.toString("latin1", body, zero), material);
      } else if (id === 0x4100) {
        objects.push({ name: name ?? "unnamed", positions: [], indices: [], uvs: [], faceMaterials: [] });
        walk(body, i + length, name, material);
      } else if (id === 0x4110) {
        const count = data.readUInt16LE(body);
        const target = objects[objects.length - 1];
        for (let v = 0; v < count; v++) {
          // Z-up -> Y-up: (x, y, z) -> (x, z, -y)
          const x = data.readFloatLE(body + 2 + v * 12);
          const y = data.readFloatLE(body + 2 + v * 12 + 4);
          const z = data.readFloatLE(body + 2 + v * 12 + 8);
          target.positions.push(round(x), round(z), round(-y));
        }
      } else if (id === 0x4120) {
        const count = data.readUInt16LE(body);
        const target = objects[objects.length - 1];
        target.faceMaterials = new Array(count).fill("");
        for (let f = 0; f < count; f++) {
          const a = data.readUInt16LE(body + 2 + f * 8);
          const b = data.readUInt16LE(body + 2 + f * 8 + 2);
          const c = data.readUInt16LE(body + 2 + f * 8 + 4);
          target.indices.push(a, c, b); // axis conversion flips winding
        }
        // Face lists can contain nested 0x4130 material assignments after the
        // fixed face records. Preserve them so the undercarriage does not get
        // painted with the body livery.
        walk(body + 2 + count * 8, i + length, name, material);
      } else if (id === 0x4130) {
        const target = objects[objects.length - 1];
        const zero = data.indexOf(0, body);
        const materialName = data.toString("latin1", body, zero);
        const countOffset = zero + 1;
        const count = data.readUInt16LE(countOffset);
        for (let face = 0; face < count; face++) {
          const faceIndex = data.readUInt16LE(countOffset + 2 + face * 2);
          if (faceIndex < target.faceMaterials.length) {
            target.faceMaterials[faceIndex] = materialName;
          }
        }
      } else if (id === 0x4140) {
        const count = data.readUInt16LE(body);
        const target = objects[objects.length - 1];
        for (let u = 0; u < count; u++) {
          target.uvs.push(round(data.readFloatLE(body + 2 + u * 8)), round(1 - data.readFloatLE(body + 2 + u * 8 + 4)));
        }
      }
      i += length;
    }
  };

  walk(0, data.length, null);

  for (const object of objects) {
    const vertexCount = object.positions.length / 3;
    for (const index of object.indices) {
      if (!(index >= 0 && index < vertexCount)) {
        throw new Error(path + " " + object.name + ": index out of range");
      }
    }
    if (object.uvs.length !== vertexCount * 2) {
      object.uvs = null;
    }
  }
  return { objects, materials };
}

// ---------- TVM (same decoder as convert-tvm.mjs) ----------

const KNOWN_STRIDES = new Set([24, 32, 36, 40, 44, 48, 56, 64]);

function parseTvm(path) {
  const data = readFileSync(path);
  let stride = 32;
  const groups = [];
  let i = 0;
  while (i < data.length - 8) {
    const tag = data.toString("latin1", i, i + 4);
    if (tag === "MSTA" || tag === "MVER" || tag === "MI16" || tag === "MI32") {
      const size = data.readUInt32LE(i + 4);
      const start = i + 8;
      if (tag === "MSTA" && size >= 28) {
        const candidate = data.readUInt32LE(start + 24);
        if (KNOWN_STRIDES.has(candidate)) {
          stride = candidate;
        }
      } else if (tag === "MVER") {
        const count = Math.floor(size / stride);
        const positions = new Array(count * 3);
        const uvs = stride >= 32 ? new Array(count * 2) : null;
        for (let v = 0; v < count; v++) {
          positions[v * 3] = round(data.readFloatLE(start + v * stride));
          positions[v * 3 + 1] = round(data.readFloatLE(start + v * stride + 4));
          positions[v * 3 + 2] = round(data.readFloatLE(start + v * stride + 8));
          if (uvs) {
            uvs[v * 2] = round(data.readFloatLE(start + v * stride + 24));
            uvs[v * 2 + 1] = round(1 - data.readFloatLE(start + v * stride + 28));
          }
        }
        groups.push({ positions, uvs, indices: [] });
      } else if (tag === "MI16" || tag === "MI32") {
        const bytes = tag === "MI16" ? 2 : 4;
        const count = Math.floor(size / bytes);
        const indices = new Array(count);
        for (let n = 0; n < count; n++) {
          indices[n] = bytes === 2 ? data.readUInt16LE(start + n * 2) : data.readUInt32LE(start + n * 4);
        }
        if (groups.length > 0) {
          groups[groups.length - 1].indices = indices;
        }
      }
      i = start + size;
    } else {
      i += 1;
    }
  }

  const positions = [];
  const uvs = [];
  const indices = [];
  for (const group of groups) {
    const offset = positions.length / 3;
    positions.push(...group.positions);
    if (group.uvs) {
      uvs.push(...group.uvs);
    }
    for (const index of group.indices) {
      indices.push(index + offset);
    }
  }
  return { positions, indices, uvs: uvs.length === (positions.length / 3) * 2 ? uvs : null };
}

// ---------- helpers ----------

function boundsOf(positions) {
  const min = [Infinity, Infinity, Infinity];
  const max = [-Infinity, -Infinity, -Infinity];
  for (let i = 0; i < positions.length; i += 3) {
    for (let axis = 0; axis < 3; axis++) {
      min[axis] = Math.min(min[axis], positions[i + axis]);
      max[axis] = Math.max(max[axis], positions[i + axis]);
    }
  }
  return { min: min.map(round), max: max.map(round) };
}

function centerOf(positions) {
  const b = boundsOf(positions);
  return [(b.min[0] + b.max[0]) / 2, (b.min[1] + b.max[1]) / 2, (b.min[2] + b.max[2]) / 2];
}

function recenterAt(positions, center) {
  return positions.map((value, index) => round(value - center[index % 3]));
}

// ---------- Impreza ----------

const imprezaData = parse3ds(join(CARS, "impreza.3ds"));
const imprezaObjects = imprezaData.objects;
const chassisObject = imprezaObjects.find((object) => object.name.toLowerCase().startsWith("chasis"));
const wheelObjects = imprezaObjects.filter((object) => object.name.toLowerCase().startsWith("rueda"));

const chassisCenter = centerOf(chassisObject.positions);
const chassis = {
  positions: recenterAt(chassisObject.positions, chassisCenter),
  indices: chassisObject.indices,
  uvs: chassisObject.uvs,
  faceMaterials: chassisObject.faceMaterials,
  bounds: null
};
chassis.bounds = boundsOf(chassis.positions);

const wheels = wheelObjects.map((object) => {
  const center = centerOf(object.positions);
  const positions = recenterAt(object.positions, center);
  return {
    name: object.name,
    // wheel hub position relative to the chassis center
    offset: [round(center[0] - chassisCenter[0]), round(center[1] - chassisCenter[1]), round(center[2] - chassisCenter[2])],
    positions,
    indices: object.indices,
    uvs: object.uvs,
    bounds: boundsOf(positions)
  };
});

console.log("impreza chassis: " + chassis.positions.length / 3 + " verts, bounds " + JSON.stringify(chassis.bounds.min) + ".." + JSON.stringify(chassis.bounds.max));
console.log("  materials " + JSON.stringify([...new Set(chassis.faceMaterials.filter(Boolean))]));
console.log("  textures " + JSON.stringify(imprezaData.materials));
for (const wheel of wheels) {
  console.log("  " + wheel.name + " offset " + JSON.stringify(wheel.offset) + " radius~" + round((wheel.bounds.max[1] - wheel.bounds.min[1]) / 2));
}

// ---------- Piste Ovale ----------

const track = parseTvm(join(ATMEL, "PisteOvale.tvm"));
console.log("piste ovale: " + track.positions.length / 3 + " verts, " + track.indices.length / 3 + " tris");

mkdirSync(dirname(OUT), { recursive: true });
writeFileSync(
  OUT,
  JSON.stringify({
    generated: new Date().toISOString(),
    source: "Models/cars/impreza.3ds + AtmelCubx/PisteOvale.tvm",
    impreza: { chassis, wheels, materials: imprezaData.materials },
    track: { ...track, bounds: boundsOf(track.positions) }
  })
);
console.log("wrote " + OUT);

// ---------- Low Cobra (second selectable car) ----------

try {
  const { objects: cobraObjects } = parse3ds(join(CARS, "Low_Cobra.3DS"));
  // authored ~200 units long along X: rotate onto Z and scale to Impreza size
  const COBRA_SCALE = 0.024;
  for (const object of cobraObjects) {
    const source = object.positions;
    const transformed = new Array(source.length);
    for (let i = 0; i < source.length; i += 3) {
      transformed[i] = round(source[i + 2] * COBRA_SCALE);
      transformed[i + 1] = round(source[i + 1] * COBRA_SCALE);
      transformed[i + 2] = round(-source[i] * COBRA_SCALE);
    }
    object.positions = transformed;
  }
  const bodyParts = cobraObjects.filter((o) => !/^tire/i.test(o.name));
  const tireParts = cobraObjects.filter((o) => /^tire/i.test(o.name));

  const bodyMerged = { positions: [], indices: [], uvs: [], faceMaterials: [] };
  let bodyUvsOk = true;
  for (const part of bodyParts) {
    const offset = bodyMerged.positions.length / 3;
    bodyMerged.positions.push(...part.positions);
    for (const index of part.indices) bodyMerged.indices.push(index + offset);
    if (part.uvs) bodyMerged.uvs.push(...part.uvs);
    else bodyUvsOk = false;
    const faceCount = part.indices.length / 3;
    const label = /glass/i.test(part.name) ? "glass" : "";
    for (let f = 0; f < faceCount; f++) bodyMerged.faceMaterials.push(part.faceMaterials?.[f] || label);
  }
  const bodyCenter = centerOf(bodyMerged.positions);
  const cobraChassis = {
    positions: recenterAt(bodyMerged.positions, bodyCenter),
    indices: bodyMerged.indices,
    uvs: bodyUvsOk && bodyMerged.uvs.length === (bodyMerged.positions.length / 3) * 2 ? bodyMerged.uvs : null,
    faceMaterials: bodyMerged.faceMaterials,
    bounds: null
  };
  cobraChassis.bounds = boundsOf(cobraChassis.positions);

  const cobraWheels = tireParts.map((part) => {
    const center = centerOf(part.positions);
    const positions = recenterAt(part.positions, center);
    return {
      name: part.name,
      offset: [round(center[0] - bodyCenter[0]), round(center[1] - bodyCenter[1]), round(center[2] - bodyCenter[2])],
      positions,
      indices: part.indices,
      uvs: part.uvs,
      bounds: boundsOf(positions)
    };
  });

  const existing = JSON.parse(readFileSync(OUT, "latin1"));
  existing.cobra = { chassis: cobraChassis, wheels: cobraWheels };
  writeFileSync(OUT, JSON.stringify(existing));
  console.log("cobra: chassis " + cobraChassis.positions.length / 3 + " verts, bounds " + JSON.stringify(cobraChassis.bounds.min) + ".." + JSON.stringify(cobraChassis.bounds.max));
  for (const wheel of cobraWheels) {
    console.log("  " + wheel.name + " offset " + JSON.stringify(wheel.offset));
  }
} catch (error) {
  console.log("SKIP cobra: " + error.message);
}
