from __future__ import print_function

import bpy
import hashlib
import json
import os
import sys
from mathutils import Vector


def tuple3(value):
    return [round(float(value[0]), 6), round(float(value[1]), 6), round(float(value[2]), 6)]


def sha256(path):
    digest = hashlib.sha256()
    with open(path, "rb") as handle:
        while True:
            chunk = handle.read(1024 * 1024)
            if not chunk:
                break
            digest.update(chunk)
    return digest.hexdigest().upper()


def material_record(material):
    textures = []
    if material:
        for slot in material.texture_slots:
            if not slot or not slot.texture:
                continue
            image = getattr(slot.texture, "image", None)
            textures.append({
                "texture": slot.texture.name,
                "image": image.name if image else None,
                "filepath": bpy.path.abspath(image.filepath) if image and image.filepath else None,
                "packed": bool(image and image.packed_file),
            })
    return {
        "name": material.name if material else None,
        "diffuse": [round(float(channel), 6) for channel in material.diffuse_color] if material else None,
        "diffuseIntensity": round(float(material.diffuse_intensity), 6) if material else None,
        "specular": [round(float(channel), 6) for channel in material.specular_color] if material else None,
        "specularIntensity": round(float(material.specular_intensity), 6) if material else None,
        "alpha": round(float(material.alpha), 6) if material else None,
        "textures": textures,
    }


def object_record(obj):
    record = {
        "name": obj.name,
        "type": obj.type,
        "parent": obj.parent.name if obj.parent else None,
        "location": tuple3(obj.location),
        "rotationEuler": tuple3(obj.rotation_euler),
        "scale": tuple3(obj.scale),
        "dimensions": tuple3(obj.dimensions),
        "hidden": bool(obj.hide or obj.hide_render),
        "layers": [index for index, enabled in enumerate(obj.layers) if enabled],
    }
    if obj.type == "MESH":
        mesh = obj.data
        record["mesh"] = {
            "name": mesh.name,
            "vertices": len(mesh.vertices),
            "edges": len(mesh.edges),
            "polygons": len(mesh.polygons),
            "uvLayers": [layer.name for layer in mesh.uv_layers],
            "materials": [material.name if material else None for material in mesh.materials],
        }
    elif obj.type == "CAMERA":
        world_rotation = obj.matrix_world.to_quaternion()
        record["camera"] = {
            "lens": round(float(obj.data.lens), 6),
            "clipStart": round(float(obj.data.clip_start), 6),
            "clipEnd": round(float(obj.data.clip_end), 6),
            "worldLocation": tuple3(obj.matrix_world.to_translation()),
            "worldForward": tuple3(world_rotation * Vector((0.0, 0.0, -1.0))),
            "worldUp": tuple3(world_rotation * Vector((0.0, 1.0, 0.0))),
        }
    elif obj.type in {"LAMP", "LIGHT"}:
        data = obj.data
        record["light"] = {
            "kind": data.type,
            "energy": round(float(data.energy), 6),
            "color": [round(float(channel), 6) for channel in data.color],
            "distance": round(float(data.distance), 6),
        }
    return record


def main():
    separator = sys.argv.index("--") if "--" in sys.argv else len(sys.argv)
    args = sys.argv[separator + 1:]
    if not args:
        raise RuntimeError("Expected output JSON path after --")
    output_path = os.path.abspath(args[0])
    source_path = bpy.data.filepath
    objects = [object_record(obj) for obj in bpy.context.scene.objects]
    materials = [material_record(material) for material in bpy.data.materials]
    images = []
    for image in bpy.data.images:
        filepath = bpy.path.abspath(image.filepath) if image.filepath else None
        images.append({
            "name": image.name,
            "filepath": filepath,
            "exists": bool(filepath and os.path.isfile(filepath)),
            "packed": bool(image.packed_file),
            "size": [int(image.size[0]), int(image.size[1])],
        })
    payload = {
        "schema": "graphysx.archive-blend-inspection/v1",
        "blenderVersion": list(bpy.app.version),
        "source": {
            "path": source_path,
            "bytes": os.path.getsize(source_path),
            "sha256": sha256(source_path),
        },
        "scene": bpy.context.scene.name,
        "activeCamera": bpy.context.scene.camera.name if bpy.context.scene.camera else None,
        "world": bpy.context.scene.world.name if bpy.context.scene.world else None,
        "objectCount": len(objects),
        "objectsByType": {},
        "meshTotals": {
            "vertices": sum(item.get("mesh", {}).get("vertices", 0) for item in objects),
            "polygons": sum(item.get("mesh", {}).get("polygons", 0) for item in objects),
        },
        "objects": objects,
        "materials": materials,
        "images": images,
    }
    for item in objects:
        payload["objectsByType"][item["type"]] = payload["objectsByType"].get(item["type"], 0) + 1
    parent = os.path.dirname(output_path)
    if parent and not os.path.isdir(parent):
        os.makedirs(parent)
    with open(output_path, "w") as handle:
        json.dump(payload, handle, indent=2, sort_keys=True)
    print("GRAPHYSX_INSPECTION=" + output_path)


main()
