# GT4 / Low Cobra archive vehicle-pack audit

Audit date: 2026-07-17

Scope: the `GT4` and `Low Cobra` `PIPELINE` items, plus only the directly relevant archived Car Selector and `CLVehicule` evidence. This audit does not change census status and does not claim restored driving physics.

## Status conclusion

Keep both vehicles `PIPELINE` at the scene/product level.

Their model packs are readable and can now be inspected exactly in an isolated browser entry, but the recovered host code never binds either model to the Car Selector or to `CLVehicule`. A model preview is not evidence of a restored vehicle choice or an authentic drivable configuration.

The archive's Car Selector is explicitly Impreza-only:

- `Yanik C++ BCKUP/CubXSolution/3DScenes.h:186` declares `QTY_CARS 1`.
- `Yanik C++ BCKUP/CubXSolution/3DScenes.cpp:159` declares `{"Impreza"}`.
- `3DScenes.cpp:165` constructs the same hard-coded `CLVehicule` for that single entry and even notes that it is always the same car model.
- `3DScenes.cpp:197-198` contains an empty `MeshClickedAction` callback.

The generic vehicle host is also Impreza-bound:

- `Vehicule.cpp:17-18` hard-codes the Impreza texture set.
- `Vehicule.cpp:21` hard-codes `chassis.tvm`, `wheel_l.tvm`, and `wheel_r.tvm`.
- No GT4, Cobra, `gt4.3DS`, `Low_Cobra.3DS`, `GT4.tvm`, or `Low Cobra.tvm` path occurs in the selector/vehicle host sources.

## Exact GT4 evidence

| Evidence | Exact result |
|---|---|
| Editable source | `Media/Models/cars/gt4.3DS` |
| Source bytes / SHA-256 | 336,272 / `3d1fdb24d6cda6bb36d134b6ea474e3351863b076127afbae39323e6278a54cf` |
| Raw 3DS mesh census | 14 objects, 10,740 point records, 8,345 triangle faces |
| Authored Z-up bounds | `[-18.357094,-54.45303,-3.602633]` to `[20.018888,46.686684,45.94915]` |
| Materials | `gt4 tex`, `gt4 glass` |
| Face assignments | 8,205 `gt4 tex`; 140 `gt4 glass` |
| Exact texture reference | `GT4 WORK.JPG`, resolved case-insensitively to adjacent `GT4 WORK.jpg` |
| Converted archive mesh | `Media/Models/cars/GT4.tvm` |
| TVM bytes / SHA-256 | 500,134 / `5195a5fbffb8667400aa3a935aa3bc294df0b886e490e9096cff7904b4372a1f` |
| Readable TVM geometry | 1 group, 12,916 vertices, 25,035 indices, 8,345 triangles |
| TVM bounds | `[-13.077926,-0.096942,-31.179939]` to `[13.077972,22.257668,31.285318]` |

All fourteen exact 3DS object names are retained: `gt4 wh fro`, `gt4 ref bo`, `gt4 wh br `, `gt4 glass`, `gt4 ref la`, `gt4 non re`, `gt4 ref b0`, `gt4 non r0`, `gt4 wh br0`, `gt4 wh rea`, `gt4 wh br1`, `gt4 wh br2`, `gt4 wh fr0`, and `gt4 wh re0`.

The textured `gt4 tex` material has an authored diffuse value of black. The inspection viewer discloses one compatibility override—white diffuse modulation—so that the exact `GT4 WORK.JPG` texture is visible. It does not alter the source or generated evidence counts.

## Exact Low Cobra evidence

| Evidence | Exact result |
|---|---|
| Editable source | `Media/Models/cars/Low_Cobra.3DS` |
| Source bytes / SHA-256 | 189,857 / `1af174e5f135a530e833fafcaadcef604abbaa159c1d07c2cd189adda40688f6` |
| Byte-identical alias | `Media/Models/cars/Low Cobra/Low_Cobra.3DS` — same SHA-256 |
| Raw 3DS mesh census | 10 objects, 6,961 point records, 3,266 triangle faces |
| Authored Z-up bounds | `[-109.553429,-58.328781,-2.790308]` to `[92.152016,46.795528,71.998558]` |
| Objects | `Carbody`, `Tire`, `Tire0`, `Tire1`, `Tire2`, `heads`, `tail`, `Glass`, `Bar`, `Scoop` |
| Material slots | 7 exact slots |
| Exact texture references | `COBTEX.TGA`, `TA_TIRE.TGA`, `TIRE_TRD.TGA`, `DISK_BRK.TGA`, `HEAD_LT.TGA`, `TAIL_LT.TGA`, `GLASS.TGA` |
| Converted archive mesh | `Media/Models/cars/Low Cobra.tvm` |
| TVM bytes / SHA-256 | 270,563 / `c794993bc77b934549bcccdd2cdf4c332d0d9043134795eb5e0cc11689d8ccb3` |
| Readable TVM geometry | 1 group, 7,298 vertices, 9,798 indices, 3,266 triangles |
| TVM bounds | `[-101.924507,-2.790306,-46.743961]` to `[92.152008,58.632885,46.795528]` |

Exact face-material totals are 2,650 body-map faces, 400 tire-sidewall faces, 96 tread faces, 8 brake-disc faces, 60 headlight faces, 40 tail-light faces, and 12 glass faces.

The adjacent `Cobra_Black.tga`, `Cobra_Blue.tga`, `Cobra_Green.tga`, `Cobra_org.tga`, and `Cobra_Red.tga` files are included paint variants. They are not the material reference stored in `Low_Cobra.3DS`; the embedded reference is `COBTEX.TGA`. The included readme confirms multiple colors and asks importers to re-weld vertices for smoothing groups. Its advertised 1,886 vertices / 3,247 faces therefore must not replace the raw-file census above: the actual 3DS contains split point records, accessories, and 3,266 face records.

## Physics evidence and boundary

`CLVehicule` does preserve useful handling evidence: a 1,500-mass convex-hull chassis, four 25-mass wheels, 0.5 wheel width, 0.372 wheel radius, 1/40/300 suspension length/shock/spring, rear-wheel torque, front-wheel steering, input power 3,000, and a 2,000 rear handbrake. But every archived construction path uses the hard-coded Impreza chassis/wheels/materials.

Therefore those settings are exact evidence for `CLVehicule`/Impreza, not for GT4 or Low Cobra. Applying them to either pack is a new design binding and must be labelled as such.

The current hidden `piste-ovale-cobra` web race overclaims this boundary:

- Its archive description cites `AtmelCubx/Vehicule.cpp handling` even though Cobra is never bound there.
- It replaces the embedded `COBTEX.TGA` body reference with `Cobra_Blue.tga`.
- It applies the web Impreza vehicle rig and newly authored Cannon values (including chassis mass 14 and spherical wheel bodies) to the Cobra.

Recommended main integration: expose GT4 and Cobra as `asset inspection` entries using the loader API below. Keep the drivable Cobra experiment hidden or relabel it plainly as a **modern speculative handling showcase**; do not call its physics or blue paint binding an exact archive restoration.

## Isolated implementation and API

- `vehicle-pack-inspector.html` is the isolated source viewer; it does not enter the main menu or race registry.
- `src/vehicle-pack-loader.ts` exports `loadArchivedVehicle3ds(spec)` and `ArchivedVehicle3dsResult`.
- The loader reads the original Vite-bundled 3DS binaries at runtime and preserves exact point arrays, triangle faces, UVs, object names, per-face material lists, and referenced textures.
- It converts Z-up to Y-up deterministically as `(x,y,z) → (x,z,-y)`, reverses winding, and applies no size normalization.
- It recomputes runtime normals rather than claiming exact 3DS smoothing-group reconstruction; this is explicitly returned in `fidelityBoundary`.
- `tools/audit-vehicle-pack.mjs --verify` independently checks the sources, aliases, texture resolution, and TVM readability without modifying assets.

## Verification

- Deterministic source verifier: 11/11 checks passed.
- Standalone Playwright interaction QA: 31/31 checks passed, zero console/page errors.
- Required `web_game_playwright_client.js`: two deterministic Low Cobra captures, ready text state, zero error files.
- Isolated production build passes. Its only advisory is the known >500 kB viewer bundle threshold.

Visual captures:

- `output/playwright/vehicle-pack-inspector/gt4.png`
- `output/playwright/vehicle-pack-inspector/low-cobra.png`
- `output/web-game/vehicle-pack-inspector-final/shot-0.png`

The separate high-positioned GT4 reflection/glass layers and Cobra `Bar`/`Scoop` accessories visible in the captures are authored objects at authored locations; the inspector intentionally does not collapse or silently hide them.
