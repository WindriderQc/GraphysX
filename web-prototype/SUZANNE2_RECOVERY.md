# Suzanne 2 isolated recovery

Status: authored 40×40 layout, active 2017 runtime semantics, XML attachments, source X-mesh geometry/material groups, textures, isolated preview, and deterministic QA recovered.

## Evidence boundary

No screenshot named for Suzanne 2 exists in the audited workspace or the external `Tech` archive. `Suzanne1.png` is not used as presentation evidence for this level. The default candidate follows the source pair that actually loads Suzanne 2:

- `Scene3D/GamePlayScreen.cpp` (level declaration, host setup, player, rings, camera, win check)
- `GraphysX_1/Scene.cpp::BuildASCIIScene` (ASCII symbol builder)
- `StockRoom/Suzanne2.ASCII`
- `StockRoom/Suzanne2.xml`

The two ASCII copies are byte-identical (1,678 bytes, SHA-256 `164693e26e1b2786aa2f2e3c506a77ba2a4362b74766945332414a1522212c69`). The two XML copies are byte-identical (1,447 bytes, SHA-256 `70140ab60681df7281f9f23a65079edb2fe9ed80f037ea3963939fcf5e6a1b52`). The later July `SceneEditor.cpp` bindings are recorded as branch drift and are not the preview default.

## Exact authored/runtime inventory

| Item | Exact count / value |
| --- | ---: |
| Grid | 40 × 40 |
| `#` / `Z` / `z` wall cubes | 192 / 65 / 56 |
| `s` effect wall cubes | 2 |
| Total ASCII collision cubes | 315 |
| Rings | 15 |
| Chain assemblies / rigid parts | 3 / 9 |
| Pistons / bodies including hidden triggers | 3 / 9 |
| Lap posts / gate segments | 4 / 2 |
| Effect cells / emitters / total capacity | 2 / 4 / 256 |
| CubX actor anchors | 2 |
| XML objects | 3 |
| Player spawn | `[10, 0.5, 6]` (the loader does not half-center `@`) |
| Finish endpoints | `[14.5, 0.5, 3.5]` → `[14.5, 0.5, 9.5]` |
| Halfway endpoints | `[20.5, 0.5, 25.5]` → `[20.5, 0.5, 33.5]` |
| Authored rings / implemented win threshold | 15 / 2 |
| Lap victory target | none in active `GameplayScreen::update()` |

The active player is `ZombieKiller`: exact `SuperCage.x` geometry at source scale `0.3`, an inner 0.225-radius sphere rotated -90° X with `FireArrow800.jpg`, and a 0.3-radius physics sphere. This is not silently replaced with a later generic BallZ skin.

## XML attachments and converted geometry

- Airplane: `Airplane/Airplane.x`, position `[-5, 2, 15]`, scale 1, one mesh, 29,298 vertices, 9,766 triangles, seven source material groups. All six referenced BMP textures are preserved.
- Boned gate: `BonedGate.x`, position `[0, 5, 15]`, scale 1, one mesh, 96 vertices, 188 triangles. Its X file has nine 200-key tracks at 24 ticks/second for `Armature`, `Armature_Bone`, and `GateBlock`; the XML runtime loads a mesh and does not establish animation playback, so the preview keeps the static bind pose.
- Billboard: legacy name `Suzanne1.Magician`, position `[40, 0, 40]`, scale `[8, 6, 0]`, `Zack.jpg`.
- Player cage: `SuperCage.x`, one mesh, 29,024 vertices, 58,128 triangles, exact source material.

## Isolated integration API

`Suzanne2AsciiEnvironment` is exported from `src/suzanne2-ascii-environment.ts`.

```ts
const environment = new Suzanne2AsciiEnvironment({
  showCubxAnchors: false,
  showPlayer: true,
  showXmlAttachments: true
});

scene.add(environment.group);
await environment.ready;
environment.update(deltaSeconds);
environment.setRingVisible(index, false);
environment.resetRings();
environment.setPistonActivation(index, 0.0 /* through 1.0 */);
environment.setXmlAttachmentsVisible(false);
environment.setCubxAnchorsVisible(true);
environment.setPlayerVisible(false);
const state = environment.getState();
environment.dispose();
```

Exact converter data is available as `Suzanne2AsciiEnvironment.data` and in `src/legacy/suzanne2-ascii-scene.json`. The isolated browser entry is `suzanne2-ascii-preview.html`. It exposes `render_game_to_text()`, deterministic `advanceTime(ms)`, and `window.__SUZANNE2_ASCII_DEBUG__`.

## Intentionally isolated or unresolved

- `Voie Lactee`, the second `AirplaneLP.TVM` spline flyer, ten unseeded random humans, the test cone, and fire are `GamePlayScreen` host-globals, not Suzanne 2-authored content. They are documented but excluded from this isolated candidate.
- The `C` cells prove two `createCubX` calls and y=5.5 actor positions. Exact CubX animation/menu state is not encoded here, so only opt-in evidence anchors are exposed.
- Particle counts, colors, texture, power, and constructor parameters survive. A deterministic seed does not, so preview particle positions are explicitly a QA-friendly visualization bridge.
- The active source advances after exactly two pickups despite authoring fifteen rings. Both values remain visible; the isolated module does not invent a corrected rule.
- Exact screenshot-era sky, exposure, and framing cannot be claimed without a Suzanne 2 image.

## Verification

```powershell
node tools/convert-suzanne2-ascii.mjs
npx tsc --noEmit
node scripts/qa-suzanne2-ascii.mjs http://127.0.0.1:4187
```

The deterministic browser QA asserts 25 layout, source-rule, layer, animation-rate, and API invariants and captures three screenshots under `output/playwright/suzanne2-ascii/`.
