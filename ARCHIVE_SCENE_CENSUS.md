# GraphysX Archive Scene Census

Last audited: 2026-07-18

This is the evidence ledger for distinct scenes found across the GraphysX,
BallZ, CubX/Atmel, Scene3D, ArduinoGUI, SBQC, StockRoom, Blender, and standalone demo
archives. It deliberately separates three things that the old folder layout
often mixed together:

1. **Application screens** — navigation/runtime states such as Main Menu,
   Gameplay, Editor, and Math Game.
2. **Implemented scene classes** — code-backed worlds such as BallZ, Car,
   CubX, FlightX, and the skybox/car selectors.
3. **Authored scene content** — ASCII maps, TVM levels, XML scene documents,
   standalone environments, and rendering demonstrations.

Repeated backup copies of the same implementation are de-duplicated.
Individual props, cars, characters, textures, and shaders are not counted as
scenes unless archive code or a scene document assembles them into one.

Revisions are evidence, not separate museum stops. When several archives
implement the same destination, the revival merges their strongest proven
parts into one canonical version. A revision increases the count only when it
contains a genuinely distinct authored world, screen, or interaction surface.

## Audit result

| Status | Count | Meaning |
|---|---:|---|
| `RESTORED` | 7 | Reachable, working end to end, and verified in the web build |
| `PARTIAL` | 47 | Meaningful behavior/content exists, but fidelity or scope is incomplete |
| `REGRESSED` | 0 | No currently exposed scene is knowingly substituted by the wrong archived concept |
| `PIPELINE` | 0 | No currently proven authored composition is waiting for its first honest port |
| `MISSING` | 0 | Identified scene has no modern implementation |
| `RESEARCH` | 0 | No listed scene is completely unidentified; format research still affects some assets |
| **Total** | **54** | Distinct audited scenes after revision aliases, components, vendor samples, and serializer-only artifacts are removed |

The same 54-record registry is visible in the running app under **Complete
Scene Index** and lives in `web-prototype/src/archive-content.ts`.

## 1. Scene3D application flow

`Scene3D/ScreenIndices.h` and `Scene3D/App.cpp` prove this six-screen flow.

| Scene | Type | Status | Revival check |
|---|---|---|---|
| Main Menu | Application screen | `RESTORED` | Modern project-family home is reachable, readable, and versioned; exact `MainMenuScreen.layout` now supplies original GUI provenance |
| Select Race | Application screen | `RESTORED` | All BallZ challenges, references, records, and load state are exposed; exact `SetRaceScreen.layout` proves the three source rule modes |
| Gameplay | Application screen | `PARTIAL` | BallZ and vehicle loops work; legacy behavior/content fidelity remains |
| After Race | Application screen | `RESTORED` | Completion, medal, records, replay, and next flow work; `AfterRaceScreen.layout` proves Total Time, Best Lap, Rings, and Next Level |
| Scene Editor Screen | Application screen | `PARTIAL` | Tile editor/live preview exist; exact `EditorScreen.layout` restores the transform/mass/static/debug and Save/Load/Clear/Add X/Add TVM/Delete vocabulary, while full scene authoring remains |
| Math Game Screen | Application screen | `PARTIAL` | Exact formula/molecule workbench scope is live with 10,000 molecules, 100 lanes, and all six source 1024px NightSky BMP faces; `MathGameScreen.layout` confirms A/B/C/M sliders and X Scale, while the implementations contain no score, goal, success/failure, or progression loop |

## 2. ArduinoGUI screen flow

`GraphysX/ArduinoGUI/ArduinoGUI/ScreenSys/ScreenList.cs` proves a separate
three-screen application branch.

| Scene | Type | Status | Revival check |
|---|---|---|---|
| Intro Physics Showcase | Application screen | `PARTIAL` | Physics Lab covers constraints/stacks, not the exact alien-floor/light/cylinder scene |
| Arduino Math Screen | Application screen | `PARTIAL` | Covered by Math Game, without exact screen composition |
| Arduino BallZ Screen | Application screen | `PARTIAL` | Covered by the modern BallZ loop, not reconstructed as this exact screen version |

## 3. Implemented legacy scene classes and modes

These are code-backed scene implementations, not filenames inferred from
model names.

| Scene | Evidence | Status | Revival check |
|---|---|---|---|
| Default Terrain / Atmosphere Scene | `Yanik C++ BCKUP/3DScenes.h` — `CLDefaultScene` | `PARTIAL` | Terrain, sharp archive skies, animated distortion water, day/night lighting, and a haze pass exist; authored heightmaps and reflection/refraction remain |
| BallZ Scene | `Archive/bckup/3DScenes.h` — `CLBallZScene` | `PARTIAL` | Shell/controller, physics, rings, and gates are playable |
| Car Scene | `AtmelCubx/AtmelCubx/CarScene.cpp` | `PARTIAL` | One Impreza/Piste trial works; terrain/opponents/network branch do not |
| Skybox Selector | `Yanik C++ BCKUP/3DScenes.cpp` — `CLSkyboxSelectScene` | `PARTIAL` | Dedicated mode restores five 50-unit cubes on the 125-unit ring, five exact sky sets, archived camera, 50°/s rotation, click/zoom and panorama orbit; TV3D chase timing remains approximated |
| Car Selector | `CubXSolution/3DScenes.cpp` — `CLCarSelectScene` | `PARTIAL` | Dedicated mode restores the one-Impreza height terrain, recovered water distortion, gravity drop, archived camera and free-camera preview; the archived click callback was empty, so no invented confirmation flow is claimed |
| CubX Hub World | `AtmelCubx/AtmelCubx/CubXScene.cpp` | `PARTIAL` | CubX/Earth/clock/domotic composition has a browser preview; the older 16-TVA CubXActor family now has a separate exact-geometry/timing inspector with all eight click meshes and the broken eighth actor slot preserved |
| CubZ Animated Cube Menu | `CubZ.cpp`, `MenuManager.cpp`, `CubeRot.tva`, `CubeOpen.tva` | `PARTIAL` | Exact eight hit centers, cube-0 immediate open, seven visible 0.933-second CubeRot ranges, visible 1.667-second CubeOpen panel tracks, and same-range reverse repair are live; the older CubXActor `CubXOpen` is explicitly excluded, while original CubZ actor geometry and TV3D-to-web quaternion mapping remain |
| FlightX Pipe Flight | `FlightXScene.cpp`, `pipe1.tvm` | `PARTIAL` | Decoded pipe1 and archive airplane are playable with the source-derived roll/pitch/thrust/airbrake/reset controls, dedicated HUD, and a verified 14-gate recovery route; exact original mission/lap intent and final human handling balance remain |
| Notes Manager | `Area.h` — `NOTEMGR_MODE`, `NotesManager.cpp` | `PARTIAL` | Dedicated CubX-system visit preserves the exact 50-slot marble note block, zero-note start, 30-unit add cube, source layout, sequential activation and reset; the GUI layout and any text/edit/delete/save/load model are absent from the archive |
| CubX Screensaver | `Screensaver.cpp`, `CubXScreensaver.tva` | `PARTIAL` | Browser QA verified the 10-second idle orbit and 5°/s behavior; original TVA animation, aim target, and settings UI remain |

`AtmelCubx/AtmelCubx/Area.h` explicitly names the major modes `CAR_MODE`,
`CUBX_MODE`, `BALLZ_MODE`, `FLIGHTX_MODE`, and `NOTEMGR_MODE`.

### 3a. Device and input diagnostics

| Scene | Evidence | Status | Revival check |
|---|---|---|---|
| Input & Device Lab | BallZ18 `SerialComm.unity` + `ArduinoCtrl.unity`; GraphysX ArduinoGUI; AtmelCubx device-control lineage | `PARTIAL` | One simulation-first best-version lab now has the source-sized live gamepad monitor, five serial profiles, exact robot frames, 182-point sonar sweep, eight I/O channels, four HHMM schedules, and repaired MeArm values. Physical Web Serial remains permission-gated and disabled pending hardware QA; no port is scanned or opened automatically |

### 3b. Newly proven authored compositions

| Scene | Evidence | Status | Revival check |
|---|---|---|---|
| Three.js Playground | `SBQC/public/Projects/3D Playground` | `RESTORED` | Complete source-backed browser composition with nine hash-locked runtime assets, all 16 original dat.GUI controllers, original FPS value/progress/color bands, Asteroids sky, Airplane.glb orbit, three shader-deformed spheres, procedural raycastable terrain, three Earth-textured bodies, cube, line and animated light. The hash-identical iGrow backup remains one alias |
| BallZ / Blender Level 1 Prototype | `blenderModel/Levels/Level1.blend`; Unity BallZ `Level1.fbx` | `RESTORED` | The best 356-vertex / 354-polygon revision is a complete authored geometry visit with its exact mesh, material, source camera and point light. It is not turned into a race because no host, spawn, physics, controls, checkpoints, rules or objective survives |
| Maison Explorer | `blenderModel/Maison/maison.blend`; `Cuisine.blend` | `RESTORED` | One explorer preserves the complete best House and Kitchen compositions as separate source-camera/overview subspaces. The kitchen's absent unpacked Desktop JPG remains explicit; `.blend1` files remain older revisions, not extra scenes |
| Unity Arena Archive | `bckup/Unity Projects/Arena/Assets/Arena.unity`; `Arena.obj`; `arena.png` | `RESTORED` | Complete scriptless 2017 Unity environment with the exact 48-vertex / 44-face octagonal OBJ, 1024² hand-painted atlas, Standard material values, source object transform, 60° camera and directional-light orientation. Overview/fill/double-sided inspection adapters are disclosed; no gameplay is invented |

## 4. Authored BallZ arenas

| Scene | Evidence | Status | Revival check |
|---|---|---|---|
| Archive Level 1 — ASCII | `levelList.xml`, `Level1_base.ASCII`, `screenShotLevel1.png` | `PARTIAL` | Exact 20×20 T layout, 20 R checkpoints, @ spawn, four posts, three laps, source 0.3-radius BallZ, ClearBlue, Alien01_B floor and archived best time are live; camera/light/handling remain |
| Archive Level 2 — ASCII | `levelList.xml`, `Level2_base.ASCII`, `screenShotLevel2.png` | `PARTIAL` | Exact 20×20 Z layout, 20 R checkpoints, @ spawn, four posts, three laps, source 0.3-radius BallZ, LostValley, checker/Wood03 materials, ten human proxies and archived best time are live; exact humans/camera/light/handling remain |
| Archive Level 3 — ASCII | `levelList.xml`, `Level3_base.ASCII`, `screenShotLevel3.png` | `PARTIAL` | Exact 20×19 M/r layout, 20 checkpoints, `$` spawn, four posts, three laps, source 0.3-radius BallZ, NightSky, Alien02 floor and archived best time are live; the Metal03 texture family survives but has no proven Level 3 binding, while camera/light/handling remain |
| BallZ18 Level 01 — AI Circuit | `BallZ18/Assets/!Scenes/Level01.unity`, `L1_floor.blend`, `CtrlerAI_Ball.cs`, `TargetLooper.cs`, `Countdown.cs` | `PARTIAL` | Exact eight-object blue/red Blender circuit, both source starts, seven-point Raceline, 0.5-radius player, torque BallAI with the scene's 15-rad/s cap, source local wood cube, repaired countdown lock, byte-identical 3/2/1 and GO audio, and exact half/lap segments are playable. The source times recurring laps but has no terminal win, so the disclosed three-lap finish is a canonical revival layer |
| Suzanne 1 | `Suzanne1.ASCII`, `Suzanne1.xml`, `Suzanne1.png` | `PARTIAL` | Correct 40×40 arena is live with 208 walls, 45 chain assemblies, 15 checkpoints, three pistons, two effects, exact 0.3-radius BallZ/start/gates and three laps; CubX actors, Airplane integration, legacy HUD/camera and exact old bindings remain |
| Suzanne 2 | `Suzanne2.ASCII`, `Suzanne2.xml` | `PARTIAL` | Dedicated visit preserves the exact 40×40 composition, 315 collision cubes, 15 rings, chains, pistons, gates, effects, SuperCage player, Airplane, BonedGate and billboard; it exposes the shipped two-pickup/no-lap-target rule conflict without inventing corrected play |

Important correction: these are no longer invented route studies. Levels
1–3 are built from the authoritative ASCII/XML facts and their recovered
period textures/skies; Suzanne 1 is built from the separate 40×40 ASCII scene
shown in `Suzanne1.png`, not the unrelated circular moving-parts model. They
remain `PARTIAL`, not `RESTORED`, where exact camera/light/physics, actors,
bindings, or integrations are still unproven.

## 5. BallZ world and slide concepts

These were BallZ-era experiments, but not all were “BallZ races.”

| Scene | Evidence | Status | Revival check |
|---|---|---|---|
| World 1 | StockRoom `World1*.TVM` assembly | `PARTIAL` | Decoded geometry is visitable with a prototype ring trial; materials/original intent remain |
| Map 1 (2011) | `BallZ 2011/BallZ/Map1.TVM` | `PARTIAL` | Decoded vertical geometry is visitable; authored materials/play rules remain |
| BallZ 2011 Level 0 | `BallZ 2011/Release/Media/Level0.TVM` | `PARTIAL` | Level0.TVM is byte-identical to SlideLarge.TVM; the decoded Great Slide is reachable/playable, while materials, original rules, and finish-loop fidelity remain |
| BallZ 2011 Level 1 | `BallZ 2011/Release/Media/Level1.TVM` | `PARTIAL` | Distinct non-race concept visit preserves the exact 828-vertex/1,648-triangle mesh and two closed components; neutral material, normalization and visit camera are labeled presentation because no loader, binding or gameplay survives |
| Slide 1 | active Atmel `Media/Models/Slide1.tvm`, `Ball.tvm`, `Slide.cpp`, `Level.cpp` | `PARTIAL` | Dedicated non-race visit preserves the correct 566v/552t revision, exact slide/Ball transforms, mass, chase offset, gravity, material and contact constants; incomplete input and explicitly temporary rings remain disclosed |
| Slide 1A | `Slide1a.tvm` | `PARTIAL` | Exact 2,271v/2,849t legacy slide is visitable in the six-source gallery with older static-host transform/StdMat evidence; the later Ball assembly is not borrowed |
| Level.Slides | `Level.Slides.TVM` | `PARTIAL` | Exact 792v/1,524t three-group geometry/material study is visitable; no host assembly or behavior survives |
| Level.Steps | `Level.Steps.TVM` | `PARTIAL` | Exact 200v/120t study is visitable with byte-locked grass/concrete/wood bindings; no host behavior survives |
| Slide Bump | `SlideBump.TVM` | `PARTIAL` | Exact 699v/1,456t topology study is visitable; its zero material record and neutral inspection fallback are explicit |
| Slide Bump GridTex | `SlideBumpGridTex.TVM` | `PARTIAL` | Distinct 685v/1,456t revision is visitable with exact EarthGri binding and topology evidence; it is not merged with Slide Bump |
| BallZ Track 1 | `BallZTrack1.tvm` | `PARTIAL` | Exact 385v/447t hosted track is visitable with source transform, Ball spawn and camera/chase evidence; commented input and temporary rings are not promoted into gameplay |

`SlideLong1.TVM` is byte-identical to the already-counted BallZ 2011
`Level1.TVM`. Every `pipe1.tvm` copy is the component loaded by the already
counted FlightX scene. Neither is counted again as a distinct scene.

## 6. Archived XML scene documents and sub-scenes

| Scene | Evidence | Status | Revival check |
|---|---|---|---|
| MyWorld | `BallZ2015.bckup/Media/MyWorld.xml` | `PARTIAL` | Dedicated evidence visit renders the exact high-detail Airplane TVM and physics cylinder; both malformed DUPLICATE path targets remain unresolved, the document's own Actions table is used, and no finished-world claim is made |
| MyWorld — Copy | `MyWorld - Copie.xml` | `PARTIAL` | Dedicated visit preserves all seven serialized transforms/physics metadata and renders four exact primitives plus exact AirplaneLP/Level2 TVMs; missing ArcheChinois stays absent and host physics/camera/light choices remain explicit |
| TestWorld | `TestWorld.xml` | `PARTIAL` | Dedicated evidence visit renders four exact primitives plus AirplaneLP.TVM; absent bush1.X and two invalid empty duplicate targets remain unresolved, and the loader fixture is not presented as a finished world |
| ObjectLibrary Catalog Grid | `ObjectLibrary.xml` | `PARTIAL` | Dedicated catalog visit preserves all 61 serialized records/transforms: 47 recovered objects, 13 missing records and one unsupported binary X asset, with family/status filters and no invented proxies or village layout |
| Spline Flight Path | `Spline.xml`, `Spline3D.cpp` | `PARTIAL` | Spline/airplane proxy is visible; original motion/mesh remain |
| Anneaux / Ring Field | `Anneaux.cpp`, `Anneaux.xml` | `PARTIAL` | Ring systems work in races; distinct authored sub-scene is not reconstructed |
| Voie Lactée / Milky Way | `VoieLactee.cpp`, `GraphysX_1/Scene.cpp::CreateVoieLactee` | `PARTIAL` | Corrected classification: five-body planetary vignette, not a star field. Exact later assets/transforms and the partial older rotation/orbit profile are directly inspectable; authored host camera/scene remains unknown |

`BaseScene.xml` and `test1.xml` are deliberately excluded from this table.
Exact signature/transform auditing proves they are serializer artifacts rather
than assembled scenes: BaseScene contains 18 identical PHYSICCUBE records at
one exact transform, while test1 is a one-cube v1.2 smoke save with newline-only
copies. Both remain reachable under **Recovery Status → XML Serializer
Artifacts** so the evidence is preserved without inflating the scene census.

## 7. Standalone environments, vehicles, gameplay, and demos

| Scene | Evidence | Status | Revival check |
|---|---|---|---|
| Common Room | `common/room.tvm` | `PARTIAL` | Dedicated standalone visit preserves the exact 180-vertex/250-triangle inward shell and logo DDS pair; no authored host assembly survives, so its camera/light remain explicitly inferred |
| Common Room 2 / Sky Space | `common/room2.tvm`, `common/sky.tvm` | `PARTIAL` | Room 2 remains a dedicated shadow-mapping visit; the exact 726-vertex/1,200-triangle sky.tvm skydome is separately inspectable and correctly classified as a component rather than a distinct scene; exact HLSL fidelity remains |
| Dominus Village / Port | Dominus asset set; `ObjectLibrary.xml` 28-row port subset; no authored host layout found | `PARTIAL` | A dedicated evidence visit preserves the only surviving multi-asset placement: 28 exact catalog rows, 27 decoded meshes and one binary-X boundary. It is explicitly an editor grid, not the absent village; the separate playable tour is labeled modern curated work |
| Piste Ovale / Impreza | `Piste Ovale.TVM`, Impreza 3DS/material set | `PARTIAL` | Driveable; nose/throttle/undercarriage corrected; handling/material groups/UI remain |
| ZombieKiller | `GraphysX_1/ZombieKiller.cpp`, `Zombie.cpp`, `Human.cpp` | `PARTIAL` | Squash loop works; shooting/infection/models/deeper AI remain |
| HLSL Shadow Mapping #23 | `#23 HLSL_Shadow_Mapping` | `PARTIAL` | Room 2 Shadow Lab reconstructs the archived room transform, teapot/light composition, DDS maps, point-shadow equivalent, camera, and A/D orbit; exact HLSL parallax/specular math remains |

## 2026-07-18 Datalake follow-up scan

The second read-only sweep covered 683 FBX, 219 BLEND, 131 3DS, 65 Unity
scene files, 60 MAX, 27 BLEND1, 18 SCN, 10 DAE, four C4D, two X3D and
the surviving WRL, plus named source archives and custom browser projects.

- The Unity **Arena** project was the one new distinct authored environment;
  it is now the 54th canonical record and is fully restored above.
- Blender `BaseWorld.blend` is a three-primitive learning setup, while
  `Scene1.blend`/`Scene2.blend` are the already tracked Suzanne authoring
  lineage and `FaceStart.blend` is a modeling/reference-image exercise. They
  are evidence revisions, not additional worlds.
- SBQC `public/Projects/3D Scenes` was committed as “3d examples”; its named
  ThreeJS tutorials, OBJ/background/video/drag samples, Base World exercise,
  and two-object Rotations exercise remain a training/example collection, not
  additional authored GraphysX destinations. The separately developed 3D
  Playground remains the canonical browser world.
- SBQC `Kart` is a binary-only Unity Karting/Microgame tutorial build
  (`KartGame`, ML-Agents, Interactive Tutorials) uploaded alongside MathGame;
  it is vendor/training material and is excluded. SBQC MathGame is another
  runnable build of the already merged BallZ Math Game.
- `Boids`, Unity Standard Assets, 3D Game Kit, tutorial scenes, SmallOrc,
  HuaYing and other imported samples remain excluded. The 4,294,967,295-byte
  `Unity Projects.zip` listing exposed no additional usable `.unity` source;
  its surviving unpacked projects are the auditable source of truth.
- `CubXScene.rar` and `AtmelCubx63_2003.rar` add exact CubX actor/menu assets,
  source and higher-resolution Earth evidence to the existing CubZ/CubX
  restoration; they do not prove another scene.

## Highest-value restoration order

1. Finish camera/light/physics and remaining asset fidelity for ASCII Levels 1–3 and Suzanne 1; turn the source-backed Suzanne 2 visit into play only after deciding how to expose—not hide—its two-pickup rule defect.
2. Finish **FlightX Pipe Flight** fidelity: research the undocumented original mission/lap intent, validate the recovered 14-gate route with human play, and tune handling/collision without losing the now-restored archive controls and HUD.
3. Finish Level0/Great Slide fidelity and validate gameplay boundaries for the active Slide1 plus the now-visible six-source Slide/Track Gallery without promoting temporary rings or absent controls into authored races.
4. Finish Room 2's exact HLSL material behavior and research whether any host
   assembly for Common Room 1/sky.tvm survives; preserve the working ObjectLibrary
   and Dominus evidence grids without presenting them as authored worlds or races.
5. Complete the CubX Hub with original meshes/TVA animations, recovered 8K Earth and exact 3D alphabet/clock sources, recover any
   missing Notes GUI/data-model evidence, and restore the original CubXScreensaver
   animation/settings around the now-working idle orbit.
6. Continue the XML document runtime beyond the now-visible MyWorld, TestWorld,
   MyWorld — Copy, and ObjectLibrary evidence lanes. Keep BaseScene/test1 in
   serializer-artifact recovery—not the scene census—and keep Suzanne 2 in its
   dedicated ASCII/XML recovery path.
7. Complete hardware QA for the simulation-first **Input & Device Lab** before enabling its permission-gated port picker. Add BallZ18's optional Unity Cage appearance and mouse-hold steering without duplicating Home, Track Select, Math Game, or hardware revisions as era-specific scenes.

## BallZ18 canonical merge map

| Source scene | Canonical destination | Decision |
|---|---|---|
| `MainMenu.unity` | Home | Provenance merged; current family-first Home is the stronger version |
| `TrackSelect.unity` | BallZ Tour | Provenance and Level01 destination merged; no duplicate selector |
| `Level01.unity` | BallZ Tour — AI Circuit | Distinct authored world; counted once in the expanded registry |
| `MathGames.unity` | Math Game | Unity's exact a=.01/b=0/c=100/m=5 preset merged into the stronger workbench |
| `SerialComm.unity` | Input & Device Lab | Merged into the live simulation-first profile/handshake console; physical transport remains permission-gated |
| `ArduinoCtrl.unity` | Input & Device Lab | Controller/joystick/sonar diagnostics are live in the same safe canonical lab |

Unity Standard Assets, Ethan, RollerBall, 3D Game Kit, and Stargate sample
content remain provenance evidence or optional backdrop research, not authored
GraphysX scenes and not additional census records.

## Rules for changing a status

A scene becomes `RESTORED` only when source/provenance is known, its project
family is correct, it is reachable from the main UI, the original interaction
is usable, visual QA has been performed, text-state/debug output covers its
important state, the console is clean, and the production build passes.
