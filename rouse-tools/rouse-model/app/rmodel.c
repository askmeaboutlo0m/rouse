#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assimp/cimport.h>
#include <assimp/config.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/version.h>

#define RMODEL_MAGIC "rmodel1"

#define WARN(...) do { \
        fprintf(stderr, __VA_ARGS__); \
        fputc('\n', stderr); \
    } while (0)

#define DIE(...) do { \
        WARN(__VA_ARGS__); \
        exit(1); \
    } while (0)


static void pack_uchar(FILE *fp, unsigned int value)
{
    unsigned char uchar = value & 0xffu;
    fwrite(&uchar, 1, 1, fp);
}

static void pack_ushort(FILE *fp, unsigned int value)
{
    pack_uchar(fp, value);
    pack_uchar(fp, value >> 8);
}

static void pack_string(FILE *fp, const char *value)
{
    pack_ushort(fp, (unsigned int) strlen(value));
    fputs(value, fp);
}

static void pack_float(FILE *fp, float value)
{
    char buf[256];
    sprintf(buf, "%f", (double) value);
    pack_uchar(fp, (unsigned int) strlen(buf));
    fputs(buf, fp);
}


static void pack_faces(FILE *fp, const char *name, unsigned int count,
                       struct aiFace *faces)
{
    fputc('u', fp);
    pack_string(fp, name);
    unsigned int divisor = faces[0].mNumIndices;
    pack_uchar(fp, divisor);
    pack_ushort(fp, count * divisor);
    for (unsigned int i = 0; i < count; ++i) {
        struct aiFace *face = &faces[i];
        unsigned int d = face->mNumIndices;
        if (d != divisor) {
            DIE("Unequal divisor: %u != %u", d, divisor);
        }
        for (unsigned int j = 0; j < d; ++j) {
            pack_ushort(fp, face->mIndices[j]);
        }
    }
}


static void pack_vector3d_components(FILE *fp, const char *name,
                                     unsigned int count,
                                     struct aiVector3D *vectors,
                                     unsigned int components)
{
    fputc('f', fp);
    pack_string(fp, name);
    pack_uchar(fp, components);
    pack_ushort(fp, count * components);
    for (unsigned int i = 0; i < count; ++i) {
        struct aiVector3D *vector = &vectors[i];
        if (components >= 1) {
            pack_float(fp, vector->x);
        }
        if (components >= 2) {
            pack_float(fp, vector->y);
        }
        if (components >= 3) {
            pack_float(fp, vector->z);
        }
    }
}

static void pack_vector3d(FILE *fp, const char *name, unsigned int count,
                          struct aiVector3D *vectors)
{
    pack_vector3d_components(fp, name, count, vectors, 3);
}

static void pack_uvs(FILE *fp, const char *name, unsigned int count,
                     struct aiVector3D *uvs)
{
    pack_vector3d_components(fp, name, count, uvs, 2);
}


static bool has_uv(struct aiMesh *mesh, unsigned int i)
{
    return i < AI_MAX_NUMBER_OF_TEXTURECOORDS && mesh->mTextureCoords[i];
}

static unsigned int count_uvs(struct aiMesh *mesh)
{
    unsigned int count = 0;
    for (unsigned int i = 0; has_uv(mesh, i); ++i) {
        unsigned int components = mesh->mNumUVComponents[i];
        if (components == 2 || components == 3) {
            ++count;
        }
    }
    return count;
}

static void process_uvs(FILE *fp, struct aiMesh *mesh)
{
    char name[32];
    for (unsigned int i = 0; has_uv(mesh, i); ++i) {
        snprintf(name, sizeof(name), "uv%u", i);
        struct aiVector3D *uvs = mesh->mTextureCoords[i];
        unsigned int components = mesh->mNumUVComponents[i];
        if (components == 2) {
            pack_uvs(fp, name, mesh->mNumVertices, uvs);
        }
        else if (components == 3) {
            pack_vector3d(fp, name, mesh->mNumVertices, uvs);
        }
        else {
            WARN("Can't deal with %u-dimensional UVs", components);
        }
    }
}

static void process_meshes(FILE *fp, const struct aiScene *scene)
{
    pack_ushort(fp, scene->mNumMeshes);
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        struct aiMesh *mesh = scene->mMeshes[i];
        pack_ushort(fp, 3 + count_uvs(mesh));
        pack_faces(fp, "indices", mesh->mNumFaces, mesh->mFaces);
        pack_vector3d(fp, "vertices", mesh->mNumVertices, mesh->mVertices);
        pack_vector3d(fp, "normals", mesh->mNumVertices, mesh->mNormals);
        process_uvs(fp, mesh);
    }
}


static void process_scene(FILE *fp, const struct aiScene *scene)
{
    fwrite(RMODEL_MAGIC, 1, strlen(RMODEL_MAGIC) + 1, fp);
    process_meshes(fp, scene);
}


int main(int argc, char **argv)
{
    if (argc != 2) {
        DIE("Usage: %s INPUT_FILE", argc < 1 ? "rmodel" : argv[0]);
    }

    unsigned int flags = aiProcess_FindDegenerates
                       | aiProcess_FindInvalidData
                       | aiProcess_JoinIdenticalVertices
                       | aiProcess_OptimizeGraph
                       | aiProcess_OptimizeMeshes
                       | aiProcess_RemoveComponent
                       | aiProcess_SortByPType
                       | aiProcess_ValidateDataStructure;

    struct aiPropertyStore *props = aiCreatePropertyStore();

    aiSetImportPropertyInteger(props, AI_CONFIG_PP_RVC_FLAGS,
                               aiComponent_CAMERAS | aiComponent_COLORS
                              | aiComponent_LIGHTS | aiComponent_MATERIALS);

    aiSetImportPropertyInteger(props, AI_CONFIG_PP_SBP_REMOVE,
                               aiPrimitiveType_POINT | aiPrimitiveType_LINE);

    const char *path = argv[1];

    const struct aiScene *scene =
        aiImportFileExWithProperties(path, flags, NULL, props);

    if (!scene) {
        DIE("Error importing '%s': %s", path, aiGetErrorString());
    }

    process_scene(stdout, scene);

    aiReleaseImport(scene);
    aiReleasePropertyStore(props);
    return 0;
}
