# Particle / Effect archive audit

## Outcome

The archive contains a particle/effect subsystem, not a hidden collection of particle "worlds." The authored line is:

1. a generic TVP runtime (`CLEffect`),
2. an `Explosion1.tvp` convenience wrapper (`CLExplosion` / `CLParticleEngine`),
3. reusable programmatic emitters for rings, ASCII effect cells, smoke and one later Human trail,
4. a library of TV3D Particle Editor presets,
5. animated flame/explosion billboard frames that are adjacent visual assets but not particle systems.

The old DirectX `ParticleEng` tutorial port and the 2008 `Particle Engine Proj.pdf` are reference/experimentation evidence. Neither should be promoted to an authored GraphysX scene.

## Scene versus reusable-feature classification

| Archive implementation | Evidence | Classification | Was it actually active? | Revival status before this pass | Result of this pass |
|---|---|---|---|---|---|
| `CLEffect` | Seven byte-identical `Effect.cpp` copies, SHA-256 `A9840E2B…ED6EB2B` | Generic TVP loader/runtime | Yes; CubX constructs it with `Explosion1.tvp` at `(0,500,0)` | No source-faithful generic TVP runtime | Audited; reusable feature, not a scene |
| `CLExplosion` | `Explosion.cpp`, later folded into `ParticleEngine.cpp` | One-shot wrapper for `Explosion1.tvp` | Yes; created in Area/TV3DMoteur, started by clicking the Sun/System cube, and started once by `CLParticleEngine` | Generic modern bursts only | Exact preset configuration now has an isolated deterministic adapter |
| TVP resource library | 19 `.TVP` files / 17 unique binary hashes | Reusable effect presets | Only `Explosion1.tvp` has a located GraphysX callsite; the rest are library resources | Not revived | Fully inventoried; not mislabeled as scenes |
| Ring pickup particles | `Anneaux.cpp` / `Anneaux.h` | Gameplay feature inside BallZ levels | Yes, on pickup | Visible modern pickup bursts, but synthetic trajectory/material | Still PARTIAL |
| ASCII `s` emitters / `createSmoke` | `GraphysX_1/Scene.cpp` and later `SceneEditor.cpp` | Level/editor feature, including Suzanne effect cells | Yes | Suzanne candidates expose exact emitter counts/colors/capacities, with deterministic inferred point placement | PARTIAL, correctly disclosed |
| Later Human smoke trail | external `GraphysX/GraphysX/Human.cpp` | Actor-attached feature | Yes when a Human is constructed with a texture | No exact trail | PIPELINE feature |
| `createFire` | `Scene.cpp`, `GamePlayScreen.cpp` | Animated billboard feature, not a particle emitter | Yes | Flame billboards are visible in Engine & FX | PARTIAL |
| TV3D port of `ParticleEng` | `ParticleEng (2).cpp`, SHA-256 `6D33EF11…6254C3` | Tutorial-derived engine experiment | Construction/update blocks are commented out in `TV3DMoteur.cpp`; only Snowfall and Smoke were ported | Not revived | Audited; no new scene claim |
| Original DirectX `ParticleEng.cpp` | Copyright Michael Fötsch, 2000 | External tutorial source | No compatible active host in GraphysX | Not revived | Reference only |
| `Particle Engine Proj.pdf` | SHA-256 `1AE19413…630B4`, author Prakash A Arul | External project report/reference | No source/callsite link to GraphysX located | Not revived | Reference only |

The current Engine & FX lab's three orange/cyan/violet fountains prove the modern engine can draw persistent particles, but their settings are new presentation settings. They do not revive any one archived emitter configuration. The new isolated `Explosion1` environment is the first exact-config recovery, while remaining `PARTIAL` because the closed-source TV3D RNG/integrator and blend implementation do not survive.

## Runtime semantics recovered from the TV3D SDK

`CTVParticleSystem.h` establishes these meanings:

- `SetEmitterPower(power, particleLifetime)` controls launch power and lifetime.
- `SetEmitterSpeed(milliseconds)` is the particle generation interval.
- emitter shapes are 0 point, 1 sphere volume, 2 box volume, 3 sphere surface and 4 box surface.
- particle-change values are 1 alpha, 2 color and 3 none.
- blend values are 0 none, 1 alpha, 2 add, 3 color, 4 add-alpha and 5 multiply.
- each emitter can specify main/random direction, gravity, alpha test/reference/depth write, looping, and absolute-time particle size/color keys.
- `CLEffect::Render` disables emitter looping after its duration, follows an optional attached mesh plus offset, and marks completion only after a nonzero particle count has returned to zero.
- `CLExplosion` guards the startup-zero case by waiting eight updates before accepting an empty system as finished.

## TVP / TVPJ library census

The resource folder has 19 compiled filenames but 17 unique binaries. `explosion1.TVP` is byte-identical to `explosion_01.TVP` (SHA-256 `99F3AD87…60F5E`), and `explosion2.TVP` is byte-identical to `explosion_02.TVP` (`2767DFD9…35B88A`). There are 16 readable `.TVPJ` documents containing 29 emitter configurations and one attractor. Fourteen readable documents pair with a compiled binary; `expl1.TVPJ` and `fire1.TVPJ` have no adjacent compiled TVP. Three unique binaries (`explosion3.TVP`, `impact1.TVP`, `shot1.TVP`) have no readable TVPJ and therefore remain opaque instead of being guessed.

### Every readable emitter configuration

Notation: `max@interval`; `P/SV/BV/SS/BS`; `direction ± random × power`; `g` gravity; `life`; `change/blend` use the enum names above. Key times are seconds, with `size : rgba`.

| Preset / emitter | Spawn and update | Texture | Lifetime and render | Particle keys |
|---|---|---|---|---|
| `expl1` E0 | `100@10ms`; P (radius 6 and box 5/20/5 are serialized but unused by P); `0/0/0 ± 1/1/1 × 1`; no g | `clumpy_blurry.dds` **missing** | 1s; alpha/add-alpha; alpha test 0; no depth; loop | `0 0.05:1,1,0,0`; `0.4 0.05:1,0,0,1`; `0.75 0.05:0,0,1,0` |
| `explosion_01` E0 | `10@10ms`; P; `0/10/0 ± 20/10/20 × 40`; g `0/-50/0` | `Glow2.dds` | 2s; color/add-alpha; alpha test 0; no depth; one-shot | `0 8:0,0,0,0`; `1 4:1,.75,0,1`; `2 2:1,0,0,0` |
| `explosion_01` E1 | `64@10ms`; SV r3; `0/5/0 ± 5/5/5 × 10`; g `0/-5/0` | `Glow2.dds` | 3s; alpha/color-blend; alpha test 0; no depth; one-shot | `0 16:1,1,1,1`; `2 48:.7,.5,0,1`; `3 128:.1,.05,0,0` |
| `explosion_01` E2 | `8@10ms`; SV r3; `0/-5/0 ± 2/10/2 × 5`; g `0/10/0` | `smokey.dds` | 4s; alpha/alpha-blend; no alpha test/depth; one-shot | `0 16:.25,.125,0,.1`; `2 16:.2,.2,.2,.4`; `4 0:.5,.5,.5,0` |
| `explosion_02` E0 | `64@1ms`; P (r1 serialized); `0/0/0 ± 1/1/1 × 25`; g `0/-1/0`; attractor below | `Glow2.dds` | 3s; no-change/add-alpha; alpha test 0; no depth; loop | `0 8:1,1,1,1`; `2 24:.3,.2,0,.5`; `3 48:.05,.025,0,0` |
| `explosion_03` E0 | `16@2ms`; SV r4; `0/0/0 ± 10/1/10 × 10`; g 0 | `Glow2.dds` | 3s; no-change/add-alpha; alpha test 0; no depth; loop | `0 8:1,1,1,1`; `1 16:.3,.2,0,.5`; `2 0:0,0,0,0` |
| `explosion_03` E1 | `8@2ms`; P; `0/.5/0 ± 1/1/1 × 30`; g `0/-30/0` | `smokey.dds` | 3s; alpha/no-blend; alpha test 0; no depth; loop | `0 2:1,1,1,1`; `.1 1:0,0,0,1`; `3 0:0,0,0,0` |
| `explosion_03` E2 | `16@1ms`; P; `0/2/0 ± 10/1/10 × 100`; g `0/-50/0` | `Glow2.dds` | 3s; alpha/color-blend; alpha test 0; no depth; loop | `0 1:1,1,1,1`; `.1 2:1,.85,.4,1`; `3 0:0,0,0,0` |
| `explosion_03` E3 | `16@1ms`; SV r12; `0/0/0 ± 1/1/1 × 10`; g 0 | `Glow2.dds` | 3s; alpha/add-alpha; alpha test 0; no depth; loop | `0 48:0,0,0,0`; `.3 48:.75,.5,.25,.25`; `3 96:0,0,0,0` |
| `explosion_03` E4 | `1@1ms`; P; stationary; no g | `circle_soft4.dds` | 3s; no-change/add-alpha; alpha test 0; no depth; loop | `0 0:0,0,0,0`; `.1 32:.75,.75,1,1`; `1.5 8:0,0,0,0` |
| `fire1` E0 | `1000@1ms`; P (r20 and box 5/20/5 serialized); `0/0/0 ± 1/1/1 × 2`; no g | `smokey.dds` | .75s; alpha/alpha-blend; alpha test 0; no depth; loop | `.1 1:0,0,1,1`; `.4 .5:.8,.6,.1,1`; `.75 0:1,1,1,1` |
| `firetrail_01` E0 | `1000@10ms`; P; `0/0/0 ± .5/.5/.5 × 5`; no g | `Glow2.dds` | 1s; alpha/add-alpha; alpha test 0; no depth; loop | no particle keys; default size 8, color `1,.5,0,1` |
| `firetrail_02` E0 | `1000@10ms`; P; `0/0/0 ± 1/1/1 × 1`; no g | `Glow2.dds` | 1s; alpha/add-alpha; alpha test 0; no depth; loop | `0 1.5:1,.75,.5,.75`; `1 .5:.75,.25,0,.1` |
| `gatling_01` E0 | `3@1ms`; P; `0/0/1`; power 0; g 0 | `circle_soft4.dds` | .1s; no-change/alpha; alpha test 0; no depth; loop | `0 .45:1,.75,.25,1`; `.1 0:1,.75,0,0` |
| `gatling_01` E1 | `1@1ms`; P; stationary; g 0 | `Glow2.dds` | 1s; no-change/no-blend; alpha test 0; no depth; loop | `0 .45:0,0,0,1`; `1 .1:0,0,0,1` |
| `gatling_01` E2 | `16@100ms`; P; `0/0/10 ± 1/1/0 × 1`; g 0 | `Glow2.dds` | 1s; alpha/add-alpha; alpha test 0; no depth; loop | `0 .6:1,.75,.25,1`; `1 0:0,0,0,0` |
| `hit_01` E0 | `3@10ms`; P; `0/0/0 ± 5/1/5 × 50`; g `0/-250/0` | `Glow2.dds` | 1s; alpha/add-alpha; alpha test 0; no depth; loop | `0 1:1,1,1,1`; `.125 2:1,1,0,0`; `.5 0:0,0,0,1` |
| `hit_01` E1 | `5@5ms`; P; `0/0/0 ± 2/2/2 × 5`; g `0/50/0` | `smokey.dds` | 1s; alpha/no-blend; alpha test 0; no depth; loop | `0 4:1,.85,.6,.75`; `1 8:0,0,0,0` |
| `hit_01` E2 | `1@1ms`; P; stationary with power 1; g 0 | `star1.dds` | 1s; alpha/alpha-blend; alpha test 0; no depth; loop | `0 8:1,.9,.8,1`; `.5 0:0,0,0,0` |
| `lightningball_01` E0 | `10@100ms`; P; `0/0/0 ± 1/1/1 × 4`; g `0/-5/0` | `star1.dds` | .5s; alpha/add-alpha; alpha test 0; no depth; loop | no keys; default size 2, white |
| `lightningball_01` E1 | `3@100ms`; P; `0/0/0 ± 1/1/1 × 6`; g 0 | `star1.dds` | .1s; alpha/add-alpha; alpha test 0; no depth; loop | no keys; default size 6, white |
| `plasmatrail_01` E0 | `1000@5ms`; P; `0/0/0 ± .0125/.0125/.0125 × 0`; no g | `ring_blur.dds` | 2s; alpha/add-alpha; alpha test 0; no depth; loop | `0 1:1,.75,1,1`; `.25 .5:.1,0,1,.5`; `2 0:0,0,0,0` |
| `shot2` E0 | `100@200ms`; P; `0/0/0 ± .001/.001/.001 × 1`; no g | `circle_soft4.dds` | 2s; alpha/alpha-blend; no alpha test/depth; loop | `0 2:1,1,0,1`; `1 2:1,1,0,1`; `2 1:1,0,1,0` |
| `shot3` E0 | `100@10ms`; P; `0/0/0 ± .001/.001/.001 × 1`; no g | `circle_soft4.dds` | 1s; alpha/alpha-blend; no alpha test/depth; loop | `0 2:1,1,0,1`; `.5 2:1,1,0,1`; `1 1:1,0,1,0` |
| `smartbomb_01` E0 | `1@1ms`; P; stationary; no g | `star1.dds` | 3s; alpha/add-alpha; alpha test 0; no depth; loop | `0 0:0,0,0,0`; `.1 512:1,1,1,1`; `1 512:0,0,0,0` |
| `smartbomb_01` E1 | `64@1ms`; P; `0/0/0 ± 1/1/1 × 200`; g 0 | `star1.dds` | 3s; alpha/alpha-blend; alpha test 0; no depth; loop | `0 0:0,0,0,0`; `.1 8:1,1,1,1`; `3 0:0,0,0,0` |
| `smoketrail_01` E0 | `1500@10ms`; P; `0/0/1 × 10`; g 0 | `Glow2.dds` | 1s; alpha/add-alpha; alpha test 0; no depth; loop | `0 .5:.3,.3,.3,.3`; `1 0:0,0,0,0` |
| `trail_01` E0 | `100@10ms`; P (r1 serialized); `0/0/1 ± 0/0/1 × 15`; no g | `Glow2.dds` | 1s; alpha/add-alpha; alpha test 0; no depth; loop | `0 2:.75,.75,1,.5`; `1 0:0,.125,1,0` |
| `trail_01` E1 | `32@100ms`; P; `0/0/1 ± 1/1/.5 × 1`; g `0/0/20` | `Glow2.dds` | 2s; alpha/add-alpha; alpha test 0; no depth; loop | `0 2:1,.5,0,1`; `1 0:1,1,1,0` |

`explosion_02` also has one non-directional attractor at `(0,0,0)`, radius 25, field direction `(0,1,0)`, repulsion constant `-2.5`, constant velocity dependency `1`, and zero attenuation.

### Texture resolution

| Texture | Archive status | SHA-256 |
|---|---|---|
| `Glow2.dds` | present, 65,664 bytes | `6DCF8614…135B0` |
| `smokey.dds` | present, 87,508 bytes | `C1B60429…29A75` |
| `circle_soft4.dds` | present, 87,508 bytes | `C3B35ADF…604F8` |
| `star1.dds` | present, 87,508 bytes | `78A4BFD7…AD21` |
| `ring_blur.dds` | present, 87,508 bytes | `6D4F79A4…F61AA` |
| `clumpy_blurry.dds` | missing from the audited resource folder | — |

## Programmatic emitter configurations outside TVPJ

| Feature | Exact configuration | Status |
|---|---|---|
| Ring pickup | Billboard, capacity 16, point, `particle.dds`/`ParticleTex`, generation 100ms, power .15, life 2s, size `2 × particleSize` or `2 × fSize`, direction `(1,1,0)` random `(.5,.5,0)`, red, gravity disabled, emitter initially disabled, non-looping, moved to the ring and reset on pickup | Gameplay feature; modern burst exists but exact settings/material are not restored |
| `ParticleEffect::Init` template | Billboard 2×2, direction `(1,1,0)` random `(.25,.5,0)`, caller-supplied generation/power/lifetime/color/looping, gravity commented out | Reusable wrapper; not a scene |
| ASCII `s` / `createSmoke` | Two point billboard emitters, capacity 64 each, `particle.dds`, 100ms, power 5, life 2s, 2×2, looping; magenta `(1,0,1,1)` plus blue `(0,.1,1,1)` | Source counts/colors represented in Suzanne candidates; exact particle motion/material remains adapted |
| Later Human trail | One point billboard emitter, capacity 64, `particle.dds`, 100ms, power 2, life 1s, looping, white; template direction/random then billboard overridden to .5×.5; follows actor position | Not revived as a distinct actor effect |
| TV3D tutorial Snowfall port | 100 billboards in active construction example, origin `(0,1000,0)`, width/depth 5000, ground 0, Snow texture, size 3; reset below ground. Random component velocities are very large and source time is divided twice; whole construction/update block is commented out | Disabled experiment, not a scene |
| TV3D tutorial Smoke port | 100 billboards in active construction example, origin `(-300,0,0)`, height 250, Snow texture, size grows from .5 to 3; reset above height and alpha falls with height; whole block commented out | Disabled experiment, not a scene |

The DirectX tutorial revision additionally defines Snowfall, Fountain, Smoke and three LawnSprinkler direction modes. Those are external tutorial classes, not separate GraphysX-authored effects.

## Isolated recovery delivered

- `web-prototype/src/particle-effect-environment.ts` owns an isolated deterministic adapter for the exact three-emitter `Explosion1` preset.
- `web-prototype/particle-effect-preview.html` is a standalone QA/inspection harness; no shared app UI, `race-scene.ts`, `prototype-app.ts`, `archive-content.ts` or shared CSS was changed.
- `web-prototype/public/assets/particles/archive/` contains byte-identical `Explosion1.tvp`, `Glow2.dds` and `smokey.dds` copies.
- Exact: compiled/readable preset hashes, three emitter configurations, capacities (10/64/8 = 82), generation intervals, shape/radius, power, gravity, life, one-shot looping flags, alpha/depth flags, keyframes, textures, and archived host position `(-5,0,0)`.
- Adapted and disclosed: deterministic seeded random direction, fixed 120 Hz integration, TV3D COLOR/ADDALPHA mapped to alpha/additive because the engine's internal blend-state recipe is unavailable, uniform display scale, floor and camera.
- The environment defaults to source one-shot behavior. The preview may auto-replay for inspection and labels that as presentation behavior.

Recommended integration is as an **Engine & FX / Archived Presets** subview, not as another World or BallZ race. Keep the existing synthetic fountain lab as a general engine diagnostic, and label this preset `PARTIAL` until TV3D trajectory/blend matching can be validated against an original runtime capture.

## Full archived preset library player delivered

The Explosion1 proof is now generalized into a standalone Engine/FX preset browser without changing the scene census or inventing scene ownership:

- `web-prototype/tools/convert-particle-preset-library.mjs` deterministically rescans the resource folder, hashes and deduplicates compiled aliases, parses all readable TVPJ emitter/attractor fields, resolves surviving DDS bindings, and regenerates `src/legacy/particle-preset-library.json` plus exact public texture copies.
- `web-prototype/src/particle-preset-library-environment.ts` exposes all 16 readable presets through a seeded 120 Hz player. Each archived emitter is one batched point-sprite draw call with the exact serialized capacity/config and texture binding. The one archived attractor uses a disclosed force adapter while retaining its exact serialized record.
- `web-prototype/particle-preset-library-preview.html` and `src/particle-preset-library-preview.ts` provide readable/opaque selection, Previous/Next, Restart, Pause, optional preview-only Auto replay, orbit/zoom, deterministic `advanceTime`, and a complete structured state surface.
- The 19 compiled filenames remain 17 unique binaries. `explosion1.TVP = explosion_01.TVP` and `explosion2.TVP = explosion_02.TVP` are displayed as exact alias evidence rather than duplicate simulations.
- `expl1` and `fire1` remain explicitly readable-source-only. `expl1` keeps the missing `clumpy_blurry.dds` binding and uses a conspicuous magenta-X diagnostic sprite, not an invented replacement texture.
- `explosion3`, `impact1`, and `shot1` remain selectable opaque compiled-only evidence with their exact hashes/reasons, zero emitter configuration, and zero simulated particles.
- Inspection normalization, seeded trajectories, blend mapping, minimum screen-legibility size, point batching, camera/floor, and optional replay are adapters. Exact source configuration remains available unchanged under `selected.emitterConfigs`, `selected.attractorConfigs`, compiled/source evidence, and texture-binding state.

Permanent `web-prototype/scripts/qa-particle-preset-library.mjs` passes 97/97 assertions with zero failed responses and zero console/page errors. It exercises all 16 readable configurations, all three opaque records, both alias groups, exact archive-wide counts, deterministic restart, pause/replay/restart/selection controls, source-only and missing-texture status, and captures every readable preset under `web-prototype/output/playwright/particle-preset-library/`. The required web-game client also exercises the isolated player and reports clean structured state under `web-prototype/output/web-game/particle-preset-library-final/`.

### Integration API

`ParticlePresetLibraryEnvironment` exposes `group`, `ready`, `activate()`, `deactivate()`, `listPresets()`, `getSelectedConfig()`, `selectPreset(id)`, `selectNext()`, `selectPrevious()`, `setPaused(bool)`, `setAutoReplay(bool)`, `restart()`, `update(seconds)`, `getCameraProfile()`, `getState()`, and `dispose()`. Mount it only inside **Engine & FX / Archived Presets**. Do not add its entries to Worlds, BallZ concepts/races, or the 50-scene restoration census.
