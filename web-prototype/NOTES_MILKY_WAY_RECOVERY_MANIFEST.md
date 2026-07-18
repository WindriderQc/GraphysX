# Notes Manager and Voie Lactée recovery manifest

This audit deliberately separates executable behavior from declarations, and exact source facts from browser-preview choices. Neither isolated module is wired into the main GraphysX UI.

## Notes Manager

### Recovered sources

- `AtmelCubx/AtmelCubx/NotesManager.{h,cpp}`: complete singleton wrapper around one `CLBlocNote(50, 50, 10, "Default", 1)`; render, get-add-button, and add-next-note calls are implemented.
- `AtmelCubx/AtmelCubx/BlocNote.{h,cpp}`: complete 3D block construction and add behavior.
- `AtmelCubx/AtmelCubx/Note.{h,cpp}`: complete minimal mesh-duplication and position wrapper.
- `AtmelCubx/AtmelCubx/Area.{h,cpp}`: `NOTEMGR_MODE` is declared, the manager is constructed, the note block is rendered inside the `CUBX_MODE` branch, and clicking its add cube opens the Notes GUI and calls `AddNote()`.
- `AtmelCubx/AtmelCubx/Simulation.cpp`: a `NOTEMGR_MODE` update branch exists, but `CLArea::RenderArea` has no matching render case. The application starts in `CAR_MODE`.
- `AtmelCubx/AtmelCubx/NoteMgrGUI.{h,cpp}` and `TV3DCEGUI.{h,cpp}`: real CEGUI loading/visibility plumbing, but only a no-op Quit callback and an unused `NoteWnd` lookup are implemented.
- Byte-identical Notes/Bloc/GUI sources also occur under `Yanik C++ BCKUP/AtmelCubx`, `AtmelCubx 1`, and `AtmelCubxCar`; these are duplicates, not additional scene variants.

Complete discovered class-file inventory: each of `AtmelCubx/AtmelCubx`, `Yanik C++ BCKUP/AtmelCubx`, `Yanik C++ BCKUP/AtmelCubx 1`, and `Yanik C++ BCKUP/AtmelCubxCar` contains `NotesManager.{h,cpp}`, `BlocNote.{h,cpp}`, `Note.{h,cpp}`, and `NoteMgrGUI.{h,cpp}`. `Yanik C++ BCKUP/NoteMgrGUI.{h,cpp}` is one further byte-identical loose GUI copy. Integration references live in the matching `Area.{h,cpp}`, `Simulation.cpp`, and `TV3DCEGUI.{h,cpp}` files. `Yanik C++ BCKUP/Area.h` plus `Area (2).cpp` are another active integration copy; `AtmelCubX code`, `CubXSolution`, `RacinX`, and `RacinX2` retain only commented-out `TV3DCEGUI` Notes references and do not add behavior.

### Exact implemented behavior

- Capacity is 50, arranged in five columns of ten.
- Every note is a `50 x 50 x 10` box. Initial active count is zero.
- For note index `i`: `column=floor(i/10)`, `columnRank=i%10`, `x=column*75`, `y=50`, and `z=columnRank*20` except row zero remains `z=0`.
- The add cube is a `30 x 30 x 30` box at `(0,-250,0)`.
- Each click enables exactly the next disabled mesh. At capacity, the source does nothing; its full-block notification exists only as a comment.
- Notes and the add cube share `Media/marble10.jpg` and one material: ambient `0.1`, diffuse `0.8`, specular `0.2`, power `20`, emissive `0`; shadow casting is enabled.
- The recovered `marble10.jpg` copies are byte-identical (`SHA-256 8ED791E950488AE7E4C1AF7446E8F494A4BFCC5B6DF3B0719CF6357C833E3D30`, 640x480, 38,814 bytes).

### What is not implemented or not recovered

- `GUI/NoteMgrGUI.layout` is absent from this workspace, the external StockRoom, and the file listing inside `AtmelCubx63_2003.rar`.
- No note text/data model, editor, delete, reorder, save/load, or persistence code exists in the recovered implementation.
- No Notes-specific camera, background, screenshot, or standalone scene transition is defined.
- `NOTEMGR_MODE` is therefore a partially wired declaration, not a complete separate scene. The actual visible subsystem belongs to the CubX area.

### Browser recovery decision

`src/notes-manager-environment.ts` is a faithful port of the complete 3D block/add behavior and asset binding. `notes-manager-preview.html` adds only an explicitly inspection-only camera, light, and keyboard shortcut. A full “Notes Manager” application cannot be called faithful without inventing the absent layout and data operations.

## Voie Lactée / Milky Way

### Recovered sources and variants

- `Archive/bckup/VoieLactee.{h,cpp}`: complete older constructor/update implementation.
- `Archive/bckup/BallZ2015.bckup/VoieLactee.{h,cpp}`: same scene behavior through a `CLSceneStaff` owner.
- `Archive/bckup/3DScenes.cpp` and `Archive/bckup/BallZ2015.bckup/3DScenes.cpp`: instantiate the subsystem as part of the BallZ scene and call `Update()` every frame. It is not a standalone scene.
- `GraphysX_1/Scene.cpp::CreateVoieLactee`: complete later static rewrite with different planet spacing, a 23-degree Earth tilt, and exact StockRoom texture names.
- `GfxNet/GfxNet.cpp`, `GraphysX_1/Itv.cpp`, `GraphysX/ConsoleGraphysX/ConsoleGraphysX.cpp`, `Scene3D/GamePlayScreen.cpp`, and `Scene3D/EditorScreen.cpp`: implemented wrappers/call sites for the later scene builder.

### Critical classification correction

No recovered `VoieLactee` implementation generates stars, a galaxy mesh, or a Milky Way background. The name identifies a five-mesh planetary vignette: Earth, transparent/blended Earth cloud shell, Moon, Mars, and Venus. A port that presents it as an authored star field would be invented.

### Exact older BallZ-2015 facts

- Earth `(−10,15,0)`, radius 6; clouds same position, radius 6.1; Moon `(−8,14,−10)`, radius 2; Mars `(−25,15,0)`, radius 12; Venus `(−35,15,0)`, radius 4.
- Per update: Earth rotates `0.02` degrees around Y, cloud shell `0.06`; Moon calls `RotateAround(-0.003 * fTimeElapsed, earthPosition, 8)`.
- `EarthNight.jpg` is loaded but never assigned.
- The Venus constructor accidentally assigns the resulting object pointer to `clMars`; the Venus mesh still exists in the global object vector, while `clVenus` remains unused.
- Exact `EarthCloudsMapHigh.png` was not recovered. A relocated `Earth.jpg` is only a candidate because the required `Media/Galaxy/Earth.jpg` path is absent. TV3D `RotateAround` phase and `cTV_BLEND_ALPHA` details are not fully recoverable from this source alone.
- The authored Mars and Venus spheres overlap: their centers are only 10 units apart while their radii sum to 16. The port preserves that source geometry.

### Exact later GraphysX facts

- Earth `(−10,15,10)`, radius 6, rotated 23 degrees around Z; cloud shell same position, radius 6.1.
- Moon `(−8,14,12)`, radius 2; Mars `(−40,15,10)`, radius 12; Venus `(−70,15,10)`, radius 4.
- No later Voie Lactée update routine is present, so this profile is static.
- Exact sources are `Galaxy/earthgood.jpg`, `EarthClouds.jpg`, `MoonMap.jpg`, `MarsMap.jpg`, and `VenusMap.jpg`.
- Asset hashes: Earth `6B189A5F…` (4096x2048), clouds `D1744C4B…` (8192x4096), Moon `B3CC11FC…` (640x320), Mars `36991F3F…` (2500x1250), Venus `096477D6…` (1440x720).
- The authored Moon is completely enclosed by Earth: center distance is about 3.46, and `3.46 + moon radius 2 < earth radius 6`. Its mesh exists and is counted but cannot be visible through the opaque Earth surface.

Complete discovered source/call-site inventory: `Archive/bckup/VoieLactee.{h,cpp}`, `Archive/bckup/3DScenes.{h,cpp}`, `Archive/bckup/BallZ2015.bckup/VoieLactee.{h,cpp}`, `Archive/bckup/BallZ2015.bckup/3DScenes.{h,cpp}`, `GraphysX_1/Scene.{h,cpp}`, `GraphysX_1/Itv.{h,cpp}`, `GfxNet/GfxNet.{h,cpp}`, `GraphysX/ConsoleGraphysX/ConsoleGraphysX.cpp`, `Scene3D/GamePlayScreen.cpp`, and `Scene3D/EditorScreen.cpp`. The two `BallZ2015.vcxproj` files only register the class in the build and add no behavior.

### Dependencies and formats

- Older branch: TrueVision3D 6.5 mesh/texture APIs, `CL3DObject`, `CL3DObjectManager`, `CLSceneStaff`/`CLSceneTools`, a Newton world pointer (no planet physics body is requested), and `fTimeElapsed`.
- Later branch: GraphysX `CLScene`, `ResourceManager`, `CLMeshPrimitive`, GfxNet wrappers, and TV3D sphere primitives.
- Assets are equirectangular JPEG maps; the old cloud source expected an alpha PNG, while the later complete profile uses the recovered high-resolution JPEG cloud map with TV3D color blending.

### Browser recovery decision

`src/milky-way-environment.ts` provides an exact later-GraphysX geometry/asset profile and an explicitly partial BallZ-2015 motion/placement profile using the later maps where the older exact files are missing. The isolated preview camera, lighting, black background, and Three.js cloud opacity are inspection choices because no subsystem-specific camera/sky/light or exact `cTV_BLEND_COLOR` mapping exists in the archive.
