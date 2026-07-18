# CubZ TVA animation audit

Status: **animation decoding is feasible and verified; main-app integration remains intentionally pending.**

This pass audits the actual `CubeRot.tva` / `CubeOpen.tva` files and the `CubZ.cpp` playback chain. It does not edit the live CubZ UI, procedural state machine, race scenes, shared archive content, or CSS.

## Result

| Asset | SHA-256 | Exact timing | Materially changing transforms |
|---|---|---:|---|
| `CubeRot.tva` | `8ADBA22A...AE7B4B` | master `0..220` at 30 fps; seven selection ranges of `28/30 = 0.9333s` | quaternion only on `GlobalCube` (node 1) |
| `CubeOpen.tva` | `CCB1ED3B...0CA27B` | unnamed `0..50` at 30 fps (`1.6667s`) | quaternion only on `Right08`, `Top08`, `Left08` |

Both files are healthy TrueVision3D 6.5 chunk streams with the same 16-byte magic/header family and a valid `MSTR -> ASTA -> MAG3 -> MHI2 -> MAMD* -> MANS -> MANI* -> AEND` sequence. The animation layer is no longer a research blocker.

The decoder proves a useful architectural fact: the seven selection clips rotate the complete `GlobalCube`, while the open clip animates only three panels belonging to `Box08`. In combination with the source's actor swap, this strongly indicates that the selection rotations bring a chosen cell to one canonical Box08 opening pose. The exact mapping from click index to named `Box01..Box08` is not directly encoded, so that last mapping remains an inference.

## Decoded TVA structure

- `MHI2`: 464-byte hierarchy records. The three leading integers form a consistent parent / first-child / next-sibling tree, followed by a 256-byte node name. The integers and names are exact; the relationship labels are a structural interpretation.
- `MANS`: 280-byte range records. Each stores a 256-byte name plus source animation, start frame, end frame, 30 fps, one retained raw field, and the MANI-track count.
- `MANI`: a 24-byte counted header followed by float32 position `[frame,x,y,z]`, rotation `[frame,x,y,z,w]`, and scale `[frame,x,y,z]` arrays.
- The rotation component order is labeled `x,y,z,w` because identity keys are `[0,0,0,~1]` and every decoded key has unit norm within `1e-6`. The exact stored float order is preserved in JSON.
- The post-`AEND` 980-byte tail is hashed and retained as unparsed evidence; it is not required to recover transform timing.

`CubeRot.tva` contains 70 hierarchy nodes and 46 MANI tracks. Export optimization leaves 211 keys on `GlobalCube`, but every selection-range start/end has an exact decoded key.

| Animation ID | Name | Source frames | Duration |
|---:|---|---:|---:|
| 0 | `Animation0` | `0..220` | 7.3333s |
| 1 | `Animation1` | `0..28` | 0.9333s |
| 2 | `Animation2` | `30..58` | 0.9333s |
| 3 | `Animation3` | `60..88` | 0.9333s |
| 4 | `Animation4` | `90..118` | 0.9333s |
| 5 | `Animation5` | `120..148` | 0.9333s |
| 6 | `Animation6` | `150..178` | 0.9333s |
| 7 | `Animation7` | `180..208` | 0.9333s |

`CubeOpen.tva` contains 69 hierarchy nodes and 45 MANI tracks. Only nodes 34 (`Right08`), 36 (`Top08`), and 38 (`Left08`) materially change; each has 51 exact quaternion keys at frames `0..50`. Their position and scale tracks are constant.

## Source playback chain

The distinct enhanced source, `Yanik C++ BCKUP/CubZ (2).cpp`, supplies the complete surviving menu transition:

1. Load separate non-looping `CubeRot` and `CubeOpen` actors.
2. Selection 0 skips rotation and opens immediately.
3. Selections 1-7 call `SetAnimationID(iCubeIndex)` and play `CubeRot` at `+1`.
4. After `IsAnimationFinished()`, hide the rotation actor, enable the open actor, and play `CubeOpen` at `+1`.
5. On completion, activate menu level `iSelectedCube + 1`; the source creates eight levels with four internal buttons per level.
6. Close plays `CubeOpen` at `-1`, swaps actors back, then calls `BackRotate()`.

Five other `CubZ.cpp` files are byte-identical baseline copies (`SHA-256 6EC6CC8A...C56BA`). `CubZ (2).cpp` is the one distinct enhanced revision (`1337AE60...28F6`), adding texture/position propagation, opening completion, menu activation, time resets, and main-button enable/disable behavior. De-duplicating by path would overstate the evidence; there are only two unique source revisions.

## Binary-confirmed pitfall

`BackRotate()` is inconsistent with the decoded range table:

- Forward selection uses IDs `1..7`.
- The binary contains only IDs `0..7`.
- Reverse selection uses `iSelectedCube + 1`, producing IDs `2..8`.

Therefore selections 1-6 reverse a different clip than they played forward, and selection 7 requests nonexistent ID 8. The surviving TV3D header does not specify invalid-ID behavior. This should be treated as an archive defect to correct, not as intended animation semantics to imitate.

The alternate `Media/CubXActor` family (`CubXGet2..8`, `CubXRot1..7`, `CubXOpen`, `CubeOpensolo`) contains 16 distinct TVA hashes and no byte alias of either target. It belongs to the separate `CLCubX` implementation that loads individual selection actors; it must not be silently substituted or merged into this target pair.

## Exact versus inferred boundary

Exact decoded evidence:

- bytes, hashes, chunk directory, hierarchy integers/names;
- animation ranges, frame bounds, 30 fps, source IDs;
- source-order position/quaternion/scale keys;
- the four materially changing nodes listed above;
- the C++ forward/open/close/reverse state calls and eight-by-four menu structure.

Not yet exact:

- TV3D-to-Three.js handedness/quaternion mapping;
- TV3D interpolation/easing between stored keys;
- the exact click-index-to-`BoxNN` semantic label mapping;
- the visual result of actor-swap continuity;
- category labels/actions for the 32 internal buttons;
- geometry/material conversion from `MAG3` / `MAMD`.

The safe integration path is to use the decoded frame/quaternion keys behind the already working state flow, then visually verify coordinate conversion, all seven forward clips, open/close continuity, and a corrected same-range reverse path before changing the UI's fidelity claim.

## Reusable sampler and repaired reverse

`src/cubz-tva-animation.ts` is the isolated runtime layer built from this evidence. It exposes exact 30 fps sampling for all seven selection ranges and the three animated `CubeOpen` panels, forward/reverse cursors, and deterministic playback objects. Sampling an authored integer frame returns the original decoded float32 quaternion unchanged. Fractional frames use normalized shortest-arc quaternion SLERP; that interpolation is deterministic and conventional, but remains explicitly marked as an inference because the archived TV3D interpolation routine has not been recovered.

Reverse playback deliberately uses the same range that forward playback selected. For example, selection 4 plays `Animation4` (`90..118`) forward and then `118..90` in reverse. The runtime also reports that the archived code would have requested ID 5 for this case—and invalid ID 8 for selection 7—so the correction remains inspectable rather than silently rewriting the historical evidence.

The layer retains source-order TV3D quaternion components and does not claim a browser coordinate conversion. It is ready to drive the existing CubZ state machine after the actor-to-browser handedness and actor-swap continuity are visually established.

## Reproducibility

```text
node tools/convert-cubz-tva.mjs
node tools/verify-cubz-tva.mjs
node scripts/qa-cubz-tva-animation.mjs http://127.0.0.1:4189
```

Generated evidence: `src/legacy/cubz-tva-animations.json`.
