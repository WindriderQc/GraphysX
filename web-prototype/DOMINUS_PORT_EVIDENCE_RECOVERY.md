# Dominus Village / Port — final evidence recovery

## Outcome

The archived Dominus family does **not** contain a recoverable authored village or port scene. It does contain one exact multi-asset placement document: the 28 port-prefixed rows in `Archive/bckup/BallZ2015.bckup/Media/ObjectLibrary.xml`.

Those rows are now available in `DominusPortEvidenceEnvironment` as an exact evidence view:

- 28 serialized port records in source order;
- 27 exact decoded DirectX text-X meshes;
- 1 explicit unsupported binary-X record (`port_crateshed.X`), represented by an amber diagnostic box rather than invented geometry;
- 23,594 source vertices, 13,860 triangles, and 126 material groups;
- exact serialized positions, zero rotations, uniform `0.01` scales, enabled state, material groups, and surviving textures;
- the canonical 27,486-byte ObjectLibrary document and SHA-256 `4BF51E84F92084E8BE2B91214391BFA8839B29BCEB4C942187DD73B2B04AF432`.

The environment is deliberately classified `PARTIAL`. The only surviving placements are editor thumbnails on a regular five-unit grid. They are not an authored village composition.

## Evidence audit

The forensic audit covers the workspace copy plus the two known archive-mirror copies. All three ObjectLibrary files are byte-identical, so they represent one logical document.

The evidence establishes:

- `ObjectLibrary.xml` contains 61 mixed editor/catalog records;
- 41 rows match the broader Dominus asset family;
- 28 rows are port-prefixed;
- every port row is at `Y=0`, has zero rotation, uses uniform scale `0.01`, and occupies the same five-unit X/Z thumbnail grid as unrelated catalog assets;
- no archived source loader callsite names the Dominus/port assets;
- no authored scene document outside ObjectLibrary references them;
- no camera, spawn, navigation, terrain, water, collision, lighting, sky, or gameplay configuration survives.

The current race-scene `DOMINUS_LAYOUT` and ring tour are therefore a modern curated prototype. They may remain as a clearly labeled adaptation, but they cannot be presented as recovered authored composition.

## Runtime module

Source: `src/dominus-port-evidence-environment.ts`

Standalone preview: `dominus-port-evidence-preview.html`

The runtime keeps source coordinates in structured state and displays DirectX source `+Z` as web `-Z`. It does not center any local mesh, so each ObjectLibrary transform applies to the asset's original local frame. The neutral background, lights, grid, selection outline, orbit/focus camera, selected-only focus isolation, and binary-X marker are disclosed presentation adapters.

Primary API:

```ts
import { DominusPortEvidenceEnvironment } from "./dominus-port-evidence-environment";

const environment = new DominusPortEvidenceEnvironment({ loadingManager });
scene.add(environment.group);
await environment.ready;

environment.update(deltaSeconds, orbitInput, camera);
environment.selectById("port_lighthouse", true, camera);
environment.selectNext(1, camera);
environment.showSourceGrid(camera);
environment.reset(camera);
const state = environment.getState();
environment.dispose();
```

`getState()` supplies the full 28-row evidence inventory, exact source/display transforms, decoder status, geometry totals, evidence boundary, camera state, and explicit `PARTIAL` recovery status. `selectFromObject()` supports WebGL raycast selection without coupling the environment to the host UI.

## Main-app integration hooks

The narrow module intentionally does not edit `race-scene.ts`, `prototype-app.ts`, `archive-content.ts`, or the shared trackers. Integration should:

1. Add a standalone archive mode such as `dominus-port-evidence` rather than silently replacing a playable race.
2. Instantiate the environment, add `environment.group` to the archive scene, await `environment.ready`, and route the archive camera through `applyToCamera()` / `update()`.
3. Expose `selectById`, `selectNext`, `showSourceGrid`, orbit, zoom, reset, and the complete `getState()` payload through the existing archive debug/text-state bridge.
4. Dispose the environment when leaving the mode.
5. Label the existing `dominus-port` race composition **Modern Curated Visit** if it remains reachable.
6. Change the Dominus scene ledger from `PIPELINE` to `PARTIAL`, not `RESTORED`: exact assets and the only source placement document are inspectable, but the authored village/world is absent.

With that integration, the scene census can legitimately report zero `PIPELINE` items while retaining an honest missing-composition boundary inside the Dominus `PARTIAL` record.

## Verification

- `node tools/verify-dominus-port-evidence.mjs`: 355 source/hash/schema assertions, 0 failed.
- `node scripts/qa-dominus-port-evidence.mjs`: 110 browser assertions, 0 console/page errors.
- `npx tsc --noEmit`: passed.
- Required `develop-web-game` client: two clean iterations under `output/web-game/dominus-port-evidence-final/`; ready structured state, no error file.
- Visually inspected the exact overview, main docks, lighthouse, unsupported crate-shed boundary, and required-client overview captures.

The verifier binds every row back to the original XML, validates every canonical source-mesh hash, validates every decoded payload hash, and re-proves the absence of authored scene evidence. The browser QA covers all 28 selections, focus/overview transitions, source counts, the binary boundary, camera controls, invalid selection safety, deterministic stepping, and reset.
