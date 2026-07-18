# Récupération Ledger — the final deep dig (2026-07-16)

Everything recoverable found in the three-front code excavation (AtmelCubx game, Yanik C++ BCKUP,
GraphysX 2016/GNet/Console/shaders), with verbatim constants. Companion to ARCHIVE_REVIVAL_TRACKER.md /
ARCHIVE_SCENE_CENSUS.md / FORGOTTEN_FEATURES.md — only NEW material is listed here.

## 0. Corrections to the record

- **Skybox axis swap (owner-reported) — root cause found & fixed.** `AtmelCubx/Atmosphere.cpp:26-31` is the authoritative face map: `north→LEFT, south→RIGHT, west→FRONT, east→BACK` (not the compass-literal mapping). LostValley re-converted accordingly (public/assets/sky/lostvalley). If any other sky set still looks one-axis-off, this file is the reference.
- **Folder dating lies:** the "BallZ 2011" folder is a **2015-10-21 build** (per its TV3D debugfile). The genuine 2011 ball is in `cpp/` — and it's kinematic (position-nudge), with an `int iMoveFactor = 0.9` truncation bug that means it barely moved. Two different ball systems, not iterations.
- **No scoring and no audio ever existed** in the 2008-2015 native generations. The "score" in the CubX screenshot is the 3D clock font. Current web audio/medals are new inventions — correctly so.
- **Archive/GNet is NOT networking** — it's "Gfx.NET", an earlier C++/CLI bridge snapshot (WinForms PictureBox hosting). Zero socket code anywhere in it. The only real network code is the AtmelCubx TCP cars skeleton (below).
- **PisteOvale was never wired into CarScene** — the 2008 car drove on `CarHeightmap.bmp` terrain. Rings/laps/score in CarScene: never implemented. The current web oval race is already beyond the archive.

## 1. Car handling — the authentic Subaru tuning (`AtmelCubx/Vehicule.cpp`)

Chassis: mass **1500**, convex hull, center-of-mass offset **(0, −1.0, +10.0)** ("make it drive better").
Suspension per wheel: height **0.5**, length **1.0**, shock **40**, spring **300**.
Wheels: mass **25**, radius **0.5**, width **0.372**; offsets FL(−0.8, 1.25) FR(0.8, 1.25) RL(−0.8, −1.425) RR(0.8, −1.425).
Wheel friction: sideslip **0.1**, maxlongslide **10000** (anti-slope-slide trick).
Chassis↔terrain: friction **0.3/0.17**, bounce **0.1**, softness **1000**; terrain side friction **0.9/1.0**.
Engine: power **3000**, **rear-wheel drive** (torque on RL/RR only), brake = −power on rears.
Steering: **stepped ±10°/frame, clamped ±45°, snaps to 0 when released** — that's the arcade feel.
Handbrake: rear wheels, force 2000. Gravity **(0, −9.8009, 0)**. Reset keys: P (upright+0.2y), O (teleport 0,5,0).
Camera: `ChaseCamera(chassis, offset(0,5,−15), stiffness 1000)`.
→ Apply to the web Impreza: RWD not AWD, stepped steering with snap-back, CoM shift, stiffness-1000 chase cam.

## 2. Ball feel — the "classic 2015" preset (`BallZ 2011/CLBallZ.cpp` — really 2015)

Mass **1250** ("hardwood"), world gravity **−25** (not −9.8 — THE tuning decision, comment trail proves it),
roll torque **fTORQ = 5000**, Newton axis remap `(z, y, −x)`, steering rot speed **0.2**,
**angular-velocity brake: ×0.5 per frame when no input and |ω| > 3** (this is why the old ball stopped so crisply),
jump = single local impulse (−10,0,0). Chase cam offset **(0,25,−50), stiffness 100**.
→ Port as a selectable "Classic 2015" physics preset alongside the current web feel. The ω-brake is the signature.

## 3. CubX menu — the real choreography (`AtmelCubx/CubX.cpp`, `CubXScene.cpp`, `Screensaver.cpp`)

States: SLEEPING→POSITIONNING→FOCUSING→SELECTING→OPENING→OPENED (CLOSING/DESELECTING never finished).
Idle spin **(20,20,20)°/s**; snap-home spin **(80,80,80)**; camera fly-in **(−10,5,10)/frame** to clamp (−75,725,−350).
8 buttons (CubXBtn1-8.tvm), per-face grab anims CubXGet2-8.tva, open anim CubXOpen.tva (poll keyframe>99 — IsAnimationFinished was broken).
Menu satellites in world space: CubX(−400,500,0) size 4, SysCube(−800,300,−250), ToolsCube(−1100,300,−500),
Earth+spinning grid(800,500,350), arrow/back(1000,600,300), red sun box(0,500,0) → launches FlightX.
**Screensaver: after 10 s idle, camera orbits the CubX at 5°/s, radius 1000, height 500** — the floating-menu memory.
Menu levels: MenuManager with per-button level tags (0 main, 666 ALLMENU, 777 ALL_EXCEPT_MAIN), 48 buttons/16 levels max.
→ Web status 2026-07-17: the source-coordinate satellite shell is live with System/Tools/Earth/Grid at level 0, Arrow at ALLMENU 666, and the Sun always available. Fleche uses decoded geometry; Box/Earth/Sphere and their original icon textures remain disclosed procedural adapters pending converter output. Tools opens the recovered one-car Car Scene, Earth stays source-faithful inspect-only because its handler is empty, Arrow restores level 0, and Sun launches unchanged `flightx-pipe`. CubZ keeps its separately decoded CubeRot/CubeOpen state machine; the older CubXActor snap/open family remains isolated in its lineage inspector.

## 4. Two whole minigames not yet in the web build

- **FlightX** (`FlightXScene.cpp`): quaternion airplane through pipe1.tvm (already decoded!). ROLL 0.9, PITCH_UP 0.4,
  PITCH_DOWN 0.5, SPACE airbrake +0.1, W/S throttle, R reset, chase cam (0,250,−800). Launched by clicking the sun.
- **BallZ slide** (`BallZ.cpp`/`Level.cpp`): ball (mass 5000, impulse ±100) on slide1.tvm at (0,−5000,0) with the
  100-ring crooked spiral (x=i·10, y=100−i, z=i·10 — "croche au démarrage pour le fun"). Slide friction 1.0/0.9;
  ball friction 0.2/0.075. Assets all decoded already (Slide1, ring, Ball trio).

## 5. Atmosphere math — the original day/night (`AtmelCubx/Atmosphere.cpp`, `Clock.cpp`)

Sun orbit R=1000, y-offset 200, full day = 86400000 virtual ms at timescale 2000 → **~43 s day**.
Sky cross-fade alpha = (sunY+R−offset)/(2R), ×2 capped 1. Sun brightness **logistic 1/(1+e^(−(a−0.6)/0.05))**;
ambient = a·0.2. Water: height 85, DUDV distortion (×150), bump speed 0.5, plane d=−height−0.75, 16×16 grid.
→ Replace the web's ad-hoc sun cycle with these exact curves; add the water plane to Scene Lab.

## 6. Shader pack — 6 files, ready recipes (`common/shaders/`, `#23 HLSL_Shadow_Mapping`)

- `meshdeep.shade` + `meshlight.shade`: **cubic (point-light) shadow mapping** — 1024 HDR float16 cube RT,
  distance-in-R depth, **8-sample PCF, BIAS 0.98**, plus parallax bump (TBN) and "Lyon" specular. Three.js: CubeCamera
  depth pass + custom ShaderMaterial. Closes tracker "shaders PARTIAL".
- `post_haze.shade`: screen-space haze → EffectComposer pass. Closes tracker "haze MISSING".
- `inoutpost.shade`: full-screen distortion transition `Scene(uv + (1−distortTex))` — scene-change wipe.
- `ppl.shade`, `inoutobject.shade`: parallax per-pixel lighting / plain textured — covered by standard materials.

## 7. Schemas & architecture worth honoring

- **EntityNET/SceneNET** (`GfxNet/`): the canonical level contract — 15 entity types (incl. PHYSICXSTATIC),
  physmats (DEF/WALL/FINISH/GROUND/BALL/HUMAN), materials (TRANSLUCENT/GLASS/MATE/METAL_SHINE), MeshControlled flag,
  ringPosList, and the four lap-gate corner points (fS/fF/hS/hF) matching LapChecker. → the JSON LevelDefinition enums.
- **GraphysX (2016)**: the owner's own final architecture — IScreenApp/ScreenList/IScreen3D with
  ScreenState{NONE,RUNNING,EXIT,NEXT,PREV} — exactly the shape the web GameStateMachine should keep mirroring.
- **`shotBullet(tex,pos,dir,scale,radius=0.15,physMat,speed=500,mass=1.0,life=1000)`** (ConsoleGraphysX): ready spec for Shooting.
- **tvScreen.cs** verbs: `c`=camera-mode switch, `d`=Newton debug wireframe, `s`=screenshot, Info inspector on pick →
  four small editor/debug features with exact legacy semantics.
- **Scene-architecture note** (`Nouveau Document texte.txt`, 2008): scene base class + Area array + COMM layer — he
  designed the modular scene manager three generations before building it.

## 8. Smaller finds

- Elevator path loop (`Elevator.cpp`): CTVPath square (0,20,0)→(100,20,0)→(100,20,100)→(0,20,100), speed 20.
- Snow + smoke CPU particle math (`ParticleEng.cpp`): smoke grows `2.5/H·(y−y0)+0.5` and fades `255−255/H·(y−y0)` — ready curves.
- TCP cars skeleton (`CarsManager/TCPComm`): 4 cars, port 33333, STEngineControl-per-car sync design (payload still "Alouette") — the multiplayer blueprint, greenfield beyond that.
- 3D alphabet WriteString: 55·size letter spacing (the glyphs are already decoded in the web catalog).
- Domotic data model: per device (Light, Fan): On/Off HHMM + enable; Atmel 4-byte serial protocol `[16,cmd,pin,value]`
  (127 set / 128 hello / 129-130 sonar / 2-8 drive / 255 off) — PhysXRobot drove a REAL robot. Archival gold; not web.
- `rancor.tva` + room/sky/ballcube in common/ — assets known; no gameplay code exists for them (author fresh).

## 9. Revival plan (proposed phases)

**Phase R1 — Feel authentique (highest nostalgia/effort ratio)**
1. Classic 2015 ball preset (mass/g/torque/ω-brake) as selectable physics mode
2. Subaru handling from §1 (RWD, stepped steering, CoM, cam stiffness) on Piste Ovale
3. Original day/night curves + ~43 s day option (§5)
4. Ghost/last-lap spline (already specced in FORGOTTEN_FEATURES #1)

**Phase R2 — The two lost minigames — DONE 2026-07-16**
5. ~~FlightX~~ — "FlightX — Pipe Loop" race: pipe1 laid flat as the circuit, kinematic quaternion flight with legacy ROLL 0.9 / PITCH 0.4-0.5 (+0.1 Space airbrake), Shift throttle, R resets wings; rings on the loop, ghost/records work. **Sun-click launch DONE 2026-07-17** — the red CubX control is raycastable and launches the unchanged `flightx-pipe` id.
6. ~~BallZ slide~~ — "The Great Slide — Level0": SlideLarge.TVM (the real 2011 Level0) rotated onto the race axis, 116-unit descent, terrain-sampled ring chain, fall-off respawn.

**Phase R3 — Look authentique — DONE 2026-07-16 (partials noted)**
7. ~~Haze~~ — EffectComposer + ShaderPass porting post_haze.shade exactly (Scene(uv + (1−distort))), driven by the RECOVERED haze.dds, enabled on atmosphere races. Cubic soft shadows: covered natively (renderer already runs PCFSoft shadow maps — the meshlight.shade recipe is only needed if pixel-matching the 2008 look). ~~Distortion scene-transition (inoutpost)~~ DONE 2026-07-17 — race-start wipe: Scene(uv + (1−distort)·amount) with squared decay and a brief darken, driven by the recovered haze texture. `playTransitionWipe()` is public — the CubZ session can fire it on menu→race handoffs too.
8b. ~~Water REFLECTION~~ DONE 2026-07-17 — a real planar Reflector (512² mirror pass, the legacy reflect-to-texture technique) now lies under the ripple film on Skybox Spiral and the Port Dominus bay; ripple film thinned to alpha 0.52 so the mirrored sky and skyline show through. This closes the CLWater reflect/refract item — refraction skipped as the visible payoff is the mirror.
8. ~~Water~~ — CLWater on Skybox Spiral: dual-layer ripple shader driven by the recovered distortiontexture.dds, legacy bump speed 0.5, alpha 0.78.
9. Screensaver idle orbit DONE (10 s idle → 5°/s camera orbit, menu-wide). **CubX choreography evidence reconciled 2026-07-17** — idle spin 20°/s per axis and source camera `(400,250,-2000)` stepping `(-10,+5,+10)` per frame to clamped `(-75,725,-350)` are live. The recovered 80°/s snap-home belongs to the older CubXActor state machine and is recorded without being forced onto CubZ's separate exact CubeRot reverse range. The global screensaver remains the single idle-orbit owner. **Satellite shell DONE** with exact source coordinates/menu tags and explicit geometry adapters as documented in §3.

**Phase R4 — Systems — DONE 2026-07-17**
10. ~~Schema~~ — `src/legacy/level-schema.ts`: the full EntityNET/SceneNET contract as TypeScript (15 entity types, physmats, materials, LapChecker two-point gates).
11. ~~Shooting~~ — F fires the ConsoleGraphysX shotBullet spec (radius 0.15, mass 1.0, life 1000 ms) in ZombieKiller; bullets squash zombies on hit, with a fire cue. ~~Newton-debug~~ — N toggles the physics-style wireframe view (tvScreen.cs 'd'). Camera-mode key: KeyC already lives in the current build (camera reset) — legacy free-cam switch still open.
12. ~~3-lap + ring bonus~~ — Suzanne 1 now runs GamePlayScreen.h mode 2: 3 laps (`nbrTour = 3`), rings optional, each ring −10 s off the final time, "LAP 2/3" and "BONUS" called in alphabet glyphs. **Ball selector DONE 2026-07-17** — CubZ exposes persistent Fire / Classic / Revival choices; Fire uses recovered BallFire geometry with Revival physics, Classic carries the recovered 2015 gravity/brake feel, and Revival retains the modern web handling.

**Phase R5 — Beyond the archives — IN PROGRESS 2026-07-16/17**
13a. ~~Dominus village~~ — "Port Dominus — Visite": 55 meshes + 38 original textures (tga→png) as a walkable port town with landmark ring tour, bay water, fish, and the woman by the pub. (Vampire benched: 65k verts.)
13b. ~~Second car~~ — "Piste Ovale — Low Cobra": Low_Cobra.3DS normalized (rotated onto Z, ×0.024), Cobra_Blue livery, four tires as bodies, shares the authentic handling.
13c-DONE 2026-07-17. **.tva FULLY decoded** — `tools/convert-tva.mjs` → `src/legacy/cubx-anim.json`: TRS keyframe tracks (pos/rot/scale, keys 0→100) per named node for CubXOpen, CubXRot1, CubXGet2, CubeOpensolo. All 76 tracks parsed with zero failures. Later lineage forensics proved this older CubXActor family is separate from CubZ's `CubeRot.tva` / `CubeOpen.tva` family, so it is preserved in the dedicated CubX Actor Lineage inspector rather than silently supplying CubZ node identities. (Earlier structural notes: `"Dd` header → MSTR string table, ASTA counts (embeds the actor mesh: 2264v/2750t = CubXMesh), MAG3 = 46 keyframes of 4×4 node matrices; node names Scene_Root, Boîte01-08, Cylindre01-03. Also confirmed: TVM has NO texture names — binding was filename convention (90Right.TVM ↔ 90Right.png).
13d. TCP cars → future online: still design-only.
13f. ~~Level1 2011~~ DONE 2026-07-17 — "Level1 — The 2011 Gauntlet": the 1135-unit mega-mesh (largest single mesh in the archives) scaled ×0.09 into a 100-unit narrow canyon race, 45 units of vertical, summit start, clearnight sky. First time this level has ever been raced. Complements the parallel session's museum-visit lab of the same mesh.
13e. ~~Heightmap terrain~~ DONE 2026-07-17 — "Terrain 2008 — Impreza": Heightmap.bmp (96×96 sample, 100% sculpted) as a 110-unit grass hillclimb under the Subaru, shared visual/physics grid, ridgeline ring tour, LostValley sky. **Archive finding:** CarHeightmap.bmp — the map CarScene actually loaded — is entirely black; the 2008 car genuinely drove on a flat plain. (Also: that BMP's header understates its file size; the converter tolerates it.)
Note: the parallel session has meanwhile added skybox-selector, car-selector, common-room, Level1-visit, Slide1-visit, MyWorld-XML, notes-manager, milky-way labs — the census is becoming the app.
