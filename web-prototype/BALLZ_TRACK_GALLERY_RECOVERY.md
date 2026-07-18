# BallZ remaining slide/track gallery

This isolated gallery revives the six requested distinct meshes as evidence-bounded visits. It does not add a ball, collision, rings, checkpoints, camera provenance, controls, or objectives unless the corresponding source survives—and even then the evidence is reported rather than turned into gameplay.

## Gallery status

| Asset | Exact geometry | Host/material evidence | Recommendation |
| --- | ---: | --- | --- |
| Slide1A legacy | 2,271v / 2,849t | Older `CLBallZ` loads it at `(0,-5000,0)` as a static body and `CLSlideObject` applies `StdMat`. The complete later Ball assembly belongs to the different Slide1 binary. | `PARTIAL`: source-backed non-race visit |
| Level.Slides | 792v / 1,524t, 3 groups | No located host. Three embedded untextured MGRP colors survive. | `PARTIAL`: exact geometry/material study only |
| Level.Steps | 200v / 120t, 3 groups | No located host. MGRP binds groups exactly to `grass.jpg`, `concrete.png`, and `wood.jpg`; browser copies are source-hash identical. | `PARTIAL`: exact textured geometry study only |
| SlideBump | 699v / 1,456t | No located host. The MGRP record is entirely zero and topology has 44 boundary plus 61 non-manifold edges. | `PARTIAL`: neutral-fallback topology study |
| SlideBump GridTex | 685v / 1,456t | No located host. Exact same bounds/triangle count as Bump, but distinct hash, vertex/UV revision, and `EarthGri.bmp` binding; topology has 8 boundary plus 61 non-manifold edges. | `PARTIAL`: exact textured revision study |
| BallZTrack1 | 385v / 447t | Static `CLBallZScene` host at unit transform; Ball spawn `(-20,310,225)`, initial camera position `(400,300,200)`, chase offset `(25,100,30)`. `UpdateScene` is empty, camera input is commented out, and 100 diagonal rings are explicitly temporary. | `PARTIAL`: non-interactive hosted visit |

All four later StockRoom/Blender exports contain exact all-zero normal arrays. The gallery preserves those arrays in generated evidence and computes display normals only to make the archived surfaces visible; state labels this `computed-for-inspection`. Slide1A and BallZTrack1 contain valid unit source normals and use them unchanged.

## De-duplication and revision boundaries

- `SlideLong1.TVM` remains excluded because it is byte-identical to the already recovered BallZ 2011 `Level1.TVM`.
- `pipe1.tvm` remains excluded because every surviving copy is the same FlightX component, not a separate BallZ world.
- The compact StockRoom `Slide1.TVM` remains a distinct audited binary revision (282v / 556t, SHA-256 `B1B5066C…839C`) and is not substituted for the active Atmel Slide1 or Slide1A. It is outside this requested six-asset gallery because no loader/behavior was located.

## Exact versus inspection-only presentation

Exact data includes source hashes, indexed TVM geometry, source normals—even when zero—UVs, triangle material assignments, group records/hashes, texture names and four texture file hashes. Hosted transforms and behavior constants are exact only where source exists.

Inspection-only adapters include reversible per-asset centering/uniform scale, computed display normals for the four all-zero exports, lighting, grid, overview/top cameras, diagnostic group colors, edges, and bounds.

## Minimal integration API

`BallzTrackGalleryEnvironment` in `src/ballz-track-gallery-environment.ts` owns a mountable Three.js group:

```ts
const gallery = new BallzTrackGalleryEnvironment();
scene.add(gallery.group);
await gallery.ready;

gallery.selectAsset("level-steps");
gallery.setMaterialMode("source-evidence"); // or "diagnostic-groups"
gallery.setEdgesVisible(true);
gallery.setBoundsVisible(false);

const camera = gallery.getCameraProfile("overview"); // or "top"
const display = gallery.sourceWorldToDisplay("level-steps", [x, y, z]);
const source = gallery.displayToSourceWorld("level-steps", display.toArray());
const state = gallery.getState();

gallery.dispose();
```

The safest product integration is one BallZ Concepts/Archive Gallery destination with six internal selectors. Do not create six playable races or duplicate the known aliases.

## Verification

```powershell
node tools/convert-ballz-slide-track-family.mjs
node tools/verify-ballz-slide-track-family.mjs
node scripts/qa-ballz-track-gallery.mjs http://127.0.0.1:4192
```

The permanent browser matrix checks all six selections, exact geometry/material/topology counts, all host boundaries, reversible conversions, source-normal adapters, texture bindings, Bump revision distinction, diagnostics, camera profiles, deterministic orbit, excluded aliases, and browser errors.

