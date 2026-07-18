# GraphysX / BallZ Archive Analysis

This file captures concrete findings from the old archives that should drive the web rebuild.

## BallZ Core

Sources:

- `Archive/bckup/BallZ2015.bckup/CLBallZ.cpp`
- `GraphysX_1/ZombieKiller.cpp`
- `Scene3D/GamePlayScreen.cpp`

Findings:

- BallZ had two visible parts: `BallZCtrl` / `BallZInside` and `BallZShell` / `ZombieKiller`.
- The old controller ball was not tiny. In `CLBallZ.cpp`, `clCtrl` used `fRadius * 0.85`.
- In `GamePlayScreen.cpp`, the later `ZombieKiller` version used `0.75 * zkSize` for the inside ball.
- The outside body owned physics collision. The inside ball was visual/control orientation.
- The useful modern interpretation is: inner ball, outer shell, and a fluid/control-transfer layer between them.
- Future upgrades should modify the fluid/control-transfer properties: grip, lag, damping, torque multiplier, bounce, chaos, inversion, air control.

## Control Model

Sources:

- `GraphysX_1/ZombieKiller.cpp`
- `Archive/bckup/BallZ2015.bckup/CLBallZ.cpp`
- `Scene3D/GamePlayScreen.cpp`

Findings:

- Old control was torque-oriented, not simple position steering.
- `ZombieKiller` built torque from the ball position toward `mouse3Dpos`.
- Mouse button controlled throttle buildup/decay.
- `Space` set `bJumping`; actual jump was applied in physics when `bAllowJumps` was true.
- `bAllowJumps` was restored by ground contact callbacks.

Web port decisions:

- Keep keyboard movement for prototyping.
- Add mouse drag camera immediately.
- Later: add mouse-directed torque/throttle as a selectable classic control mode.
- Keep jump gated by grounded state, not by exact zero vertical velocity.

## Camera

Sources:

- `Archive/bckup/BallZ2015.bckup/Camera.cpp`
- `GraphysX_1/Camera.cpp`
- `Scene3D/GamePlayScreen.cpp`

Findings:

- There were two camera modes: chase/follow and keyboard/mouse free camera.
- Mouse deltas adjusted camera angles.
- Camera angle X was clamped around +/- 1.3 radians.
- The chase camera used a hidden camera aim mesh to avoid inheriting target rotation.

Web port decisions:

- Use a chase/orbit camera around BallZ.
- Mouse drag changes yaw/pitch.
- Wheel changes camera distance.
- Later: add classic `C` toggle for free camera versus chase camera.

## Sky And Atmosphere

Sources:

- `GraphysX_1/Sky.cpp`
- `Archive/bckup/BallZ2015.bckup/Atmosphere.cpp`
- `Yanik C++ BCKUP/BallZ 2011/Release/Media/Sky`

Findings:

- The archive includes real skyboxes: `ClearBlue`, `ClearNight`, `SkyX`, `Winter`, `LostValley`.
- Old code supported day/night blending, sun billboard, directional sun/moon lights, and atmosphere render.
- `LostValley` exists mostly as BMP and can be converted later.

Web port decisions:

- Use `CubeTextureLoader` now with copied JPG skyboxes.
- Keep day/night blending as a later atmosphere module.
- Convert `LostValley` BMPs later through the asset pipeline.

## Race / Lap / Finish

Sources:

- `Scene3D/LapChecker.h`
- `Scene3D/GamePlayScreen.cpp`
- `GraphysX_1/Scene.cpp`

Findings:

- Finish and half-lap were not simple Z thresholds.
- `LapChecker` built hidden 3D box zones from two points for finish and halfway.
- It tracked ghost/last-lap splines.
- `GraphysX_1/Scene.cpp` ASCII maps encoded player position, rings, finish start/end, half start/end, walls, chains, pistons, and particles.

Web port decisions:

- Use proper gate zones with width/depth instead of hard-coded centerline thresholds.
- Later: port ASCII/XML scene definitions into JSON and generate race geometry from that.

## Recovered Media Used Now

Copied into `public/assets`:

- `Ball/FireArrow800.JPG`
- `Ball/GridXL.bmp`
- `GreenGrid.png`
- `Damier.jpg`
- `AbstractCubes.jpg`
- `xboxes01.jpg`
- `xboxes02.jpg`
- `marble09.jpg`
- `rock2.bmp`
- `Medrust3.png`
- `metal13.jpg`
- `Sky/ClearBlue/*`
- `Sky/ClearNight/*`
- `Sky/SkyX/*`
- `Sky/Winter/*`

## Remaining High-Value Archive Targets

- Convert `SuperCage.x`, `BallShell.tvm`, and `BallCtrl.tvm` into GLB.
- Convert `Suzanne1.*` moving parts into GLB.
- Port XML scene files: `Suzanne1.xml`, `MyWorld.xml`, `TestWorld.xml`.
- Port ASCII scene parser from `GraphysX_1/Scene.cpp`.
- Rebuild `CLAnneaux` ring particles and proximity behavior.
- Rebuild `CLSky` day/night blending and sun/moon lights.
- Rebuild editor screens: race select, map editor, math game, high-score form.
