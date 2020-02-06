import os
import shutil

import bpy

OBJ_PATH = "/home/zodiac/Code/HW/S2/HWComps/data/objs/"


class SceneParser(object):
    def __init__(self):
        if os.path.isdir(OBJ_PATH):
            shutil.rmtree(OBJ_PATH)
        os.makedirs(OBJ_PATH)
        # Deselect all objects in the scene
        for obj in bpy.data.objects:
            obj.select_set(state=False)

    def exportObjs(self):
        for obj in bpy.data.objects:
            if obj.type != "MESH": continue
            print(f"{'-'*25} Exporting {obj.name}")
            obj.select_set(state=True)
            bpy.ops.object.transform_apply(rotation=True, scale=True)
            bpy.ops.export_scene.obj(
                                filepath=f"{OBJ_PATH}{obj.name}.obj",
                                check_existing=False, axis_forward='-Z', axis_up='Y',
                                filter_glob="*.obj;*.mtl", use_selection=True, use_animation=False,
                                use_mesh_modifiers=True, use_edges=True, use_smooth_groups=False,
                                use_smooth_groups_bitflags=False,
                                use_normals=True, use_uvs=True,
                                use_materials=False, use_triangles=True, use_nurbs=False,
                                use_vertex_groups=False, use_blen_objects=True, group_by_object=False,
                                group_by_material=False, keep_vertex_order=False, global_scale=1,
            )
            obj.select_set(state=False)


if __name__ == '__main__':
    print(f"{'-'*50} Starting export")
    scene_parser = SceneParser()
    scene_parser.exportObjs()
    print(f"{'-'*50} Export done!")
