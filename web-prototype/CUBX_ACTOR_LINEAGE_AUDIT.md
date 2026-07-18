# CubXActor lineage audit

## Verdict

The 16 files under `Yanik C++ BCKUP/Media/CubXActor` are a complete, byte-distinct actor family that can recover exact geometry, hierarchy, transforms, timing, and the surviving `CLCubX` host flow. They do **not** recover a defensible CubZ click-index-to-`BoxNN` semantic mapping.

The conflict is source-level, not a decoder gap:

- The eight click TVMs spatially overlap closed cells in this exact order: `Box05`, `Box04`, `Box03`, `Box02`, `Box08`, `Box06`, `Box07`, `Box01`.
- The host maps click indices 1-7 to actor files `CubXGet2.tva` through `CubXGet8.tva`.
- Click 8 indexes actor slot 7, but the constructor initializes only slots 0-6 and the render loop also stops at slot 6.
- `CubXOpen.tva` replaces closed `Box05` with a `Cube/Right/Top/Left` panel assembly, while the homologous standalone export calls that assembly `Box08/Right08/Top08/Left08`.

Click index, spatial cell, actor filename, and panel-export label must therefore remain separate evidence fields.

## Asset census and timing

Every TVA contains one unnamed animation range at 30 fps. “Motion end” is the first key of the terminal quaternion plateau; the remaining range is an authored hold.

| Asset | Bytes | SHA-256 prefix | Range | Motion end | Terminal hold | Materially animated node(s) |
|---|---:|---|---:|---:|---:|---|
| `CubXGet2.tva` | 191,814 | `FFC2AF71` | 0..100 | 30 | 70 | `Group01` |
| `CubXGet3.tva` | 213,178 | `FE2E1BBF` | 0..100 | 50 | 50 | `Group01` |
| `CubXGet4.tva` | 192,740 | `11983C32` | 0..100 | 30 | 70 | `Group01` |
| `CubXGet5.tva` | 217,165 | `F79F6336` | 0..100 | 50 | 50 | `Group01` |
| `CubXGet6.tva` | 210,942 | `E5DEB98E` | 0..100 | 50 | 50 | `Group01` |
| `CubXGet7.tva` | 216,932 | `595CEC05` | 0..100 | 50 | 50 | `Group01` |
| `CubXGet8.tva` | 209,818 | `ED98FEB2` | 0..100 | 50 | 50 | `Group01` |
| `CubXRot1.tva` | 167,196 | `BDF3036C` | 0..20 | 20 | 0 | `Group01` |
| `CubXRot2.tva` | 178,343 | `614E2731` | 0..30 | 30 | 0 | `Group01` |
| `CubXRot3.tva` | 168,931 | `58592DC0` | 0..20 | 20 | 0 | `Group01` |
| `CubXRot4.tva` | 179,851 | `7C979641` | 0..30 | 30 | 0 | `Group01` |
| `CubXRot5.tva` | 177,076 | `5DF1A76C` | 0..30 | 30 | 0 | `Group01` |
| `CubXRot6.tva` | 180,163 | `ABCE2475` | 0..30 | 30 | 0 | `Group01` |
| `CubXRot7.tva` | 175,457 | `52EB4AF8` | 0..30 | 30 | 0 | `Group01` |
| `CubXOpen.tva` | 165,449 | `3D86D878` | 0..100 | 50 | 50 | `Right`, `Top`, `Left` |
| `CubeOpensolo.tva` | 19,680 | `A00A952B` | 0..50 | 50 | 0 | `Right08`, `Top08`, `Left08` |

The host explicitly waits until a Get actor reports keyframe `> 99`, explaining why those files retain a 0..100 range after motion stops at frame 30 or 50. At 30 fps, each host-loaded Get lasts 3.333 seconds. `CubXOpen` also lasts 3.333 seconds, with its actual panel motion complete at 1.667 seconds.

`CubXRot1..7` and `CubeOpensolo` have no literal host reference in any surviving C++/header file. The seven Rot assets pair naturally with `Get2..8` by filename and closely matching terminal quaternion, but they must remain classified as unexecuted prototypes rather than host playback.

## Exact click-to-actor flow

The companion click meshes are small 24-vertex / 12-triangle boxes authored directly at the eight cell positions. Their centers match the closed actor cell bounds with less than one source unit of export drift.

| Click | Click TVM center | Spatial `BoxNN` | Selected integer | Actor array slot | Host-loaded actor | Filename label | Result |
|---:|---|---|---:|---:|---|---|---|
| 1 | `(25, 25, -25)` | `Box05` | 1 | 0 | `CubXGet2.tva` | `Box02` | valid |
| 2 | `(24.897, 25.132, 24.984)` | `Box04` | 2 | 1 | `CubXGet3.tva` | `Box03` | valid |
| 3 | `(-25, 25.119, 25.019)` | `Box03` | 3 | 2 | `CubXGet4.tva` | `Box04` | valid |
| 4 | `(-25, 25, -25)` | `Box02` | 4 | 3 | `CubXGet5.tva` | `Box05` | valid |
| 5 | `(24.699, -25, -25.114)` | `Box08` | 5 | 4 | `CubXGet6.tva` | `Box06` | valid |
| 6 | `(25.467, -25, 25)` | `Box06` | 6 | 5 | `CubXGet7.tva` | `Box07` | valid |
| 7 | `(-25.038, -25, 25)` | `Box07` | 7 | 6 | `CubXGet8.tva` | `Box08` | valid |
| 8 | `(-25, -24.864, -25)` | `Box01` | 8 | 7 | none | none | uninitialized access |

The “Filename label” column is only the number embedded in `CubXGetN`; it is not promoted as the clicked cell's semantic name. TV3D handedness and actor-swap continuity have not been visually established, and the source's own indexing is incomplete.

## Geometry and opening lineage

The converter now decodes the actor geometry payload instead of only inventorying chunk tags:

- `MHI2`: exact hierarchy links/names, signed mesh-data index, and initial world/local 4×4 matrices.
- `MAMD`: a 124-byte header, 32-byte position/normal/UV vertices, uint16 triangle indices, uint32 face-material assignments, and uint32 source-vertex remap.
- Every index is range-checked; every MAMD counted payload fills its chunk exactly.

Family geometry totals:

- `CubXGet2..8` and `CubXOpen`: 2,264 vertices, 2,750 triangles, 23 geometry chunks.
- `CubXRot1..7`: 2,148 vertices, 2,688 triangles, 20 geometry chunks. These contain the eight closed boxes plus 12 hinge cylinders.
- `CubeOpensolo`: 140 vertices, 74 triangles, four geometry chunks.
- Companion `CubXMesh.tvm`: 2,264 vertices, 2,750 triangles, 23 material groups, exact bounds `(-37.5,-37.5,-37.5)` to `(37.5,37.5,37.5)`.

`CubXOpen` contains seven closed `Bo_teNN` cells—01, 02, 03, 04, 06, 07, 08—and replaces absent `Bo_te05` with `Cube`, `Right`, `Top`, and `Left`. `CubeOpensolo` calls its assembly `Box08`, `Right08`, `Top08`, and `Left08`.

The three panel geometry payloads are byte-identical between the two opening exports. Their first 51 rotation keys differ by at most `2.0862e-7`, consistent with float32 re-export noise, and `CubXOpen` adds one terminal key at frame 100. This proves a shared panel lineage, but the `Box05` versus `Box08` labels prove that `BoxNN` was not stable across exports.

## Material and host transform

All decoded MAMD faces use embedded material index 0. More importantly, the host replaces actor appearance with its explicit `StdMat`:

- ambient `(0.1, 0.1, 0.1, 1)`;
- diffuse `(0.8, 0.8, 0.8, 1)`;
- specular `(0.2, 0.2, 0.2, 1)`;
- power `20`;
- emissive `(0, 0, 0, 1)`;
- no texture override.

The scene constructs `CLCubX(4, NULL, (-400,500,0), true)`, but both closed-mesh and actor scaling are commented out. The defensible source transform is therefore position `(-400,500,0)` at unit scale; constructor size `4` must not be applied.

## Surviving host defects

Four byte-identical `CLCubX` implementations and four byte-identical `CLCubXScene` implementations survive. They preserve the same defects:

1. The actor loop `for(i=1; i<8; i++)` initializes seven slots with `Get2..8`, but the click host accepts selection 8 and indexes slot 7.
2. The actor render loop is also `i<7`, so only slots 0..6 can render.
3. `DisableCubXButtons()` calls `Enable(true)` just like `EnableCubXButtons()`.
4. `CLOSING`, `DESELECTING`, `FOCUSED`, `OPENED`, and `DEFOCUSING` have no transition behavior, and the internal CubX menu registration block is commented out.
5. Source loads `Media\CubX\...`, but no surviving `Media/CubX` directory exists. The assets are under `Yanik C++ BCKUP/Media/CubXActor`, so recovery requires an explicit path remap.

These defects mean this branch is valuable animation/geometry evidence but not a complete working menu implementation.

## CubZ mapping conclusion

This audit adds exact visual evidence that the earlier CubZ pass lacked, especially button centers, full actor geometry, the canonical opening assembly, and the separate rotation files. It still cannot assign CubZ click index 0-7 to a stable `BoxNN` label without invention because:

- click-number and spatial-box order differ;
- click-number and `GetN` filename order differ;
- click 8 has no valid actor;
- opening exports disagree between `Box05` and `Box08` naming;
- no TV3D handedness or actor-swap visual capture survives.

A future repair may use the exact per-file animation and geometry, but its state should expose all four identities separately: click index, spatial cell, actor file, and opening assembly. Any unified user-facing label is a modern repair decision, not recovered archive fact.

## Reproducibility

```text
node tools/convert-cubx-actor-lineage.mjs
node tools/verify-cubx-actor-lineage.mjs
```

Generated evidence: `src/legacy/cubx-actor-lineage.json`.
