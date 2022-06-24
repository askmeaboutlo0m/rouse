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
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <cglm/struct.h>
#include <rouse_config.h>
#include "common.h"
#include "geom.h"
#include "parse.h"
#include "model.h"
#include "refcount.h"
#include "string.h"


static void check_file_magic(R_Parse *parse)
{
    static const char FILE_MAGIC[] = "rmodel2";
    R_parse_read_bytes(parse, sizeof(FILE_MAGIC));
    if (memcmp(parse->buffer, FILE_MAGIC, sizeof(FILE_MAGIC)) != 0) {
        R_PARSE_DIE(parse, "bad file magic, want \"%s\"", FILE_MAGIC);
    }
}


#define DEF_READ_VALUES(NAME, CALL, TYPE) \
    static void NAME(R_Parse *parse, TYPE *arr, int count) \
    { \
        for (int i = 0; i < count; ++i) { \
            arr[i] = CALL(parse); \
        } \
    }

DEF_READ_VALUES(read_ushorts, R_parse_read_ushort, unsigned short)
DEF_READ_VALUES(read_floats,  R_parse_read_float,  float)

static R_V3 read_v3(R_Parse *parse)
{
    R_V3 v;
    v.x = R_parse_read_float(parse);
    v.y = R_parse_read_float(parse);
    v.z = R_parse_read_float(parse);
    return v;
}

static R_Qn read_qn(R_Parse *parse)
{
    R_Qn qn;
    qn.x = R_parse_read_float(parse);
    qn.y = R_parse_read_float(parse);
    qn.z = R_parse_read_float(parse);
    qn.w = R_parse_read_float(parse);
    return qn;
}

static R_M4 read_m4(R_Parse *parse)
{
    R_M4 m;
    m.m00 = R_parse_read_float(parse);
    m.m01 = R_parse_read_float(parse);
    m.m02 = R_parse_read_float(parse);
    m.m03 = R_parse_read_float(parse);
    m.m10 = R_parse_read_float(parse);
    m.m11 = R_parse_read_float(parse);
    m.m12 = R_parse_read_float(parse);
    m.m13 = R_parse_read_float(parse);
    m.m20 = R_parse_read_float(parse);
    m.m21 = R_parse_read_float(parse);
    m.m22 = R_parse_read_float(parse);
    m.m23 = R_parse_read_float(parse);
    m.m30 = R_parse_read_float(parse);
    m.m31 = R_parse_read_float(parse);
    m.m32 = R_parse_read_float(parse);
    m.m33 = R_parse_read_float(parse);
    return m;
}


static R_MeshBuffer *read_buffer(R_Parse *parse)
{
    int type = R_parse_read_uchar(parse);
    R_PARSE_DEBUG(parse, "   buffer type: %d", type);
    if (type != R_BUFFER_TYPE_USHORT && type != R_BUFFER_TYPE_FLOAT) {
        R_PARSE_DIE(parse, "unknown mesh buffer type '%d'", type);
    }

    char *name = R_parse_read_string(parse);
    R_PARSE_DEBUG(parse, "   buffer name \"%s\"", name ? name : "");

    int divisor = R_parse_read_uchar(parse);
    R_PARSE_DEBUG(parse, "   buffer element divisor: %d", divisor);

    int count = R_parse_read_ushort(parse);
    R_PARSE_DEBUG(parse, "   buffer element count: %d", count);

    if (divisor == 0 || count % divisor != 0) {
        R_PARSE_DIE(parse, "mesh buffer element count %d isn't divisible by divisor %d",
                  count, divisor);
    }

    R_MeshBufferValues values;
    switch (type) {
        case R_BUFFER_TYPE_USHORT:
            values.ushorts = R_ANEW(values.ushorts, R_int2size(count));
            read_ushorts(parse, values.ushorts, count);
            break;
        case R_BUFFER_TYPE_FLOAT:
            values.floats = R_ANEW(values.floats, R_int2size(count));
            read_floats(parse, values.floats, count);
            break;
        default:
            R_PARSE_DIE(parse, "impossible mesh buffer type '%c'", type);
    }

    R_MeshBuffer *mbuf = R_NEW_INIT_STRUCT(mbuf, R_MeshBuffer,
        R_MAGIC_INIT(R_MeshBuffer) 1, (R_BufferType) type, name, count, divisor,
        {values});
    return mbuf;
}

static R_Bone *read_bone(R_Parse *parse)
{
    char *name = R_parse_read_string(parse);
    R_PARSE_DEBUG(parse, "   bone name \"%s\"", name ? name : "");

    R_M4 offset = read_m4(parse);
    R_PARSE_DEBUG(parse, "   bone offset (%f, %f, %f, %f,",
                  offset.m00, offset.m01, offset.m02, offset.m03);
    R_PARSE_DEBUG(parse, "                %f, %f, %f, %f,",
                  offset.m10, offset.m11, offset.m12, offset.m13);
    R_PARSE_DEBUG(parse, "                %f, %f, %f, %f,",
                  offset.m20, offset.m21, offset.m22, offset.m23);
    R_PARSE_DEBUG(parse, "                %f, %f, %f, %f)",
                  offset.m30, offset.m31, offset.m32, offset.m33);

    R_Bone *bone = R_NEW_INIT_STRUCT(bone, R_Bone,
        R_MAGIC_INIT(R_Bone) 1, name, offset, -1, -1);
    return bone;
}

static R_Mesh *read_buffers(R_Parse *parse, int id)
{
    int mbuf_count = R_parse_read_ushort(parse);
    R_PARSE_DEBUG(parse, "  buffer count: %d", mbuf_count);
    R_MeshBuffer **mbufs = R_ANEW(mbufs, R_int2size(mbuf_count));
    for (int i = 0; i < mbuf_count; ++i) {
        R_PARSE_DEBUG(parse, "  buffer %d/%d", i + 1, mbuf_count);
        mbufs[i] = read_buffer(parse);
    }

    int bone_count = R_parse_read_ushort(parse);
    R_PARSE_DEBUG(parse, "  bone count: %d", bone_count);
    R_Bone **bones = R_ANEW(bones, R_int2size(bone_count));
    for (int i = 0; i < bone_count; ++i) {
        R_PARSE_DEBUG(parse, "  bone %d/%d", i + 1, bone_count);
        bones[i] = read_bone(parse);
    }

    R_Mesh *mesh = R_NEW_INIT_STRUCT(mesh, R_Mesh,
        R_MAGIC_INIT(R_Mesh) 1, id, {mbuf_count, mbufs}, {bone_count, bones});
    return mesh;
}

static void read_meshes(R_Parse *parse, R_Model *model)
{
    int mcount = R_parse_read_ushort(parse);
    R_PARSE_DEBUG(parse, " mesh count: %d", mcount);
    model->mesh.count  = mcount;
    model->mesh.values = R_ANEW(model->mesh.values, R_int2size(mcount));
    for (int i = 0; i < mcount; ++i) {
        R_PARSE_DEBUG(parse, " mesh %d/%d", i + 1, mcount);
        model->mesh.values[i] = read_buffers(parse, i);
    }
}


static void resolve_bone_refs(R_Parse *parse, R_Model *model, R_Node *node)
{
    int type;
    while ((type = R_parse_read_uchar(parse)) != 0) {
        R_PARSE_DEBUG(parse, "  ref type %d", type);

        int mesh_index = R_parse_read_ushort(parse);
        R_PARSE_DEBUG(parse, "  ref mesh index %d", mesh_index);
        if (mesh_index >= model->mesh.count) {
            R_PARSE_DIE(parse, "Ref mesh index %d out of bounds (0 ... %d)",
                        mesh_index, model->mesh.count);
        }
        R_Mesh *mesh = model->mesh.values[mesh_index];

        int bone_index = R_parse_read_ushort(parse);
        R_PARSE_DEBUG(parse, "  ref bone index %d", bone_index);
        if (bone_index >= mesh->bone.count) {
            R_PARSE_DIE(parse, "Ref bone index %d out of bounds (0 ... %d)",
                        bone_index, mesh->bone.count);
        }
        R_Bone *bone = mesh->bone.values[bone_index];

        switch (type) {
            case 97: /* 'a' in ASCII */
                if (bone->armature_id == -1) {
                    bone->armature_id = node->id;
                }
                else {
                    R_PARSE_DIE(parse,
                                "Duplicate armature ref for mesh %d, bone %d",
                                mesh_index, bone_index);
                }
                break;
            case 110: /* 'n' in ASCII */
                if (bone->node_id == -1) {
                    bone->node_id = node->id;
                }
                else {
                    R_PARSE_DIE(parse,
                                "Duplicate node ref for mesh %d, bone %d",
                                mesh_index, bone_index);
                }
                break;
            default:
                R_PARSE_DIE(parse, "Unknown ref type %d", type);
        }
    }
}

static int read_node(R_Parse *parse, R_Model *model, int parent_id)
{
    int  id        = R_parse_read_ushort(parse);
    char *name     = R_parse_read_string(parse);
    R_V3 position  = read_v3(parse);
    R_Qn rotation  = read_qn(parse);
    R_V3 scaling   = read_v3(parse);
    R_M4 transform = glms_scale(glms_quat_rotate(glms_translate_make(position),
                                                 rotation), scaling);

    int mesh_id_count    = R_parse_read_ushort(parse);
    int *mesh_ids        = R_ANEW(mesh_ids, R_int2size(mesh_id_count));
    int model_mesh_count = model->mesh.count;
    for (int i = 0; i < mesh_id_count; ++i) {
        int mesh_id = R_parse_read_ushort(parse);
        if (mesh_id < model_mesh_count) {
            mesh_ids[i] = mesh_id;
        }
        else {
            R_PARSE_DIE(parse, "Node mesh id %d beyond mesh count %d",
                        mesh_id, model_mesh_count);
        }
    }

    R_Node *node = R_NEW_INIT_STRUCT(node, R_Node,
        R_MAGIC_INIT(R_Node) 1, id, parent_id, name, position, rotation,
        scaling, transform, {mesh_id_count, mesh_ids}, {0, NULL});

    if (id < model->node.count) {
        if (!model->node.values[id]) {
            model->node.values[id] = node;
        }
        else {
            R_PARSE_DIE(parse, "Duplicate node id %d", id);
        }
    }
    else {
        R_PARSE_DIE(parse, "Node id %d out of bounds", id);
    }

    resolve_bone_refs(parse, model, node);

    int child_id_count = R_parse_read_ushort(parse);
    int *child_ids     = R_ANEW(child_ids, R_int2size(child_id_count));
    for (int i = 0; i < child_id_count; ++i) {
        child_ids[i] = read_node(parse, model, id);
    }
    node->child_id.count  = child_id_count;
    node->child_id.values = child_ids;

    return id;
}

static void read_nodes(R_Parse *parse, R_Model *model)
{
    int    count   = R_parse_read_ushort(parse);
    R_Node **nodes = R_ANEW(nodes, R_int2size(count));
    for (int i = 0; i < count; ++i) {
        nodes[i] = NULL;
    }
    model->node.count  = count;
    model->node.values = nodes;
    read_node(parse, model, -1);
    if (count < 1 || !nodes[0]) {
        R_PARSE_DIE(parse, "Model has no root node (%d nodes)", count);
    }
}


static R_NodeChannel read_node_channel(R_Parse *parse)
{
    int node_id = R_parse_read_ushort(parse);

    int                pkf_count = R_parse_read_ushort(parse);
    R_PositionKeyFrame *pkfs     = R_ANEW(pkfs, R_int2size(pkf_count));
    for (int i = 0; i < pkf_count; ++i) {
        R_PositionKeyFrame *pkf = &pkfs[i];
        pkf->ticks              = R_parse_read_double(parse);
        pkf->value              = read_v3(parse);
    }

    int                rkf_count = R_parse_read_ushort(parse);
    R_RotationKeyFrame *rkfs     = R_ANEW(rkfs, R_int2size(rkf_count));
    for (int i = 0; i < rkf_count; ++i) {
        R_RotationKeyFrame *rkf = &rkfs[i];
        rkf->ticks              = R_parse_read_double(parse);
        rkf->value              = read_qn(parse);
    }

    int                skf_count = R_parse_read_ushort(parse);
    R_ScalingKeyFrame *skfs      = R_ANEW(skfs, R_int2size(skf_count));
    for (int i = 0; i < skf_count; ++i) {
        R_ScalingKeyFrame *skf = &skfs[i];
        skf->ticks             = R_parse_read_double(parse);
        skf->value             = read_v3(parse);
    }

    return (R_NodeChannel){
        node_id, {pkf_count, pkfs}, {rkf_count, rkfs}, {skf_count, skfs}};
}

static R_Animation *read_animation(R_Parse *parse)
{
    char   *name              = R_parse_read_string(parse);
    double duration           = R_parse_read_double(parse);
    double ticks_per_second   = R_parse_read_double(parse);

    int           nc_count = R_parse_read_ushort(parse);
    R_NodeChannel *ncs     = R_ANEW(ncs, R_int2size(nc_count));
    for (int i = 0; i < nc_count; ++i) {
        ncs[i] = read_node_channel(parse);
    }

    R_Animation *anim = R_NEW_INIT_STRUCT(anim, R_Animation,
        R_MAGIC_INIT(R_Animation) 1, name, duration, ticks_per_second,
        {nc_count, ncs});
    return anim;
}

static void read_animations(R_Parse *parse, R_Model *model)
{
    int         count   = R_parse_read_ushort(parse);
    R_Animation **anims = R_ANEW(anims, R_int2size(count));
    for (int i = 0; i < count; ++i) {
        anims[i] = read_animation(parse);
    }
    model->animation.count  = count;
    model->animation.values = anims;
}


static inline void check_mesh_buffer(
    R_UNUSED_UNLESS_DEBUG_OR_MAGIC R_MeshBuffer *mbuf)
{
    R_MAGIC_CHECK(R_MeshBuffer, mbuf);
    R_assert(mbuf->refs > 0, "refcount must always be positive");
}

static inline void check_bone(R_UNUSED_UNLESS_DEBUG_OR_MAGIC R_Bone *bone)
{
    R_MAGIC_CHECK(R_Bone, bone);
    R_assert(bone->refs > 0, "refcount must always be positive");
}

static inline void check_mesh(R_UNUSED_UNLESS_DEBUG_OR_MAGIC R_Mesh *mesh)
{
    R_MAGIC_CHECK(R_Mesh, mesh);
    R_assert(mesh->refs > 0, "refcount must always be positive");
}

static inline void check_node(R_UNUSED_UNLESS_DEBUG_OR_MAGIC R_Node *node)
{
    R_MAGIC_CHECK(R_Node, node);
    R_assert(node->refs > 0, "refcount must always be positive");
}

static inline void check_animation(
    R_UNUSED_UNLESS_DEBUG_OR_MAGIC R_Animation *anim)
{
    R_MAGIC_CHECK(R_Animation, anim);
    R_assert(anim->refs > 0, "refcount must always be positive");
}

static inline void check_model(R_UNUSED_UNLESS_DEBUG_OR_MAGIC R_Model *model)
{
    R_MAGIC_CHECK(R_Model, model);
    R_assert(model->refs > 0, "refcount must always be positive");
}


#ifdef ROUSE_DEBUG

static int count_node_refs(R_Model *model, R_Node *node, int id)
{
    int refs = node->id == id ? 1 : 0;
    for (int i = 0; i < node->child_id.count; ++i) {
        int child_id = node->child_id.values[i];
        refs += count_node_refs(model, model->node.values[child_id], id);
    }
    return refs;
}

static void check_mesh_tree(R_Model *model, int count,
                            R_Mesh *meshes[static count])
{
    R_Node *root = model->node.values[0];
    for (int i = 0; i < count; ++i) {
        R_Mesh *mesh = meshes[i];
        check_mesh(mesh);
        for (int j = 0; j < mesh->buffer.count; ++j) {
            R_MeshBuffer *mbuf = mesh->buffer.values[j];
            check_mesh_buffer(mbuf);
        }
        for (int j = 0; j < mesh->bone.count; ++j) {
            R_Bone *bone = mesh->bone.values[j];
            check_bone(bone);
            if (bone->armature_id != -1) {
                R_assert(count_node_refs(model, root, bone->armature_id) == 1,
                         "armature ref must point to exactly one node");
            }
            if (bone->node_id != -1) {
                R_assert(count_node_refs(model, root, bone->node_id) == 1,
                         "node ref must point to exactly one node");
            }
        }
    }
}

static void check_node_tree(R_Model *model)
{
    R_Node *root = model->node.values[0];
    for (int i = 0; i < model->node.count; ++i) {
        R_Node *node = model->node.values[i];
        check_node(node);
        for (int j = 0; j < node->child_id.count; ++j) {
            int child_id = node->child_id.values[j];
            R_assert(count_node_refs(model, root, child_id) == 1,
                        "node child ref must point to exactly one node");
        }
    }
}

static void check_animation_tree(R_Model *model)
{
    R_Node *root = model->node.values[0];
    for (int i = 0; i < model->animation.count; ++i) {
        R_Animation *anim = model->animation.values[i];
        check_animation(anim);
        for (int j = 0; j < anim->node_channel.count; ++j) {
            R_NodeChannel *nc = &anim->node_channel.values[j];
            R_assert(count_node_refs(model, root, nc->node_id) == 1,
                     "node channel ref must point to exactly one node");
        }
    }
}

static void check_tree(R_Model *model)
{
    check_model(model);
    check_mesh_tree(model, model->mesh.count, model->mesh.values);
    check_node_tree(model);
    check_animation_tree(model);
}

#endif


R_Model *R_model_new(const char *title, R_ParseReadFn read, R_UserData user,
                     int bufsize, unsigned char buffer[static bufsize])
{
    R_Parse parse = R_parse(title, read, user, bufsize, buffer);
    check_file_magic(&parse);

    R_Model *model = R_NEW(model);
    R_MAGIC_SET(R_Model, model);
    model->refs = 1;
    R_PARSE_DEBUG(&parse, "parsing model with bufsize %d", bufsize);
    read_meshes(&parse, model);
    read_nodes(&parse, model);
    read_animations(&parse, model);

    R_parse_die_unless_eof(&parse);
#ifdef ROUSE_DEBUG
    check_tree(model);
#endif
    return model;
}


R_Model *R_model_from_file(const char *path)
{
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        R_die("Can't open '%s': %s", path, strerror(errno));
    }

    unsigned char buffer[1024];
    R_Model *model = R_model_new(path, R_parse_from_file, R_user_data(fp),
                                 sizeof(buffer), buffer);

    if (fclose(fp) != 0) {
        R_warn("Error closing '%s': %s", path, strerror(errno));
    }

    return model;
}


static void free_animations(int acount, R_Animation *anims[R_STATIC(acount)])
{
    for (int i = 0; i < acount; ++i) {
        R_animation_decref(anims[i]);
    }
    free(anims);
}

static void free_nodes(int ncount, R_Node *nodes[R_STATIC(ncount)])
{
    for (int i = 0; i < ncount; ++i) {
        R_node_decref(nodes[i]);
    }
    free(nodes);
}

static void free_meshes(int mcount, R_Mesh *meshes[R_STATIC(mcount)])
{
    for (int i = 0; i < mcount; ++i) {
        R_mesh_decref(meshes[i]);
    }
    free(meshes);
}

static void free_model(R_Model *model)
{
    free_animations(model->animation.count, model->animation.values);
    free_nodes(model->node.count, model->node.values);
    free_meshes(model->mesh.count, model->mesh.values);
    R_MAGIC_POISON(R_Model, model);
    free(model);
}

R_DEFINE_REFCOUNT_FUNCS(R_Model, model, refs)


static void write_ushort(R_MeshBuffer *mbuf, int i, R_String *s)
{
    R_string_printf(s, "%u", (unsigned int) mbuf->ushorts[i]);
}

static void write_float(R_MeshBuffer *mbuf, int i, R_String *s)
{
    R_string_printf(s, "%f", (double) mbuf->floats[i]);
}

static void dump_mesh_buffer_values_to(
    R_MeshBuffer *mbuf, R_String *s,
    void (*write)(R_MeshBuffer *, int, R_String *))
{
    int count   = mbuf->count;
    int divisor = mbuf->divisor;
    for (int i = 0; i < count; ++i) {
        int mod = i % divisor;
        if (mod == 0) {
            R_string_append(s, "        (");
        }
        write(mbuf, i, s);
        if (mod == divisor - 1) {
            R_string_append(s, ")\n");
        }
        else {
            R_string_append(s, ", ");
        }
    }
}

static void dump_mesh_buffer_to(R_MeshBuffer *mbuf, R_String *s)
{
    R_string_printf(s, "buffer with %d elements and divisor %d\n",
                    mbuf->count, mbuf->divisor);
    if (mbuf->name) {
        R_string_printf(s, "      name: '%s'\n", mbuf->name);
    }

    if (mbuf->type == R_BUFFER_TYPE_USHORT) {
        R_string_append(s, "      ushort values:\n");
        dump_mesh_buffer_values_to(mbuf, s, write_ushort);
    }
    else if (mbuf->type == R_BUFFER_TYPE_FLOAT) {
        R_string_append(s, "      float values:\n");
        dump_mesh_buffer_values_to(mbuf, s, write_float);
    }
    else {
        R_string_append(s, "      unknown type, can't decode values\n");
    }
}

static void dump_mesh_to(R_Mesh *mesh, R_String *s)
{
    int buffer_count = mesh->buffer.count;
    R_string_printf(s, "mesh with %d buffer(s)\n", buffer_count);
    for (int i = 0; i < buffer_count; ++i) {
        R_string_printf(s, "    [%d] ", i);
        dump_mesh_buffer_to(mesh->buffer.values[i], s);
    }
}

static void dump_model_to(R_Model *model, R_String *s)
{
    int mesh_count = model->mesh.count;
    R_string_printf(s, "model with %d mesh(es)\n", mesh_count);
    for (int i = 0; i < mesh_count; ++i) {
        R_string_printf(s, "  [%d] ", i);
        dump_mesh_to(model->mesh.values[i], s);
    }
}

char *R_model_dump(R_Model *model)
{
    check_model(model);
    R_String *s = R_string_new(0);
    dump_model_to(model, s);
    return R_string_free_keep_buffer(s);
}


R_Mesh *R_model_mesh_by_id(R_Model *model, int id)
{
    check_model(model);
    if (id < 0 || id >= model->mesh.count) {
        R_die("Model mesh id %d out of bounds (0 ... %d)",
              id, model->mesh.count);
    }
    return model->mesh.values[id];
}

R_Node *R_model_root_node(R_Model *model)
{
    check_model(model);
    return model->node.values[0];
}

R_Node *R_model_node_by_id(R_Model *model, int id)
{
    check_model(model);
    if (id < 0 || id > model->node.count) {
        R_die("Model node id %d out of bounds (0 ... %d)",
              id, model->node.count);
    }
    return model->node.values[id];
}

R_Node *R_model_node_child(R_Model *model, R_Node *node, int child_index)
{
    check_node(node);
    if (child_index < 0 || child_index > node->child_id.count) {
        R_die("Node child index %d out of bounds (0 ... %d)",
              child_index, node->child_id.count);
    }
    return R_model_node_by_id(model, node->child_id.values[child_index]);
}


static void free_mesh(R_Mesh *mesh)
{
    int bonecount = mesh->bone.count;
    for (int i = 0; i < bonecount; ++i) {
        R_bone_decref(mesh->bone.values[i]);
    }
    free(mesh->bone.values);

    int bufcount = mesh->buffer.count;
    for (int i = 0; i < bufcount; ++i) {
        R_mesh_buffer_decref(mesh->buffer.values[i]);
    }

    free(mesh->buffer.values);
    R_MAGIC_POISON(R_Mesh, mesh);
    free(mesh);
}

R_DEFINE_REFCOUNT_FUNCS(R_Mesh, mesh, refs)

int R_mesh_buffer_index_by_name(R_Mesh *mesh, const char *name)
{
    check_mesh(mesh);
    for (int i = 0; i < mesh->buffer.count; ++i) {
        R_MeshBuffer *buffer = mesh->buffer.values[i];
        check_mesh_buffer(buffer);
        if (R_str_equal(name, buffer->name)) {
            return i;
        }
    }
    return -1;
}

R_MeshBuffer *R_mesh_buffer_by_index(R_Mesh *mesh, int index)
{
    check_mesh(mesh);
    if (index < 0 || index >= mesh->buffer.count) {
        R_die("Mesh buffer index %d out of bounds (0 ... %d)",
              index, mesh->buffer.count);
    }
    R_MeshBuffer *buffer = mesh->buffer.values[index];
    check_mesh_buffer(buffer);
    return buffer;
}

R_MeshBuffer *R_mesh_buffer_by_name(R_Mesh *mesh, const char *name)
{
    check_mesh(mesh);
    int           index  = R_mesh_buffer_index_by_name(mesh, name);
    R_MeshBuffer *buffer = mesh->buffer.values[index];
    check_mesh_buffer(buffer);
    return buffer;
}


static const char *buffer_type_name(R_BufferType type)
{
    switch (type) {
        case R_BUFFER_TYPE_USHORT:
            return "USHORT";
        case R_BUFFER_TYPE_FLOAT:
            return "FLOAT";
        default:
            return "unknown";
    }
}

#define R_DEF_MESH_BUFFER_BY_FNS(RETURN_TYPE, NAME, BUFFER_TYPE) \
    RETURN_TYPE *R_mesh_ ## NAME ## _by_index(R_Mesh *mesh, int index, \
                                              int *out_count) \
    { \
        R_MeshBuffer *buffer = R_mesh_buffer_by_index(mesh, index); \
        if (buffer->type != BUFFER_TYPE) { \
            R_die("Bad %s buffer type for '%s', wanted %d (%s) but got %d (%s)", \
                  #NAME, buffer->name ? buffer->name : "", \
                  BUFFER_TYPE,  buffer_type_name(BUFFER_TYPE), \
                  buffer->type, buffer_type_name(buffer->type)); \
        } \
        if (out_count) { \
            *out_count = buffer->count; \
        } \
        return buffer->NAME; \
    } \
    \
    RETURN_TYPE *R_mesh_ ## NAME ## _by_name(R_Mesh *mesh, const char *name, \
                                             int *out_count) \
    { \
        int index = R_mesh_buffer_index_by_name(mesh, name); \
        if (index < 0) { \
            R_die("Can't find %s buffer '%s'", #NAME, name); \
        } \
        return R_mesh_ ## NAME ## _by_index(mesh, index, out_count); \
    }

R_DEF_MESH_BUFFER_BY_FNS(unsigned short, ushorts, R_BUFFER_TYPE_USHORT)
R_DEF_MESH_BUFFER_BY_FNS(float,          floats,  R_BUFFER_TYPE_FLOAT)



R_MeshBuffer *R_mesh_buffer_new(R_BufferType type, const char *name, int count,
                                int divisor)
{
    R_assert(type == R_BUFFER_TYPE_FLOAT || type == R_BUFFER_TYPE_USHORT,
             "Mesh buffer type must be valid");
    R_assert(count > 0, "Mesh buffer count must be greater than zero");
    R_assert(divisor > 0, "Mesh buffer divisor must be greater than zero");
    R_assert(count % divisor == 0, "Mesh buffer divisor must divide count");

    R_MeshBufferValues values;
    size_t size = R_int2size(count);
    switch (type) {
        case R_BUFFER_TYPE_FLOAT:
            values.floats = R_malloc(sizeof(*values.floats) * size);
            break;
        case R_BUFFER_TYPE_USHORT:
            values.ushorts = R_malloc(sizeof(*values.ushorts) * size);
            break;
        default:
            R_die("Impossible mesh buffer type '%d'", (int) type);
    }

    R_MeshBuffer *mbuf = R_NEW_INIT_STRUCT(mbuf, R_MeshBuffer,
        R_MAGIC_INIT(R_MeshBuffer) 1, type, R_strdup(name), count, divisor,
        {values});
    check_mesh_buffer(mbuf);
    return mbuf;
}

static void free_mesh_buffer(R_MeshBuffer *mbuf)
{
    free(mbuf->name);
    switch (mbuf->type) {
        case R_BUFFER_TYPE_USHORT:
            free(mbuf->ushorts);
            break;
        case R_BUFFER_TYPE_FLOAT:
            free(mbuf->floats);
            break;
        default: /* Uhh... that's not a valid model? */
            R_warn("Can't free mesh buffer with unknown type '%d'", mbuf->type);
            break;
    }
    R_MAGIC_POISON(R_MeshBuffer, mbuf);
    free(mbuf);
}

R_DEFINE_REFCOUNT_FUNCS(R_MeshBuffer, mesh_buffer, refs)


static void free_bone(R_Bone *bone)
{
    free(bone->name);
    R_MAGIC_POISON(R_Bone, bone);
    free(bone);
}

R_DEFINE_REFCOUNT_FUNCS(R_Bone, bone, refs)


static void free_node(R_Node *node)
{
    free(node->mesh_id.values);
    free(node->child_id.values);
    free(node->name);
    R_MAGIC_POISON(R_Node, node);
    free(node);
}

R_DEFINE_REFCOUNT_FUNCS(R_Node, node, refs)


static void free_animation(R_Animation *anim)
{
    int node_animation_count = anim->node_channel.count;
    for (int i = 0; i < node_animation_count; ++i) {
        R_NodeChannel *nc = &anim->node_channel.values[i];
        free(nc->scaling_key_frame.values);
        free(nc->rotation_key_frame.values);
        free(nc->position_key_frame.values);
    }
    free(anim->node_channel.values);
    free(anim->name);
    R_MAGIC_POISON(R_Animation, anim);
    free(anim);
}

R_DEFINE_REFCOUNT_FUNCS(R_Animation, animation, refs)
