# Forgotten Features — recovered from archive CODE (not assets)

Sweep of 2026-07-16, triggered by "the ghost replay was implemented somewhere."
Correct: it was. The earlier sweeps indexed assets and screens; these live in code
details and design comments that only keyword-hunting found.

## 1. Ghost / last-lap spline — `Scene3D/LapChecker.h`
The real implementation:
- Ball position sampled every `SAMPLING_FRAMES = 100` frames into `clSplineGhost` (hidden while recording)
- Lap detection via two invisible bounding-box zones (finish + half-lap), built from two points each
- On lap completion: `clSplineLastLap->setSpline(clSplineGhost->getSpline())` — **your last lap becomes a visible 3D spline in the world**, recorder clears
- Web port: record `playerGroup.position` on a distance/time threshold; on finish draw the path as a glowing line; persist best-lap path per race (localStorage) and optionally animate a translucent shell along it ("race your ghost" — the natural modern upgrade)

## 2. The original game-mode design — `Scene3D/GamePlayScreen.h` (TODO block)
The rebuild only implements mode 1 of THREE designed modes:
- "Get All Rings → Best Time" ← current web gameplay
- "**5/10 Laps → Best time + bonus for rings (−10 sec)**" — multi-lap races where rings subtract time (`Level.cs` confirms: `nbrTour = 3`, races were 3 laps!)
- "**2/5 minutes race → Most Laps + ring fraction bonus**" — time-attack: most laps within a fixed clock

## 3. Ball selection screen — same TODO block — DONE 2026-07-17
"Select Ball Screen — Fire Ball, Ice Ball, etc... avec different controle values → friction, speed"
- Different balls with different physics (friction/speed/control)
- **BallFire.TVM is already decoded** — it was the Fire Ball skin all along
- Web port: the CubZ BallZ menu now exposes persistent Fire / Classic / Revival choices. Fire uses recovered BallFire geometry with Revival physics; Classic restores the recovered 2015 gravity/brake feel; Revival keeps the modern web handling. Structured state and the `B` cycle expose the same three selections.

## 4. The rescue plane — same TODO block
"Plane qui get Ball when falling... Balljoint + slider under plane. Plane grabs ball... ball slides along slider upon plane speed."
- The airplane was designed to CATCH falling balls and carry them back — not decoration
- Web port: when ball falls out of bounds, the spline airplane swoops, ball attaches under it, gets carried to the track and dropped — replaces the instant respawn

## 5. Ring classes — same TODO block
"Ring Class - STD rings + Hard to Get Ring" — special hard-to-reach rings worth more (fits the −10s lap-bonus economy)

## 6. Bullet system — `GraphysX_1/Bullet.cpp/.h`
Full projectile class: physics body (CLDynamicMesh), direction, lifetime, speed,
static bullet count. The "shooting" backlog item has reference code.

## 7. Screenshot key — `Scene3D/IScreenApp.cpp:150`
Saved `screenshot_<timestamp>.png` from a key press. Trivial web port
(`renderer.domElement.toDataURL` on P key).

## 8. Pause — `Scene3D/App.cpp` / engine files
Pause existed in the app loop. Web has no pause key.

## 9. AtmelCubx is a full second GAME, not just hardware glue
`AtmelCubx/AtmelCubx/` contains a complete game codebase: **CarScene.cpp / CarXGUI.cpp**
(the original car game the PisteOvale belongs to), `Ball/BallZ/Anneau` (its own ring +
ball logic), `Area.cpp` (trigger areas), `Atmosphere`, `Animation`, `BlocNote`,
`CLExplosion` reference (explosion effect class). Only PisteOvale.tvm was mined
from it so far. Worth a dedicated code sweep for the car handling values
(the authentic Impreza feel likely lives here).

The CubX main constellation is now live in the web lab with exact recovered source coordinates and MenuManager tags: System/Tools/Earth/Grid are level 0, Arrow is ALLMENU 666, and the Sun remains the FlightX launch. Tools reaches the recovered one-car Impreza preview; System and Earth deliberately remain inspect-only where their source destination is unavailable/empty. Fleche uses decoded geometry, while Box/Earth/Sphere and original icon textures remain explicit browser adapters pending conversion.

## Suggested order
1. Ghost spline (last-lap path + best-lap ghost) — the one that was asked for
2. Multi-lap mode with ring time-bonus (3 laps, −10s/ring) — restores the real game loop
3. ~~Ball selector (Fire/Ice/Classic with physics params)~~ — DONE as Fire / Classic / Revival with recovered BallFire/BallShell geometry
4. Rescue plane on fall — replaces respawn, pure charm
5. AtmelCubx CarScene sweep → tune the Impreza with original handling values
6. Bullets → ZombieKiller shooting
7. Screenshot key + pause — quick wins
