#include "tap.h"
#include <rouse.h>


static void buffer_ok(R_Attribute *attr, const char *name, R_AttributeType type,
                      int count, int divisor)
{
    const char *an = R_attribute_name(attr);
    const char *mq = an   ? "\"" : "";
    const char *nq = name ? "\"" : "";
    str_eq_ok(an, name, "attribute name %s%s%s eq %s%s%s", mq,
              an ? an : "NULL", mq, nq, name ? name : "NULL", nq);

    int got_type  = (int) R_attribute_type(attr);
    int want_type = (int) type;
    int_eq_ok(got_type, want_type, "attribute type %d == %d", got_type, want_type);

    int ac = R_attribute_count(attr);
    int_eq_ok(ac, count, "element count %d == %d", ac, count);

    int ad = R_attribute_divisor(attr);
    int_eq_ok(ad, divisor, "divisor %d == %d", ad, divisor);
}

static void ushort_attribute_ok(R_Attribute *attr, const char *name, int count,
                             int divisor, unsigned short *values)
{
    buffer_ok(attr, name, R_ATTRIBUTE_TYPE_USHORT, count, divisor);
    unsigned short *ushorts = R_attribute_data(attr);
    for (int i = 0; i < count; ++i) {
        unsigned short m = ushorts[i];
        unsigned short v = values[i];
        ushort_eq_ok(m, v, "ushorts[%d]: %u == %u", i, m, v);
    }
}

static void float_attribute_ok(R_Attribute *attr, const char *name, int count,
                            int divisor, float *values)
{
    buffer_ok(attr, name, R_ATTRIBUTE_TYPE_FLOAT, count, divisor);
    float *floats = R_attribute_data(attr);
    for (int i = 0; i < count; ++i) {
        float m = floats[i];
        float v = values[i];
        float_eq_eps_ok(m, v, 1e-7f, "floats[%d]: %f == %f", i, m, v);
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
    int_eq_ok(R_model_mesh_count(cube), 1, "cube has 1 mesh");

    R_Mesh *mesh = R_model_mesh_by_id(cube, 0);
    int_eq_ok(R_mesh_attribute_count(mesh), 3, "cube has 3 buffers");

    ushort_attribute_ok(R_mesh_attribute_at(mesh, 0), "indices",
                        R_LENGTH(cube_indices), 3, cube_indices);

    float_attribute_ok(R_mesh_attribute_at(mesh, 1), "vertices",
                    R_LENGTH(cube_vertices), 3, cube_vertices);

    float_attribute_ok(R_mesh_attribute_at(mesh, 2), "normals",
                       R_LENGTH(cube_normals), 3, cube_normals);

    R_model_decref(cube);
}


TAP_BEGIN
    test_cube();
TAP_END
