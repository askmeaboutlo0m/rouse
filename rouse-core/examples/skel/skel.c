/*
 * Copyright (c) 2019 - 2022 askmeaboutloom
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
#include <rouse.h>


#define NBONES 20

#define MOVE_FORWARD  (1 << 0)
#define MOVE_BACKWARD (1 << 1)
#define MOVE_LEFT     (1 << 2)
#define MOVE_RIGHT    (1 << 3)
#define MOVE_UP       (1 << 4)
#define MOVE_DOWN     (1 << 5)
#define LOOK_UP       (1 << 6)
#define LOOK_DOWN     (1 << 7)
#define LOOK_LEFT     (1 << 8)
#define LOOK_RIGHT    (1 << 9)

typedef enum BufferIndex {
    BUFFER_INDEX_INDICES,
    BUFFER_INDEX_VERTICES,
    BUFFER_INDEX_NORMALS,
    BUFFER_INDEX_UVS,
    BUFFER_INDEX_BONES,
    BUFFER_INDEX_WEIGHTS,
    BUFFER_INDEX_COUNT,
} BufferIndex;

typedef struct SceneNode {
    R_M4 offset;
    R_M4 animated_transform;
} SceneNode;

typedef struct SceneData {
    R_FrameBuffer   *fb;
    R_FrameRenderer *fr;
    R_FirstPerson   *fp;
    R_Camera        *camera;
    unsigned int    texture;
    double          seconds;
    R_Model         *model;
    R_M4            global_inverse_transform;
    SceneNode       *nodes;
    unsigned int    program;
    int             u_pv, u_sampler, u_bones;
    unsigned int    buffers[BUFFER_INDEX_COUNT];
    float           bones[NBONES * 16];
    R_V3            speed;
    unsigned int    input_flags;
} SceneData;


static unsigned int on_key(SDL_Scancode scancode)
{
    switch (scancode) {
        case SDL_SCANCODE_W:      return MOVE_FORWARD;
        case SDL_SCANCODE_A:      return MOVE_LEFT;
        case SDL_SCANCODE_S:      return MOVE_BACKWARD;
        case SDL_SCANCODE_D:      return MOVE_RIGHT;
        case SDL_SCANCODE_SPACE:  return MOVE_UP;
        case SDL_SCANCODE_LSHIFT: return MOVE_DOWN;
        case SDL_SCANCODE_Q:      return LOOK_LEFT;
        case SDL_SCANCODE_E:      return LOOK_RIGHT;
        case SDL_SCANCODE_R:      return LOOK_UP;
        case SDL_SCANCODE_F:      return LOOK_DOWN;
        default:                  return 0;
    }
}

static void on_event(R_Scene *scene, SDL_Event *event)
{
    SceneData *sd = scene->user.data;
    switch (event->type) {
        case SDL_KEYDOWN:
            sd->input_flags |= on_key(event->key.keysym.scancode);
            break;
        case SDL_KEYUP:
            sd->input_flags &= ~on_key(event->key.keysym.scancode);
            break;
        default:
            break;
    }
}


static float apply_direction(unsigned int input_flags, unsigned int positive,
                             unsigned int negative)
{
    if (input_flags & positive) {
        if (!(input_flags & negative)) {
            return 1.0f;
        }
    }
    else if (input_flags & negative) {
        return -1.0f;
    }
    return 0.0f;
}

static void move(R_FirstPerson *fp, R_V3 *speed, unsigned int input_flags)
{
    float forward = apply_direction(input_flags, MOVE_FORWARD, MOVE_BACKWARD);
    float right   = apply_direction(input_flags, MOVE_RIGHT,   MOVE_LEFT    );
    float up      = apply_direction(input_flags, MOVE_UP,      MOVE_DOWN    );
    speed->x      = speed->x * 0.9f + forward * 0.05f;
    speed->y      = speed->y * 0.9f + right   * 0.05f;
    speed->z      = speed->z * 0.9f + up      * 0.05f;
    R_first_person_move(fp, speed->x, speed->y, speed->z);
}

static void look(R_FirstPerson *fp, unsigned int input_flags)
{
    float h = apply_direction(input_flags, LOOK_LEFT, LOOK_RIGHT);
    float v = apply_direction(input_flags, LOOK_UP,   LOOK_DOWN );
    R_first_person_look(fp, h * 0.02f, v * 0.02f);
}

static void on_tick(R_Scene *scene, R_UNUSED bool rendered)
{
    SceneData *sd = scene->user.data;
    move(sd->fp, &sd->speed, sd->input_flags);
    look(sd->fp, sd->input_flags);
    sd->seconds += R_tick_length / 1000.0;
}


static void dump_m4(const char *title, R_M4 m)
{
    R_info("%s (%f, %f, %f, %f,", title, m.m00, m.m01, m.m02, m.m03);
    R_info("%s  %f, %f, %f, %f,", title, m.m10, m.m11, m.m12, m.m13);
    R_info("%s  %f, %f, %f, %f,", title, m.m20, m.m21, m.m22, m.m23);
    R_info("%s  %f, %f, %f, %f)", title, m.m30, m.m31, m.m32, m.m33);
}

static bool d;

static R_M4 m4_lerp(R_M4 from, R_M4 to, float t)
{
    R_V4 from_position;
    R_M4 from_rotation;
    R_V3 from_scale;
    glms_decompose(from, &from_position, &from_rotation, &from_scale);

    R_V4 to_position;
    R_M4 to_rotation;
    R_V3 to_scale;
    glms_decompose(to, &to_position, &to_rotation, &to_scale);

    R_V4 position = R_v4_lerp(from_position, to_position, t);
    R_Qn rotation = R_qn_slerp(R_m4_quat(from_rotation), R_m4_quat(to_rotation), t);
    R_V3 scaling  = R_v3_lerp(from_scale, to_scale, t);
    R_M4 m = R_m4_identity();
    m = R_m4_translate_to(m, glms_vec4_copy3(position));
    m = R_qn_rotate(m, rotation);
    m = R_m4_scale_xyz(m, scaling);
    return m;
}

static R_V3 get_position_key_frame(R_NodeChannel *nc, double t)
{
    int count = nc->position_key_frame.count;
    R_PositionKeyFrame *pkfs = nc->position_key_frame.values;
    for (int i = count - 1; i >= 0; --i) {
        R_PositionKeyFrame *pkf  = &pkfs[i];
        double             ticks = pkf->ticks;
        if (ticks <= t) {
            int next = i + 1;
            if (next < count) {
                R_PositionKeyFrame *next_pkf = &pkfs[next];
                double t1 = t - ticks;
                double t2 = next_pkf->ticks - ticks;
                return R_v3_lerp(pkf->value, next_pkf->value,
                                 R_double2float(t1 / t2));
            }
            else {
                return pkf->value;
            }
        }
    }
    return count == 0 ? R_v3_zero() : pkfs[0].value;
}

static R_Qn get_rotation_key_frame(R_NodeChannel *nc, double t)
{
    int count = nc->rotation_key_frame.count;
    R_RotationKeyFrame *rkfs = nc->rotation_key_frame.values;
    for (int i = count - 1; i >= 0; --i) {
        R_RotationKeyFrame *rkf  = &rkfs[i];
        double             ticks = rkf->ticks;
        if (ticks <= t) {
            int next = i + 1;
            if (next < count) {
                R_RotationKeyFrame *next_rkf = &rkfs[next];
                double t1 = t - ticks;
                double t2 = next_rkf->ticks - ticks;
                return R_qn_slerp(rkf->value, next_rkf->value,
                                  R_double2float(t1 / t2));
            }
            else {
                return rkf->value;
            }
        }
    }
    return count == 0 ? R_qn_identity() : rkfs[0].value;
}

static R_V3 get_scaling_key_frame(R_NodeChannel *nc, double t)
{
    int count = nc->scaling_key_frame.count;
    R_ScalingKeyFrame *skfs = nc->scaling_key_frame.values;
    for (int i = count - 1; i >= 0; --i) {
        R_ScalingKeyFrame *skf  = &skfs[i];
        double             ticks = skf->ticks;
        if (ticks <= t) {
            int next = i + 1;
            if (next < count) {
                R_ScalingKeyFrame *next_skf = &skfs[next];
                double t1 = t - ticks;
                double t2 = next_skf->ticks - ticks;
                return R_v3_lerp(skf->value, next_skf->value,
                                 R_double2float(t1 / t2));
            }
            else {
                return skf->value;
            }
        }
    }
    return count == 0 ? R_v3_zero() : skfs[0].value;
}

static R_M4 get_animation_transform(R_Animation *anim, R_Node *node, double seconds)
{
    double t = fmod(seconds * anim->ticks_per_second, anim->duration);
    for (int i = 0; i < anim->node_channel.count; ++i) {
        R_NodeChannel *nc = &anim->node_channel.values[i];
        if (nc->node_id == node->id) {
            if (!d) R_info("Node %d '%s'", node->id, node->name);
            R_V3 position = get_position_key_frame(nc, t);
            R_Qn rotation = get_rotation_key_frame(nc, t);
            R_V3 scaling  = get_scaling_key_frame(nc, t);
            if (!d) R_info("position %f %f %f", position.x, position.y, position.z);
            if (!d) R_info("rotation %f %f %f %f", rotation.x, rotation.y, rotation.z, rotation.w);
            if (!d) R_info("scaling  %f %f %f", scaling.x, scaling.y, scaling.z);
            R_M4 m = R_m4_identity();
            m = R_m4_translate_to(m, position);
            m = R_qn_rotate(m, rotation);
            m = R_m4_scale_xyz(m, scaling);
            return m;
        }
    }
    if (!d) R_info("Node %d '%s' no aanim", node->id, node->name);
    return node->transform;
}

static void apply_bone_pose(SceneData *sd, int node_id, R_M4 parent_transform)
{
    R_Node *node = R_model_node_by_id(sd->model, node_id);
    R_assert(node->id == node_id, "node id equal");
    if (!d) {
        R_info("Node %d %s", node->id, node->name);
    }
    // R_M4 current_local_transform = sd->model->animation.count == 0
    //                              ? node->transform
    //                              : get_animation_transform(
    //                                     sd->model->animation.values[1],
    //                                     node, sd->seconds);
    R_M4 current_local_transform = m4_lerp(
        get_animation_transform(sd->model->animation.values[0], node, sd->seconds),
        get_animation_transform(sd->model->animation.values[1], node, sd->seconds),
        0.5f);
    R_M4 current_transform = R_m4_mul(parent_transform, current_local_transform);
    R_M4 animated_transform = R_m4_mul(current_transform, sd->nodes[node_id].offset);
    sd->nodes[node_id].animated_transform = animated_transform;
    for (int i = 0; i < node->child_id.count; ++i) {
        apply_bone_pose(sd, node->child_id.values[i], current_transform);
    }
}

static void bind_bones(SceneData *sd, int bone_count,
                       R_Bone *bones[R_STATIC(bone_count)], R_M4 c)
{
    int armature_id = -1;
    for (int i = 0; i < bone_count; ++i) {
        SceneNode *sn = &sd->nodes[bones[i]->node_id];
        sn->offset = bones[i]->offset;
        sn->animated_transform = c;
        if (armature_id == -1) {
            armature_id = bones[i]->armature_id;
        }
        else if (armature_id != bones[i]->armature_id){
            R_die("Multiple armatures: %d != %d", armature_id, bones[i]->armature_id);
        }
    }

    if (!d) {
        dump_m4("c", c);
    }

    apply_bone_pose(sd, armature_id, R_m4_identity());

    if (!d) {
        R_M4 a = sd->nodes[bones[0]->node_id].animated_transform;
        dump_m4("a", a);
    }

    for (int i = 0; i < bone_count; ++i) {
        // R_Node *node = sd->nodes[bone->node_id].node;
        // R_M4   a     = sd->nodes[bone->armature_id].bind_transform;
        // R_M4   m     = sd->nodes[bone->node_id].bind_transform;
        //R_M4   m     = R_m4_mul(a, c);
        // R_M4 m = R_m4_mul(c, sd->nodes[bone->node_id].animated_transform);
        R_M4 m = sd->nodes[bones[i]->node_id].animated_transform;
        memcpy(&sd->bones[i * 16], m.raw, sizeof(float[16]));
    }
    R_GL(glUniformMatrix4fv, sd->u_bones, NBONES, GL_FALSE, sd->bones);
    d = true;
}

static int bind_index(unsigned int glbuf, R_MeshBuffer *mbuf)
{
    R_GL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, glbuf);
    R_gl_mesh_buffer_data(mbuf, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
    return mbuf->count;
}

static void bind_vertex(unsigned int glbuf, R_MeshBuffer *mbuf,
                        unsigned int index)
{
    R_GL(glBindBuffer, GL_ARRAY_BUFFER, glbuf);
    R_gl_mesh_buffer_data(mbuf, GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    R_gl_mesh_buffer_vertex_attrib_pointer(mbuf, index, GL_FALSE, 0, NULL);
}

static void render_node(SceneData *sd, R_Node *node, R_M4 parent_transform)
{
    R_M4 transform = R_m4_mul(parent_transform, node->transform);

    int mesh_count = node->mesh_id.count;
    if (mesh_count > 0) {
        // R_GL(glUniformMatrix4fv, sd->u_m, 1, GL_FALSE, R_M4_GL(model));
        int *mesh_ids = node->mesh_id.values;
        for (int i = 0; i < mesh_count; ++i) {
            R_Mesh *mesh = R_model_mesh_by_id(sd->model, mesh_ids[i]);
            bind_bones(sd, mesh->bone.count, mesh->bone.values, transform);
            int count = bind_index(sd->buffers[BUFFER_INDEX_INDICES],
                                   R_mesh_buffer_by_name(mesh, "indices"));
            bind_vertex(sd->buffers[BUFFER_INDEX_VERTICES],
                        R_mesh_buffer_by_name(mesh, "vertices"), 0);
            bind_vertex(sd->buffers[BUFFER_INDEX_NORMALS],
                        R_mesh_buffer_by_name(mesh, "normals"), 1);
            bind_vertex(sd->buffers[BUFFER_INDEX_UVS],
                        R_mesh_buffer_by_name(mesh, "uv0"), 2);
            bind_vertex(sd->buffers[BUFFER_INDEX_BONES],
                        R_mesh_buffer_by_name(mesh, "bones"), 3);
            bind_vertex(sd->buffers[BUFFER_INDEX_WEIGHTS],
                        R_mesh_buffer_by_name(mesh, "weights"), 4);
            R_GL(glDrawElements, GL_TRIANGLES, count, GL_UNSIGNED_SHORT, NULL);
        }
    }

    R_Model *model         = sd->model;
    int     child_id_count = node->child_id.count;
    for (int i = 0; i < child_id_count; ++i) {
        render_node(sd, R_model_node_child(model, node, i), transform);
    }
}

static void on_render(R_Scene *scene)
{
    R_GL(glEnable, GL_DEPTH_TEST);
    R_GL(glEnable, GL_CULL_FACE);
    R_GL(glDisable, GL_BLEND);
    R_GL(glDepthFunc, GL_LESS);
    R_GL(glCullFace, GL_BACK);
    for (unsigned int i = 0; i < BUFFER_INDEX_COUNT; ++i) {
        R_GL(glEnableVertexAttribArray, i);
    }
    unsigned int max_vertex_attribs = R_int2uint(R_gl_max_vertex_attribs);
    for (unsigned int i = BUFFER_INDEX_COUNT; i < max_vertex_attribs; ++i) {
        R_GL(glDisableVertexAttribArray, i);
    }

    SceneData *sd = scene->user.data;
    R_first_person_apply(sd->fp, sd->camera);
    R_frame_buffer_bind(sd->fb);

    R_gl_clear(0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0);

    R_GL(glUseProgram, sd->program);
    R_M4 pv = R_m4_mul(sd->camera->proj, sd->camera->view);
    R_GL(glUniformMatrix4fv, sd->u_pv, 1, GL_FALSE, R_M4_GL(pv));
    R_gl_texture_bind(0, sd->texture, sd->u_sampler);
    render_node(sd, R_model_root_node(sd->model), R_m4_identity());

    R_frame_buffer_unbind();
    R_gl_clear(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0);
    R_frame_renderer_draw(sd->fr, sd->fb);
}


static void on_free(R_Scene *scene)
{
    SceneData *sd = scene->user.data;
    R_GL(glDeleteBuffers, BUFFER_INDEX_COUNT, sd->buffers);
    R_gl_program_free(sd->program);
    R_gl_texture_free(sd->texture);
    R_model_decref(sd->model);
    R_first_person_free(sd->fp);
    R_camera_free(sd->camera);
    R_frame_renderer_free(sd->fr);
    R_frame_buffer_free(sd->fb);
    free(sd);
}


static R_Scene *init_scene(R_UNUSED void *user)
{
    SceneData *sd = R_NEW(sd);

    R_FrameBufferOptions fbo = R_frame_buffer_options();
    fbo.width      = 1920;
    fbo.height     = 1080;
    fbo.color_type = R_FRAME_BUFFER_ATTACHMENT_TEXTURE;
    fbo.depth_type = R_FRAME_BUFFER_ATTACHMENT_BUFFER;

    sd->fb          = R_frame_buffer_new(&fbo);
    sd->fr          = R_frame_renderer_new(false);
    sd->fp          = R_first_person_new(R_v3(0.0f, 5.0f, 20.0f), R_PI, 0.0f);
    sd->camera      = R_camera_new_perspective(R_to_rad(60.0f), 16.0f / 9.0f,
                                               0.1f, 1000.0f);

    R_TextureOptions texture_options = R_texture_options();
    sd->texture   = R_gl_texture_new("test/data/cuboid.png", &texture_options);
    sd->model     = R_model_from_file("test/data/cuboid.rmodel");

    sd->seconds                  = 0.0;
    sd->global_inverse_transform = R_m4_inv(sd->model->node.values[0]->transform);

    sd->nodes = R_ANEW(sd->nodes, R_int2size(sd->model->node.count));
    for (int i = 0; i < sd->model->node.count; ++i) {
        sd->nodes[i].offset             = R_m4_identity();
        sd->nodes[i].animated_transform = R_m4_identity();
    }
    // char *c = R_model_dump(sd->model); puts(c); free(c);

    sd->program   = R_gl_program_new("<glsl/skel.vert", "<glsl/skel.frag");
    // sd->u_m       = R_gl_uniform_location(sd->program, "u_m");
    sd->u_pv      = R_gl_uniform_location(sd->program, "u_pv");
    sd->u_bones   = R_gl_uniform_location(sd->program, "u_bones");
    sd->u_sampler = R_gl_uniform_location(sd->program, "u_sampler");
    R_GL(glGenBuffers, BUFFER_INDEX_COUNT, sd->buffers);

    R_M4 identity = R_m4_identity();
    for (int i = 0; i < NBONES; ++i) {
        memcpy(&sd->bones[i * 16], identity.raw, sizeof(float[16]));
    }

    sd->speed       = R_v3_zero();
    sd->input_flags = 0;

    R_Scene *scene   = R_scene_new();
    scene->on_event  = on_event;
    scene->on_tick   = on_tick;
    scene->on_render = on_render;
    scene->on_free   = on_free;
    scene->user.data = sd;
    return scene;
}

int main(R_UNUSED int argc, R_UNUSED char **argv)
{
    R_MainArgs args   = R_main_args(init_scene, NULL);
    args.window.title = "skel";
    R_main(&args);
    return 0;
}
