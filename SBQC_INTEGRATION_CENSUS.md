# SBQC → GraphysX integration census

Source reviewed: `WindriderQc/SBQC` public main tree at commit `42c210f3599c55038efde8fad61d9fe5bf5c0f24`.

This is an implementation census, not a licensing decision. Source paths are kept so provenance stays recoverable while licensing is handled separately.

## Live in GraphysX

| World | SBQC source family | GraphysX surface | Agent controls |
| --- | --- | --- | --- |
| Flock Planet | `public/Projects/Nature of Code/flock`, `public/js/Boid3D.js`, `Flock3D.js` | Four focused demonstrations: separation, alignment, cohesion, complete flock | lesson, weights, speed, source-length trails, live boid spawning, pause/step/reset |
| Forces & Flow Garden | `public/Projects/Nature of Code/s1`, `s2`, `sAll/flowfield.js` | Four focused demonstrations: two-walker random walk, attraction/mass, flow field, combined forces | lesson, attraction, flow, speed, particle release, walker scatter, pause/step/reset |
| Living Forest | `public/Projects/Nature of Code/s4`, `s5/forest*`, `s5/ecosystem*` | Four focused demonstrations: recursive growth, leaf fall, DNA/evolution, complete life cycle | lesson, mutation, season, autumn, generation advance, planting, pause/step/reset |
| Orbital Observatory | `public/js/{Globe,Trajectory,Starfield,earth3D,threeEarth}.js`, `public/data/quakes.csv` | `web-prototype/src/orbital-observatory.ts` | orbit/spin/pass/quake parameters, layers, observer, recipe, reset |

The four world families now expose 13 focused demonstrations. The Nature-of-Code mechanisms are taught independently before the three combined synthesis lessons; they are no longer compressed into three simultaneous animations. Each lesson names its direct source, concept, observation target, effective rules, phase, and interaction. `render_game_to_text()` reports all of that state. `window.__GRAPHYSX_WORLDS__` exposes list/current/load/select/selectLesson/setParameter/setLayer/setObserver/pause/step/act/reset so an AI agent can collaborate with the same experiments a person sees.

The general engine layer is now live beside those study-specific controls. `window.__GRAPHYSX__` implements `graphysx.agent-world/v2`: agents can compose primitives, hierarchy, lights, materials, tags and deterministic behaviors; query or observe serializable state; submit atomic transactions; undo; pause/step time; and export/save/load worlds. The visible Agent World Studio demonstrates the same contract rather than a privileged hard-coded scene path.

## Reusable source families

### 3D world and scene-manager lineage

- `archive/3D Playground/GraphysX/sceneManager.js`, `generalLights.js`, and `graphysx.js`: earlier browser-side GraphysX scene composition.
- `archive/3D Playground/GraphysX/scenes/{earth,morphSphere,planeZ,airplane}.js`: small scene modules suitable for a future scene registry.
- `public/js/{Globe,Trajectory,Starfield,earth3D}.js`: orbital globe, trajectory, and star-field family.
- `public/Three Earth/` and related Three.js pages: layered Earth/world presentation studies.

### Nature of Code collection

The public tree contains 60 application/source blobs after excluding bundled p5 libraries and type definitions:

- `s1`: walkers and stochastic motion.
- `s2`: forces, attraction, particle systems, following, and randomness.
- `s3`: steering, flocking, and obstacle behavior.
- `s4`: recursive trees and falling leaves.
- `s5`: DNA, evolution, forests, and ecosystems.
- `sAll`: flow-field, path-following, and boid combinations.

### 2D and data surfaces

- p5 sketches and canvas experiments can become textures, holograms, dashboards, or authored in-world instruments.
- Leaflet maps can become geographic surfaces and mission planners.
- Chart.js and amCharts experiments can become live simulation telemetry panels.

## Integration queue

1. In-world 2D surfaces: mount p5/map/chart outputs onto reusable screens and materials.
2. Composition adapters: mount the focused Nature studies as reusable rooms or biomes through World API entities instead of switching the whole preview.
3. Collaboration metadata: add annotations, ownership, intent, camera bookmarks, and semantic constraints to persistent v2 worlds.
4. Observatory feeds: optionally replace deterministic telemetry with live ISS/TLE and quake connectors while retaining replayable snapshots.

## Runtime contracts

The current Nature Lab deliberately establishes the smallest useful contract:

```ts
type WorldRecipe = {
  schema: "graphysx.world/v1";
  study: "flock-planet" | "forces-garden" | "living-forest" | "orbital-observatory";
  lesson?: string;
  seed: number;
  settings: Partial<Record<string, number>>;
  layers: Partial<Record<string, boolean>>;
  observer?: { label: string; latitude: number; longitude: number };
};
```

That focused `graphysx.world/v1` recipe remains the concise interface for Nature lessons. The general `graphysx.agent-world/v2` layer now supplies composable entities, behaviors, environments, safe edits, observations, and persistence without requiring an agent to manipulate low-level Three.js objects directly. See `web-prototype/AGENT_WORLD_API.md` for the public surface and executable examples.
