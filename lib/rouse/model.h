/*
 * model.h - models, meshes and joints.
 *
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

/*
 * What kind of data is in a `R_MeshBuffer`. I really only ever need unsigned
 * short and float buffers, since OpenGL ES 2.0 is limited in that area.
 */
typedef enum R_BufferType {
    R_BUFFER_TYPE_USHORT = 117, /* 'u' in ASCII */
    R_BUFFER_TYPE_FLOAT  = 102, /* 'f' in ASCII */
} R_BufferType;

/* A buffer in a mesh. Usually some kind of index or vertex buffer. */
typedef struct R_MeshBuffer {
    R_MAGIC_FIELD
    /* The data type of the buffer, from `R_BufferType`. */
    int type;
    /*
     * Some name to refer to the buffer by. For example "indices", "vertices",
     * "normals", "uvs" etc. is what I like to call mine. May be `NULL`!
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
        unsigned short *ushorts; /* R_BUFFER_TYPE_USHORT */
        float          *floats;  /* R_BUFFER_TYPE_FLOAT  */
    };
} R_MeshBuffer;

typedef struct R_Mesh {
    R_MAGIC_FIELD
    struct {
        int           count;
        R_MeshBuffer *values;
    } buffer;
} R_Mesh;

typedef struct R_Model {
    R_MAGIC_FIELD
    struct {
        int    count;
        R_Mesh *values;
    } mesh;
} R_Model;


/*
 * Callback function to read a chunk of `size` bytes into the buffer `out`,
 * returning the number of characters *actually* read. Sometimes a lower count
 * is expected (e.g. to check that there's nothing beyond the supposed end of
 * input), so don't bail out in that case. The `out` buffer is guaranteed to be
 * able to hold `size` bytes, but not a bit more, so don't overfill it. The
 * `user` pointer is whatever you pass to `R_model_new`.
 */
typedef int (*R_ModelRead)(uint8_t *out, int size, void *user);

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
R_Model *R_model_new(const char *title, R_ModelRead read, void *user,
                     uint8_t *buffer, int bufsize);

R_Model *R_model_from_file(const char *path);

void R_model_free(R_Model *model);


R_Mesh *R_model_mesh_by_index(R_Model *model, int index);


int R_mesh_buffer_index_by_name(R_Mesh *mesh, const char *name);

R_MeshBuffer *R_mesh_buffer_by_index(R_Mesh *mesh, int index);
R_MeshBuffer *R_mesh_buffer_by_name (R_Mesh *mesh, const char *name);

unsigned short *R_mesh_ushorts_by_index(R_Mesh *mesh, int index,
                                        int *out_count);
unsigned short *R_mesh_ushorts_by_name(R_Mesh *mesh, const char *name,
                                       int *out_count);

float *R_mesh_floats_by_index(R_Mesh *mesh, int index, int *out_count);
float *R_mesh_floats_by_name(R_Mesh *mesh, const char *name, int *out_count);
