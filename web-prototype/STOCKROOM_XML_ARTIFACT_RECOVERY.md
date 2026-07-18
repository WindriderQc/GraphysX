# StockRoom `BaseScene.xml` / `test1.xml` recovery

## Conclusion

Neither file is evidence for a distinct assembled scene.

- `BaseScene.xml` is a v1.1 serializer/default-duplication artifact: 18 identical `PHYSICCUBE` records occupy the same transform.
- `test1.xml` is a minimal v1.2 serializer smoke document: one unit `PHYSICCUBE` at the origin, with no surrounding composition.

Both should be removed from the **scene census** and retained in a **serializer/schema artifact inventory**. This is a classification correction, not a scene restoration or status promotion.

## Located copies

| Logical artifact | Copy | Bytes | SHA-256 | Relationship |
|---|---|---:|---|---|
| BaseScene | `E:/Media/Datalake/Tech/StockRoom/BaseScene.xml` | 5,006 | `1F56E50E0FE48B6F7BF34024FA11C010755C3F59FA6FB2F70377BA0B8574CCA8` | canonical |
| BaseScene | `E:/Media/Datalake/Tech/Repos/GraphysX/StockRoom/BaseScene.xml` | 5,006 | `1F56E50E0FE48B6F7BF34024FA11C010755C3F59FA6FB2F70377BA0B8574CCA8` | byte-identical |
| test1 | `E:/Media/Datalake/Tech/StockRoom/test1.xml` | 856 | `BB8396EBA946E01948BA1072C2D683A505719CBC43AA4BE535784CA31FE0672F` | canonical |
| test1 | `E:/Media/Datalake/Tech/Repos/GraphysX/StockRoom/test1.xml` | 857 | `56C4459DD6D9A483BB35B25978D7CD99C1AB05BF316D2B50D6DCB315FDAF24EB` | newline-only raw variant |

The two `test1.xml` copies normalize to the same whitespace-semantic SHA-256, `9D6BB2BEF670EA858CE5FEAFA3E439EBAA1334A55B9B3289CDCE9B30718497E0`. Their parsed header, attributes, fields, and object record are identical.

## Exact serialized evidence

### `BaseScene.xml`

- Root/schema: `Scene3D/Obj3D`, header `V1_1`.
- Objects: 18.
- Every object is type `7 = PHYSICCUBE`, named `CubeTestPhys`, enabled, mass `0`, `MeshControlled=false`, `NewtonMat=0`.
- Every position is `(0,0,0)` and every scale is `(10,10,10)`.
- Every object has an empty mesh path and the literal texture token `TwoWay`.
- Every `Rot` field is absent. The inspection renderer uses identity only as a disclosed adapter.
- Unique complete object signatures: 1.
- Distinct serialized transforms: 1.
- Exact coincident group: 18 objects.
- No map size, ring list, camera, light, sky, gravity, timestep, interaction, player, or objective is serialized.

Rendering the file faithfully therefore looks like **one large cube**, because all 18 records occupy the same surface. Separating them would invent a layout.

### `test1.xml`

- Root/schema: `Scene3D/Obj3D`, header `V1_2`.
- Objects: 1.
- The object is type `7 = PHYSICCUBE`, named `cubBasic`, enabled, mass `0`, `MeshControlled=true`, `NewtonMat=1`.
- Exact transform: position `(0,0,0)`, rotation `(0,0,0)`, scale `(1,1,1)`.
- Empty mesh path and empty texture name.
- `mapSize=0`; the serialized ring list is empty.
- The header adds `DUPLICATEMESH`, `VERTEXSTRIP`, and physics-material identifiers, demonstrating schema evolution.
- No camera, light, sky, gravity, timestep, interaction, player, or objective is serialized.

This is sufficient to verify the v1.2 serializer, but insufficient to establish an authored scene.

## Host/parser lineage

- `E:/Media/Datalake/Tech/Repos/Obj3D.h` (1,708 bytes, SHA-256 `5ABE0627ACFA15440B01CCFF024DFAEDBD43064D51A4BA4F14C1EDFA5535DE9C`) is the only located `Obj3D` source lineage. It confirms the attribute/vector model, default physics-cube metadata, and literal `TwoWay` default token.
- `GraphysX_1/MeshPrimitive.cpp` confirms the later native continuation creates boxes directly from serialized dimensions and applies position/rotation.
- `GraphysX_1/PhysGeom.cpp` confirms the later native continuation treats mass-zero physics primitives as static.
- `GraphysX_1/TextureCache.cpp` expects a filename. An exact same-folder `twoway.jpg` survives, but the v1.1 host code proving extensionless `TwoWay` resolution does not. The preview therefore keeps this exact file as an opt-in candidate, disabled by default.

The exact `twoway.jpg` candidate is 20,876 bytes with SHA-256 `4998527ACE4E853954E77EC5B937024CB2D2E23064FBBDE4A039224E978A2B21`.

## Census recommendation

Recommended independent census delta:

- `BaseScene.xml`: remove from scene census; add to serializer/schema artifact inventory.
- `test1.xml`: remove from scene census; add to serializer/schema artifact inventory.
- Distinct assembled scenes contributed by these files: **0**.
- Delta from the current checked-in census: **total −2, PIPELINE −2**.
- If applied to the current `3 RESTORED / 42 PARTIAL / 5 PIPELINE / 50 total` baseline, the result is **3 RESTORED / 42 PARTIAL / 3 PIPELINE / 48 total**.

If other concurrent audits change the baseline, apply only the independent `−2 total / −2 PIPELINE` delta.

## Isolated implementation

- `tools/convert-stockroom-xml-artifacts.mjs`: deterministic source converter and exact-asset copier.
- `src/legacy/stockroom-xml-artifacts.json`: generated source/copy/hash/schema/object evidence.
- `src/stockroom-xml-artifact-environment.ts`: exact-transform environment; no source object is separated or repositioned.
- `stockroom-xml-artifact-preview.html` and `src/stockroom-xml-artifact-preview.ts`: standalone preview with `render_game_to_text` and deterministic `advanceTime`.
- `tools/verify-stockroom-xml-artifacts.mjs`: 72 converter/hash/schema invariants.
- `scripts/qa-stockroom-xml-artifacts.mjs`: 34 live browser assertions and three visual captures.

### Minimal main-integration API

```ts
const environment = new StockroomXmlArtifactEnvironment({ loadingManager });
scene.add(environment.group);
await environment.ready;

environment.selectArtifact("base-scene", camera); // or "test1"
environment.setCandidateTextureEnabled(false);    // keep false by default
environment.update(deltaSeconds, orbitInput, camera);
const state = environment.getState();
environment.dispose();
```

The UI should label the destination **XML Serializer Artifacts**, not Archived Worlds, and must retain `distinctAssembledScene: false`.

## Verification

- Deterministic converter: pass, byte-identical output over consecutive runs.
- Static invariants: 72 assertions pass.
- Standalone Playwright: 34 assertions pass, 0 console/page errors.
- Required develop-web-game client: two iterations pass with structured state (`18/18`, overlap `18`) and no error artifact.
- Production TypeScript/Vite build: pass; only the existing large-chunk advisory remains.

Visual captures:

- `output/playwright/stockroom-xml-artifacts/00-base-scene-exact-overlap-neutral.png`
- `output/playwright/stockroom-xml-artifacts/01-base-scene-opt-in-texture-candidate.png`
- `output/playwright/stockroom-xml-artifacts/02-test1-exact-single-cube.png`
- `output/web-game/stockroom-xml-artifacts/shot-0.png`
- `output/web-game/stockroom-xml-artifacts/shot-1.png`
