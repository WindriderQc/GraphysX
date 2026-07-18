import { readFile, writeFile } from "node:fs/promises";
import path from "node:path";

const [, , sourcePath, destinationPath, requestedLimit = "160"] = process.argv;

if (!sourcePath || !destinationPath) {
  throw new Error("Usage: node tools/convert-sbqc-quakes.mjs <quakes.csv> <output.json> [limit]");
}

const limit = Math.max(1, Math.min(500, Number.parseInt(requestedLimit, 10) || 160));
const lines = (await readFile(sourcePath, "utf8")).split(/\r?\n/).filter(Boolean);
const events = lines
  .slice(1)
  .map((line) => {
    // The fields needed here precede the quoted place field, so a full CSV
    // parser is unnecessary for this deterministic conversion.
    const [time, latitude, longitude, depth, magnitude] = line.split(",");
    return {
      time,
      latitude: Number(latitude),
      longitude: Number(longitude),
      depthKm: Number(depth),
      magnitude: Number(magnitude)
    };
  })
  .filter(
    (event) =>
      event.time &&
      Number.isFinite(event.latitude) &&
      Number.isFinite(event.longitude) &&
      Number.isFinite(event.depthKm) &&
      Number.isFinite(event.magnitude)
  )
  .sort((left, right) => right.magnitude - left.magnitude || left.time.localeCompare(right.time))
  .slice(0, limit)
  .sort((left, right) => left.time.localeCompare(right.time));

const payload = {
  source: "WindriderQc/SBQC/public/data/quakes.csv",
  sourceCommit: "42c210f3599c55038efde8fad61d9fe5bf5c0f24",
  selection: `top ${events.length} events by magnitude from ${lines.length - 1} valid/source rows`,
  events
};

await writeFile(destinationPath, `${JSON.stringify(payload, null, 2)}\n`, "utf8");
console.log(`${path.basename(destinationPath)}: ${events.length} events, M${Math.min(...events.map((event) => event.magnitude)).toFixed(1)}–M${Math.max(...events.map((event) => event.magnitude)).toFixed(1)}`);
