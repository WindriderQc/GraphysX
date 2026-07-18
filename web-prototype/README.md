# GraphysX Web Prototype

Minimal web spike to bring BallZ and the GraphysX creative harness back as a browser runtime.

## Stack

- `Node.js`
- `Vite`
- `TypeScript`
- `Three.js`
- `cannon-es`

## What this proves

- A clean state-driven loop works well on the web.
- The legacy BallZ flow can map to browser scenes without native engine dependencies.
- The old GraphysX engine harness can return as web-native modules for physics, particles, shaders, math visuals, scene tools, and game modes.
- `window.__GRAPHYSX__` now proves an agent-native 3D engine surface: a persistent multi-level editor API with region edits, discoverable semantic tiles, fire/ice force fields, ASCII adapters, and the editable **Funny Zigzagger** showcase; plus one-call starter worlds, the Physics Sketchbook, visible agent participants, semantic archive textures, primitives/models/splines/lights, static/dynamic/kinematic physics, observable impulse interactions, direct 3D selection and transform handles, JSON/XML world files, queries, atomic transactions, actor-aware revision guards, history, undo, deterministic time, and snapshots. `window.__GRAPHYSX_AGENT_BRIDGE__` exposes the same surface as discoverable async tools and a same-origin message protocol; `npm run agent:stdio` makes it available to an external LLM tool host over JSON Lines.
- BallZ's core fantasy is readable in the browser: a controllable inner ball rolling a transparent outer shell.

## Implemented flow

- `Menu`
- `Race Select`
- `Gameplay`
- `After Race`

Gameplay includes:

- keyboard movement through the inner controller ball
- transparent/grid outer shell
- rigid-body physics through `cannon-es`
- ring pickups
- particle bursts
- half-lap gate
- finish gate
- timer
- per-race local records
- archive champion records
- animated rotating cube menu
- moving rotators, pistons, and elevator-style hazards

## Race Maps

- `Green Grid Run`: classic grid corridor inspired by `GridXL.bmp`, `GreenGrid.png`, `Damier.jpg`, and ring logic.
- `Rotator Cube Works`: machine-room race inspired by `Suzanne1.RotatorCube.x`, `RotatorUV.png`, and box textures.
- `Piston Gateworks`: piston and elevator race inspired by `Suzanne1.Piston*`, `Gate1.x`, and `World1Elevator1.TVM`.
- `Skybox Spiral`: atmospheric race inspired by skyboxes, heightmaps, airplane spline ideas, and galaxy experiments.

## Run

```bash
npm install
npm run dev
```

For an external agent process, keep the development server running and start the persistent JSON-lines adapter in another terminal:

```bash
npm run agent:stdio -- --url http://127.0.0.1:5173/
```

## Build

```bash
npm run build
```

## Legacy mapping

- `Scene3D/MainMenuScreen.cpp` -> menu panel
- `Scene3D/SelectRaceScreen.cpp` -> race cards
- `Scene3D/GamePlayScreen.cpp` -> race scene loop
- `Scene3D/AfterRaceScreen.cpp` -> completion panel
- `GraphysX_1/Scene.cpp` -> live named-level/ASCII editor bridge and future freeform scene builder
- `Scene3D/Anneaux.cpp` -> ring system behavior
- `Scene3D/LapChecker.h` -> lap trigger behavior
- `Archive/bckup/BallZ2015.bckup/CLBallZ.cpp` -> inner ball plus outer shell concept
- `Archive/bckup/BallZ2015.bckup/Media/Suzanne1.xml` -> moving race machinery inspiration

See `AGENT_WORLD_API.md` for the live World API v2 contract and examples.

See `HARNESS.md` for the revival direction.

See `ARCHIVE_ANALYSIS.md` for concrete findings from the old source and media archives.

Optional tools such as Blender, ffmpeg, ImageMagick, Rapier, and live-tuning GUIs are documented in `HARNESS.md`.

The engine path now continues from the stable shared contract rather than exposing low-level Three.js objects. Useful next increments should be driven by concrete agent experiments: small sensors and measurements, a few friendly conditional triggers, reusable in-world 2D instruments, and richer collaboration receipts—not another parallel scene format.
