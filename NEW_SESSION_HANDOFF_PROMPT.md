# GraphysX clean-session continuation prompt

Copy everything below into a new Codex session opened at `C:\Users\Yanik\codes\GraphysX`.

---

You are continuing the GraphysX resurrection and agent-world project with its owner. Work as an implementation partner: inspect the repository, make useful progress, test the result in the real browser, preserve archive provenance, and finish coherent increments instead of only discussing them.

## 1. Product intent

GraphysX began as an engine for creating easy 3D worlds. Its modern purpose is now clearer:

1. It is a manipulable 3D world for an AI agent.
2. The agent can use it to illustrate spatial ideas, test real physics, arrange explanations, and create small interactive experiences.
3. The visible editor is a communication surface between a person and an agent. Either participant can point at, create, select, move, inspect, texture, simulate, and interact with the same entities.
4. What an agent can do through the API, a person should be able to do through an understandable UI when practical. Both surfaces must call one validated runtime; never build a second UI-only scene model.
5. GraphysX also remains a careful resurrection of the BallZ, CubX/CubZ, SBQC, Nature of Code, Three.js, archive-world, editor, physics, shader, math, device, and experimental work found in the owner's archives.

The owner explicitly wants momentum, friendliness, and efficiency. Avoid over-engineering. Prefer one small, complete, demonstrated capability over an abstract framework. Use the running experience to decide what is actually missing.

## 2. Important owner feedback to retain

- “Nature of Code” material became too compact and is not demonstrated as richly as the original HTML work. Do not reduce substantial experiments to tiny cards or a shallow catalog. A future Nature increment should give individual concepts room, visible behavior, adjustable parameters, explanatory state, and agent-readable observations.
- Do not lose focus on the 3D API for AI agents while restoring archive scenes.
- The old engine created entities through serialized data, possibly XML: primitives, splines, custom/complex models, lights, static/dynamic physics, mass, and mesh-controlled/kinematic objects. The modern v2 JSON schema now carries those meanings.
- The ASCII editor is not merely a textarea. It is an adapter over a persistent named semantic level library. Agents should be able to manage many levels and larger regions efficiently; ASCII is one import/export representation, not the canonical limit.
- “The Funny Zigzagger” is the key example level: fire and ice fight through forces. Fire repels/launches; ice attracts/preserves momentum. A person edits the same semantic level an agent queries and patches.
- Texture was the first requested increment after core parity. Textures, visible agent entities, real impulse interactions, and XML migration now exist.
- Licensing review is intentionally deferred by the owner for now. Preserve source/provenance labels and do not make legal claims, but do not derail implementation work into a licensing project unless the owner reopens it.

## 3. Repository and launch

- Repository root: `C:\Users\Yanik\codes\GraphysX`
- Active browser prototype: `C:\Users\Yanik\codes\GraphysX\web-prototype`
- Primary stack: TypeScript, Vite, Three.js, cannon-es, Playwright.
- Install/run:

```powershell
cd C:\Users\Yanik\codes\GraphysX\web-prototype
npm install
npm run dev
```

- Production verification:

```powershell
npm run build
```

The current release should have been committed, pushed, and merged to `origin/main` by the session that wrote this handoff. Start by checking, not assuming:

```powershell
git status -sb
git log --oneline --decorate -8
git remote -v
```

Do not reset or discard user work. The repository historically tracks a small subset of QA images, but `web-prototype/output/` is ignored for new generated runs so browser captures and temporary builds do not make the tree dirty.

## 4. Current agent-facing architecture

The public browser API is `window.__GRAPHYSX__` with schema `graphysx.agent-api/v2`. It combines:

- World API schema: `graphysx.agent-world/v2`
- World state schema: `graphysx.agent-world-state/v2`
- Named level API/schema: `graphysx.agent-level-api/v1` / `graphysx.agent-level/v1`
- Discoverable bridge: `window.__GRAPHYSX_AGENT_BRIDGE__`, schema `graphysx.agent-tool-bridge/v1`
- Structured request/response schemas: `graphysx.agent-tool-request/v1` and `graphysx.agent-tool-response/v1`
- Direct async bridge calls and same-origin `window.postMessage`
- State-change subscription and `graphysx:agent-world-state` DOM events
- External Playwright-backed JSON-lines adapter: `tools/graphysx-agent-stdio.mjs`

The world coordinate convention is Three.js-style: `+x` right/east, `+y` up, and `-z` default forward.

Core source files:

- `web-prototype/src/agent-world-runtime.ts`: public types, validation, state, physics integration, entity creation/update/query, interactions, persistence, transactions, history, undo, textures, and agent avatar rendering.
- `web-prototype/src/agent-world-bridge.ts`: discoverable tool manifest, direct/request/message transports, and events.
- `web-prototype/src/agent-world-textures.ts`: stable semantic recovered-texture descriptors.
- `web-prototype/src/agent-world-assets.ts`: stable recovered complex-model descriptors.
- `web-prototype/src/agent-world-starters.ts`: one-call editable starter worlds, including Physics Sketchbook.
- `web-prototype/src/agent-world-legacy-xml.ts`: migration of both archived `Object3D` XML layouts into v2.
- `web-prototype/src/agent-level-library.ts`: named semantic levels, region reads, atomic patch/fill/resize/transactions, undo, ASCII adapters.
- `web-prototype/src/prototype-app.ts`: shared human editor, public globals, file import/export, visible controls, and app routing.
- `web-prototype/src/race-scene.ts`: integration boundary between the main renderer/simulation and agent runtime.
- `web-prototype/tools/graphysx-agent-stdio.mjs`: minimal external process adapter.
- `web-prototype/AGENT_WORLD_API.md`: public contract and usage examples.
- `web-prototype/progress.md`: chronological implementation/verification log.

## 5. What the 3D World API can do now

### World lifecycle and observation

- `open()`, `demo()`, `state()`
- `create(definition)`, `clear(id?, label?)`
- `export()`, `save(name)`, `load(nameOrDefinition)`
- `query(filter)`, `observe(filter?)`
- `pause(boolean)`, `step(seconds)`
- `undo()`, `history(sinceRevision?)`

### Entities

Supported types include:

- hierarchy: `group`
- semantic participant: `agent`
- primitives: `box`, `sphere`, `icosahedron`, `cylinder`, `cone`, `torus`, `plane`
- paths: `spline`
- complex recovered asset: `model`
- lighting: `ambient-light`, `directional-light`, `point-light`

Every ordinary entity can have a stable ID, label, parent, tags, visibility, transform, material, geometry, behaviors, interactions, and optional physics where meaningful.

### Transforms and selection

- `select(ids)` shares selection among agent state, direct 3D picking, outliner, inspector, and TransformControls.
- People can drag move/rotate/scale gizmos; release commits a normal validated `update()`.
- W/E/R switch translate/rotate/scale.
- World/local space and snapping are visible controls.
- Agent updates and human gizmo updates produce the same revisions/events.

### Materials and textures

Materials support color/tint, emissive color/intensity, roughness, metalness, opacity, and an optional semantic texture with repeat, offset, and rotation.

`textures()` currently returns eleven stable recovered descriptors:

- `checker`
- `green-grid`
- `abstract-cubes`
- `two-way`
- `eroded-metal`
- `rusted-metal`
- `marble`
- `wood-floor`
- `worn-wood`
- `earth`
- `spheres`

The renderer caches source images and clones per-material transforms. It preserves a model's native map when no override is requested and disposes only agent-owned texture instances. Texture state survives query/state/export/import/snapshot/update.

The human Create and Selected Entity panels expose the same texture library with thumbnails and compact material/UV controls.

### Physics

- Physics modes: `static`, `dynamic`, and `kinematic`.
- Dynamic bodies support mass, material, friction, restitution, initial/current linear velocity, sleep/wake state, and real cannon-es simulation.
- Complex models use an explicit predictable box collider from supplied geometry dimensions.
- World gravity is serializable.
- Physics bodies remain at world root to avoid ambiguous parent-space simulation.
- `pause()` plus `step(seconds)` gives agents deterministic experiments.
- Query/state expose current velocity so an agent can measure outcomes rather than only trigger visuals.

### Behaviors

Supported serializable behaviors are `spin`, `bob`, `orbit`, `pulse`, `look-at`, and `follow-spline`. Spline-driven physics entities should use kinematic mode.

### Interactions

- `toggle-visibility`: toggles stable target IDs.
- `apply-impulse`: applies a real impulse to dynamic target IDs and returns resulting velocity.
- People can compose both in the friendly inspector; advanced interaction JSON remains available.
- A 3D click, accessible UI action, `interact(id, interactionId?)`, transaction command, and collaborative commit all share the same atomic path.

### Agent participants

An entity with `type: "agent"` renders a compact body/head/direction marker/perception ring and stores:

- `role`
- `status`
- `perceptionRadius`
- semantic `capabilities[]`

This represents a participant inside a shared spatial explanation. It does not invent a second autonomous AI runtime.

### Recovered models and splines

- `assets()` discovers recovered complex assets by stable ID.
- `model` entities can be visual-only, static, dynamic, or kinematic with an explicit collider.
- `spline` entities are visible/queryable/exportable and can drive `follow-spline` behavior.

### Editing, transactions, and collaboration

- `spawn`, `update`, `remove`, behavior attach/detach, interaction, environment edits, selection, and prefab creation work as individual calls.
- `transaction(commands)` is atomic; one invalid command rolls back the complete batch.
- `commit(changeSet)` records actor ID/label/kind, intent, expected revision, and command summary.
- Stale expected revisions reject safely.
- Human UI commits and agent commits share history.

## 6. Minimal API examples

Create a lit physics world:

```js
const gx = window.__GRAPHYSX__;

gx.create({
  schema: "graphysx.agent-world/v2",
  id: "agent-lab",
  label: "Agent Physics Lab",
  environment: {
    background: "#071522",
    ground: { visible: true, size: 32, color: "#102d33", grid: true, gridColor: "#36bfc4" },
    physics: { gravity: [0, -9.81, 0] }
  },
  entities: [
    { id: "ambient", type: "ambient-light", intensity: 0.8, material: { color: "#b6ddff" } },
    { id: "sun", type: "directional-light", intensity: 3, transform: { position: [8, 14, 6] } }
  ]
});
```

Add a textured dynamic ball and an impulse pad:

```js
gx.transaction([
  { op: "spawn", entity: {
    id: "test-ball",
    type: "sphere",
    geometry: { radius: 0.6 },
    transform: { position: [0, 5, 0] },
    material: { color: "#ffffff", texture: { id: "spheres" } },
    physics: { mode: "dynamic", mass: 1, material: "ball", restitution: 0.7 },
    tags: ["experiment", "physics:dynamic"]
  } },
  { op: "spawn", entity: {
    id: "launch-pad",
    type: "box",
    geometry: { width: 2, height: 0.25, depth: 2 },
    transform: { position: [0, 0.2, 0] },
    material: { color: "#ffffff", texture: { id: "two-way" } },
    physics: { mode: "static", material: "ground" },
    interactions: [{
      id: "launch-ball",
      label: "Launch the test ball",
      type: "apply-impulse",
      targetIds: ["test-ball"],
      impulse: [5, 7, 0]
    }]
  } }
]);

gx.pause(true);
gx.interact("launch-pad", "launch-ball");
gx.step(0.5);
gx.query({ ids: ["test-ball"] })[0].physics.linearVelocity;
```

Add a visible agent participant:

```js
gx.spawn({
  id: "lab-guide",
  type: "agent",
  transform: { position: [-3, 0, 3] },
  agent: {
    role: "physics explainer",
    status: "observing",
    perceptionRadius: 10,
    capabilities: ["observe", "edit", "explain", "interact"]
  }
});
```

Use the complete starter instead:

```js
gx.starters();
gx.loadStarter("physics-sketchbook");
```

Physics Sketchbook contains sixteen ordinary editable entities: lights, wood floor, checker ramp, contrasting dynamic bodies, collision wall, restitution ball, impulse pad, coordinate board/axes, Earth orbit/spin illustration, and visible lab agent.

## 7. Named level / ASCII API

`gx.levels` manages up to 64 named levels and 4,096 cells per level, max 64 cells per axis. The canonical level document stores semantic tiles, dimensions, cell size, and revision.

Important methods:

- `tiles`, `tileSemantics`
- `active()`, `list()`, `get(id)`
- `create(options)`, `remove(id)`, `open(id)`
- `region(id, rect)`
- `patch(id, changes, options?)`
- `fill(id, rect, tile, options?)`
- `resize(id, width, height, defaultTile?, options?)`
- `transaction(id, operations, options?)`
- `undo(id)`
- `importAscii(source)`, `exportAscii(id)`
- `play(id)`

Semantic tiles include floor, wall, start, ring, half, finish, hazard, fire, and ice. ASCII uses `^` for fire and `~` for ice.

Do large edits by bounded region and optimistic revision, not by retransmitting every level:

```js
const area = gx.levels.region("funny-zigzagger", { x: 8, y: 6, width: 8, height: 6 });

gx.levels.patch("funny-zigzagger", [
  { x: 10, y: 8, tile: "fire" },
  { x: 13, y: 8, tile: "ice" }
], { expectedRevision: area.revision });
```

## 8. Legacy XML migration

`importLegacyXml(xml, { id?, label? })` supports both discovered archive layouts:

1. Attribute form, such as `<Object3D Type="8" Name="Ball" masse="1"><Pos x="0" y="4" z="0"/></Object3D>`.
2. Nested form, with child `<Type>`, `<Name>`, `<Pos><x>...</x></Pos>`, etc.

The converter maps known primitive/physical type codes, position/scale, enabled state, mass, mesh-controlled mode, Newton material, and known texture names. Custom/duplicate/XMesh records whose source model is not registered become obvious box proxies with legacy tags and warnings. Never silently claim those proxy meshes are exact recovered models.

The human World Editor accepts `.json` and `.xml` through the same file control. The 5 MB and 512-object bounds are deliberate safety limits.

## 9. External agent process

With GraphysX already served:

```powershell
npm run agent:manifest -- --url http://127.0.0.1:5173/
npm run agent:stdio -- --url http://127.0.0.1:5173/
```

The persistent adapter accepts one request per line:

```json
{"schema":"graphysx.agent-tool-request/v1","id":"starter","method":"loadStarter","args":["physics-sketchbook"]}
{"schema":"graphysx.agent-tool-request/v1","id":"bodies","method":"query","args":[{"tag":"physics:dynamic"}]}
{"schema":"graphysx.agent-tool-request/v1","id":"launch","method":"interact","args":["impulse-pad"]}
{"schema":"graphysx.agent-tool-request/v1","id":"observe","method":"observe","args":[{"ids":["ramp-ball"]}]}
```

This is intentionally a thin adapter over the browser bridge. Do not add WebSocket/network infrastructure until a real agent host needs it. If a concrete MCP host is chosen later, wrap this request/manifest contract instead of duplicating the runtime.

## 10. Current human UI parity

The Agent World Studio currently provides:

- one-click starter cards and prefab cards
- direct 3D selection and outliner selection
- move/rotate/scale gizmos, snapping, world/local space
- entity outliner and selected-entity inspector
- friendly creation for primitives, agent, lights, recovered model, and spline
- hierarchy, tags, transform, geometry/light fields
- semantic texture thumbnails and material/UV controls
- static/dynamic/kinematic physics fields
- agent identity fields
- friendly toggle/impulse interaction composer
- behavior attach/detach
- accessible interaction buttons
- complete JSON world editor
- atomic transaction JSON
- actor-aware commit JSON
- named snapshots
- JSON download and JSON/XML import
- pause, step, undo, clear, query-visible feedback

When extending API vocabulary, add the smallest useful friendly UI counterpart in the same increment unless the capability is intentionally expert-only and clearly documented.

## 11. Verification baselines

These baselines were green at handoff:

- `npm run qa:agent-world`: 69 assertions, 0 browser errors.
- `npm run qa:agent-world-ui`: 73 assertions, 0 browser errors.
- `npm run qa:agent-entities`: 26 assertions, 0 browser errors.
- `npm run qa:agent-levels`: 38 assertions, 0 browser errors.
- `npm run build`: strict TypeScript and Vite build, 113 modules.
- Focused r13 suites: Three.js Playground 43/43, Blender/Maison 34/34, Arena 18/18.
- Complete r13 restoration matrix: 293/293 with zero browser errors at the last full release verification.
- External adapter: manifest discovery and a real `textures` request succeeded.

Before claiming completion after code changes:

1. Run strict TypeScript/build.
2. Run the focused suite closest to the change.
3. Run `qa:agent-world-ui` for shared human/agent parity changes.
4. Use the required generic web-game Playwright client from the `develop-web-game` skill.
5. Inspect actual screenshots, not only assertion counts.
6. Inspect `render_game_to_text()` for accessible/agent-readable state.
7. Check browser console and page errors.
8. Run the full matrix when integration scope warrants it.

Generated outputs belong under `web-prototype/output/` and should stay uncommitted unless a specific durable evidence artifact is intentionally selected.

## 12. Archive and fidelity boundaries

This repository contains a large amount of recovered and adapted content. Preserve these rules:

- Search source and media evidence before inventing missing behavior.
- Keep exact source assets/hashes/transforms where proven.
- Label browser conversions and presentation adapters honestly.
- Do not turn catalogs, serializer fixtures, or asset families into “lost worlds” without composition evidence.
- Do not claim a proxy is an exact model.
- Do not inflate the canonical scene census with aliases, vendor tutorials, duplicated copies, or primitive exercises.
- Existing source-bounded galleries and inspections should remain separated from modern curated playable work.
- Preserve unrelated user changes in the large repository.

The r13 census at handoff is 54 canonical records: 7 `RESTORED`, 47 `PARTIAL`, 0 `REGRESSED`, 0 `PIPELINE`, 0 `MISSING`, and 0 `RESEARCH`. Here `PARTIAL` usually means a visible documented evidence/hardware boundary, not forgotten first-pass work.

## 13. Recommended next product increment

Do not immediately add a broad new subsystem. First open the running app, use Physics Sketchbook and Nature Lab, and pick one concrete experiment.

The strongest next candidate is a **Nature concept as a full agent experiment**, for example forces/attraction or vectors:

1. Give the concept its own spacious view instead of another compact card.
2. Build it from ordinary World API entities, behaviors, physics, textures, interactions, and agent-readable state.
3. Expose two or three meaningful parameters to people and agents.
4. Add a small measurement vocabulary only if the concept needs it—examples: distance, speed, energy estimate, collision count, or a sampled trail.
5. Let a person change the experiment in the UI and let an agent observe the exact result.
6. Let the agent change it and visibly update the same editor.
7. Include one concise explanation and one “try this” action.
8. Prove it with an end-to-end human/agent browser test and a screenshot.

If that experiment proves a reusable missing primitive, add only that primitive. Likely candidates, in order:

- read-only sensors/measurements
- one friendly proximity or threshold trigger
- reusable in-world label/plot instrument
- richer commit/change receipts

Avoid for now unless driven by a concrete host or experiment:

- arbitrary scripts/eval inside world definitions
- a second scene DSL
- a duplicate React/editor application
- generic networking or multiplayer infrastructure
- a large plugin architecture
- speculative ECS rewrites
- unbounded material/asset pipelines
- silently loading arbitrary cross-origin resources

## 14. Working style for the new session

- Lead with running evidence and user-visible outcomes.
- Inspect before editing; use `rg`/`rg --files` first.
- Read applicable skills completely, especially `develop-web-game` for browser iteration and Git publishing guidance when publishing.
- Use `apply_patch` for edits.
- Keep changes narrow and cohesive.
- Preserve dirty/unrelated work; never use destructive reset/checkout operations casually.
- Prefer atomic runtime methods and stable IDs.
- Keep public state serializable and renderer-independent.
- Maintain validation, rollback, revision, history, query, export/import, and UI parity for new state.
- Update `AGENT_WORLD_API.md`, tests, and `progress.md` in the same increment.
- Visually inspect the result.
- Stop and explain only when a real authority/evidence choice is needed; otherwise make sensible implementation assumptions and continue.

## 15. First commands for the new session

```powershell
cd C:\Users\Yanik\codes\GraphysX
git status -sb
git log --oneline --decorate -8

cd C:\Users\Yanik\codes\GraphysX\web-prototype
npm run build
npm run dev
```

Then open the app and inspect:

1. Home → Creative Tools → Agent World API.
2. Load Physics Sketchbook.
3. Click/select the visible lab agent, the impulse pad, ramp ball, and coordinate board.
4. Inspect textures/materials in the selected entity panel.
5. Pause, interact with the impulse pad, step time, and observe velocity.
6. Open/play The Funny Zigzagger and inspect fire/ice semantics.
7. Inspect Nature Lab and compare its depth to the source HTML-era intent.

After that inspection, state the smallest concrete increment you will complete and make it real.

---

End of clean-session prompt.
