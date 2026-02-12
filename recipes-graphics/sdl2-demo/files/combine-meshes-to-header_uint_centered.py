# Usage:
# python3 combine-meshes-to-header_uint_centered.py Meshes/ > model_data.h

# Meshes/ = directory containing exported .obj Alias Mesh(s) of the model using FreeCAD v1.0.2

import os
import sys

def parse_obj(filepath):
    vertices, indices, colors = [], [], []
    with open(filepath, 'r') as f:
        for line in f:
            parts = line.split()
            if not parts: continue
            if parts[0] == 'v':
                x, y, z = float(parts[1]), float(parts[2]), float(parts[3])
                vertices.append((x, y, z))
                if len(parts) >= 7:
                    r, g, b = float(parts[4])/255.0, float(parts[5])/255.0, float(parts[6])/255.0
                else:
                    r, g, b = 1.0, 1.0, 1.0
                colors.append((r, g, b))
            elif parts[0] == 'f':
                indices.append(int(parts[1].split('/')[0]) - 1)
                indices.append(int(parts[2].split('/')[0]) - 1)
                indices.append(int(parts[3].split('/')[0]) - 1)
    return vertices, colors, indices

def generate_header(directory):
    obj_files = sorted([f for f in os.listdir(directory) if f.endswith('.obj')])
    
    # PASS 1: Find Global Bounds
    g_min = [float('inf')] * 3
    g_max = [float('-inf')] * 3
    
    for filename in obj_files:
        v_list, _, _ = parse_obj(os.path.join(directory, filename))
        for v in v_list:
            for i in range(3):
                if v[i] < g_min[i]: g_min[i] = v[i]
                if v[i] > g_max[i]: g_max[i] = v[i]

    g_center = [(g_min[i] + g_max[i]) / 2.0 for i in range(3)]

    # PASS 2: Output Header with Global Offset applied
    print("#ifndef MODEL_DATA_H\n#define MODEL_DATA_H\n#include <SDL2/SDL_opengles2.h>\n")
    print("typedef struct {")
    print("    const GLfloat* vertices;")
    print("    const GLuint* indices;")
    print("    int num_indices;")
    print("    int num_vertices;")
    print("    GLuint vbo;")
    print("    GLuint ibo;")
    print("} RenderObject;\n")

    object_names = []
    for filename in obj_files:
        name = filename.replace(' ', '_').replace('(', '').replace(')', '').replace('-', '_').replace('.', '_')
        object_names.append(name)
        v_list, c_list, i_list = parse_obj(os.path.join(directory, filename))
        
        print(f"const GLfloat {name}_verts[] = {{")
        for v, c in zip(v_list, c_list):
            # SUBTRACT GLOBAL CENTER HERE
            print(f"    {v[0]-g_center[0]:.4f}f, {v[1]-g_center[1]:.4f}f, {v[2]-g_center[2]:.4f}f, {c[0]:.3f}f, {c[1]:.3f}f, {c[2]:.3f}f,")
        print("};\n")

        print(f"const GLuint {name}_indices[] = {{")
        for i in range(0, len(i_list), 3):
            print(f"    {i_list[i]}, {i_list[i+1]}, {i_list[i+2]},")
        print("};\n")

    print("RenderObject my_model[] = {")
    for name in object_names:
        print(f"    {{ {name}_verts, {name}_indices, sizeof({name}_indices)/sizeof(GLuint), sizeof({name}_verts)/(6*sizeof(GLfloat)), 0, 0 }},")
    print(f"}};\nconst int num_objects = {len(object_names)};\n#endif")

if __name__ == "__main__":
    generate_header(sys.argv[1])
