# Classic ASCII levels and Suzanne 1 fidelity

This note separates facts surviving in source from browser presentation adapters. It covers the three `Level*_base.ASCII` boards and the distinct 40×40 `Suzanne1.ASCII` arena.

## Source-backed facts

- `levelList.xml` binds Level 1 to `Alien01_B_diff/normal` + ClearBlue, Level 2 to `Checkerboard.png` + LostValley, and Level 3 to `Alien02_diff/normal` + NightSky. Every entry has three laps; only Level 2 requests ten humans.
- The boards are exactly 20×20, 20×20 and 20×19. Each contains exactly 20 checkpoint symbols plus one each of `F/f/H/h`; Level 1/2 use `@`, while Level 3 uses a solid `$` spawn tile.
- `BallZScreen.cs` establishes a 0.3 BallZ visual radius, `RingSys.setBillboardSize(0.5f)`, `addRings(0.45f)`, mouse-down/up throttle, Space jump, B fire, and two point lights at `(8,20,8)` and `(32,20,32)` with range 40.
- That same screen initializes the camera with `Cam_setPos(0,10,-10, 10,5,10)` and updates it through `Cam_alignWith(mapCenter, player)`. The exact implementation of `Cam_alignWith` is inside the unavailable managed/native boundary, so a numeric chase offset must remain an adapter.
- The C# screen constructs BallZ at `(5,2,5)` even though the ASCII files author `@/$` markers. Both facts are retained; the web races currently favor the authored markers because they match the supplied board screenshots and traversable starts.
- `Suzanne1.ASCII` is an exact 40×40 composition with 208 walls, 45 chains, 15 ZRing spheres, three pistons, two effect cells and two unresolved CubX anchors. `Suzanne1.png` is the only visual reference for its older grass/wood/large-pickup profile.

## Corrections in this pass

- Level 3 vertical faces now use the purple `Alien02_diff.bmp` surface visible below the yellow arrow tops in `screenShotLevel3.png`. The previous all-arrow cube wrapped the top image onto faces that are visibly purple in the archive.
- Classic style data now retains the exact camera initialization/update semantics, two archived point-light records, checkpoint size calls and input/handling facts instead of leaving them only in prose.
- Web exposure values are brighter for all three classic boards, especially NightSky Level 3, so the source textures remain legible like the supplied captures. These intensities are disclosed browser adapters; the archived RGB values remain separately recorded.
- Suzanne’s loaded grass, wood, `3D_Spheres.jpg`, `ZRing.png`, podium and magician textures are now multiplied by white. Their former fallback colors remained active after loading, which darkened the grass/wood and shifted the exact yellow/red ZRing art toward orange.

## Remaining integration boundary

`race-scene.ts` still uses its generic chase-camera constants during live play. A future integration can consume `style.camera.sourceUpdate`, but it must label the numeric browser offset as an adapter because the archived `Cam_alignWith` body does not survive. The exact archived camera facts are now available without inventing that missing implementation.

Run `node tools/verify-classic-suzanne-fidelity.mjs` to recheck the external StockRoom layouts, level list, source code facts, Suzanne hashes and narrow restoration bindings.
