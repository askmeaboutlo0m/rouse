#include "tap.h"
#include <rouse.h>


static void buffer_ok(R_MeshBuffer *mb, const char *name, int type, int count,
                      int divisor)
{
    const char *mq = mb->name ? "\"" : "";
    const char *nq = name     ? "\"" : "";
    ok(R_str_equal(mb->name, name), "buffer name %s%s%s eq %s%s%s",
       mq, mb->name ? mb->name : "NULL", mq, nq, name ? name : "NULL", nq);

    ok(mb->type == type, "buffer type %d == %d", mb->type, type);
    ok(mb->count == count, "element count %d == %d", mb->count, count);
    ok(mb->divisor == divisor, "divisor %d == %d", mb->divisor, divisor);
}

static void ushort_buffer_ok(R_MeshBuffer *mb, const char *name, int count,
                             int divisor, unsigned short *values)
{
    buffer_ok(mb, name, R_BUFFER_TYPE_USHORT, count, divisor);
    for (int i = 0; i < count; ++i) {
        unsigned short m = mb->ushorts[i],
                       v = values[i];
        ok(m == v, "ushorts[%d]: %u == %u", i, m, v);
    }
}

static void float_buffer_ok(R_MeshBuffer *mb, const char *name, int count,
                            int divisor, float *values)
{
    buffer_ok(mb, name, R_BUFFER_TYPE_FLOAT, count, divisor);
    for (int i = 0; i < count; ++i) {
        float m = mb->floats[i],
              v = values[i];
        ok(fabsf(m - v) < 1e-7, "floats[%d]: %f == %f", i, m, v);
    }
}


static unsigned short cube_indices[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 0, 18, 1, 3,
    19, 4, 6, 20, 7, 9, 21, 10, 12, 22, 13, 15, 23, 16,
};

static float cube_vertices[] = {
    1, -1, -1, -1, -1, 1, -1, -1, -1, -1, 1, -1, 1, 1, 1, 1, 1, -1, 1, 1, -1, 1,
    -1, 1, 1, -1, -1, 1, 1, 1, -1, -1, 1, 1, -1, 1, -1, -1, 1, -1, 1, -1, -1,
    -1, -1, 1, -1, -1, -1, 1, -1, 1, 1, -1, 1, -1, 1, -1, 1, 1, 1, 1, 1, -1, 1,
    1, -1, 1, 1, -1, -1, -1
};

static float cube_normals[] = {
    0, -1, 0, 0, -1, 0, 0, -1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0,
    1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, -1, 0, 0, -1, 0, 0, -1, 0, 0, 0, 0, -1,
    0, 0, -1, 0, 0, -1, 0, -1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, -1, 0, 0, 0, 0, -1
};

static void test_cube(void)
{
    R_Model *cube = R_model_from_file("test/data/cube.rmodel");
    ok(cube->mesh.count == 1, "cube has 1 mesh");

    R_Mesh *mesh = R_model_mesh_by_index(cube, 0);
    ok(mesh->buffer.count == 3, "cube has 3 buffers");

    ushort_buffer_ok(R_mesh_buffer_by_index(mesh, 0), "indices",
                     R_LENGTH(cube_indices), 3, cube_indices);

    float_buffer_ok(R_mesh_buffer_by_index(mesh, 1), "vertices",
                    R_LENGTH(cube_vertices), 3, cube_vertices);

    float_buffer_ok(R_mesh_buffer_by_index(mesh, 2), "normals",
                    R_LENGTH(cube_normals), 3, cube_normals);

    R_model_free(cube);
}


TAP_BEGIN
    test_cube();
TAP_END
