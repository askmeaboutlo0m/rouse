/*
 * Copyright (c) 2019 askmeaboutloom
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "../common.h"


typedef struct SceneData {
    R_Model  *cube;
    R_Binder *binder;
    float    rotation_x, rotation_y;
    int      u_model, u_view, u_proj;
} SceneData;


static void on_tick(void *data, R_UNUSED bool rendered)
{
    SceneData *sd = data;
    sd->rotation_x += 0.01f;
    sd->rotation_y += 0.03f;
}


static void on_render(void *data, R_Camera *camera)
{
    SceneData *sd = data;
    R_gl_clear(0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0);
    R_binder_begin(sd->binder);

    R_M4 model = R_m4_rotate_x(
            R_m4_rotate_y(R_m4_identity(), sd->rotation_y), sd->rotation_x);

    R_GL(glUniformMatrix4fv, sd->u_model, 1, GL_FALSE, R_M4_GL(model));
    R_camera_bind(camera, sd->u_proj, sd->u_view);

    for (int i = 0; i < sd->cube->mesh.count; ++i) {
        R_Mesh *mesh = R_model_mesh_by_index(sd->cube, i);
        R_binder_draw(sd->binder, mesh);
    }

    R_binder_end(sd->binder);
}


static void on_free(void *data)
{
    SceneData *sd = data;
    R_binder_free(sd->binder);
    R_model_free(sd->cube);
    free(sd);
}


static R_Binder *init_binder(R_Mesh *mesh)
{
    int indices  = R_mesh_buffer_index_by_name(mesh, "indices");
    int vertices = R_mesh_buffer_index_by_name(mesh, "vertices");
    int normals  = R_mesh_buffer_index_by_name(mesh, "normals");
    R_Binder *binder = R_binder_new(
            R_BINDER_INDEXED | R_BINDER_DEPTH_TEST | R_BINDER_CULLING,
            R_binder_draw_mesh_by_index, R_user_int(indices),
            "<glsl/cube.vert", "<glsl/cube.frag",
            R_bind_mesh_buffer_by_index, R_user_int(indices),
            R_bind_mesh_buffer_by_index, R_user_int(vertices),
            R_bind_mesh_buffer_by_index, R_user_int(normals),
            NULL);
    return binder;
}

static void *on_init(R_UNUSED void *user)
{
    SceneData *sd   = R_NEW(sd);
    sd->cube        = R_model_from_file("test/data/cube.rmodel");
    sd->binder      = init_binder(R_model_mesh_by_index(sd->cube, 0));
    sd->rotation_x  = 0.0f;
    sd->rotation_y  = 0.0f;
    sd->u_model     = R_binder_uniform_location(sd->binder, "u_model");
    sd->u_view      = R_binder_uniform_location(sd->binder, "u_view");
    sd->u_proj      = R_binder_uniform_location(sd->binder, "u_proj");
    return sd;
}


static R_Scene *init_scene(void *user)
{
    return common_init(on_init, user, NULL, on_tick, on_render, on_free);
}

int main(R_UNUSED int argc, R_UNUSED char **argv)
{
    R_main("cube", 1280, 720, 1, init_scene, NULL);
    return 0;
}
