from __future__ import print_function

import argparse
import hashlib
import json
import sys

import bpy


def rounded(value):
    return round(float(value), 6)


def convert_vector(vector):
    """Blender Z-up/right-handed -> Unity-authored Y-up -> Three.js right-handed."""
    return [rounded(vector.x), rounded(vector.z), rounded(-vector.y)]


def mesh_role(name):
    if name == "Cube":
        return "level"
    if name.startswith("Cube."):
        return "track-wall"
    if name in ("StartL", "StartR"):
        return "start-marker"
    if name == "FinishLine":
        return "finish-line"
    return "decor"


def mesh_bounds(positions):
    minimum = [float("inf"), float("inf"), float("inf")]
    maximum = [float("-inf"), float("-inf"), float("-inf")]
    for index in range(0, len(positions), 3):
        for axis in range(3):
            value = positions[index + axis]
            minimum[axis] = min(minimum[axis], value)
            maximum[axis] = max(maximum[axis], value)
    return {"min": [rounded(value) for value in minimum], "max": [rounded(value) for value in maximum]}


def export_mesh_object(source_object):
    mesh = source_object.data
    world = source_object.matrix_world
    normal_matrix = world.to_3x3().inverted().transposed()
    uv_layer = mesh.uv_layers.active.data if mesh.uv_layers.active else None
    slots = [slot.name for slot in mesh.materials]
    groups = {}

    for polygon in mesh.polygons:
        loop_indices = list(polygon.loop_indices)
        if len(loop_indices) < 3:
            continue
        material_name = slots[polygon.material_index] if polygon.material_index < len(slots) else "Unassigned"
        group = groups.setdefault(material_name, {"positions": [], "normals": [], "uvs": [], "indices": []})

        for fan_index in range(1, len(loop_indices) - 1):
            triangle = [loop_indices[0], loop_indices[fan_index], loop_indices[fan_index + 1]]
            # The axis conversion changes handedness once; reverse the triangle
            # so the Three.js front face matches the Unity scene.
            triangle.reverse()
            for loop_index in triangle:
                loop = mesh.loops[loop_index]
                vertex = world * mesh.vertices[loop.vertex_index].co
                normal = (normal_matrix * loop.normal).normalized()
                converted_position = convert_vector(vertex)
                converted_normal = convert_vector(normal)
                group["positions"].extend(converted_position)
                group["normals"].extend(converted_normal)
                if uv_layer:
                    uv = uv_layer[loop_index].uv
                    group["uvs"].extend([rounded(uv.x), rounded(uv.y)])
                group["indices"].append(len(group["indices"]))

    exported = []
    for material_name in sorted(groups.keys()):
        group = groups[material_name]
        if not group["positions"]:
            continue
        exported.append({
            "name": source_object.name + ":" + material_name,
            "positions": group["positions"],
            "normals": group["normals"],
            "uvs": group["uvs"] if group["uvs"] else None,
            "indices": group["indices"],
            "material": material_name,
            "bounds": mesh_bounds(group["positions"]),
        })

    return {
        "source": source_object.name,
        "role": mesh_role(source_object.name),
        "meshes": exported,
    }


def material_record(material):
    return {
        "name": material.name,
        "diffuse": [rounded(value) for value in material.diffuse_color],
        "diffuseIntensity": rounded(material.diffuse_intensity),
        "specular": [rounded(value) for value in material.specular_color],
        "specularIntensity": rounded(material.specular_intensity),
        "hardness": rounded(material.specular_hardness),
    }


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--source", required=True)
    parser.add_argument("--output", required=True)
    script_args = sys.argv[sys.argv.index("--") + 1:] if "--" in sys.argv else []
    args = parser.parse_args(script_args)

    with open(args.source, "rb") as source_file:
        source_hash = hashlib.sha256(source_file.read()).hexdigest()

    hidden_waypoints = set()
    raceline = bpy.data.objects.get("Raceline")
    waypoints = []
    if raceline:
        for index, child in enumerate(raceline.children):
            hidden_waypoints.add(child.name)
            waypoints.append({
                "index": index,
                "name": child.name,
                "position": convert_vector(child.matrix_world.to_translation()),
            })

    objects = []
    for source_object in sorted(bpy.context.scene.objects, key=lambda item: item.name):
        if source_object.type != "MESH" or source_object.name in hidden_waypoints:
            continue
        objects.append(export_mesh_object(source_object))

    all_positions = []
    for exported_object in objects:
        for mesh in exported_object["meshes"]:
            all_positions.extend(mesh["positions"])

    payload = {
        "format": "graphysx.ballz18-level01/v1",
        "source": args.source,
        "sourceSha256": source_hash,
        "axisConversion": "Blender (x,y,z) -> Three/Unity presentation (x,z,-y)",
        "materials": [material_record(material) for material in sorted(bpy.data.materials, key=lambda item: item.name)],
        "objects": objects,
        "aiWaypoints": waypoints,
        "bounds": mesh_bounds(all_positions),
    }

    with open(args.output, "w") as output_file:
        json.dump(payload, output_file, separators=(",", ":"), sort_keys=True)
        output_file.write("\n")

    print("GRAPHYSX_BALLZ18_LEVEL01", len(objects), len(waypoints), payload["bounds"])


if __name__ == "__main__":
    main()
