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

typedef enum R_AttributeType {
    R_ATTRIBUTE_TYPE_USHORT = 117, /* 'u' in ASCII */
    R_ATTRIBUTE_TYPE_FLOAT  = 102, /* 'f' in ASCII */
} R_AttributeType;

typedef struct R_Model R_Model;
typedef struct R_Mesh R_Mesh;
typedef struct R_Attribute R_Attribute;
typedef struct R_Bone R_Bone;
typedef struct R_Node R_Node;
typedef struct R_Animation R_Animation;
typedef struct R_NodeChannel R_NodeChannel;


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

int R_model_mesh_count(R_Model *model);
R_Mesh *R_model_mesh_by_id(R_Model *model, int id);

int R_model_node_count(R_Model *model);
int R_model_node_id_by_name(R_Model *model, const char *name);
R_Node *R_model_node_by_id(R_Model *model, int id);
R_Node *R_model_node_by_name (R_Model *model, const char *name);
R_Node *R_model_root_node(R_Model *model);

int R_model_animation_count(R_Model *model);
int R_model_animation_id_by_name(R_Model *model, const char *name);
R_Animation *R_model_animation_by_id(R_Model *model, int id);
R_Animation *R_model_animation_by_name(R_Model *model, const char *name);


R_Model *R_mesh_model(R_Mesh *mesh);
int R_mesh_id(R_Mesh *mesh);

int R_mesh_attribute_count(R_Mesh *mesh);
int R_mesh_attribute_index_by_name(R_Mesh *mesh, const char *name);
R_Attribute *R_mesh_attribute_at(R_Mesh *mesh, int index);
R_Attribute *R_mesh_attribute_by_name(R_Mesh *mesh, const char *name);

int R_mesh_bone_count(R_Mesh *mesh);
R_Bone *R_mesh_bone_at(R_Mesh *mesh, int index);


R_Model *R_attribute_model(R_Attribute *attr);
R_AttributeType R_attribute_type(R_Attribute *attr);
const char *R_attribute_name(R_Attribute *attr);
int R_attribute_count(R_Attribute *attr);
int R_attribute_divisor(R_Attribute *attr);

size_t R_attribute_element_size(R_Attribute *attr);
size_t R_attribute_size(R_Attribute *attr);
void *R_attribute_data(R_Attribute *attr);
unsigned int R_attribute_gl_type(R_Attribute *attr);

/*
 * Call `glBufferData` forwarding the given `target` (e.g. `GL_ARRAY_BUFFER`)
 * and `usage` (e.g. `GL_STATIC_DRAW`) while using `attr`'s contents as the
 * `data` and `size` parameters.
 */
void R_attribute_gl_buffer_data(R_Attribute *attr, unsigned int target,
                                unsigned int usage);

/*
 * Call `glVertexAttribuPointer`, forwarding the given `index`, `normalized`,
 * `stride` and `pointer` while using `attr`'s properties as the `size` and
 * `type` parameters.
 */
void R_attribute_gl_vertex_attrib_pointer(R_Attribute *attr,
                                                 unsigned int index,
                                                 unsigned char normalized,
                                                 int stride, void *pointer);


R_Model *R_bone_model(R_Bone *bone);
const char *R_bone_name(R_Bone *bone);
R_M4 R_bone_offset(R_Bone *bone);
int R_bone_armature_id(R_Bone *bone);
int R_bone_node_id(R_Bone *bone);
R_Node *R_bone_armature(R_Bone *bone);
R_Node *R_bone_node(R_Bone *bone);


R_Model *R_node_model(R_Node *node);
int R_node_id(R_Node *node);
const char *R_node_name(R_Node *node);
R_V3 R_node_position(R_Node *node);
R_Qn R_node_rotation(R_Node *node);
R_V3 R_node_scaling(R_Node *node);
R_M4 R_node_transform(R_Node *node);

int R_node_mesh_count(R_Node *node);
int R_node_mesh_id_at(R_Node *node, int index);
R_Mesh *R_node_mesh_at(R_Node *node, int index);

int R_node_child_count(R_Node *node);
int R_node_child_id_at(R_Node *node, int index);
R_Node *R_node_child_at(R_Node *node, int index);


R_Model *R_animation_model(R_Animation *anim);
const char *R_animation_name(R_Animation *anim);
double R_animation_duration_ticks(R_Animation *anim);
double R_animation_ticks_per_second(R_Animation *anim);

int R_animation_node_channel_count(R_Animation *anim);
R_NodeChannel *R_animation_node_channel_at(R_Animation *anim, int index);
R_NodeChannel *R_animation_search_node_channel_by_node_id(R_Animation *anim,
                                                          int node_id);

double R_animation_seconds_to_ticks(R_Animation *anim, double seconds);
double R_animation_ticks_to_seconds(R_Animation *anim, double ticks);
double R_animation_duration_seconds(R_Animation *anim);


R_Model *R_node_channel_model(R_NodeChannel *nc);
int R_node_channel_node_id(R_NodeChannel *nc);
R_Node *R_node_channel_node(R_NodeChannel *nc);

R_V3 R_node_channel_position_at(R_NodeChannel *nc, double ticks, R_EaseFn ease,
                                R_UserData user);

R_Qn R_node_channel_rotation_at(R_NodeChannel *nc, double ticks, R_EaseFn ease,
                                R_UserData user);

R_V3 R_node_channel_scaling_at(R_NodeChannel *nc, double ticks, R_EaseFn ease,
                               R_UserData user);
