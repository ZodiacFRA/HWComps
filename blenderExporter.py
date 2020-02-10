import os
import shutil
import subprocess
import time
import struct

import bpy


OBJ_PATH = "/home/zodiac/Code/HW/S2/HWComps/data/objs/"
TEX_PATH = "/home/zodiac/Code/HW/S2/HWComps/data/textures/"
V4_HEADER_SIZE = 108
COLOR_INFO_SIZE = 68
HEADER_OFF = 14
DATA_OFF_FIELD = 10
SIZE_OFF  =  2


def setupScene(Override):
    if os.path.isdir(OBJ_PATH):
        shutil.rmtree(OBJ_PATH)
    os.makedirs(OBJ_PATH)
    if os.path.isdir(TEX_PATH):
        shutil.rmtree(TEX_PATH)
    os.makedirs(TEX_PATH)
    # Deselect all objects in the scene
    for obj in bpy.data.objects:
        obj.select_set(state=False)

def exportObjs():
    hidden = [o.name for o in bpy.context.view_layer.objects if not o.hide_viewport and not o.visible_get()]
    for obj in bpy.data.objects:
        if obj.type != "MESH" or obj.name in hidden: continue
        print(f"{'-'*25} Exporting {obj.name}")

        obj.select_set(state=True)
        tmpPos = [obj.location[0], obj.location[1], obj.location[2]]
        # bpy.ops.object.transform_apply(rotation=True, scale=True)
        bpy.ops.object.mode_set(mode='EDIT')
        # bpy.ops.uv.unwrap()
        bpy.ops.uv.smart_project()#use_aspect=False, stretch_to_bounds=True)

        tex_name = f"{TEX_PATH}{obj.name}.bmp"
        if not os.path.isfile(f"{TEX_PATH}{obj.name}.bmp"):
            bpy.ops.uv.export_layout(
                                filepath = f"{TEX_PATH}{obj.name}.png",
                                export_all=False, mode='PNG',
                                size=[1024, 1024], opacity=1.0
            )
        else:
            print(f"Texture {tex_name} found, skipping texture export")
        bpy.ops.object.mode_set(mode='OBJECT')
        bpy.ops.export_scene.obj(
                            filepath=f"{OBJ_PATH}{obj.name}.obj",
                            check_existing=False, axis_forward='-Z', axis_up='Y',
                            filter_glob="*.obj;*.mtl", use_selection=True, use_animation=False,
                            use_mesh_modifiers=True, use_edges=True, use_smooth_groups=False,
                            use_smooth_groups_bitflags=False,
                            use_normals=True, use_uvs=True,
                            use_materials=False, use_triangles=True, use_nurbs=False,
                            use_vertex_groups=False, use_blen_objects=True, group_by_object=False,
                            group_by_material=False, keep_vertex_order=True, global_scale=1,
        )
        obj.location = tmpPos
        obj.select_set(state=False)


def rewriteHeader(old_bmp_name, new_bmp_name=None):
    header = bytes.fromhex("""424d 3600 3000 0000 0000 3600 0000 2800
0000 0004 0000 0004 0000 0100 1800 0000
0000 0000 3000 130b 0000 130b 0000 0000
0000 0000 0000""")
    to_remove = bytes.fromhex("""424d 8a00 3000 0000 0000 8a00 0000 7c00
0000 0004 0000 0004 0000 0100 1800 0000
0000 0000 3000 130b 0000 130b 0000 0000
0000 0000 0000 0000 ff00 00ff 0000 ff00
0000 0000 00ff 4247 5273 0000 0000 0000
0000 0000 0000 0000 0000 0000 0000 0000
0000 0000 0000 0000 0000 0000 0000 0000
0000 0000 0000 0000 0000 0400 0000 0000
0000 0000 0000 0000 0000""")
    if new_bmp_name is None:
        new_bmp_name = old_bmp_name
    data = bytearray(open(old_bmp_name, "rb").read())
    final = header + data[len(to_remove):]
    with open(new_bmp_name, "wb") as output_file:
        output_file.write(final)


if __name__ == '__main__':
    print(f"{'-'*50} Starting export")
    setupScene(input("Override already existing textures? [y]/[n]\n") == 'y')
    exportObjs()
    print(f"{'-'*50} Objs export done")

    print("Converting png to bmps...")
    pngs = [os.path.join(TEX_PATH, f) for f in os.listdir(TEX_PATH) if f.endswith(".png")]
    for png in pngs:
        child = subprocess.Popen(["convert", png, "-type", "truecolor", png[:png.rfind('.')] + ".bmp"])
    print(f"{'-'*50} Conversions done, sleeping for 4s before deleting pngs")
    time.sleep(4)
    for png in pngs:
        os.remove(png)
    print(f"{'-'*50} Removing color info!")
    bmps = [os.path.join(TEX_PATH, f) for f in os.listdir(TEX_PATH) if f.endswith(".bmp")]
    for bmp in bmps:
        rewriteHeader(bmp)

    print(f"{'-'*50} Finished!")
