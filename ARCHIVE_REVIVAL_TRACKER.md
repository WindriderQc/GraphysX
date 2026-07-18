# GraphysX Archive Revival Tracker

Last audited: 2026-07-18

This is the persistent source of truth for the revival. A feature is only marked
`RESTORED` when it is visible, usable, correctly categorized, and verified in
the running web build. Having source files, a decoder, a static proxy, or a
hidden implementation does not count as restored.

The complete de-duplicated archive scene list is maintained in
[`ARCHIVE_SCENE_CENSUS.md`](ARCHIVE_SCENE_CENSUS.md) and mirrored by the
in-app **Complete Scene Index**. The 2026-07-18 census contains 54 distinct
screens/scenes: 7 restored, 47 partial, 0 regressed, 0 pipeline, and 0
missing.

The product rule is **complete best-version consolidation**, not preservation
of every evolutionary snapshot as a separate destination. Revisions are
mined for their strongest proven geometry, mechanics, UI, materials, and
assets; overlapping scenes merge, while every genuinely distinct world or
interaction surface remains independently tracked.

## Latest reconciliation — progressed versus forgotten

The r13 completion pass closes every currently proven first-port pipeline:

- **Three.js Playground is complete** — all 16 archived dat.GUI controllers
  and the original FPS value/progress/color behavior now join its nine
  hash-locked assets and full authored composition. Focused QA passes 43/43.
- **BallZ / Blender Level 1 is complete at its honest scope** — deterministic
  Blender export restores the best 356-vertex / 354-polygon mesh, material,
  saved camera and point light. No race is invented from a folder name.
- **Maison Explorer is complete** — the best House and 87-object Kitchen are
  one source-separated explorer with saved cameras/lights/hierarchy and an
  explicit boundary around the one missing unpacked Desktop JPG. The combined
  Blender suite passes 34/34.
- **Unity Arena recovered in the follow-up scan** — the exact scriptless scene,
  48-vertex / 44-face OBJ, 1024² hand-painted atlas, Standard material,
  authored transform, 60° camera and directional light are reachable as the
  54th canonical record. Its permanent suite passes 18/18 with zero browser
  errors.
- **No queued or missing scene remains** — the registry is now 7 RESTORED / 47
  PARTIAL / 0 PIPELINE / 0 MISSING. `PARTIAL` continues to mean a reachable
  scene with an explicit source-fidelity boundary, never a forgotten first
  port or permission to fabricate absent behavior.
- **Second Datalake sweep** — SBQC's committed “3d examples” and Unity Karting
  Microgame build are correctly excluded as training/vendor material;
  BaseWorld and Scene1/Scene2 are primitive/Suzanne lineage rather than extra
  worlds; CubX archives enrich the existing menu/actor restoration. The full
  evidence and extension census is recorded in `ARCHIVE_SCENE_CENSUS.md`.

The following r12 results remain part of the consolidated build:

This audit found that implementation had moved ahead of the status UI and
ledgers. The r12 best-version pass restores BallZ18 audio, the merged device lab, and the first newly proven Datalake world while
keeping fidelity claims conservative:

- **BallZ18 Level 01** — the Unity archive contributes one distinct authored
  world (the addition that first raised the canonical census to 49). Its exact eight-object
  `L1_floor.blend` circuit, two starts, seven Raceline points, 0.5-radius
  BallZ, torque-driven BallAI with the scene's 15-rad/s cap, exact half/lap
  segments, local WoodFloor05 cube, and repaired countdown lock are playable.
  The byte-identical archived `beepShort.mp3` now plays at 3/2/1 after the
  source half-second wait, and `beep01.mp3` plays at GO.
  The archived loop has no terminal win; the visible three-lap finish is
  explicitly the canonical revival progression layer.
- **Revision merge, not duplicate eras** — BallZ18 MainMenu merges into Home,
  TrackSelect into BallZ Tour, and MathGames into the existing Math Game. The
  latter now exposes the exact Unity a=.01/b=0/c=100/m=5 preset. Standard
  Assets/Ethan and 3D Game Kit content are excluded from authored GraphysX
  scene counts.
- **Input & Device Lab** — `SerialComm.unity`, `ArduinoCtrl.unity`, GraphysX
  ArduinoGUI, and AtmelCubx now merge into one distinct safe lab. It has live
  source-mapped controller input, five simulated protocol profiles, robot/pin
  commands, the 182-point sonar sweep, eight I/O channels, four HHMM schedules,
  and repaired MeArm controls. Simulation is the default, ARM/STOP is explicit,
  and physical serial remains disabled pending permission/lifecycle hardware QA.
- **The archive was not exhausted** — the r12 Datalake pass proved three more
  canonical experiences: the browser-native Three.js Playground, a distinct
  BallZ / Blender Level 1 prototype, and the unified Maison Explorer. The
  Playground first became a reachable visit with exact runtime assets and
  authored composition; the latter two entered visible `PIPELINE` records
  instead of being forgotten. r13 subsequently completes all three and the
  Unity Arena found by the follow-up scan.
- **Original UI evidence recovered** — the exact CEGUI Main Menu, Set Race,
  After Race, Math, Editor, and Splash layouts survive. The Tour now exposes
  the source's three rule modes, and the tracker no longer calls those layouts
  absent.
- **r12 verification** — focused BallZ18/Math/audio coverage passes 28/28, Input &
  Device Lab passes 31/31, and the isolated Three.js Playground passes 33/33,
  all with zero browser errors. The production build passes at visible build
  `revival-2026.07.17-r12`; the full integrated matrix passes 279/279 and the
  required generic client completes two clean iterations. r13 adds 43/43
  Playground, 34/34 Blender-composition, and 18/18 Unity Arena coverage before
  rerunning the production build and integrated matrix.

- **Dominus placement boundary** — the last scene-level pipeline record now
  has an evidence-bounded visit: all 28 port-prefixed ObjectLibrary rows retain
  their exact ordering/transforms, 27 decoded meshes render, and the one binary
  X record remains diagnostic. The regular editor grid is never called the
  lost authored village, and the separate playable tour is labeled modern
  curated work; Dominus therefore advances only to `PARTIAL`.
- **Menu and runtime coherence** — every one of the six recovered-world
  progression entries is visible in a named family, all direct archive cards
  refresh their ready state instead of sticking on “Loading…”, five buried
  labs now have Home destinations, and the 28-mode switcher is collapsed until
  requested. The Engine & FX CTA now reaches the particle lab directly.
- **Input and pause** — standard gamepads and a mobile touch dock now drive
  the same movement/action keys as keyboard input. A visible P/touch pause
  freezes gameplay state and releases held inputs. Full settings/audio and
  remapping remain separate unfinished production work.
- **Presentation repairs** — vehicle camera collision no longer treats Piste
  Ovale's banked road as a blocking wall; BallZ wall avoidance retains a
  readable elevated fallback; panorama lenses reduce low-resolution skybox
  magnification; and CubZ isolates its selected cube/panels at an inspection
  distance while retaining recovered TVA timing.

- **MyWorld / TestWorld XML evidence** — both distinct serialized documents
  now have direct main-UI visits using each file's own embedded Actions table.
  MyWorld renders 2/4 exact records and TestWorld renders 5/8; malformed
  duplicates and absent assets remain unresolved, and neither is called a
  finished world.
- **Serializer-artifact correction** — `BaseScene.xml` and `test1.xml` are no
  longer counted as worlds. Exact inspection proves an 18-way coincident cube
  save and a one-cube smoke document; both remain reachable under Recovery
  Status with `distinctAssembledScene=false`.
- **GT4 / Low Cobra source gallery** — exact 3DS objects, faces, UVs,
  material-face lists and texture references are visible in the vehicle
  family. Both remain `PIPELINE` as vehicles because the archive never binds
  them to its one-Impreza selector or `CLVehicule` physics.
- **Chase-camera collision** — active BallZ and vehicle cameras now raycast
  through the real Cannon world and shorten before blocking bodies. The
  deterministic low-wall/high-angle regression covers both collision and
  clearance behavior.
- **Math Game scope correction** — source audit confirms both surviving
  implementations are visual formula workbenches, with no archived score,
  goal, success/failure, or progression loop to restore. Modern presets remain
  labeled inspection aids.

- **Archive Levels 1–3** — exact ASCII symbols, lap posts, 20 checkpoints,
  three-lap rules, sky assignments, best times, recovered floor/tile maps and
  Level 2's ten-human population are live. Remaining camera/light/physics and
  actor/material gaps stay explicit rather than being called restored.
- **Suzanne 1** — the unrelated circular machinery model is no longer passed
  off as the screenshot world. The correct recovered 40×40 ASCII arena now
  supplies 208 walls, 45 chain assemblies, 15 checkpoints, three pistons, two
  effects, exact start/gate segments and three-lap play.
- **Skybox and Car selectors** — both are dedicated reachable modes. The
  five-cube rotating sky selector has click/zoom/panorama behavior; the car
  preview has its recovered height terrain, distortion water, Impreza,
  gravity drop, archive camera and free movement.
- **Recovered-but-forgotten archive components** — Common Room 1/sky.tvm,
  BallZ 2011 Level1.TVM, CubX Notes, and Voie Lactée now have direct,
  correctly categorized inspection destinations with exact evidence and
  explicit reconstruction boundaries.
- **BallZ Slide1** — the active Atmel revision is now a distinct non-race
  concept visit with its exact 566-vertex/552-triangle assembly, Ball spawn,
  transforms, mass and chase offset. The temporary-ring material and absent
  rules remain disclosed instead of being invented.
- **Remaining BallZ slide/track family** — Slide1A, Level.Slides,
  Level.Steps, both distinct SlideBump revisions and BallZTrack1 are now one
  six-source archive gallery with exact geometry, material/topology evidence,
  host boundaries and reversible inspection transforms. All six remain
  non-gameplay PARTIAL visits rather than becoming invented races.
- **MyWorld — Copy** — six of seven serialized XML objects are visible in a
  standalone scene visit. Four procedural definitions and the exact AirplaneLP
  and Level2 meshes are preserved; the absent `ArcheChinois` stays unresolved.
- **Suzanne 2** — its separate 40×40 authored scene is reachable with 315
  collision cubes, 15 rings, chains, pistons, gates, emitters, SuperCage and
  surviving XML attachments. The shipped 15-ring versus two-pickup victory
  conflict is reported, not silently rewritten.
- **CubZ source motion** — `CubeRot.tva` and `CubeOpen.tva` are decoded and
  now drive the visible procedural adapter as well as exact clip telemetry.
  Cube 0's no-rotation path is preserved and the archived broken
  `iSelectedCube+1` reverse expression is repaired explicitly. The older
  CubXActor `CubXOpen` export is isolated in its own evidence inspector rather
  than being substituted for `CubeOpen`. Original actor geometry and the
  TV3D-to-Three quaternion/handedness adapter remain unfinished.
- **ObjectLibrary catalog** — the exact 61-record editor grid is now a distinct
  filtered visit: 47 recovered assets render, 13 missing records and one
  unsupported binary X stay explicit, and no proxy or village composition is
  invented.
- **Archived particle preset library** — Engine & FX now exposes all 19
  compiled names / 17 unique binaries: 16 readable presets drive 29 exact
  emitter configurations and one attractor, while three opaque compiled-only
  presets remain selectable evidence with no guessed emitters. Exact aliases,
  surviving DDS bindings, the missing `clumpy_blurry.dds` diagnostic, and
  restart/pause/auto-replay controls are visible beside the separately labeled
  modern diagnostic fountains.
- **Dominus source assets** — a dedicated gallery exposes all 65 logical
  assets by their seven proven filename families: 63 exact meshes render and
  the two unsupported binary-X records remain explicit. The separate 28-row
  placement-evidence visit closes scene-level pipeline status while preserving
  the fact that no authored village composition survived.

- **BallZ 2011 Level0 / SlideLarge** — the two TVM files are byte-identical.
  The decoded Great Slide is reachable, starts, and accepts live movement.
- **FlightX Pipe Flight** — decoded `pipe1` and airplane geometry are reachable
  as a kinematic flight mode. Archive arrow roll/pitch, W/S thrust, Space
  airbrake and R orientation reset are separated correctly; a 14-gate pipe
  route, dedicated HUD/help and live flight text state are working.
- **Atmosphere systems** — `post_haze.shade` has a modern distortion pass and
  Scene Lab water now has animated recovered distortion rather than only a
  flat material.
- **Legacy behavior helpers** — a classic-2015 BallZ physics preset, persisted
  best-run ghost path/shell, and the CubX 10-second idle orbit exist. The
  BallZ preset and ghost recording state are now visible in the gameplay HUD
  and text state instead of being hidden.
- **Standalone Room 2 / Shadow Mapping #23** — a dedicated non-race visit now
  uses decoded indexed room geometry, the original diffuse/normal DDS pair,
  archived room/camera transforms, teapot, light marker, point-shadow
  equivalent, black background and A/D orbit. No BallZ or race HUD is present.

The forgotten/incoherent layer is still substantial:

- All four newly proven Datalake compositions now have complete first-class
  ports. Remaining `PARTIAL` rows below are source-fidelity or hardware
  boundaries inside already reachable experiences, not forgotten scenes.
- Input & Device Lab simulation is live; the physical Web Serial picker remains
  intentionally disabled until cancellable parsing, bounded sending,
  disconnect/visibility cleanup, and real hardware QA are complete. The old
  hard-coded COM5/COM7 and LAN assumptions must never run automatically.
- BallZ18 contains an intact six-face 2048² Clear Sky set that can improve the
  current low-resolution sky presentation, but its Unity orientation/rotation
  must be visually validated before replacing a canonical sky.
- FlightX's exact original mission/lap intent is not documented; the 14-gate
  route is an honest recovery route rather than a claimed original layout.
- Great Slide still needs authored materials, original checkpoint/rule intent,
  camera/physics comparison and human completion tuning.
- Ghost recording is visible, but still lacks clear/manage controls and a
  verified full saved-run playback/record replacement flow.
- Room 2 still uses a modern point-shadow/material equivalent; the exact HLSL
  parallax/specular/depth shader behavior and the separate `sky.tvm` space are
  not restored.
- Dominus contains a large recoverable prop/character/port asset family, but
  no authored world composition survived. `ObjectLibrary.xml` is a 61-entry
  catalog grid, not evidence for a village. A catalog browser is honest; any
  future explorable environment must be labeled as a new reconstruction.
- The combined Concepts/Flight/Vehicle selector still needs final product-level
  separation, though Room 2 now has its own standalone Home destination.

## Status legend

- `RESTORED` — working end to end and visually checked.
- `PARTIAL` — meaningful implementation exists, but fidelity, behavior, UI, or
  content is incomplete.
- `REGRESSED` — present, but currently wrong or worse than the archive intent.
- `PIPELINE` — source is understood or convertible, but not a usable mode yet.
- `MISSING` — not implemented.
- `RESEARCH` — archive intent or proprietary format still needs investigation.

## Project-family boundaries

These families must stay visibly separate in the main UI and documentation.
Files living in a BallZ-era folder do not automatically make their contents a
BallZ race.

| Family | What belongs here | What does not belong here |
|---|---|---|
| BallZ races | Shell/controller gameplay, authored race routes, rings, halfway/finish logic, race hazards, records and medals | Unauthored 3D worlds that were later given rings only for the prototype |
| BallZ concepts and world experiments | World 1, Map 1, BallZ 2011 Level0/Level1, slide/track/corridor/pipe sets and other BallZ world ideas | Standalone rooms or generic 3D environments merely found beside BallZ code |
| Standalone 3D environments | Common rooms, terrain/sky scenes, XML world visits, Dominus asset catalog, free-roam discovery spaces | BallZ races, BallZ slide/level concepts, or vehicle experiments |
| Vehicle experiments | Piste Ovale, Impreza and the remaining car pack | BallZ race progression |
| ZombieKiller / gameplay experiments | Human/zombie AI, infection, squash, shooting, projectiles, triggers | Generic engine/particle demonstrations |
| GraphysX engine labs | Physics constraints, particles, shaders, lighting, sky, water, reflections, haze/post-processing, spline flight | Finished game content |
| Math Game | Formula selection, A/B/C/M/X controls, 10,000-molecule field, and a readable visual workbench | Generic Scene Lab content or an invented score/goal loop absent from both surviving implementations |
| Editor tools | Object creation/manipulation, materials, triggers, splines, lights, NPCs, import, save/load/export | BallZ-only tile painting |
| CubX / Atmel | CubX menu world, 21 TVA animations, clock and domotic controls | BallZ archive worlds |
| Asset museum | Provenance, conversion status, decoded models and textures | A substitute for playable restoration |

## Immediate correction queue

| Item | Status | Acceptance test |
|---|---|---|
| Separate classic BallZ, later BallZ concepts, standalone 3D environments and vehicles | `PARTIAL` | Home and archive copy now name the families correctly; the combined archive-trials screen still needs dedicated BallZ-concepts, 3D-environments and vehicle selectors |
| Archive screenshot fidelity targets | `PARTIAL` | Level 1/2/3 use exact ASCII/XML layouts, posts, checkpoints, laps, skies and recovered material bindings. Suzanne uses the correct 40×40 ASCII scene. Exact camera/light/physics, actors and uncertain bindings remain tracked per row |
| Impreza front/back orientation | `RESTORED` | Visual nose points in the actual forward-drive direction |
| Impreza undercarriage | `RESTORED` | Downward/lower chassis faces use the archived grey mechanical undercarriage texture, never the top livery |
| Impreza throttle after countdown | `RESTORED` | Vehicle bodies no longer sleep through W/S input; delayed-input browser QA must show nonzero travel |
| BallZ shell/controller identity | `PARTIAL` | Decoded geometry, UVs, FireArrow texture and an internal direction arrow are visible; further visual tuning remains |
| BallZ visibility isolation | `RESTORED` | Shell/controller appears only in BallZ/world race previews and gameplay, never in Math/Scene/FX/editor labs |
| Visible build/version label | `RESTORED` | Persistent UI badge and text-state payload expose the exact running web-revival version/build |
| Particle engine visibility | `PARTIAL` | Engine & FX exposes the full audited preset library: 19 compiled names / 17 unique binaries, 16 readable presets, 29 emitters, one attractor, three honest opaque records, exact aliases and surviving DDS bindings. Continue with missing texture recovery, TV3D force/blend fidelity and additional proven callsites |
| Skybox presentation | `PARTIAL` | Source resolution is disclosed; blur is disabled and a moderately wider camera lens reduces magnification, but original 256/512 px sources still limit fidelity |
| Runtime texture coverage/alignment | `PARTIAL` | Static runtime references resolve and generated arenas use world-scaled face UVs; continue a visual material audit of every recovered mesh and decode authored TVM material groups |
| Math Game UI | `RESTORED` | Formula, presets, labeled controls, values and live result are visible without hunting through recovery cards |
| CubZ animated menu | `PARTIAL` | Eight exact source hit boxes now follow decoded CubeRot/CubeOpen ranges and timing, unfold four panels, accept an action and reverse home. Original actor/button geometry and a validated TV3D quaternion/handedness visual adapter remain |
| Complete scene census | `RESTORED` | 54 distinct screens, runtime classes, authored scenes, environments, vehicle scenes and demos are de-duplicated, sourced, statused, documented and visible from Home. There are 0 pipeline and 0 missing records; revision aliases, vendor samples, components, and serializer-only artifacts are not double-counted |
| Revival tracking | `RESTORED` | Reconciled through r13 with code, census, tracker, progress, source hashes, permanent browser QA, and the second read-only Datalake scan kept together |

## Playable content

### BallZ race family

| Content | Status | Remaining work |
|---|---|---|
| Core shell + inner controller physics | `PARTIAL` | Decoded UVs and internal arrow are restored; further compare feel against CLBallZ/ZombieKiller torque modes |
| Archive Level 1 | `PARTIAL` | Exact 20×20 T/R/@/F/f/H/h layout, three laps, source 0.3-radius BallZ, ClearBlue, Alien01_B floor and archived score are live. Finish camera/light/handling and uncertain T-face binding fidelity |
| Archive Level 2 | `PARTIAL` | Exact 20×20 Z/R/@/F/f/H/h layout, three laps, source 0.3-radius BallZ, LostValley, checker/Wood03 maps, ten deterministic human proxies and archived score are live. Recover exact human models/seed plus camera/light/handling |
| Archive Level 3 | `PARTIAL` | Exact 20×19 M/r/`$`/F/f/H/h layout, three laps, source 0.3-radius BallZ, NightSky, Alien02 floor and archived score are live. `$` is the spawn cell—not a prop. The Metal03 family survives but has no proven Level 3 binding; camera/light/handling remain |
| BallZ18 Level 01 — AI Circuit | `PARTIAL` | Exact blue/red Blender circuit, starts, seven-point Raceline, BallAI, 0.5-radius BallZ, local WoodFloor05 cube, source trigger segments, countdown lock, and byte-identical 3/2/1 + GO audio are live. Add optional Unity Cage appearance, mouse-hold steering, and final human handling/camera tuning; retain the disclosed three-lap revival layer |
| Skybox Spiral prototype race | `PARTIAL` | Clarify that it is a new archive-inspired challenge, not a recovered authored level |
| Suzanne 1 | `PARTIAL` | Correct 40×40 ASCII arena, screenshot-backed grass/wood profile, 208 walls, 45 chains, 15 checkpoints, three pistons, effects, exact 0.3-radius BallZ/gates/start and three-lap callbacks are live. Restore CubX actors, Airplane link, legacy HUD/camera and exact older bindings |
| Suzanne 2 | `PARTIAL` | Distinct exact 40×40 source visit exposes 315 collision cubes, 15 authored rings, three chains, three pistons, four gate posts, four emitters, two CubX anchors, SuperCage and surviving XML objects. Resolve the source's 15-ring/two-pickup victory conflict and restore intended gameplay before calling it playable |
| ZombieKiller | `PARTIAL` | Shooting/projectiles, deeper AI, infection balance, character fidelity |
| Records, medals, completion flow | `RESTORED` | Named high-score entry and save migration remain product work |
| Classic-2015 BallZ preset | `PARTIAL` | Physics preset persists, toggles with B, and is visible in HUD/text state; compare and tune archive feel |
| Best-run ghost / last-lap spline | `PARTIAL` | Recording status is visible and persistence/shell/last-lap code exists; add clear/manage controls and verify complete saved-run playback |
| BallZ map editor | `PARTIAL` | Useful tile editor exists; it is not the full GraphysX scene editor |

### BallZ concepts and standalone environments

| Content | Status | Remaining work |
|---|---|---|
| World 1 assembled geometry | `PARTIAL` | Categorize as a BallZ world concept; recover material table and original gameplay/visit intent |
| BallZ 2011 Map 1 geometry | `PARTIAL` | Categorize as a BallZ world concept; recover materials and original gameplay/visit intent |
| BallZ 2011 Level0 / Great Slide | `PARTIAL` | `Level0.TVM` and `SlideLarge.TVM` hash identically; decoded geometry, generated route and movement work. Recover authored materials/rules/checkpoint intent and complete the finish loop |
| BallZ 2011 Level1 | `PARTIAL` | Exact 828-vertex/1,648-triangle mesh and two closed components are exposed as a non-race concept visit. Research the absent loader/material/gameplay evidence; keep neutral material and presentation explicitly inferred |
| BallZ Slide1 active Atmel revision | `PARTIAL` | Exact 566-vertex/552-triangle assembly, source transforms, Ball spawn/scale/mass and chase offset are inspectable. Recover missing authored rules and material binding before adding gameplay |
| Remaining slide/track/corridor series | `PARTIAL` | Great Slide/Level0 and active Slide1 remain separate; the six-source gallery now exposes exact Slide1A, Level.Slides/Steps, both bump revisions and BallZ Track 1 with evidence-bounded materials/hosts. Restore only gameplay proven by source; keep `SlideLong1`/Level1 and FlightX `pipe1` deduplicated |
| Common Room 2 / HLSL Shadow Lab | `PARTIAL` | Dedicated non-race visit has exact decoded room geometry, archived transforms/maps/light/camera and A/D orbit; restore exact HLSL material behavior |
| Common Room 1 / `sky.tvm` component | `PARTIAL` | Dedicated non-race visit exposes Room 1's exact shell/logo maps plus the exact skydome diagnostic. Search for a lost authored host assembly; camera/light remain labeled inference and sky.tvm remains classified as a component, not an invented scene |
| MyWorld — Copy XML scene | `PARTIAL` | Six of seven objects are live with four exact procedural definitions plus exact AirplaneLP and Level2 meshes. `ArcheChinois` is absent; physics metadata is preserved but not simulated without the missing host settings |
| MyWorld / TestWorld XML evidence | `PARTIAL` | Main-UI evidence visit renders MyWorld 2/4 and TestWorld 5/8 exact serialized records with per-document Actions tables; malformed duplicates and absent bush remain explicit, and neither is called a finished world |
| BaseScene / test1 serializer artifacts | `RESTORED` | Exact Recovery Status inspector preserves the 18-way overlap and one-cube smoke save while keeping both out of the 48-scene census (`distinctAssembledScene=false`) |
| ObjectLibrary.xml catalog grid | `PARTIAL` | Dedicated browser preserves all 61 exact serialized rows and transforms: 47 recovered objects, 13 missing records and one unsupported binary X asset, with family/status filters and no invented proxies. It remains a catalog, not a world composition |
| Dominus source-asset gallery and placement evidence | `PARTIAL` | All 65 logical records are reachable; the separate exact 28-row port grid renders 27 decoded meshes plus one binary-X boundary. Neither evidence surface is presented as the absent authored village; the playable tour is disclosed as modern curated work |
| Scene Lab terrain/water/spline preview | `PARTIAL` | Animated distortion water and atmosphere haze now exist; actual heightmaps, reflection/refraction, authored controls, and replacement of remaining proxies still remain |

### Vehicle family

| Content | Status | Remaining work |
|---|---|---|
| Piste Ovale + Impreza | `PARTIAL` | Nose orientation, grey mechanical undercarriage, three-slot chassis materials, face-projected archived concrete and source `EngineCtrl.Power = 3000` are live. A disclosed 3.5% Cannon traction adapter makes the rear-wheel torque move reliably; the readable chase camera no longer clips into the banked road. Exact TVM material groups and closed-engine tire/contact fidelity remain |
| GT4 | `PIPELINE` | Exact 14-object / 10,740-vertex / 8,345-triangle source and texture are inspectable; no archive selector or physics binding survives, so driving remains pipeline |
| Low Cobra | `PIPELINE` | Exact 10-object / 6,961-vertex / 3,266-triangle source and seven texture bindings are inspectable; no archive selector or physics binding survives, so driving remains pipeline |
| Jeep / Primera / C-Class / FreeCar / truck | `MISSING` | Convert and validate per-model pivots/materials |
| Vehicle selection UI | `PARTIAL` | Dedicated archive Car Selector correctly remains one-Impreza; a separate exact GT4/Cobra source gallery exposes the orphan packs without pretending they were selectable or driveable |

## Engine and visual systems

| System | Status | Remaining work |
|---|---|---|
| Cannon rigid-body world | `RESTORED` | Performance/edge-case tuning |
| Hinges, distance constraints, stack, wrecking ball | `PARTIAL` | More original joint demos and direct interaction |
| Particle emitter | `PARTIAL` | All 16 readable TVPJ presets drive their exact 29 serialized emitter configurations and one attractor with deterministic playback, selection, restart, pause and auto-replay; continue TV3D-private RNG/integrator/blend fidelity and force/editor controls |
| Projectiles / shooting | `PARTIAL` | Static preview exists; implement live firing, collision and damage |
| Lighting/day-night | `PARTIAL` | Current preview only; match archive controls and expose tuning |
| Skyboxes | `PARTIAL` | ClearBlue/ClearNight/SkyX are 512 px faces, Winter is 256 px, LostValley is 1024 px; sharp wider-lens presentation and labels are active. BallZ18's intact 2048² Clear Sky set is now located but needs Unity-face orientation/rotation QA before canonical adoption |
| Water/reflection/refraction | `PARTIAL` | Recovered distortion texture now drives animated shader water; true reflection/refraction and archive control fidelity remain |
| Shaders | `PARTIAL` | Fluid shader exists; archived shader pack is not translated feature-by-feature |
| Haze/post-processing | `PARTIAL` | EffectComposer/ShaderPass ports the `post_haze.shade` distortion idea on atmosphere races; tune against archive output and restore other post effects |
| Particles/fire/flare textures | `PARTIAL` | Every surviving audited DDS binding is used by the full preset browser; exact aliases are retained and the absent `clumpy_blurry.dds` is a conspicuous magenta-X diagnostic rather than a replacement. Continue searching for the missing source and validating TV3D blend behavior |
| Voie Lactée planetary vignette | `PARTIAL` | Exact later Earth/cloud/Moon/Mars/Venus assets and transforms plus partial 2015 motion are inspectable. Preserve the authored overlap/enclosed-Moon oddities; research any missing host camera/scene and older cloud-alpha binding |
| Camera collision | `PARTIAL` | Active BallZ/vehicle chase cameras raycast from the player target through the actual Cannon physics world and shorten before blocking bodies; the elevated BallZ fallback and Piste road exclusion keep both views readable. Deterministic low-wall/high-angle/vehicle QA passes; standalone visit/orbit cameras still use their own controls |

## Math Game

| Item | Status | Remaining work |
|---|---|---|
| Parabola and slope formulas | `RESTORED` | Add more formula families only if supported by archive evidence |
| 10,000-molecule field | `RESTORED` | Exact 100-lane molecules and the source's six 1024px NightSky BMP faces are live; add a performance scaling option for weak/mobile devices |
| A/B/C/M/X live parameters | `RESTORED` | Prominent labeled workbench, live values, parameter meanings and inactive-state styling are working |
| Presets / goals / feedback | `RESTORED` | Source audit confirms both surviving Math Game implementations are visual formula workbenches with no goals, score, success/failure or progression. The exact BallZ18 Unity a=.01/b=0/c=100/m=5 preset is labeled archive-backed; the remaining presets are explicitly modern inspection aids |
| Camera/control help | `RESTORED` | The visible workbench now says drag-to-orbit / wheel-to-zoom, exposes Reset 3D View, and reports the same local camera/control state without leaking the hidden BallZ player |
| Original GUI fidelity | `PARTIAL` | Exact `MainMenuScreen`, `SetRaceScreen`, `AfterRaceScreen`, `MathGameScreen`, `EditorScreen`, and `SplashScreen` CEGUI layouts survive under BallZ2015 Debug. The Tour now exposes the three source rule labels; continue translating exact composition where it improves the stronger canonical UI |

## Editor recovery

| Item | Status | Remaining work |
|---|---|---|
| BallZ tile map painting | `PARTIAL` | Named semantic levels, region edits, revision guards and ASCII adapters work; add a visible file workflow and larger-world ergonomics when needed |
| Live 3D draft preview | `RESTORED` | Shared runtime preview, outliner/inspector selection and direct viewport editing work |
| Object picking | `RESTORED` | 3D clicks select visible entity geometry by stable ID; the outliner covers non-rendered groups/lights, and interactive entities retain their authored action |
| Add primitives / meshes | `RESTORED` | Friendly UI and agent API create primitives, groups, lights, splines and recovered models with optional physics |
| Transform gizmos | `RESTORED` | Snapped translate/rotate/scale, W/E/R switching and world/local space commit through ordinary v2 updates |
| Materials/textures | `RESTORED` | The shared editor exposes color, emissive/glow, roughness, metalness, opacity and a compact visual browser for eleven recovered semantic textures with repeat, offset and rotation; agents discover and apply the same stable texture IDs through `gx.textures()` |
| Triggers/moving parts | `PARTIAL` | Behaviors and labeled visibility interactions are authorable; add a friendlier multi-step trigger composer only when a real scene needs it |
| Splines/lights/NPCs | `PARTIAL` | Splines and three light types are authorable; an explicit NPC/agent entity contract remains |
| Load/save/import | `RESTORED` | Complete v2 JSON download/import, named snapshots, and archived GraphysX serializer XML migration share the validated runtime; unresolved custom meshes remain explicit proxy warnings rather than fabricated assets |

## CubX / Atmel recovery

| Item | Status | Remaining work |
|---|---|---|
| CubZ rotate/open/close state machine | `PARTIAL` | Interactive and visibly driven by decoded `CubeRot.tva`/`CubeOpen.tva`: seven exact rotation ranges at 30 fps, the special cube-0 direct-open path, exact Right08/Top08/Left08 50-frame panel tracks, action selection, close and same-range reverse repair. After the source clip, a disclosed stable 2×2 inspection layout keeps all four menu panels readable; the older CubXActor `CubXOpen` is not substituted |
| Original CubX/CubZ meshes and button models | `PIPELINE` | Replace procedural boxes/panels with decoded `CubXMesh.tvm`, `CubXBtn*.tvm` and `CubXBtn/Btn*.tvm` assets |
| Earth/grid object | `PARTIAL` | Replace proxy/material shortcuts with exact Earth/cubx models and the recovered 8192×4096 Earth source |
| Clock/domotic panels | `PARTIAL` | Eight I/O channels and four schedules now function in Input & Device Lab; replace CubX Hub proxies with exact MenuDomo, 3D alphabet/clock, light/fan, and status-sphere assets |
| FlightX Pipe Flight | `PARTIAL` | Archive input semantics, 14-gate route, FlightX HUD/debug state and deterministic completion work; research exact original mission/lap intent and complete human handling/balance QA |
| CubX Notes Manager subsystem | `PARTIAL` | Exact 50-slot marble block and add/reset interaction are live. The referenced GUI layout and any note text/edit/delete/save/load model are absent; recover evidence before expanding behavior |
| CubX screensaver | `PARTIAL` | Verified menu-idle camera orbit starts after 10 seconds and advances at the archived 5°/s; decode the TVA animation and restore aim/radius/settings behavior |
| 21 TVA animations | `PARTIAL` | CubeRot/CubeOpen plus all 16 byte-distinct CubXActor TVAs are decoded with exact geometry, hierarchy, ranges and stored keys. The separate CubXActor inspector exposes Get2–8, Rot1–7, full/solo Open, terminal holds, click meshes and the broken eighth slot; generalized rig/handedness validation remains |
| TVMO `gran.tvm` / `house.tvm` | `RESEARCH` | Add old-header decoder support |

## Input & Device Lab

| Item | Status | Remaining work |
|---|---|---|
| BallZ18 input monitor | `RESTORED` | Live axis 0 / inverted axis 1, 0.19 deadzone, x100 values, 145px ring, 25px pointer and Fire1/2/3/Jump held-state lamps use byte-identical source graphics |
| Protocol simulator | `RESTORED` | Five profiles preserve PhysX 4-byte, BallZ18 COMOK, Scene3D 5-byte, MeArm text, and AtmelCubx ASCII/raw frames in a bounded TX/RX log |
| Robot / sonar | `RESTORED` | Commands 2/4/6/8, source pin 8, identify, single sonar and both inclusive passes of the 182-point sweep are live in simulation |
| Atmel I/O / schedules | `RESTORED` | Eight D/E channels and four C+alarm+HHMM+HHMM+state schedules are visible and interactive |
| MeArm | `RESTORED` | Source defaults, z/x/c/v commands, 0–180 clamps, ARM/STOP gating and the archived left/right/claw assignment repair are live |
| Physical serial transport | `PIPELINE` | Add explicit Web Serial port picker only after cancellable read loop, partial-frame handling, bounded queue/rate, visibility/unload cleanup and real hardware QA; never scan or auto-open |

## Content and fidelity backlog

| Area | Status | Remaining work |
|---|---|---|
| TVM decoder | `PARTIAL` | 183/185 decode; MATT texture tables still need parsing |
| X decoder/converter | `PARTIAL` | Good for current text X assets; needs batch scene pipeline and material validation |
| 3DS decoder | `PARTIAL` | Geometry/UVs work; material-face assignments and pivots need fuller parsing |
| TVA decoder | `PARTIAL` | CubeRot/CubeOpen and all 16 CubXActor variants have verified ranges, hierarchy, geometry and transform keys; generalize the pipeline beyond these families and validate TV3D-to-Three rig mapping |
| TVMO variant | `MISSING` | Two models blocked |
| Archive texture provenance | `PARTIAL` | Continue source-to-runtime mapping and prevent generic substitutions |
| Audio/music | `PARTIAL` | BallZ18 countdown uses exact beepShort/beep01; BallZ ring pickup and jump use exact `coin.wav` and `Jump.wav`. Review `intro.mp3` / `Piano.mp3` provenance and rights before enabling the longer music tracks |
| Characters/creatures | `MISSING` | Woman, vampire, bat, fish, mech, tank, droid and others |
| Props/toys | `PARTIAL` | Some catalog display; most are not usable scene objects |

## Product and production

| Item | Status | Remaining work |
|---|---|---|
| Main information architecture | `PARTIAL` | All major labs and every recovered-world progression entry have direct, source-separated routes; the complete 28-mode browser is collapsed contextually. Continue reducing Home density and split the remaining oversized family index |
| Pause/settings | `PARTIAL` | Gameplay pause is visible and deterministic; audio, graphics, remapping and broader accessibility settings remain |
| Gamepad/mobile controls | `RESTORED` | Standard gamepad mapping and an on-screen touch dock drive the shared input controller; browser QA covers movement, pause and mobile build visibility |
| Human play balancing | `PARTIAL` | Automated completion passes, but broader manual timing/feel tests remain |
| Permanent regression tests | `PARTIAL` | Existing scripts/artifacts need stable checked-in test commands |
| Performance budgets | `MISSING` | Bundle, draw calls, particles, large worlds and mobile targets |
| Error/retry UX | `PARTIAL` | Legacy race loading handles errors; other lazy content does not |
| Save migration | `MISSING` | Versioned schema |
| Release/version bookkeeping | `RESTORED` | Visible desktop/mobile badge and text state report `revival-2026.07.18-r13`; the cumulative source, focused browser, integrated matrix and required game-client verification history is recorded in `web-prototype/progress.md` |
| Attribution/recovery notes | `PARTIAL` | Source paths exist in places; needs a complete in-product credits/provenance view |
| Git baseline | `MISSING` | Repository content is largely untracked; preserve user files and establish an intentional baseline later |
| Deployment | `MISSING` | Static build is ready in principle; hosting/release process not established |

## Verification gates for every future “restored” claim

1. Source/provenance is identified.
2. The feature is categorized under the correct project family.
3. It is reachable from the UI without hidden debug hooks.
4. It is interactive when the archive idea was interactive.
5. It has a readable UI/help surface.
6. Desktop and mobile layouts are checked.
7. A gameplay or interaction screenshot is visually inspected.
8. `render_game_to_text` exposes relevant live state.
9. Browser console is clean.
10. `npm run build` passes.
