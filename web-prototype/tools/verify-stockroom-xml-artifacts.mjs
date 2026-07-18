import assert from "node:assert/strict";
import { createHash } from "node:crypto";
import { readFile } from "node:fs/promises";
import { resolve } from "node:path";
import { spawnSync } from "node:child_process";

const prototypeRoot = resolve(import.meta.dirname, "..");
const dataPath = resolve(prototypeRoot, "src/legacy/stockroom-xml-artifacts.json");

function sha256(bytes) {
  return createHash("sha256").update(bytes).digest("hex").toUpperCase();
}

function runConverter() {
  const result = spawnSync(process.execPath, [resolve(prototypeRoot, "tools/convert-stockroom-xml-artifacts.mjs")], {
    cwd: prototypeRoot,
    encoding: "utf8"
  });
  assert.equal(result.status, 0, result.stderr || result.stdout);
}

runConverter();
const firstBytes = await readFile(dataPath);
runConverter();
const secondBytes = await readFile(dataPath);
assert.equal(sha256(firstBytes), sha256(secondBytes), "Converter output is byte-deterministic across consecutive runs");

const data = JSON.parse(secondBytes.toString("utf8"));
assert.equal(data.schema, "graphysx.stockroom-xml-artifacts/v1");
assert.equal(data.converter.deterministic, true);
assert.deepEqual(data.classification, {
  distinctAssembledScenes: 0,
  serializerArtifacts: 2,
  conclusion: "BaseScene.xml and test1.xml are distinct schema artifacts, not distinct assembled worlds."
});

const base = data.documents.find((document) => document.id === "base-scene");
const test1 = data.documents.find((document) => document.id === "test1");
assert.ok(base && test1, "Both StockRoom XML artifacts are present");

assert.equal(base.sha256, "1F56E50E0FE48B6F7BF34024FA11C010755C3F59FA6FB2F70377BA0B8574CCA8");
assert.equal(base.bytes, 5006);
assert.equal(base.header.version, "V1_1");
assert.equal(base.root, "Scene3D");
assert.equal(base.objectCount, 18);
assert.equal(base.uniqueObjectSignatureCount, 1);
assert.equal(base.distinctSerializedTransformCount, 1);
assert.deepEqual(base.exactOverlapGroupSizes, [18]);
assert.equal(base.copies.length, 2);
assert.equal(base.copiesByteIdentical, true);
assert.equal(base.copiesSemanticallyEquivalent, true);
assert.ok(base.objects.every((object) => object.type === 7 && object.action === "PHYSICCUBE"));
assert.ok(base.objects.every((object) => object.name === "CubeTestPhys" && object.enabled));
assert.ok(base.objects.every((object) => object.mass === 0 && object.massAttributeName === "masse"));
assert.ok(base.objects.every((object) => object.meshControlled === false && object.newtonMaterial === 0));
assert.ok(base.objects.every((object) => JSON.stringify(object.position) === "[0,0,0]"));
assert.ok(base.objects.every((object) => object.rotation === null && object.absentSerializedFields.includes("Rot")));
assert.ok(base.objects.every((object) => JSON.stringify(object.scale) === "[10,10,10]"));
assert.ok(base.objects.every((object) => object.pathToMesh === "" && object.textureName === "TwoWay"));
assert.equal(base.mapSize, null);
assert.equal(base.ringPositionListSerialized, false);
assert.equal(base.unresolvedRecords[0].count, 18);
assert.equal(base.unresolvedRecords[1].kind, "runtime-texture-binding-unproven");

assert.equal(test1.bytes, 856);
assert.equal(test1.sha256, "BB8396EBA946E01948BA1072C2D683A505719CBC43AA4BE535784CA31FE0672F");
assert.equal(test1.header.version, "V1_2");
assert.equal(test1.objectCount, 1);
assert.equal(test1.uniqueObjectSignatureCount, 1);
assert.equal(test1.distinctSerializedTransformCount, 1);
assert.deepEqual(test1.exactOverlapGroupSizes, []);
assert.equal(test1.copies.length, 2);
assert.deepEqual(test1.copies.map((copy) => copy.sha256), [
  "BB8396EBA946E01948BA1072C2D683A505719CBC43AA4BE535784CA31FE0672F",
  "56C4459DD6D9A483BB35B25978D7CD99C1AB05BF316D2B50D6DCB315FDAF24EB"
]);
assert.equal(test1.copiesByteIdentical, false);
assert.equal(test1.copiesSemanticallyEquivalent, true);
assert.equal(test1.copies[0].whitespaceSemanticSha256, test1.copies[1].whitespaceSemanticSha256);
assert.equal(test1.objects[0].type, 7);
assert.equal(test1.objects[0].action, "PHYSICCUBE");
assert.equal(test1.objects[0].name, "cubBasic");
assert.equal(test1.objects[0].massAttributeName, "Masse");
assert.equal(test1.objects[0].meshControlled, true);
assert.equal(test1.objects[0].newtonMaterial, 1);
assert.deepEqual(test1.objects[0].position, [0, 0, 0]);
assert.deepEqual(test1.objects[0].rotation, [0, 0, 0]);
assert.deepEqual(test1.objects[0].scale, [1, 1, 1]);
assert.equal(test1.objects[0].textureName, "");
assert.equal(test1.mapSize, 0);
assert.equal(test1.ringPositionListSerialized, true);
assert.equal(test1.ringPositionCount, 0);
assert.deepEqual(test1.header.physicsMaterials, {
  0: "DEFAULT_MAT",
  1: "WOOD",
  2: "FINISH",
  3: "LEVEL",
  4: "BALL",
  5: "ELEVATOR"
});

for (const document of [base, test1]) {
  assert.equal(document.serializedSubsystems.camera, false);
  assert.equal(document.serializedSubsystems.lights, false);
  assert.equal(document.serializedSubsystems.sky, false);
  assert.equal(document.serializedSubsystems.gravity, false);
  assert.equal(document.serializedSubsystems.physicsTimestep, false);
  assert.equal(document.serializedSubsystems.interactions, false);
  assert.equal(document.serializedSubsystems.gameplayObjective, false);
}

assert.equal(data.textureCandidate.sha256, "4998527ACE4E853954E77EC5B937024CB2D2E23064FBBDE4A039224E978A2B21");
assert.match(data.textureCandidate.status, /runtime-binding-unproven/);
const browserTexture = await readFile(resolve(prototypeRoot, "public/assets/stockroom-xml/twoway.jpg"));
assert.equal(sha256(browserTexture), data.textureCandidate.sha256, "Browser texture candidate is byte-identical to source");
assert.equal(data.hostEvidence.length, 4);
assert.ok(data.hostEvidence.some((entry) => entry.id === "obj3d-lineage"));
assert.equal(data.fidelityBoundary.forbiddenInference.length, 3);

console.log(JSON.stringify({
  assertions: 72,
  artifacts: 2,
  rawCopies: 4,
  distinctAssembledScenes: 0,
  deterministicOutputSha256: sha256(secondBytes)
}, null, 2));
