# GraphysX Web Harness

This prototype is no longer only a BallZ sketch. It is the seed of a browser-native GraphysX creative harness.

## Harness Goals

- 3D and 2D visuals in one runtime.
- Real-time physics experiments.
- Particle and visual-effect generators.
- BallZ game loop and arcade mechanics.
- Shader, texture, material, and lighting experiments.
- Scene-builder and editor experiments.
- Math-game and graph-visualization experiments.
- Media recovery from the old archive folders.

## Current Modules

- `src/engine/input-controller.ts`: keyboard input surface.
- `src/engine/physics-world.ts`: `cannon-es` wrapper for rigid-body physics.
- `src/engine/particle-emitter.ts`: reusable burst particle system.
- `src/race-definitions.ts`: archive-inspired race maps, moving parts, champions, and revival backlog.
- `src/scoreboard.ts`: browser-local per-race records seeded by archive champions.
- `src/race-scene.ts`: BallZ race renderer, shell controller, moving hazards, rings, gates, and menu cube.
- `src/prototype-app.ts`: DOM menu, race selection, HUD, after-race scoreboard, and backlog panel.

## BallZ Core

BallZ is not just a ball rolling on a track.

- The player is an inner controller ball that nearly fills the shell.
- Archive ratios were roughly `0.75x` to `0.85x` shell radius, not a tiny marble inside a big shell.
- The world collision body is the larger transparent shell around it.
- The gap between inner ball and shell is treated as a fluid/control-transfer layer.
- Future modifiers can increase, reduce, invert, delay, damp, or distort how much control transfers through that fluid.
- Race scoring is per map: time, rings, target time, archive champion, and local best.

## Current Race Set

- `Green Grid Run`: corridor race using grid, damier, gates, and ring collection.
- `Rotator Cube Works`: rotating machine-room race inspired by Suzanne rotators and box textures.
- `Piston Gateworks`: piston, trigger, gate, and elevator-style race.
- `Skybox Spiral`: atmospheric skybox/heightmap/airplane-spline inspired race.

## Revival Backlog

- Scene builder: ASCII maps, XML scene files, trigger zones, and object-library import.
- BallZ modifiers: shell grip, inner-ball weight, air control, friction, bounce, and chaos upgrades.
- Physics lab: Newton-style joints, hinges, sliders, push plates, chains, elevators, pistons, and dry rolling friction.
- Particle lab: ring bursts, flames, smoke, billboards, texture cycles, explosion presets, and emitter editor.
- Shader lab: per-pixel lighting, bump mapping, projection, haze, water refraction/reflection, and post effects.
- Sky and atmosphere: day/night skyboxes, LostValley, clear night, sun pass, galaxy objects, and heightmap terrain.
- 3D toys: CubX actor, airplane spline follow, billboards, dynamic mesh tests, alphabet TVM meshes, and rotator assets.
- Game shells: main menu, race select, after-race, high scores, math-game screen, editor screen, and arcade HUD.
- Media recovery: convert TVM/X/3DS/OBJ assets to GLB, normalize textures, and tag source inspiration per scene.

See `ARCHIVE_ANALYSIS.md` for the deeper source/media findings that support these decisions.

## Technology Direction

Use web-first Three.js for now.

Why:
- Immediate iteration in the browser.
- Easy integration with math, visualization, shaders, particles, and game tooling.
- No dependency on obsolete native renderer stacks.

Keep options open:
- Add `cannon-es` while the physics needs are simple.
- Move to Rapier later if we need stronger collision/constraint performance.
- Add React or a small UI framework only when the tool panels become complex enough to justify it.

## Optional Toolchain

These are not required for the current prototype. Add them only when the workflow needs them.

- `Blender`: clean old 3D assets, rebuild materials, export browser-ready `.glb` files.
- `ffmpeg`: convert old videos, audio clips, sprite captures, and preview recordings.
- `ImageMagick`: batch-clean textures, sprites, masks, thumbnails, and archive screenshots.
- `Rapier`: replace or supplement `cannon-es` when physics needs stronger collision, constraints, or performance.
- `lil-gui` or `tweakpane`: live-tune particles, physics constants, camera settings, shaders, and scene parameters.

## Next Revival Targets

1. Convert one real legacy GraphysX scene file to JSON.
2. Convert one old mesh group such as `Suzanne1` or `Gate1` to browser-ready `.glb`.
3. Add actual modifier/upgrades for the BallZ inner-ball-to-shell control model.
4. Recreate one particle or shader experiment from the archive.
5. Recreate the math-game screen as a 3D graph visualizer.
