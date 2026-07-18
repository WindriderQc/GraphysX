# GraphysX Archives vs Web Rebuild — Full Revision

Revised 2026-07-05 after the TVM format was cracked and a complete asset census of every archive folder was run. Supersedes the first-pass analysis from 2026-07-04, which underestimated the archive: it only examined `Archive/bckup/BallZ2015.bckup/Media`. The full sweep found **386 mesh/animation files** across five generations of folders.

## Part 1 — What is playable in the web prototype today

| Feature | Source | Status |
|---|---|---|
| BallZ core (inner ball + shell, 0.75–0.85 ratios) | CLBallZ.cpp, ZombieKiller.cpp | Playable |
| 4 rebuilt arena races + rings, gates, lap timer, hazards | Scene3D screens | Playable |
| **Suzanne 1 — Classic**: real 2015 level, all 8 .x meshes, trimesh collision, piston/rotator/door animated, original ring path | Suzanne1.* archives | Playable |
| **ZombieKiller Arena**: Human.cpp wander AI, hunting zombies, infection spread, squash-to-win | ZombieKiller/Human/Zombie.cpp | Playable |
| Medals (gold 100 / silver 75 / bronze 50) + career score + sequential unlock | Level.cs | Playable |
| Map editor: paint 11×11, play as real race, auto-save draft | EditorScreen.cpp (spirit) | Playable |
| Math Game: 10,000-molecule field, parabola/slope, live sliders | Formulas.cpp, MathGameScreen.cpp | Playable |
| Physics Lab: pendulum chain, hinge seesaw, stack, wrecking ball | Newton joints (concept) | Playable |
| Skyboxes: ClearBlue, ClearNight, SkyX, Winter, **LostValley** (recovered) | BallZ 2011 Media/Sky | In game |
| Day/night sun cycle + sun.jpg billboard | Sky.cpp, Atmosphere.cpp | In game |
| Airplane flying Spline.xml path | Airplane.x, Spline.xml | In game |
| Audio cues: coin, halfway, finish, start, squash, infect | (synth stand-ins) | In game |
| Asset Catalog: GRAPHYSX alphabet marquee + 14 decoded TVM toys | TVM archive | In game |

## Part 2 — Complete asset census (2026-07-05)

Formats: **185 TVM** (183 decode cleanly — 98.9%; 2 use an older "TVMO" header variant: gran.tvm, house.tvm), **122 .x** (117 text = parseable, 5 binary), **58 .3ds** (decoder not yet written), **21 .tva** animations (format not yet cracked).

### Environment sets by generation

| # | Environment set | Contents | Converted | Playable |
|---|---|---|---|---|
| 1 | **BallZ 2015 / Suzanne era** | Suzanne1 level + pistons/rotators/gates, Anneaux ring path | YES | YES |
| 2 | **World 1 set** | Level3 terrain + World1Core + Elevator1 + Finish + LevelHole1/2 (shared coordinate space) + Level.TVM, Level1/2/4.TVM variants | YES (`world1-level.json`) | **NOT YET — next candidate** |
| 3 | **BallZ 2011** | Map1 (in TVM+3DS+.x!), Level0, Level1, FinishStation, Ball trio (**BallCtrl / BallShell / BallFire** — the original player ball meshes), Slide1/1a | no | no |
| 4 | **Slide/track series** | Slide1, Slide1a, SlideBump, SlideBumpGridTex, SlideLarge, SlideLong1, BallZTrack1, Corridor, 90Right, pipe1, fleche | showcase only | no |
| 5 | **CubX actor / CubZ menu** | CubXMesh + 8 buttons + recovered TVA families; source-coordinate System/Tools/Earth/Grid/Arrow/Sun shell | meshes + animations YES | dedicated lineage inspector + live evidence-bounded CubZ shell; original Box/Earth/Sphere browser geometry pending |
| 6 | **Alphabet** | full a–z + 0–9 (36 glyphs) | YES | catalog marquee |
| 7 | **Dominus Art village** | ~60 .x meshes: port town (cottages, inn, pub, lighthouse, windmill, docks, shipyard, sunken ship), trees ×10, bushes, grass, tents, swords/shields, characters | no | no |
| 8 | **Cars pack** | GT4, Low Cobra, Jeep, Impreza, Primera, C-Class, FreeCar, chassis, wheels, truck/tire | no | no |
| 9 | **Characters & creatures** | woman, woman1, debbie, vampire, bat, Fish1–3, skmech, tank, DROID02 | no | no |
| 10 | **common/ rooms** | room.tvm, room2.tvm, sky.tvm, grass_bill, ballcube, rancor.tva | no | no |
| 11 | **AtmelCubx** | PisteOvale.tvm (oval driving track for the Arduino-connected build) | no | no |
| 12 | **Props & toys** | Earth, Skull, gears, MenuDomo, LightSwitch, NumSelect, Plus, accept, teapot, terrain, fountain, boat, missile, dna, coke can, boulder, InvertSphere, HalfEmptyBall, Prisme | ~1/3 in catalog | catalog only |

### Corrections to the first-pass analysis
- "70 TVM meshes" → actual count is **185 TVM** across all folders.
- "TVM needs Blender re-export" → wrong; format cracked, direct decode works.
- "several expected level files missing" → partially wrong; World 1 and the 2011 Map1/Level0/Level1 levels exist as TVM geometry, they were just invisible before the decoder.
- Missing GUI layouts/audio: still true — no `.layout`/`.scheme` files, no `coin.wav`/`intro.mp3`/`Piano.mp3` anywhere in the repo. Synth stand-ins remain.
- `.tva` (TV3D animation) and the "TVMO"-variant TVM remain the only undecoded proprietary formats.

## Part 3 — Improvements list (proposed order)

### A. Turn converted data into gameplay (cheap, high payoff)
1. ~~**World 1 race**~~ **DONE 2026-07-05** — "World 1 — Recovered" playable: Level3 terrain + core + holes as static trimesh, the original elevator as a kinematic platform, rings sampled from real terrain vertices, fall-off-the-world respawn, start on the terrain's highest point.
2. ~~**BallZ 2011 Map1 race**~~ **DONE 2026-07-06** — "Map 1 — BallZ 2011": the vertical descent flagship decoded from Map1.TVM, summit spawn, terrain-sampled rings top to bottom. (Level0/Level1 2011 worlds remain — huge coordinate spans, need splitting.)
3. ~~**Original ball skins**~~ **DONE 2026-07-06** — the player ball is now the real 2011 BallShell.tvm with BallFire.TVM as the inner controller, at the authentic 0.776 shell ratio straight from the mesh radii.
4. ~~**Real ring mesh**~~ **DONE 2026-07-06** — every race's rings are now the decoded ring.tvm (325 verts), torus kept only as fallback.
5. ~~**Alphabet in-game**~~ **DONE 2026-07-06** — 3-2-1-GO countdown on race start and a FINISH banner on completion, built from the real alphabet meshes, camera-attached with a pulse.

### B. Engine feel & fidelity
6. ~~**Grounded check fix**~~ **DONE 2026-07-05** — contact-normal based grounding; jumps now work on any surface at any height, and terrain races respawn the ball at the start if it falls out of the world.
7. ~~**UV + texture pipeline**~~ **DONE 2026-07-05** — both decoders now extract UVs (DirectX→GL V-flip); Suzanne's level renders with its original Suzanne1UV.png and the rotator cube with twoway.jpg. Remaining: texture tables for World 1/TVM assets whose original texture names live in the (unparsed) MATT chunks.
8. **Trigger logic** — Suzanne's PistonTrigger should actually fire the piston/door on ball contact (currently on timers).
9. **Classic mouse-torque control** — the ZombieKiller.cpp control mode (torque toward mouse point, throttle on button) as a selectable scheme.
10. ~~**Lazy-load level JSONs**~~ **DONE 2026-07-06** — all six legacy datasets are dynamic imports in their own chunks; the scene rebuilds itself when a chunk arrives. Initial bundle: **195 KB gzipped** (was 943 KB); Suzanne/World1/Map1/cars/TVM data download only when their race or lab is opened. Deploy-ready for specialblend.ca.

### C. New decoders (unlock the rest)
11. ~~**.3ds decoder**~~ **DONE 2026-07-05** (`tools/convert-cars.mjs`) — chunk parser with Z-up→Y-up conversion; Impreza, GT4 and Low Cobra all validate. 55 more .3ds files ready to convert on demand (Map1.3ds, characters, terrain...).
12. ~~**.tva decoder**~~ **DONE 2026-07-17** — the CubXActor and CubZ animation families are decoded and inspectable. They remain explicitly separate because surviving evidence does not support a shared node/click identity map.
13. **TVMO-variant support** — the 2 older-header TVMs (gran, house).

### D. Bigger swings
14. **Dominus village exploration mode** — batch-convert the ~60 village .x meshes into a free-roam "visit" scene (they're text .x; the existing converter handles them today).
15. ~~**PisteOvale + cars**~~ **DONE 2026-07-05** — "Piste Ovale — Impreza": the Subaru decoded from impreza.3ds (chassis + 4 wheels as separate bodies) driving as a cannon-es RigidVehicle — AWD, front-wheel steering, ChassisSTi.bmp livery — on the banked AtmelCubx oval, rings on the racing line, fall-off respawn. Remaining: GT4/Cobra/Jeep as selectable cars.
16. **Shooting** — the last unported Tier-1 gameplay verb (projectiles vs zombies).
17. **High-score name entry** — port HighScoreForm's named leaderboard on top of the existing scoreboard store.
18. **Music** — originals are gone from the repo; either drop in new loops or synthesize a Piano.mp3-style menu theme.

### E. Publishing
19. **Deploy to specialblend.ca** — the app is a static Vite build (`npm run build` → `dist/`), so it can be hosted anywhere: upload `dist/` to the site, or point a subdomain (e.g. ballz.specialblend.ca) at it. If it will live in a subfolder (specialblend.ca/ballz/), set `base: "/ballz/"` in `vite.config.ts` first. Worth doing item 10 (lazy-load level JSONs) before going public to cut the ~3.4 MB initial download.

## Suggested next three
1 (World 1) + 6 (grounded fix) together, then 7 (textures/UVs) — those three make everything already converted look and play like it used to.
