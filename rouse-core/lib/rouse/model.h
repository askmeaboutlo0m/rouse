/*
 * model.h - models, meshes and joints.
 *
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

/*
 * What kind of data is in a `R_MeshBuffer`. OpenGL ES 2.0 only supports float
 * and unsigned short buffers, so that's what's currently available here.
 */
typedef enum R_BufferType {
    R_BUFFER_TYPE_USHORT = 117, /* 'u' in ASCII */
    R_BUFFER_TYPE_FLOAT  = 102, /* 'f' in ASCII */
} R_BufferType;

typedef union R_MeshBufferValues {
    unsigned short *ushorts; /* R_BUFFER_TYPE_USHORT */
    float          *floats;  /* R_BUFFER_TYPE_FLOAT  */
} R_MeshBufferValues;

/* A buffer in a mesh. Usually some kind of index or vertex buffer. */
typedef struct R_MeshBuffer {
    R_MAGIC_FIELD
    int refs;
    /* The data type of the buffer, from `R_BufferType`. */
    R_BufferType type;
    /*
     * Some name to refer to the buffer by. For example "indices", "vertices",
     * "normals", "uv0" etc., if you're using rouse-model. May be NULL!
     */
    char *name;
    /* The length of the buffer, i.e. how many numbers are in it. */
    int count;
    /*
     * Size of a single "logical" element in the buffer. For triangles or
     * normals, this will usually be 3, for UVs this will be 2 etc. It's
     * called `divisor` because when you divide `count` by it, you get the
     * number of logical elements. A divisor that doesn't cleanly divide
     * `count` is invalid.
     */
    int divisor;
    /*
     * The actual buffer with the data in it. Which field in this union you
     * need depends on the `type` of this mesh buffer.
     */
    union {
        R_MeshBufferValues values;
        unsigned short     *ushorts;
        float              *floats;
    };
} R_MeshBuffer;

typedef struct R_Node R_Node;

typedef struct R_Bone {
    R_MAGIC_FIELD
    int    refs;
    char   *name;
    R_M4   offset;
    int    armature_id; /* -1 means none */
    int    node_id;     /* -1 means none */
} R_Bone;

typedef struct R_Mesh {
    R_MAGIC_FIELD
    int refs;
    int id;
    struct {
        int          count;
        R_MeshBuffer **values;
    } buffer;
    struct {
        int    count;
        R_Bone **values;
    } bone;
} R_Mesh;

struct R_Node {
    R_MAGIC_FIELD
    int  refs;
    int  id;
    int  parent_id;
    char *name;
    R_V3 position;
    R_Qn rotation;
    R_V3 scaling;
    R_M4 transform;
    struct {
        int count;
        int *values;
    } mesh_id;
    struct {
        int count;
        int *values;
    } child_id;
};

typedef struct R_PositionKeyFrame {
    double ticks;
    R_V3   value;
} R_PositionKeyFrame;

typedef struct R_RotationKeyFrame {
    double ticks;
    R_Qn   value;
} R_RotationKeyFrame;

typedef struct R_ScalingKeyFrame {
    double ticks;
    R_V3   value;
} R_ScalingKeyFrame;

typedef struct R_NodeChannel {
    int node_id;
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
} R_NodeChannel;

typedef struct R_Animation {
    R_MAGIC_FIELD
    int    refs;
    char   *name;
    double duration;
    double ticks_per_second;
    struct {
        int           count;
        R_NodeChannel *values;
    } node_channel;
} R_Animation;

typedef struct R_Model {
    R_MAGIC_FIELD
    int refs;
    struct {
        int    count;
        R_Mesh **values;
    } mesh;
    struct {
        int    count;
        R_Node **values;
    } node;
    struct {
        int         count;
        R_Animation **values;
    } animation;
} R_Model;


/*
 * Low-level function to read a model from some kind of byte stream. Unless
 * you're implementing your own format to read from, this is the wrong thing,
 * look at `R_model_from_file` instead.
 *
 * The `title` is used for error and log messages, and it should be the file
 * name or something else identifying the source of your model to a human.
 * `read` is the function to read bytes and it's passed the `user` pointer so
 * that you can carry your file handle along with you or whatever. `buffer`
 * should be `bufsize`-sized and is used to read stuff into. 256 bytes should
 * be plenty enough space for it.
 *
 * You'll either get back a fully loaded model or this function will `R_die`
 * horribly if something goes wrong. This *could* probably be made more robust
 * to have a less deadly variant, but eh, cleanup is hard.
 *
 * I'll describe the file format some day... for now, read the code I guess.
 */
R_Model *R_model_new(const char *title, R_ParseReadFn read, R_UserData user,
                     int bufsize, unsigned char buffer[R_STATIC(bufsize)]);

R_Model *R_model_from_file(const char *path);

R_Model *R_model_decref(R_Model *model);
R_Model *R_model_incref(R_Model *model);

int R_model_refs(R_Model *model);

char *R_model_dump(R_Model *model);

R_Mesh *R_model_mesh_by_id(R_Model *model, int id);

R_Node *R_model_root_node(R_Model *model);
R_Node *R_model_node_by_id(R_Model *model, int id);
R_Node *R_model_node_child(R_Model *model, R_Node *node, int child_index);


R_Mesh *R_mesh_decref(R_Mesh *mesh);
R_Mesh *R_mesh_incref(R_Mesh *mesh);

int R_mesh_refs(R_Mesh *mesh);


int R_mesh_buffer_index_by_name(R_Mesh *mesh, const char *name);

R_MeshBuffer *R_mesh_buffer_by_index(R_Mesh *mesh, int index);
R_MeshBuffer *R_mesh_buffer_by_name (R_Mesh *mesh, const char *name);

unsigned short *R_mesh_ushorts_by_index(R_Mesh *mesh, int index,
                                        int *out_count);
unsigned short *R_mesh_ushorts_by_name(R_Mesh *mesh, const char *name,
                                       int *out_count);

float *R_mesh_floats_by_index(R_Mesh *mesh, int index, int *out_count);
float *R_mesh_floats_by_name(R_Mesh *mesh, const char *name, int *out_count);


R_MeshBuffer *R_mesh_buffer_new(R_BufferType type, const char *name, int count,
                                int divisor);

R_MeshBuffer *R_mesh_buffer_decref(R_MeshBuffer *mbuf);
R_MeshBuffer *R_mesh_buffer_incref(R_MeshBuffer *mbuf);

int R_mesh_buffer_refs(R_MeshBuffer *mbuf);


R_Bone *R_bone_incref(R_Bone *bone);
R_Bone *R_bone_decref(R_Bone *bone);

int R_bone_refs(R_Bone *bone);


R_Node *R_node_decref(R_Node *node);
R_Node *R_node_incref(R_Node *node);

int R_node_refs(R_Node *node);


R_Animation *R_animation_decref(R_Animation *anim);
R_Animation *R_animation_incref(R_Animation *anim);

int R_animation_refs(R_Animation *anim);
