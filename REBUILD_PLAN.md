# GraphysX Rebuild Plan (Modern, Simple, Performant)

## 1) What We Have Today

This repo contains multiple generations:

- `GraphysX_1/`: native C++ engine core (TV3D + Newton + FMOD + CEGUI).
- `Scene3D/`: full game app flow (menu/gameplay/editor/math/after-race) on top of GraphysX.
- `GraphysX/ArduinoGUI/ArduinoGUI/`: C# WinForms screen system and level-selection logic using managed wrappers.
- `GfxNet/`: C++/CLI bridge layer (`Tv`, `SceneNET`, `EntityNET`) between native engine and .NET app code.
- `Archive/bckup/BallZ2015.bckup/Media/`: largest coherent media bundle we can reuse now.

## 2) Reusable Code (Keep Concepts, Re-implement Cleanly)

### A. Screen/state architecture (strong reusable design)

Use this as the baseline game flow:

- `Scene3D/ScreenList.cpp`
- `Scene3D/IScreen3D.h`
- `Scene3D/MainMenuScreen.cpp`
- `Scene3D/SelectRaceScreen.cpp`
- `Scene3D/GamePlayScreen.cpp`
- `Scene3D/AfterRaceScreen.cpp`
- `Scene3D/MathGameScreen.cpp`
- `Scene3D/EditorScreen.cpp`

Why reuse:
- Clear state-machine transitions.
- Good separation of per-screen `build/onEntry/update/render/onExit`.

### B. Level/gameplay model

- `Scene3D/BallZLevel.h`
- `GraphysX/ArduinoGUI/ArduinoGUI/Level.cs`
- `GraphysX/ArduinoGUI/ArduinoGUI/LevelList.cs`

Why reuse:
- Simple data contract for level content and progression.
- Serializable persistent scores and level metadata.

### C. Core gameplay rules

- ASCII map tile semantics and builder logic: `GraphysX_1/Scene.cpp` (`BuildASCIIScene`).
- Lap zone completion logic: `Scene3D/LapChecker.h`.
- Ring pickup/proximity loop: `Scene3D/Anneaux.cpp`.

Why reuse:
- Rules are game-defining and portable.

### D. Scene/Entity schema

- `GraphysX_1/STEntityParam.h`
- `GraphysX_1/STSceneParam.h`
- `GfxNet/SceneNET.h`
- `GfxNet/EntityNET.h`

Why reuse:
- Existing contract for map/object exchange can become JSON schema.

## 3) What Not To Carry Forward

- TV3D renderer, Newton, CEGUI, C++/CLI bridge, and mixed native/managed runtime coupling.
- Hardcoded relative paths to `..\\StockRoom\\...`.
- Manual memory-heavy patterns (`new/delete`, global statics, cache lifetime ambiguity).
- Per-screen direct file I/O and UI toolkit lock-in.

## 4) Reconstructed Storyboard (from existing code)

Primary flow:

1. Main Menu
2. Select Race
3. Gameplay
4. After Race
5. Back to Gameplay (next level) or Main Menu

Secondary flows:

- Main Menu -> Editor
- Main Menu -> Math Game -> Main Menu

Derived from:

- `Scene3D/MainMenuScreen.cpp`
- `Scene3D/SelectRaceScreen.cpp`
- `Scene3D/GamePlayScreen.cpp`
- `Scene3D/AfterRaceScreen.cpp`
- `Scene3D/MathGameScreen.cpp`
- `Scene3D/EditorScreen.cpp`

## 5) Media/Content Reality Check

### Available now (good salvage source)

`Archive/bckup/BallZ2015.bckup/Media/` has:
- 70 `.TVM` meshes
- 14 `.x` meshes
- 4 `.obj`
- 7 `.xml` scene/spline files
- 4 `.dds`
- 14 `.png`
- 34 `.bmp`
- 48 `.jpg`

Includes key files referenced by code:
- `Suzanne1.xml`
- `Suzanne1.obj`
- `Spline.xml`
- `Airplane/AirplaneLP.TVM`
- shader files under `Media/shaders/`

### Missing or incomplete for a direct build

Not found in repo:
- GUI layouts/schemes (`*.layout`, `*.scheme`) referenced by screen code.
- audio files used by gameplay (`intro.mp3`, `Piano.mp3`, `coin.wav`).
- several expected level files (`Suzanne1.ASCII`, `Suzanne2.ASCII`, `Suzanne2.xml`, `Level1_base.ASCII`, etc.).

Implication:
- We can start with core mechanics + available models/textures.
- Menus, missing levels, and audio need recreation/replacement.

## 6) Active Direction

### Current spike in repo: Web prototype

There is now a working web spike under `web-prototype/` using:
- `Node.js`
- `Vite`
- `TypeScript`
- `Three.js`

What it proves:
- The legacy BallZ screen flow maps cleanly to a browser runtime.
- We can prototype gameplay without rebuilding the old native engine stack.
- A web-first vertical slice is practical before committing to full asset conversion.
- The harness can host reusable physics, particle, shader, math-visual, scene-editor, and game-mode systems.

### Alternative still worth keeping in reserve: Godot 4 (C#)

Godot remains a valid fallback if later requirements push harder on:
- built-in editor tooling
- native export priority over browser delivery
- heavier scene authoring workflows

## 7) Target Architecture for Rebuild

- `GameStateMachine`: MainMenu, SelectRace, Gameplay, AfterRace, Editor, MathGame, CreativeLab.
- `EngineHarness`: shared renderer, input, physics, particles, camera, shaders, content loading.
- `LevelDefinition` (JSON): map source, floor material, skybox, ring path, NPC count, screenshot.
- `AsciiMapBuilder`: ports tile semantics from legacy `BuildASCIIScene`.
- `RingSystem`: proximity check + score + FX.
- `LapSystem`: half/finish gate check + lap timer.
- `PlayerController`: throttle/jump/shoot + camera follow.
- `ParticleLab`: emitters, fields, forces, and visual-effect experiments.
- `ShaderLab`: shader/material/texture experiments recovered from the archives.
- `MathLab`: 3D graph/math-game experiments recovered from `MathGameScreen`.
- `ContentRegistry`: asset lookup, no hardcoded absolute/relative legacy paths.

## 8) Launch Plan (Execution Phases)

## Phase 0 - Foundation (1-2 days)
- Create prototype repo/folder: `web-prototype/`.
- Setup Vite + TypeScript + Three.js.
- Add folder standards: `src/`, `assets/`, `data/levels/`.
- Define JSON schema for `LevelDefinition`.

Exit criteria:
- App boots with empty Main Menu scene and state machine.

## Phase 1 - Vertical Slice (3-5 days)
- Implement screen transitions:
  - MainMenu -> SelectRace -> Gameplay -> AfterRace -> MainMenu
- Port one playable level using available data:
  - `Archive/bckup/BallZ2015.bckup/Media/Suzanne1.xml`
  - `Archive/bckup/BallZ2015.bckup/Media/Spline.xml`
- Implement:
  - ball movement (throttle/jump)
  - ring pickups
  - lap completion
  - HUD score + lap time

Exit criteria:
- One complete race loop playable end-to-end.

## Phase 2 - Content Recovery & Tooling (3-7 days)
- Build importer/converter scripts:
  - legacy XML -> new JSON level format
  - optional ring/object extraction from OBJ/XML
- Recreate missing ASCII maps where absent.
- Replace missing audio with temporary pack.
- Rebuild menu UI assets (layout + style).

Exit criteria:
- 2+ levels playable with stable asset loading pipeline.

## Phase 3 - Polish and Performance (3-5 days)
- Optimize update loops and collision layers.
- Add pooling for projectiles/effects.
- Add save system for scores/progression.
- Add automated checks (formatting + basic gameplay smoke test).

Exit criteria:
- Stable alpha with reproducible build and documented content pipeline.

## 9) Immediate Next Actions (Start Now)

1. Freeze the spike target as "web-first BallZ core race loop".
2. Import one real level dataset starting with `Suzanne1`.
3. Convert legacy scene data into JSON the prototype can load.
4. Treat missing GUI/audio files as recreate-or-replace, not blockers.

---

If you want, next step I can do in this repo is generate:
- a concrete migration checklist (`MIGRATION_CHECKLIST.md`) mapped file-by-file from legacy to new modules,
- and a starter JSON schema for levels based on your existing `STSceneParam`/`Level` models.
