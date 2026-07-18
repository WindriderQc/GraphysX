# BallZ2015 XML world recovery

## Result

`MyWorld.xml` and `TestWorld.xml` are distinct serialized compositions, but neither is evidence for a finished discoverable world. The first is a broken editor save; the second is a scene-loader coverage fixture. Their exact recoverable portions now have isolated previews without placeholder geometry.

| Document | Classification | Exact rendered | Unresolved |
| --- | --- | ---: | ---: |
| `Media/MyWorld.xml` | Distinct broken editor-save composition | 2 / 4 | 2 malformed duplicate targets |
| `Media/TestWorld.xml` | Distinct loader coverage fixture | 5 / 8 | Missing bush source and 2 empty duplicate targets |

These files must remain separate from `MyWorld - Copie.xml`. `MyWorld.xml` uses the high-detail 10,261-vertex airplane at `(0,10,0)` plus a tiny physics cylinder. `TestWorld.xml` lays out four physics primitives at x=5/10/15/20 and the 5,367-vertex AirplaneLP at x=25 before attempting a missing bush/duplicate test.

## Schema trap

The documents' embedded `Actions` headers are authoritative. Their archived host snapshot later inserted `PHYSICXSTATIC` into the C++ enum and shifted `DUPLICATE`, but `SceneBuilder` does not persist a schema-version migration rule.

In `MyWorld.xml`, Type 14 therefore means `DUPLICATE`. Although the two `PathToMesh` values happen to name surviving Suzanne X files, `MeshFactory::CreateDuplicate` treats that field as the name of a previously built mesh. No matching prior names exist, so loading the X files would be an invented repair. They remain visible only as unresolved state records.

`TestWorld.xml` explicitly maps Type 13 to `DUPLICATE`. Both duplicate records have empty target fields. `c:/media/Nature/bush1.X` was not found under the workspace or StockRoom search roots.

## Host and callsite evidence

- `SceneBuilder.cpp:49-54` performs generic XML deserialization; `:60-177` dispatches the serialized type.
- `MeshFactory.cpp:113-125` proves duplicate resolution searches prior mesh names.
- `MainForm.h:451-459` exposes generic `c:/test/MyWorld.xml` load/save buttons.
- Startup instead loads `../StockRoom/Suzanne1.xml` (`MainForm.h:44-46`).
- No surviving literal `TestWorld.xml` callsite was found.

This makes both documents editor/test artifacts which were loadable through generic tooling, not default application worlds.

## Exact recovered assets

- `Airplane/Airplane.tvm`: 10,261 vertices, 11,168 triangles, 48 legacy MGR4 material groups.
- `Airplane/AirplaneLP.TVM`: 5,367 vertices, 9,202 triangles, 8 MGRP material groups.
- AirplaneLP binds the exact six adjacent BMP textures: FUS, HTAIL, RED, VTAIL, WHEEL, and WING.
- The two TVMs and all six textures have byte-identical copies under `E:/Media/Datalake/Tech/StockRoom/Airplane`.
- `yellowtwoway.jpg` is the source-backed `TwoWay` texture used by the primitive factory.

The standalone camera and lights are disclosed inspection adapters. Physics is not simulated because neither XML contains gravity, timestep, ground, or interaction settings.

## Isolated integration API

`BallzXmlWorldsEnvironment` owns one `group` and exposes:

- `setScene("myworld" | "testworld", camera?)`
- `focusObject(index | null, camera?)` and `focusNext(camera?)`
- `setObjectVisible(index, visible)`
- `orbitByRadians(delta, camera?)`, `update(dt, input, camera?)`, and `reset(camera?)`
- `getState()` with exact/unresolved counts, source/web transforms, TVM counts, parser rule, and evidence boundary
- `ready` and `dispose()`

The preview is `ballz-xml-worlds-preview.html`. Main UI/race/content/census files were deliberately not edited.
