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
#include "refcount.h"
#include "string.h"
#include "anim/ease.h"
#include "3rdparty/gles2_inc.h"
#include "render/gl.h"
#include "model.h"


struct R_Model {
    R_MAGIC_FIELD
    int refs;
    int id;
    struct {
        int    count;
        R_Mesh *values;
    } mesh;
    struct {
        int    count;
        R_Node *values;
    } node;
    struct {
        int         count;
        R_Animation *values;
    } animation;
};

struct R_Mesh {
    R_MAGIC_FIELD
    R_Model *model;
    int     id;
    struct {
        int         count;
        R_Attribute *values;
    } attribute;
    struct {
        int    count;
        R_Bone *values;
    } bone;
};

typedef union R_AttributeValues {
    unsigned short *ushorts;
    float          *floats;
} R_AttributeValues;

struct R_Attribute {
    R_MAGIC_FIELD
    R_Model         *model;
    R_AttributeType type;
    char            *name;
    int             count;
    int             divisor;
    union {
        R_AttributeValues values;
        unsigned short    *ushorts;
        float             *floats;
    };
};

struct R_Bone {
    R_MAGIC_FIELD
    R_Model *model;
    char    *name;
    R_M4    offset;
    int     armature_id; /* -1 means none */
    int     node_id;     /* -1 means none */
};

struct R_Node {
    R_MAGIC_FIELD
    R_Model *model;
    int     id;
    int     parent_id;
    char    *name;
    R_V3    position;
    R_Qn    rotation;
    R_V3    scaling;
    R_M4    transform;
    bool    child_meshes;
    struct {
        int count;
        int *values;
    } mesh_id;
    struct {
        int count;
        int *values;
    } child_id;
};

struct R_Animation {
    R_MAGIC_FIELD
    R_Model *model;
    char    *name;
    double  duration_ticks;
    double  ticks_per_second;
    struct {
        int           count;
        R_NodeChannel *values;
    } node_channel;
};

typedef struct R_PositionKeyFrame {
    R_MAGIC_FIELD
    double ticks;
    R_V3   value;
} R_PositionKeyFrame;

typedef struct R_RotationKeyFrame {
    R_MAGIC_FIELD
    double ticks;
    R_Qn   value;
} R_RotationKeyFrame;

typedef struct R_ScalingKeyFrame {
    R_MAGIC_FIELD
    double ticks;
    R_V3   value;
} R_ScalingKeyFrame;

struct R_NodeChannel {
    R_MAGIC_FIELD
    R_Model *model;
    int     node_id;
    struct {
        int                count;
        R_PositionKeyFrame *values;
    } position_key_frame;
    struct {
        int                count;
        R_RotationKeyFrame *values;
    } rotation_key_frame;
    struct {
        int               count;
        R_ScalingKeyFrame *values;
    } scaling_key_frame;
};


static void check_file_magic(R_Parse *parse)
{
    static const char FILE_MAGIC[] = "rmodel2";
    R_parse_read_bytes(parse, sizeof(FILE_MAGIC));
    if (memcmp(parse->buffer, FILE_MAGIC, sizeof(FILE_MAGIC)) != 0) {
        R_PARSE_DIE(parse, "bad file magic, want \"%s\"", FILE_MAGIC);
    }
}


static void read_ushorts(R_Parse *parse, int count,
                         unsigned short arr[R_STATIC(count)])
{
    for (int i = 0; i < count; ++i) {
        arr[i] = R_parse_read_ushort(parse);
    }
}

static void read_floats(R_Parse *parse, int count, float arr[R_STATIC(count)])
{
    for (int i = 0; i < count; ++i) {
        arr[i] = R_parse_read_float(parse);
    }
}

static R_V3 read_v3(R_Parse *parse, R_UNUSED_UNLESS_DEBUG const char *name)
{
    R_V3 v;
    v.x = R_parse_read_float(parse);
    R_PARSE_DEBUG(parse, "%s x %f", name, v.x);
    v.y = R_parse_read_float(parse);
    R_PARSE_DEBUG(parse, "%s y %f", name, v.y);
    v.z = R_parse_read_float(parse);
    R_PARSE_DEBUG(parse, "%s z %f", name, v.z);
    return v;
}

static R_Qn read_qn(R_Parse *parse, R_UNUSED_UNLESS_DEBUG const char *name)
{
    R_Qn qn;
    qn.x = R_parse_read_float(parse);
    R_PARSE_DEBUG(parse, "%s x %f", name, qn.x);
    qn.y = R_parse_read_float(parse);
    R_PARSE_DEBUG(parse, "%s y %f", name, qn.y);
    qn.z = R_parse_read_float(parse);
    R_PARSE_DEBUG(parse, "%s z %f", name, qn.z);
    qn.w = R_parse_read_float(parse);
    R_PARSE_DEBUG(parse, "%s w %f", name, qn.w);
    return qn;
}

static R_M4 read_m4(R_Parse *parse, R_UNUSED_UNLESS_DEBUG const char *name)
{
    R_M4 m;
    m.m00 = R_parse_read_float(parse);
    R_PARSE_DEBUG(parse, "%s m00 %f", name, m.m00);
    m.m01 = R_parse_read_float(parse);
    R_PARSE_DEBUG(parse, "%s m01 %f", name, m.m01);
    m.m02 = R_parse_read_float(parse);
    R_PARSE_DEBUG(parse, "%s m02 %f", name, m.m02);
    m.m03 = R_parse_read_float(parse);
    R_PARSE_DEBUG(parse, "%s m03 %f", name, m.m03);
    m.m10 = R_parse_read_float(parse);
    R_PARSE_DEBUG(parse, "%s m10 %f", name, m.m10);
    m.m11 = R_parse_read_float(parse);
    R_PARSE_DEBUG(parse, "%s m11 %f", name, m.m11);
    m.m12 = R_parse_read_float(parse);
    R_PARSE_DEBUG(parse, "%s m12 %f", name, m.m12);
    m.m13 = R_parse_read_float(parse);
    R_PARSE_DEBUG(parse, "%s m13 %f", name, m.m13);
    m.m20 = R_parse_read_float(parse);
    R_PARSE_DEBUG(parse, "%s m20 %f", name, m.m20);
    m.m21 = R_parse_read_float(parse);
    R_PARSE_DEBUG(parse, "%s m21 %f", name, m.m21);
    m.m22 = R_parse_read_float(parse);
    R_PARSE_DEBUG(parse, "%s m22 %f", name, m.m22);
    m.m23 = R_parse_read_float(parse);
    R_PARSE_DEBUG(parse, "%s m23 %f", name, m.m23);
    m.m30 = R_parse_read_float(parse);
    R_PARSE_DEBUG(parse, "%s m30 %f", name, m.m30);
    m.m31 = R_parse_read_float(parse);
    R_PARSE_DEBUG(parse, "%s m31 %f", name, m.m31);
    m.m32 = R_parse_read_float(parse);
    R_PARSE_DEBUG(parse, "%s m32 %f", name, m.m32);
    m.m33 = R_parse_read_float(parse);
    R_PARSE_DEBUG(parse, "%s m33 %f", name, m.m33);
    return m;
}


static R_Attribute read_attribute(R_Parse *parse, R_Model *model)
{
    int type = R_parse_read_uchar(parse);
    R_PARSE_DEBUG(parse, "type %d", type);

    char *name   = R_parse_read_string(parse);
    R_PARSE_DEBUG(parse, "name '%s'", name ? name : "");

    int divisor = R_parse_read_uchar(parse);
    R_PARSE_DEBUG(parse, "divisor %d", divisor);

    int count = R_parse_read_ushort(parse);
    R_PARSE_DEBUG(parse, "count, %d", count);

    if (divisor == 0 || count % divisor != 0) {
        R_PARSE_DIE(
            parse, "attribute element count %d isn't divisible by divisor %d",
            count, divisor);
    }

    R_AttributeValues values;
    switch (type) {
        case R_ATTRIBUTE_TYPE_USHORT:
            R_PARSE_DEBUG(parse, "read %d ushorts", count);
            values.ushorts = R_ANEW(values.ushorts, R_int2size(count));
            read_ushorts(parse, count, values.ushorts);
            break;
        case R_ATTRIBUTE_TYPE_FLOAT:
            R_PARSE_DEBUG(parse, "read %d floats", count);
            values.floats = R_ANEW(values.floats, R_int2size(count));
            read_floats(parse, count, values.floats);
            break;
        default:
            R_PARSE_DIE(parse, "unknown attribute type '%d'", type);
    }

    return (R_Attribute){
        R_MAGIC_INIT(R_Attribute) model, (R_AttributeType) type, name, count,
        divisor, {values}};
}

static R_Bone read_bone(R_Parse *parse, R_Model *model)
{
    char *name = R_parse_read_string(parse);
    R_PARSE_DEBUG(parse, "name '%s'", name ? name : "");
    R_M4 offset = read_m4(parse, "offset");
    return(R_Bone){R_MAGIC_INIT(R_Bone) model, name, offset, -1, -1};
}

static R_Mesh read_mesh(R_Parse *parse, R_Model *model, int id)
{
    int attr_count = R_parse_read_ushort(parse);
    R_PARSE_DEBUG(parse, "%d attributes", attr_count);
    R_Attribute *attrs = R_ANEW(attrs, R_int2size(attr_count));
    for (int i = 0; i < attr_count; ++i) {
        R_PARSE_DEBUG(parse, "read attribute %d", i);
        attrs[i] = read_attribute(parse, model);
    }

    int bone_count = R_parse_read_ushort(parse);
    R_PARSE_DEBUG(parse, "%d bones", bone_count);
    R_Bone *bones = R_ANEW(bones, R_int2size(bone_count));
    for (int i = 0; i < bone_count; ++i) {
        R_PARSE_DEBUG(parse, "read bone %d", i);
        bones[i] = read_bone(parse, model);
    }

    return(R_Mesh){R_MAGIC_INIT(R_Mesh) model, id, {attr_count, attrs},
                   {bone_count, bones}};
}

static void read_meshes(R_Parse *parse, R_Model *model)
{
    int mcount  = R_parse_read_ushort(parse);
    R_PARSE_DEBUG(parse, "%d meshes", mcount);
    R_Mesh *meshes = R_ANEW(model->mesh.values, R_int2size(mcount));
    for (int i = 0; i < mcount; ++i) {
        R_PARSE_DEBUG(parse, "read mesh %d", i);
        meshes[i] = read_mesh(parse, model, i);
    }
    model->mesh.count  = mcount;
    model->mesh.values = meshes;
}


static int read_bone_ref(R_Parse *parse)
{
    int type = R_parse_read_uchar(parse);
    R_PARSE_DEBUG(parse, "bone ref type %d", type);
    return type;
}

static void resolve_bone_refs(R_Parse *parse, R_Model *model, R_Node *node)
{
    int type;
    while ((type = read_bone_ref(parse)) != 0) {
        int mesh_id = R_parse_read_ushort(parse);
        R_PARSE_DEBUG(parse, "mesh id %d", mesh_id);
        if (mesh_id >= model->mesh.count) {
            R_PARSE_DIE(parse, "Ref mesh id %d out of bounds (0 ... %d)",
                        mesh_id, model->mesh.count);
        }
        R_Mesh *mesh = &model->mesh.values[mesh_id];

        int bone_index = R_parse_read_ushort(parse);
        R_PARSE_DEBUG(parse, "bone index %d", bone_index);
        if (bone_index >= mesh->bone.count) {
            R_PARSE_DIE(parse, "Ref bone index %d out of bounds (0 ... %d)",
                        bone_index, mesh->bone.count);
        }
        R_Bone *bone = &mesh->bone.values[bone_index];

        switch (type) {
            case 97: /* 'a' in ASCII */
                if (bone->armature_id == -1) {
                    bone->armature_id = node->id;
                }
                else {
                    R_PARSE_DIE(parse,
                                "Duplicate armature ref for mesh %d, bone %d",
                                mesh_id, bone_index);
                }
                break;
            case 110: /* 'n' in ASCII */
                if (bone->node_id == -1) {
                    bone->node_id = node->id;
                }
                else {
                    R_PARSE_DIE(parse,
                                "Duplicate node ref for mesh %d, bone %d",
                                mesh_id, bone_index);
                }
                break;
            default:
                R_PARSE_DIE(parse, "Unknown ref type %d", type);
        }
    }
}

static R_Node *read_node(R_Parse *parse, R_Model *model, int parent_id)
{
    int id = R_parse_read_ushort(parse);
    R_PARSE_DEBUG(parse, "id %d", id);

    char *name = R_parse_read_string(parse);
    R_PARSE_DEBUG(parse, "name '%s'", name ? name : "");

    R_V3 position  = read_v3(parse, "position");
    R_Qn rotation  = read_qn(parse, "rotation");
    R_V3 scaling   = read_v3(parse, "scaling");
    R_M4 transform = glms_scale(glms_quat_rotate(glms_translate_make(position),
                                                 rotation), scaling);

    int mesh_id_count = R_parse_read_ushort(parse);
    R_PARSE_DEBUG(parse, "mesh id count %d", mesh_id_count);

    int *mesh_ids        = R_ANEW(mesh_ids, R_int2size(mesh_id_count));
    int model_mesh_count = model->mesh.count;
    for (int i = 0; i < mesh_id_count; ++i) {
        int mesh_id = R_parse_read_ushort(parse);
        R_PARSE_DEBUG(parse, "mesh id %d = %d", i, mesh_id);
        if (mesh_id < model_mesh_count) {
            mesh_ids[i] = mesh_id;
        }
        else {
            R_PARSE_DIE(parse, "Node mesh id %d beyond mesh count %d",
                        mesh_id, model_mesh_count);
        }
    }

    R_Node *node;
    if (id < model->node.count) {
        if (model->node.values[id].id == -1) {
            node = &model->node.values[id];
        }
        else {
            R_PARSE_DIE(parse, "Duplicate node id %d", id);
        }
    }
    else {
        R_PARSE_DIE(parse, "Node id %d out of bounds", id);
    }

    *node = (R_Node){R_MAGIC_INIT(R_Node) model, id, parent_id, name, position,
                     rotation, scaling, transform, false,
                     {mesh_id_count, mesh_ids}, {0, NULL}};
    resolve_bone_refs(parse, model, node);

    int child_id_count = R_parse_read_ushort(parse);
    R_PARSE_DEBUG(parse, "%d child ids", child_id_count);

    int  *child_ids   = R_ANEW(child_ids, R_int2size(child_id_count));
    bool child_meshes = false;
    for (int i = 0; i < child_id_count; ++i) {
        R_PARSE_DEBUG(parse, "child %d", i);
        R_Node *child = read_node(parse, model, id);
        child_ids[i] = child->id;
        if (!child_meshes && (child->mesh_id.count || child->child_meshes)) {
            child_meshes = true;
        }
    }
    node->child_meshes    = child_meshes;
    node->child_id.count  = child_id_count;
    node->child_id.values = child_ids;

    return node;
}

static void read_nodes(R_Parse *parse, R_Model *model)
{
    int count  = R_parse_read_ushort(parse);
    R_PARSE_DEBUG(parse, "node count %d", count);

    R_Node *nodes = R_ANEW(nodes, R_int2size(count));
    for (int i = 0; i < count; ++i) {
        nodes[i] = (R_Node){R_MAGIC_INIT_POISON NULL, -1, -1, NULL, R_v3_zero(),
                            R_qn_identity(), R_v3_zero(), R_m4_identity(),
                            false, {0, NULL},  {0, NULL}};
    }
    model->node.count  = count;
    model->node.values = nodes;
    read_node(parse, model, -1);

    if (count < 1 || nodes[0].id == -1) {
        R_PARSE_DIE(parse, "Model has no root node (%d nodes)", count);
    }
}

static int compare_ticks(double ticks_a, double ticks_b)
{
    return ticks_a < ticks_b ? -1 : ticks_a > ticks_b ? 1 : 0;
}

static int sort_position_key_frame(const void *a, const void *b)
{
    R_MAGIC_CHECK(R_PositionKeyFrame, a);
    R_MAGIC_CHECK(R_PositionKeyFrame, b);
    double ticks_a = ((const R_PositionKeyFrame *)a)->ticks;
    double ticks_b = ((const R_PositionKeyFrame *)b)->ticks;
    return compare_ticks(ticks_a, ticks_b);
}

static int sort_rotation_key_frame(const void *a, const void *b)
{
    R_MAGIC_CHECK(R_RotationKeyFrame, a);
    R_MAGIC_CHECK(R_RotationKeyFrame, b);
    double ticks_a = ((const R_RotationKeyFrame *)a)->ticks;
    double ticks_b = ((const R_RotationKeyFrame *)b)->ticks;
    return compare_ticks(ticks_a, ticks_b);
}

static int sort_scaling_key_frame(const void *a, const void *b)
{
    R_MAGIC_CHECK(R_ScalingKeyFrame, a);
    R_MAGIC_CHECK(R_ScalingKeyFrame, b);
    double ticks_a = ((const R_ScalingKeyFrame *)a)->ticks;
    double ticks_b = ((const R_ScalingKeyFrame *)b)->ticks;
    return compare_ticks(ticks_a, ticks_b);
}

static R_NodeChannel read_node_channel(R_Parse *parse, R_Model *model)
{
    int node_id = R_parse_read_ushort(parse);
    R_PARSE_DEBUG(parse, "node id %d", node_id);

    int pkf_count = R_parse_read_ushort(parse);
    R_PARSE_DEBUG(parse, "%d position key frames", pkf_count);

    R_PositionKeyFrame *pkfs = R_ANEW(pkfs, R_int2size(pkf_count));
    for (int i = 0; i < pkf_count; ++i) {
        R_PARSE_DEBUG(parse, "position key frame %d", i);
        double ticks = R_parse_read_double(parse);
        R_PARSE_DEBUG(parse, "ticks %f", ticks);
        R_V3 value = read_v3(parse, "value");
        pkfs[i]    = (R_PositionKeyFrame){
            R_MAGIC_INIT(R_PositionKeyFrame) ticks, value};
    }
    qsort(pkfs, R_int2size(pkf_count), sizeof(*pkfs), sort_position_key_frame);

    int rkf_count = R_parse_read_ushort(parse);
    R_PARSE_DEBUG(parse, "%d rotation key frames", rkf_count);

    R_RotationKeyFrame *rkfs = R_ANEW(rkfs, R_int2size(rkf_count));
    for (int i = 0; i < rkf_count; ++i) {
        R_PARSE_DEBUG(parse, "rotation key frame %d", i);
        double ticks = R_parse_read_double(parse);
        R_PARSE_DEBUG(parse, "ticks %f", ticks);
        R_Qn value = read_qn(parse, "value");
        rkfs[i]    = (R_RotationKeyFrame){
            R_MAGIC_INIT(R_RotationKeyFrame) ticks, value};
    }
    qsort(rkfs, R_int2size(rkf_count), sizeof(*rkfs), sort_rotation_key_frame);

    int skf_count = R_parse_read_ushort(parse);
    R_PARSE_DEBUG(parse, "%d scaling key frames", skf_count);

    R_ScalingKeyFrame *skfs = R_ANEW(skfs, R_int2size(skf_count));
    for (int i = 0; i < skf_count; ++i) {
        R_PARSE_DEBUG(parse, "scaling key frame %d", i);
        double ticks = R_parse_read_double(parse);
        R_PARSE_DEBUG(parse, "ticks %f", ticks);
        R_V3 value = read_v3(parse, "value");
        skfs[i]    = (R_ScalingKeyFrame){
            R_MAGIC_INIT(R_ScalingKeyFrame) ticks, value};
    }
    qsort(skfs, R_int2size(skf_count), sizeof(*skfs), sort_scaling_key_frame);

    return (R_NodeChannel){
        R_MAGIC_INIT(R_NodeChannel) model, node_id, {pkf_count, pkfs},
        {rkf_count, rkfs}, {skf_count, skfs}};
}

static int compare_node_ids(int node_id_a, int node_id_b)
{
    return node_id_a < node_id_b ? -1 : node_id_a > node_id_b ? 1 : 0;
}

static int sort_node_channels(const void *a, const void *b)
{
    R_MAGIC_CHECK(R_NodeChannel, a);
    R_MAGIC_CHECK(R_NodeChannel, b);
    int node_id_a = ((R_NodeChannel *)a)->node_id;
    int node_id_b = ((R_NodeChannel *)b)->node_id;
    return compare_node_ids(node_id_a, node_id_b);
}

static R_Animation read_animation(R_Parse *parse, R_Model *model)
{
    char *name = R_parse_read_string(parse);
    R_PARSE_DEBUG(parse, "name '%s'", name ? name : "");

    double duration_ticks = R_parse_read_double(parse);
    R_PARSE_DEBUG(parse, "duration_ticks %f", duration_ticks);

    double ticks_per_second = R_parse_read_double(parse);
    R_PARSE_DEBUG(parse, "ticks_per_second %f", ticks_per_second);

    int nc_count = R_parse_read_ushort(parse);
    R_PARSE_DEBUG(parse, "%d node channels", nc_count);

    R_NodeChannel *ncs = R_ANEW(ncs, R_int2size(nc_count));
    for (int i = 0; i < nc_count; ++i) {
        R_PARSE_DEBUG(parse, "node channel %d", i);
        ncs[i] = read_node_channel(parse, model);
    }
    qsort(ncs, R_int2size(nc_count), sizeof(*ncs), sort_node_channels);

    return (R_Animation){R_MAGIC_INIT(R_Animation) model, name,
                         duration_ticks, ticks_per_second, {nc_count, ncs}};
}

static void read_animations(R_Parse *parse, R_Model *model)
{
    int count = R_parse_read_ushort(parse);
    R_PARSE_DEBUG(parse, "animation count %d", count);

    R_Animation *anims = R_ANEW(anims, R_int2size(count));
    for (int i = 0; i < count; ++i) {
        R_PARSE_DEBUG(parse, "animation %d", i);
        anims[i] = read_animation(parse, model);
    }
    model->animation.count  = count;
    model->animation.values = anims;
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
        refs += count_node_refs(model, &model->node.values[child_id], id);
    }
    return refs;
}

static void check_mesh_tree(R_Model *model, int count,
                            R_Mesh meshes[static count])
{
    R_Node *root = &model->node.values[0];
    for (int i = 0; i < count; ++i) {
        R_Mesh *mesh = &meshes[i];
        R_MAGIC_CHECK(R_Mesh, mesh);
        R_assert(mesh->model == model, "Mesh model back reference");
        for (int j = 0; j < mesh->attribute.count; ++j) {
            R_Attribute *attr = &mesh->attribute.values[j];
            R_MAGIC_CHECK(R_Attribute, attr);
            R_assert(attr->model == model, "Attribute model back reference");
        }
        for (int j = 0; j < mesh->bone.count; ++j) {
            R_Bone *bone = &mesh->bone.values[j];
            R_MAGIC_CHECK(R_Bone, bone);
            R_assert(bone->model == model, "Bone model back reference");
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
    R_Node *root = &model->node.values[0];
    for (int i = 0; i < model->node.count; ++i) {
        R_Node *node = &model->node.values[i];
        R_MAGIC_CHECK(R_Node, node);
        R_assert(node->model == model, "Node model back reference");
        for (int j = 0; j < node->child_id.count; ++j) {
            int child_id = node->child_id.values[j];
            R_assert(count_node_refs(model, root, child_id) == 1,
                     "node child ref must point to exactly one node");
        }
    }
}

static void check_animation_tree(R_Model *model)
{
    R_Node *root = &model->node.values[0];
    for (int i = 0; i < model->animation.count; ++i) {
        R_Animation *anim = &model->animation.values[i];
        R_MAGIC_CHECK(R_Animation, anim);
        R_assert(anim->model == model, "Animation model back reference");
        for (int j = 0; j < anim->node_channel.count; ++j) {
            R_NodeChannel *nc = &anim->node_channel.values[j];
            R_MAGIC_CHECK(R_NodeChannel, nc);
            R_assert(nc->model == model, "Node channel model back reference");
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

    static int last_model_id = 0;
    model->id = ++last_model_id;
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



static void free_node(R_Node *node)
{
    free(node->mesh_id.values);
    free(node->child_id.values);
    free(node->name);
    R_MAGIC_POISON(R_Node, node);
}

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
}

static void free_animations(int acount, R_Animation anims[R_STATIC(acount)])
{
    for (int i = 0; i < acount; ++i) {
        free_animation(&anims[i]);
    }
    free(anims);
}

static void free_nodes(int ncount, R_Node nodes[R_STATIC(ncount)])
{
    for (int i = 0; i < ncount; ++i) {
        free_node(&nodes[i]);
    }
    free(nodes);
}

static void free_bone(R_Bone *bone)
{
    free(bone->name);
    R_MAGIC_POISON(R_Bone, bone);
}

static void free_attribute(R_Attribute *attr)
{
    free(attr->name);
    switch (attr->type) {
        case R_ATTRIBUTE_TYPE_USHORT:
            free(attr->ushorts);
            break;
        case R_ATTRIBUTE_TYPE_FLOAT:
            free(attr->floats);
            break;
        default:
            R_warn("Can't free attribute with unknown type '%d'", attr->type);
            break;
    }
    R_MAGIC_POISON(R_Attribute, attr);
}

static void free_mesh(R_Mesh *mesh)
{
    int bcount = mesh->bone.count;
    for (int i = 0; i < bcount; ++i) {
        free_bone(&mesh->bone.values[i]);
    }
    free(mesh->bone.values);

    int acount = mesh->attribute.count;
    for (int i = 0; i < acount; ++i) {
        free_attribute(&mesh->attribute.values[i]);
    }
    free(mesh->attribute.values);

    R_MAGIC_POISON(R_Mesh, mesh);
}

static void free_meshes(int mcount, R_Mesh meshes[R_STATIC(mcount)])
{
    for (int i = 0; i < mcount; ++i) {
        free_mesh(&meshes[i]);
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

int R_model_id(R_Model *model)
{
    check_model(model);
    return model->id;
}

int R_model_mesh_count(R_Model *model)
{
    check_model(model);
    return model->mesh.count;
}

R_Mesh *R_model_mesh_by_id(R_Model *model, int id)
{
    check_model(model);
    R_assert(id >= 0 && id < model->mesh.count, "Model mesh id in bounds");
    return &model->mesh.values[id];
}

int R_model_node_count(R_Model *model)
{
    check_model(model);
    return model->node.count;
}

int R_model_node_id_by_name(R_Model *model, const char *name)
{
    check_model(model);
    int    count  = model->node.count;
    R_Node *nodes = model->node.values;
    for (int i = 0; i < count; ++i) {
        R_Node *node = &nodes[i];
        R_MAGIC_CHECK(R_Node, node);
        if (R_str_equal(name, node->name)) {
            return i;
        }
    }
    return -1;
}

R_Node *R_model_node_by_id(R_Model *model, int id)
{
    check_model(model);
    R_assert(id >= 0 && id < model->node.count, "Model node id in bounds");
    return &model->node.values[id];
}

R_Node *R_model_node_by_name (R_Model *model, const char *name)
{
    check_model(model);
    int id = R_model_node_id_by_name(model, name);
    return R_model_node_by_id(model, id);
}

R_Node *R_model_root_node(R_Model *model)
{
    check_model(model);
    return &model->node.values[0];
}

int R_model_animation_count(R_Model *model)
{
    check_model(model);
    return model->animation.count;
}

int R_model_animation_id_by_name(R_Model *model, const char *name)
{
    check_model(model);
    int         count  = model->animation.count;
    R_Animation *anims = model->animation.values;
    for (int i = 0; i < count; ++i) {
        R_Animation *anim = &anims[i];
        R_MAGIC_CHECK(R_Animation, anim);
        if (R_str_equal(name, anim->name)) {
            return i;
        }
    }
    return -1;
}

R_Animation *R_model_animation_by_id(R_Model *model, int id)
{
    check_model(model);
    R_assert(id >= 0 && id < model->animation.count,
             "Model animation id in bounds");
    return &model->animation.values[id];
}

R_Animation *R_model_animation_by_name(R_Model *model, const char *name)
{
    check_model(model);
    int id = R_model_animation_id_by_name(model, name);
    return R_model_animation_by_id(model, id);
}


R_Model *R_mesh_model(R_Mesh *mesh)
{
    R_MAGIC_CHECK(R_Mesh, mesh);
    return mesh->model;
}

int R_mesh_id(R_Mesh *mesh)
{
    R_MAGIC_CHECK(R_Mesh, mesh);
    return mesh->id;
}

int R_mesh_attribute_count(R_Mesh *mesh)
{
    R_MAGIC_CHECK(R_Mesh, mesh);
    return mesh->attribute.count;
}

int R_mesh_attribute_index_by_name(R_Mesh *mesh, const char *name)
{
    R_MAGIC_CHECK(R_Mesh, mesh);
    int         count  = mesh->attribute.count;
    R_Attribute *attrs = mesh->attribute.values;
    for (int i = 0; i < count; ++i) {
        R_Attribute *attr = &attrs[i];
        R_MAGIC_CHECK(R_Attribute, attr);
        if (R_str_equal(name, attr->name)) {
            return i;
        }
    }
    return -1;
}

R_Attribute *R_mesh_attribute_at(R_Mesh *mesh, int index)
{
    R_MAGIC_CHECK(R_Mesh, mesh);
    R_assert(index >= 0 && index < mesh->attribute.count,
             "Mesh attribute index in bounds");
    R_Attribute *attr = &mesh->attribute.values[index];
    R_MAGIC_CHECK(R_Attribute, attr);
    return attr;
}

R_Attribute *R_mesh_attribute_by_name(R_Mesh *mesh, const char *name)
{
    R_MAGIC_CHECK(R_Mesh, mesh);
    int id = R_mesh_attribute_index_by_name(mesh, name);
    return R_mesh_attribute_at(mesh, id);
}

int R_mesh_bone_count(R_Mesh *mesh)
{
    R_MAGIC_CHECK(R_Mesh, mesh);
    return mesh->bone.count;
}

R_Bone *R_mesh_bone_at(R_Mesh *mesh, int index)
{
    R_MAGIC_CHECK(R_Mesh, mesh);
    R_assert(index >= 0 && index < mesh->bone.count,
             "Mesh bone index in bounds");
    return &mesh->bone.values[index];
}


R_Model *R_attribute_model(R_Attribute *attr)
{
    R_MAGIC_CHECK(R_Attribute, attr);
    return attr->model;
}

R_AttributeType R_attribute_type(R_Attribute *attr)
{
    R_MAGIC_CHECK(R_Attribute, attr);
    return attr->type;
}

const char *R_attribute_name(R_Attribute *attr)
{
    R_MAGIC_CHECK(R_Attribute, attr);
    return attr->name;
}

int R_attribute_count(R_Attribute *attr)
{
    R_MAGIC_CHECK(R_Attribute, attr);
    return attr->count;
}

int R_attribute_divisor(R_Attribute *attr)
{
    R_MAGIC_CHECK(R_Attribute, attr);
    return attr->divisor;
}

size_t R_attribute_element_size(R_Attribute *attr)
{
    R_MAGIC_CHECK(R_Attribute, attr);
    R_AttributeType type = attr->type;
    switch (type) {
        case R_ATTRIBUTE_TYPE_USHORT:
            return sizeof(*attr->ushorts);
        case R_ATTRIBUTE_TYPE_FLOAT:
            return sizeof(*attr->floats);
        default:
            R_die("Unknown buffer type '%d'", (int) type);
    }
}

size_t R_attribute_size(R_Attribute *attr)
{
    return R_int2size(R_attribute_count(attr)) * R_attribute_element_size(attr);
}

void *R_attribute_data(R_Attribute *attr)
{
    R_MAGIC_CHECK(R_Attribute, attr);
    R_AttributeType type = attr->type;
    switch (type) {
        case R_ATTRIBUTE_TYPE_USHORT:
            return attr->ushorts;
        case R_ATTRIBUTE_TYPE_FLOAT:
            return attr->floats;
        default:
            R_die("Unknown buffer type '%d'", (int) type);
    }
}

unsigned int R_attribute_gl_type(R_Attribute *attr)
{
    R_MAGIC_CHECK(R_Attribute, attr);
    R_AttributeType type = attr->type;
    switch (type) {
        case R_ATTRIBUTE_TYPE_USHORT:
            return GL_UNSIGNED_SHORT;
        case R_ATTRIBUTE_TYPE_FLOAT:
            return GL_FLOAT;
        default:
            R_die("Unknown buffer type '%d'", (int) type);
    }
}

void R_attribute_gl_buffer_data(R_Attribute *attr, unsigned int target,
                                unsigned int usage)
{
    R_MAGIC_CHECK(R_Attribute, attr);
    ptrdiff_t size  = R_size2ptrdiff(R_attribute_size(attr));
    void      *data = R_attribute_data(attr);
    R_GL_CLEAR_ERROR();
    R_GL(glBufferData, target, size, data, usage);
}

void R_attribute_gl_vertex_attrib_pointer(R_Attribute *attr, unsigned int index,
                                          unsigned char normalized, int stride,
                                          void *pointer)
{
    R_MAGIC_CHECK(R_Attribute, attr);
    int          size = R_attribute_divisor(attr);
    unsigned int type = R_attribute_gl_type(attr);
    R_GL(glVertexAttribPointer, index, size, type, normalized, stride, pointer);
}


R_Model *R_bone_model(R_Bone *bone)
{
    R_MAGIC_CHECK(R_Bone, bone);
    return bone->model;
}

const char *R_bone_name(R_Bone *bone)
{
    R_MAGIC_CHECK(R_Bone, bone);
    return bone->name;
}

R_M4 R_bone_offset(R_Bone *bone)
{
    R_MAGIC_CHECK(R_Bone, bone);
    return bone->offset;
}

int R_bone_armature_id(R_Bone *bone)
{
    R_MAGIC_CHECK(R_Bone, bone);
    return bone->armature_id;
}

int R_bone_node_id(R_Bone *bone)
{
    R_MAGIC_CHECK(R_Bone, bone);
    return bone->node_id;
}

R_Node *R_bone_armature(R_Bone *bone)
{
    R_MAGIC_CHECK(R_Bone, bone);
    int id = bone->armature_id;
    return id >= 0 ? R_model_node_by_id(bone->model, id) : NULL;
}

R_Node *R_bone_node(R_Bone *bone)
{
    R_MAGIC_CHECK(R_Bone, bone);
    int id = bone->node_id;
    return id >= 0 ? R_model_node_by_id(bone->model, id) : NULL;
}



R_Model *R_node_model(R_Node *node)
{
    R_MAGIC_CHECK(R_Node, node);
    return node->model;
}

int R_node_id(R_Node *node)
{
    R_MAGIC_CHECK(R_Node, node);
    return node->id;
}

const char *R_node_name(R_Node *node)
{
    R_MAGIC_CHECK(R_Node, node);
    return node->name;
}

R_V3 R_node_position(R_Node *node)
{
    R_MAGIC_CHECK(R_Node, node);
    return node->position;
}

R_Qn R_node_rotation(R_Node *node)
{
    R_MAGIC_CHECK(R_Node, node);
    return node->rotation;
}

R_V3 R_node_scaling(R_Node *node)
{
    R_MAGIC_CHECK(R_Node, node);
    return node->scaling;
}

R_M4 R_node_transform(R_Node *node)
{
    R_MAGIC_CHECK(R_Node, node);
    return node->transform;
}

bool R_node_child_meshes(R_Node *node)
{
    R_MAGIC_CHECK(R_Node, node);
    return node->child_meshes;
}

int R_node_mesh_count(R_Node *node)
{
    R_MAGIC_CHECK(R_Node, node);
    return node->mesh_id.count;
}

int R_node_mesh_id_at(R_Node *node, int index)
{
    R_MAGIC_CHECK(R_Node, node);
    R_assert(index >= 0 && index < node->mesh_id.count,
             "Node mesh id index in bounds");
    return node->mesh_id.values[index];
}

R_Mesh *R_node_mesh_at(R_Node *node, int index)
{
    return R_model_mesh_by_id(node->model, R_node_mesh_id_at(node, index));
}

int R_node_child_count(R_Node *node)
{
    R_MAGIC_CHECK(R_Node, node);
    return node->child_id.count;
}

int R_node_child_id_at(R_Node *node, int index)
{
    R_MAGIC_CHECK(R_Node, node);
    R_assert(index >= 0 && index < node->child_id.count,
             "Node child id index in bounds");
    return node->child_id.values[index];
}

R_Node *R_node_child_at(R_Node *node, int index)
{
    return R_model_node_by_id(node->model, R_node_child_id_at(node, index));
}


R_Model *R_animation_model(R_Animation *anim)
{
    R_MAGIC_CHECK(R_Animation, anim);
    return anim->model;
}

const char *R_animation_name(R_Animation *anim)
{
    R_MAGIC_CHECK(R_Animation, anim);
    return anim->name;
}

double R_animation_duration_ticks(R_Animation *anim)
{
    R_MAGIC_CHECK(R_Animation, anim);
    return anim->duration_ticks;
}

double R_animation_ticks_per_second(R_Animation *anim)
{
    R_MAGIC_CHECK(R_Animation, anim);
    return anim->ticks_per_second;
}

int R_animation_node_channel_count(R_Animation *anim)
{
    R_MAGIC_CHECK(R_Animation, anim);
    return anim->node_channel.count;
}

R_NodeChannel *R_animation_node_channel_at(R_Animation *anim, int index)
{
    R_MAGIC_CHECK(R_Animation, anim);
    R_assert(index >= 0 && index < anim->node_channel.count,
             "Animation node channel index in bounds");
    return &anim->node_channel.values[index];
}

static int search_node_channel(const void *k, const void *v)
{
    return compare_node_ids(*(const int *)k, ((R_NodeChannel *)v)->node_id);
}

R_NodeChannel *R_animation_search_node_channel_by_node_id(R_Animation *anim,
                                                          int node_id)
{
    R_MAGIC_CHECK(R_Animation, anim);
    return bsearch(&node_id, anim->node_channel.values,
                   R_int2size(anim->node_channel.count),
                   sizeof(*anim->node_channel.values), search_node_channel);
}

double R_animation_seconds_to_ticks(R_Animation *anim, double seconds)
{
    return R_animation_ticks_per_second(anim) * seconds;
}

double R_animation_ticks_to_seconds(R_Animation *anim, double ticks)
{
    return ticks / R_animation_ticks_per_second(anim);
}

double R_animation_duration_seconds(R_Animation *anim)
{
    return R_animation_ticks_to_seconds(anim, R_animation_duration_ticks(anim));
}


R_Model *R_node_channel_model(R_NodeChannel *nc)
{
    R_MAGIC_CHECK(R_NodeChannel, nc);
    return nc->model;
}

int R_node_channel_node_id(R_NodeChannel *nc)
{
    R_MAGIC_CHECK(R_NodeChannel, nc);
    return nc->node_id;
}

R_Node *R_node_channel_node(R_NodeChannel *nc)
{
    return R_model_node_by_id(R_node_channel_model(nc),
                              R_node_channel_node_id(nc));
}


static int search_pkf(const void *k, const void *v)
{
    double                   ticks = *(const double *)k;
    const R_PositionKeyFrame *prev = v;
    const R_PositionKeyFrame *next = prev + 1;
    R_MAGIC_CHECK(R_PositionKeyFrame, prev);
    R_MAGIC_CHECK(R_PositionKeyFrame, next);
    return ticks < prev->ticks ? -1 : ticks >= next->ticks ? 1 : 0;
}

static int search_rkf(const void *k, const void *v)
{
    double                   ticks = *(const double *)k;
    const R_RotationKeyFrame *prev = v;
    const R_RotationKeyFrame *next = prev + 1;
    R_MAGIC_CHECK(R_RotationKeyFrame, prev);
    R_MAGIC_CHECK(R_RotationKeyFrame, next);
    return ticks < prev->ticks ? -1 : ticks >= next->ticks ? 1 : 0;
}

static int search_skf(const void *k, const void *v)
{
    double                  ticks = *(const double *)k;
    const R_ScalingKeyFrame *prev = v;
    const R_ScalingKeyFrame *next = prev + 1;
    R_MAGIC_CHECK(R_ScalingKeyFrame, prev);
    R_MAGIC_CHECK(R_ScalingKeyFrame, next);
    return ticks < prev->ticks ? -1 : ticks >= next->ticks ? 1 : 0;
}

static float lerp_ticks(double ticks, R_EaseFn ease, R_UserData user,
                        double prev_ticks, double next_ticks)
{
    R_assert(prev_ticks <= ticks, "ticks after previous");
    R_assert(ticks < next_ticks, "ticks before next");
    float k = R_double2float((ticks - prev_ticks) / (next_ticks - prev_ticks));
    return ease ? ease(k, user) : k;
}

static R_V3 lerp_v3(double ticks, R_EaseFn ease, R_UserData user,
                    double prev_ticks, R_V3 prev_value, double next_ticks,
                    R_V3 next_value)
{
    float t = lerp_ticks(ticks, ease, user, prev_ticks, next_ticks);
    return R_v3_lerp(prev_value, next_value, t);
}

static R_Qn lerp_qn(double ticks, R_EaseFn ease, R_UserData user,
                    double prev_ticks, R_Qn prev_value, double next_ticks,
                    R_Qn next_value)
{
    float t = lerp_ticks(ticks, ease, user, prev_ticks, next_ticks);
    return R_qn_lerp(prev_value, next_value, t);
}

R_V3 R_node_channel_position_at(R_NodeChannel *nc, double ticks, R_EaseFn ease,
                                R_UserData user)
{
    R_MAGIC_CHECK(R_NodeChannel, nc);
    int count = nc->position_key_frame.count;
    if (count == 0) {
        return R_node_position(R_node_channel_node(nc));
    }

    R_PositionKeyFrame *pkfs = nc->position_key_frame.values;
    R_PositionKeyFrame *prev = bsearch(&ticks, pkfs, R_int2size(count - 1),
                                       sizeof(*prev), search_pkf);
    if (prev) {
        R_PositionKeyFrame *next = prev + 1;
        return lerp_v3(ticks, ease, user, prev->ticks, prev->value,
                       next->ticks, next->value);
    }
    else if (ticks < pkfs[0].ticks) {
        return pkfs[0].value;
    }
    else {
        return pkfs[count - 1].value;
    }
}

R_Qn R_node_channel_rotation_at(R_NodeChannel *nc, double ticks, R_EaseFn ease,
                                R_UserData user)
{
    R_MAGIC_CHECK(R_NodeChannel, nc);
    int count = nc->rotation_key_frame.count;
    if (count == 0) {
        return R_node_rotation(R_node_channel_node(nc));
    }

    R_RotationKeyFrame *rkfs = nc->rotation_key_frame.values;
    R_RotationKeyFrame *prev = bsearch(&ticks, rkfs, R_int2size(count - 1),
                                       sizeof(*prev), search_rkf);
    if (prev) {
        R_RotationKeyFrame *next = prev + 1;
        return lerp_qn(ticks, ease, user, prev->ticks, prev->value,
                       next->ticks, next->value);
    }
    else if (ticks < rkfs[0].ticks) {
        return rkfs[0].value;
    }
    else {
        return rkfs[count - 1].value;
    }
}

R_V3 R_node_channel_scaling_at(R_NodeChannel *nc, double ticks, R_EaseFn ease,
                               R_UserData user)
{
    R_MAGIC_CHECK(R_NodeChannel, nc);
    int count = nc->scaling_key_frame.count;
    if (count == 0) {
        return R_node_scaling(R_node_channel_node(nc));
    }

    R_ScalingKeyFrame *skfs = nc->scaling_key_frame.values;
    R_ScalingKeyFrame *prev = bsearch(&ticks, skfs, R_int2size(count - 1),
                                       sizeof(*prev), search_skf);
    if (prev) {
        R_ScalingKeyFrame *next = prev + 1;
        return lerp_v3(ticks, ease, user, prev->ticks, prev->value,
                       next->ticks, next->value);
    }
    else if (ticks < skfs[0].ticks) {
        return skfs[0].value;
    }
    else {
        return skfs[count - 1].value;
    }
}
