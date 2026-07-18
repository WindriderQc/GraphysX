# Dominus Source Asset Gallery

## Classification

This recovery is an **asset gallery**, not a revived village, port environment, world, or gameplay scene. The exhaustive forensic audit found 65 unique mesh files and their local model/material data, but no surviving host loader, scene document outside the unrelated ObjectLibrary catalog, inter-asset transforms, camera, spawn, navigation, terrain, water, or interactions.

It must remain separate from the ObjectLibrary grid. That XML document reuses 41 Dominus basenames as editor-catalog entries; it is not evidence for a Dominus composition.

## Exact recovered inventory

| Family | Assets |
| --- | ---: |
| Bush | 4 |
| Camp | 4 |
| Character | 8 |
| Grass | 5 |
| Port | 28 |
| Weapon | 6 |
| Tree | 10 |
| **Total** | **65** |

- 63 text DirectX X meshes are decoded.
- `port_crateshed.X` and `renzokscale.X` are binary DirectX X and remain unsupported, with no substitute geometry.
- The text assets contain 283 source mesh parts, 40,836 vertices, 26,317 triangles, and 308 material groups.
- All 83 referenced texture filenames resolve. Fifty-two JPG/PNG files are copied byte-for-byte; 31 TGA files are losslessly decoded to browser PNG.
- Twenty-one source textures contain at least 5% exact RGB `(255,0,255)` pixels. Their archived/exported bytes remain unchanged; the browser applies a shader-side magenta color-key compatibility adapter.

## Exact versus inspection-only

Exact data includes each source filename, family, source bytes/hash, local frame transforms, transformed local vertices, raw UVs, triangulated faces, face-material assignment, material parameters, material groups, texture references, and source bounds.

Inspection-only behavior includes:

- centering one selected local model at the origin;
- one uniform display scale derived from its largest source bound;
- source `+Z` shown as web `-Z`;
- the gallery camera, orbit, zoom, lights, background, labels, and UI;
- runtime vertex normals derived from the exact transformed vertices and triangles.

No two source assets are placed together, so the gallery cannot be mistaken for an authored layout.

## Generated data

- `src/legacy/dominus-asset-gallery.json` is the small 65-record manifest.
- `public/assets/dominus-gallery/meshes/*.json` contains one lazy exact-geometry payload per decoded text X asset.
- `public/assets/dominus-gallery/textures/*` contains the 83 browser texture outputs.
- `tools/convert-dominus-asset-gallery.mjs` regenerates the manifest and payloads from the audited canonical archive root.
- `tools/verify-dominus-asset-gallery.mjs` verifies all source/payload/texture hashes and geometry/material invariants.

## Standalone browser and controls

Open `dominus-asset-gallery-preview.html` through Vite. The browser filters by the seven proven families and by recovered/unsupported status, displays one asset at a time, and exposes exact selected-source metadata.

- `A` / `D` or arrow keys: orbit
- pointer drag: orbit
- wheel: zoom
- `N` / `P`: next/previous matching asset
- `R`: reset filters, selection, orbit, and zoom
- `F`: fullscreen

`window.render_game_to_text()` exposes the same selected asset, counts, filters, normalization, camera, and evidence boundary as the visible UI. `window.advanceTime(ms)` supplies deterministic stepping for browser QA.

## Minimal integration API

```ts
const gallery = new DominusAssetGalleryEnvironment();
scene.add(gallery.group);
await gallery.ready;

await gallery.setFamilyFilter("port");
await gallery.setStatusFilter("recovered");
await gallery.selectById("port_lighthouse");
gallery.update(deltaSeconds, orbitInput, camera);
const state = gallery.getState();

gallery.dispose();
```

Useful methods are `applyToCamera`, `orbitByRadians`, `zoomBy`, `setFamilyFilter`, `setStatusFilter`, `selectById`, `selectIndex`, `selectNext`, `reset`, `getState`, and `dispose`.

If integrated, place it under an archive/source-asset gallery or model-library destination. Do not place it under a playable-world family and do not promote the separate Dominus Village / Port scene record beyond `PIPELINE`; an authored composition still does not survive.

## Verification

```powershell
node tools\convert-dominus-asset-gallery.mjs
node tools\verify-dominus-asset-gallery.mjs
node scripts\qa-dominus-asset-gallery.mjs
```
