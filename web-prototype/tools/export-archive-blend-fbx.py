from __future__ import print_function

import bpy
import hashlib
import json
import os
import sys


def sha256(path):
    digest = hashlib.sha256()
    with open(path, "rb") as handle:
        while True:
            chunk = handle.read(1024 * 1024)
            if not chunk:
                break
            digest.update(chunk)
    return digest.hexdigest().upper()


def main():
    separator = sys.argv.index("--") if "--" in sys.argv else len(sys.argv)
    args = sys.argv[separator + 1:]
    if len(args) < 2:
        raise RuntimeError("Expected output FBX and manifest paths after --")
    output_path = os.path.abspath(args[0])
    manifest_path = os.path.abspath(args[1])
    for path in (output_path, manifest_path):
        parent = os.path.dirname(path)
        if parent and not os.path.isdir(parent):
            os.makedirs(parent)

    source_path = bpy.data.filepath
    bpy.ops.export_scene.fbx(
        filepath=output_path,
        use_selection=False,
        object_types={"EMPTY", "CAMERA", "LAMP", "MESH"},
        use_mesh_modifiers=True,
        mesh_smooth_type="FACE",
        add_leaf_bones=False,
        bake_anim=False,
        axis_forward="-Z",
        axis_up="Y",
        path_mode="AUTO",
        embed_textures=False,
        use_custom_props=True,
    )
    manifest = {
        "schema": "graphysx.archive-blend-fbx-export/v1",
        "blenderVersion": list(bpy.app.version),
        "source": {
            "path": source_path,
            "bytes": os.path.getsize(source_path),
            "sha256": sha256(source_path),
        },
        "export": {
            "path": output_path,
            "bytes": os.path.getsize(output_path),
            "sha256": sha256(output_path),
            "axisForward": "-Z",
            "axisUp": "Y",
            "includes": ["EMPTY", "CAMERA", "LAMP", "MESH"],
            "bakeAnimation": False,
            "meshModifiers": True,
        },
    }
    with open(manifest_path, "w") as handle:
        json.dump(manifest, handle, indent=2, sort_keys=True)
    print("GRAPHYSX_EXPORT=" + output_path)
    print("GRAPHYSX_MANIFEST=" + manifest_path)


main()
