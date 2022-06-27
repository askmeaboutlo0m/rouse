#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/config.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/version.h>

#define RMODEL_MAGIC "rmodel2"

#define NBONES   20
#define NWEIGHTS 3

#ifdef NDEBUG
#    define DEBUG(...) \
        do {           \
        } while (0)
#else
#    define DEBUG(...)                    \
        do {                              \
            fputs("DEBUG: ", stderr);     \
            fprintf(stderr, __VA_ARGS__); \
            fputc('\n', stderr);          \
        } while (0)
#endif

#define WARN(...)                     \
    do {                              \
        fprintf(stderr, __VA_ARGS__); \
        fputc('\n', stderr);          \
    } while (0)

#define DIE(...)           \
    do {                   \
        WARN(__VA_ARGS__); \
        exit(1);           \
    } while (0)

#define AISTRING_FMT(S) (S).length, (S).data


struct NodeEntry {
    aiNode *m_node;
    unsigned int m_id;

    NodeEntry(aiNode *node, unsigned int id) : m_node{node}, m_id{id}
    {
    }

    NodeEntry() : NodeEntry{nullptr, 0}
    {
    }
};

struct Context {
    FILE *m_fp;
    const aiScene *m_scene;
    std::vector<NodeEntry> m_entries;

    Context(FILE *fp, const aiScene *scene)
        : m_fp{fp}, m_scene{scene}, m_entries{}
    {
    }

    NodeEntry *search_node_by_name(const aiString &name)
    {
        NodeEntry *found = nullptr;
        for (NodeEntry &entry : m_entries) {
            if (name == entry.m_node->mName) {
                if (found) {
                    DIE("Duplicate node name '%*s'", AISTRING_FMT(name));
                }
                else {
                    found = &entry;
                }
            }
        }
        return found;
    }

    unsigned int find_node_id_by_name(const aiString &name)
    {
        NodeEntry *entry = search_node_by_name(name);
        if (entry) {
            return entry->m_id;
        }
        else {
            DIE("Node '%*s' not found", AISTRING_FMT(name));
        }
    }

    void put_node(aiNode *node, unsigned int id)
    {
        m_entries.push_back(NodeEntry{node, id});
    }


    void pack_uchar(unsigned int value)
    {
        if (value <= UINT8_MAX) {
            unsigned char uchar = static_cast<unsigned char>(value & 0xffu);
            fwrite(&uchar, 1, 1, m_fp);
        }
        else {
            DIE("Value %u too large for 8 bit byte", value);
        }
    }

    void pack_ushort(unsigned int value)
    {
        if (value <= UINT16_MAX) {
            pack_uchar(value & 0xffu);
            pack_uchar(value >> 8);
        }
        else {
            DIE("Value %u too large for 16 bit unsigned short", value);
        }
    }

    void pack_lstring(const char *value, size_t length)
    {
        pack_ushort((unsigned int)length);
        fputs(value, m_fp);
    }

    void pack_string(const char *value)
    {
        pack_lstring(value, strlen(value));
    }

    void pack_aistring(const aiString *value)
    {
        pack_lstring(value->data, value->length);
    }

    void pack_double(double value)
    {
        char buf[256];
        sprintf(buf, "%f", value);
        pack_uchar((unsigned int)strlen(buf));
        fputs(buf, m_fp);
    }

    void pack_float(float value)
    {
        pack_double(value);
    }


    void pack_faces(const char *name, unsigned int count, aiFace *faces)
    {
        fputc(117, m_fp); /* 'u' in ASCII */
        pack_string(name);
        unsigned int divisor = faces[0].mNumIndices;
        pack_uchar(divisor);
        pack_ushort(count * divisor);
        for (unsigned int i = 0; i < count; ++i) {
            aiFace *face = &faces[i];
            unsigned int d = face->mNumIndices;
            if (d != divisor) {
                DIE("Unequal divisor: %u != %u", d, divisor);
            }
            for (unsigned int j = 0; j < d; ++j) {
                pack_ushort(face->mIndices[j]);
            }
        }
    }


    void pack_matrix4x4(const aiMatrix4x4 *matrix)
    {
        /* Assimp uses row-major format, pack it in column-major format. */
        pack_float(matrix->a1);
        pack_float(matrix->b1);
        pack_float(matrix->c1);
        pack_float(matrix->d1);
        pack_float(matrix->a2);
        pack_float(matrix->b2);
        pack_float(matrix->c2);
        pack_float(matrix->d2);
        pack_float(matrix->a3);
        pack_float(matrix->b3);
        pack_float(matrix->c3);
        pack_float(matrix->d3);
        pack_float(matrix->a4);
        pack_float(matrix->b4);
        pack_float(matrix->c4);
        pack_float(matrix->d4);
    }

    void pack_quaternion(const aiQuaternion *quat)
    {
        pack_float(quat->x);
        pack_float(quat->y);
        pack_float(quat->z);
        pack_float(quat->w);
    }

    void pack_vector3d(const aiVector3D *vector)
    {
        pack_float(vector->x);
        pack_float(vector->y);
        pack_float(vector->z);
    }


    void pack_float_buffer_preamble(const char *name, unsigned int count,
                                    unsigned int components)
    {
        fputc(102, m_fp); /* 'f' in ASCII */
        pack_string(name);
        pack_uchar(components);
        pack_ushort(count * components);
    }

    void pack_vector3d_buffer(const char *name, unsigned int count,
                              aiVector3D *vectors)
    {
        pack_float_buffer_preamble(name, count, 3);
        for (unsigned int i = 0; i < count; ++i) {
            pack_vector3d(&vectors[i]);
        }
    }

    void pack_uvs(const char *name, unsigned int count, aiVector3D *uvs)
    {
        pack_float_buffer_preamble(name, count, 2);
        for (unsigned int i = 0; i < count; ++i) {
            aiVector3D *uv = &uvs[i];
            pack_float(uv->x);
            pack_float(1.0f - uv->y); /* Flip UVs for OpenGL usage. */
        }
    }

    void pack_floats(const char *name, unsigned int count,
                     unsigned int components, float *values)
    {
        pack_float_buffer_preamble(name, count, components);
        for (unsigned int i = 0; i < count * components; ++i) {
            pack_float(values[i]);
        }
    }


    void pack_mesh_ref(unsigned int type, unsigned int mesh_index,
                       unsigned int bone_index)
    {
        pack_uchar(type);
        pack_ushort(mesh_index);
        pack_ushort(bone_index);
    }


    bool has_uv(aiMesh *mesh, unsigned int i)
    {
        return i < AI_MAX_NUMBER_OF_TEXTURECOORDS && mesh->mTextureCoords[i];
    }

    unsigned int count_uvs(aiMesh *mesh)
    {
        unsigned int count = 0;
        for (unsigned int i = 0; has_uv(mesh, i); ++i) {
            unsigned int components = mesh->mNumUVComponents[i];
            if (components == 2) {
                ++count;
            }
        }
        return count;
    }

    void process_uvs(aiMesh *mesh)
    {
        char name[32];
        for (unsigned int i = 0; has_uv(mesh, i); ++i) {
            snprintf(name, sizeof(name), "uv%u", i);
            aiVector3D *uvs = mesh->mTextureCoords[i];
            unsigned int components = mesh->mNumUVComponents[i];
            if (components == 2) {
                pack_uvs(name, mesh->mNumVertices, uvs);
            }
            else {
                WARN("Can't deal with %u-dimensional UVs", components);
            }
        }
    }


    void pack_bone_buffers(aiMesh *mesh)
    {
        unsigned int vertex_count = mesh->mNumVertices;
        float *bones = (float *)calloc(vertex_count, sizeof(*bones) * NWEIGHTS);
        float *weights =
            (float *)calloc(vertex_count, sizeof(*weights) * NWEIGHTS);
        unsigned int *used =
            (unsigned int *)calloc(vertex_count, sizeof(*used));
        for (unsigned int i = 0; i < vertex_count; ++i) {
            for (unsigned int j = 0; j < NWEIGHTS; ++j) {
                bones[i * NWEIGHTS + j] = 0.0f;
                weights[i * NWEIGHTS + j] = 0.0f;
            }
            used[i] = 0;
        }

        for (unsigned int i = 0; i < mesh->mNumBones; ++i) {
            aiBone *bone = mesh->mBones[i];
            for (unsigned int j = 0; j < bone->mNumWeights; ++j) {
                aiVertexWeight *weight = &bone->mWeights[j];
                unsigned int uindex = weight->mVertexId;
                unsigned int offset = used[uindex]++;
                if (offset >= NWEIGHTS) {
                    DIE("Bone weights exceeded: bone %u, weight %u, offset %u",
                        i, j, offset);
                }
                unsigned int windex = uindex * NWEIGHTS + offset;
                bones[windex] = (float)i;
                weights[windex] = weight->mWeight;
            }
        }

        pack_floats("bones", vertex_count, NWEIGHTS, bones);
        pack_floats("weights", vertex_count, NWEIGHTS, weights);

        free(used);
        free(weights);
        free(bones);
    }

    void pack_bones(aiMesh *mesh)
    {
        pack_ushort(mesh->mNumBones);
        for (unsigned int i = 0; i < mesh->mNumBones; ++i) {
            aiBone *bone = mesh->mBones[i];
            pack_aistring(&bone->mName);
            pack_matrix4x4(&bone->mOffsetMatrix);
            DEBUG("Bone '%*s' %u weights", AISTRING_FMT(bone->mName),
                  bone->mNumWeights);
            DEBUG("Offset (%f, %f, %f, %f,", bone->mOffsetMatrix.a1,
                  bone->mOffsetMatrix.a2, bone->mOffsetMatrix.a3,
                  bone->mOffsetMatrix.a4);
            DEBUG("        %f, %f, %f, %f,", bone->mOffsetMatrix.b1,
                  bone->mOffsetMatrix.b2, bone->mOffsetMatrix.b3,
                  bone->mOffsetMatrix.b4);
            DEBUG("        %f, %f, %f, %f,", bone->mOffsetMatrix.c1,
                  bone->mOffsetMatrix.c2, bone->mOffsetMatrix.c3,
                  bone->mOffsetMatrix.c4);
            DEBUG("        %f, %f, %f, %f)", bone->mOffsetMatrix.d1,
                  bone->mOffsetMatrix.d2, bone->mOffsetMatrix.d3,
                  bone->mOffsetMatrix.d4);
        }
    }

    void process_bones(aiMesh *mesh)
    {
        DEBUG("%u bones", mesh->mNumBones);
        if (mesh->mNumBones > 0) {
            pack_bone_buffers(mesh);
            pack_bones(mesh);
        }
        else {
            pack_ushort(mesh->mNumBones);
        }
    }


    void process_meshes()
    {
        DEBUG("%u meshes", m_scene->mNumMeshes);
        pack_ushort(m_scene->mNumMeshes);
        for (unsigned int i = 0; i < m_scene->mNumMeshes; ++i) {
            aiMesh *mesh = m_scene->mMeshes[i];
            pack_ushort(3 + count_uvs(mesh) + (mesh->mNumBones == 0 ? 0 : 2));
            pack_faces("indices", mesh->mNumFaces, mesh->mFaces);
            unsigned int vertex_count = mesh->mNumVertices;
            DEBUG("Mesh %u: %u vertices", i, vertex_count);
            pack_vector3d_buffer("vertices", vertex_count, mesh->mVertices);
            pack_vector3d_buffer("normals", vertex_count, mesh->mNormals);
            process_uvs(mesh);
            process_bones(mesh);
        }
    }


    unsigned int count_nodes(aiNode *node)
    {
        unsigned int count = 1;
        for (unsigned int i = 0; i < node->mNumChildren; ++i) {
            count += count_nodes(node->mChildren[i]);
        }
        return count;
    }

    void process_node_meshes(aiNode *node)
    {
        DEBUG("%u node meshes", node->mNumMeshes);
        pack_ushort(node->mNumMeshes);
        for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
            DEBUG("Node mesh %u", node->mMeshes[i]);
            pack_ushort(node->mMeshes[i]);
        }
    }

    void process_node_bone_refs(aiNode *node)
    {
        for (unsigned int i = 0; i < m_scene->mNumMeshes; ++i) {
            aiMesh *mesh = m_scene->mMeshes[i];
            for (unsigned int j = 0; j < mesh->mNumBones; ++j) {
                const aiBone *bone = mesh->mBones[j];
                if (node == bone->mArmature) {
                    DEBUG("Node '%*s' is armature of '%*s.%*s'",
                          AISTRING_FMT(node->mName), AISTRING_FMT(mesh->mName),
                          AISTRING_FMT(bone->mName));
                    pack_mesh_ref(97, i, j); /* 97 for 'a' in ASCII */
                }
                if (node == bone->mNode) {
                    DEBUG("Node '%*s' is node of '%*s.%*s'",
                          AISTRING_FMT(node->mName), AISTRING_FMT(mesh->mName),
                          AISTRING_FMT(bone->mName));
                    pack_mesh_ref(110, i, j); /* 110 for 'n' in ASCII */
                }
            }
        }
        pack_uchar(0);
    }

    unsigned int process_node(aiNode *node, unsigned int next_id)
    {
        unsigned int id = next_id++;
        DEBUG("Node %u '%*s' %p with %u children", id, node->mName.length,
              node->mName.data, (void *)node, node->mNumChildren);
        put_node(node, id);
        pack_ushort(id);
        pack_aistring(&node->mName);

        aiVector3D scaling, position;
        aiQuaternion rotation;
        node->mTransformation.Decompose(scaling, rotation, position);
        pack_vector3d(&position);
        pack_quaternion(&rotation);
        pack_vector3d(&scaling);

        process_node_meshes(node);
        process_node_bone_refs(node);
        pack_ushort(node->mNumChildren);
        for (unsigned int i = 0; i < node->mNumChildren; ++i) {
            next_id = process_node(node->mChildren[i], next_id);
        }
        return next_id;
    }

    void process_nodes()
    {
        pack_ushort(count_nodes(m_scene->mRootNode));
        process_node(m_scene->mRootNode, 0);
    }


    bool is_usable_animation(aiAnimation *anim)
    {
        return anim->mNumChannels != 0; // Only node animations are supported.
    }

    unsigned int count_usable_animations()
    {
        unsigned int count = 0;
        for (unsigned int i = 0; i < m_scene->mNumAnimations; ++i) {
            if (is_usable_animation(m_scene->mAnimations[i])) {
                ++count;
            }
        }
        return count;
    }

    void pack_node_anim_positions(aiNodeAnim *na)
    {
        pack_ushort(na->mNumPositionKeys);
        for (unsigned int i = 0; i < na->mNumPositionKeys; ++i) {
            const aiVectorKey *pk = &na->mPositionKeys[i];
            pack_double(pk->mTime);
            pack_vector3d(&pk->mValue);
        }
    }

    void pack_node_anim_rotations(aiNodeAnim *na)
    {
        pack_ushort(na->mNumRotationKeys);
        for (unsigned int i = 0; i < na->mNumRotationKeys; ++i) {
            const aiQuatKey *rk = &na->mRotationKeys[i];
            pack_double(rk->mTime);
            pack_quaternion(&rk->mValue);
        }
    }

    void pack_node_anim_scalings(aiNodeAnim *na)
    {
        pack_ushort(na->mNumScalingKeys);
        for (unsigned int i = 0; i < na->mNumScalingKeys; ++i) {
            const aiVectorKey *sk = &na->mScalingKeys[i];
            pack_double(sk->mTime);
            pack_vector3d(&sk->mValue);
        }
    }

    void pack_animation(aiAnimation *anim)
    {
        DEBUG("Animation '%*s', duration %f, ticks/s %f, channels %u",
              AISTRING_FMT(anim->mName), anim->mDuration, anim->mTicksPerSecond,
              anim->mNumChannels);
        pack_aistring(&anim->mName);
        pack_double(anim->mDuration);
        pack_double(anim->mTicksPerSecond);
        pack_ushort(anim->mNumChannels);
        for (unsigned int i = 0; i < anim->mNumChannels; ++i) {
            aiNodeAnim *na = anim->mChannels[i];
            DEBUG("NodeAnim on '%*s', %u positions, %u rotations, %u scales",
                  AISTRING_FMT(na->mNodeName), na->mNumPositionKeys,
                  na->mNumRotationKeys, na->mNumScalingKeys);
            unsigned int node_id = find_node_id_by_name(na->mNodeName);
            pack_ushort(node_id);
            pack_node_anim_positions(na);
            pack_node_anim_rotations(na);
            pack_node_anim_scalings(na);
        }
    }

    void pack_animations()
    {
        for (unsigned int i = 0; i < m_scene->mNumAnimations; ++i) {
            aiAnimation *anim = m_scene->mAnimations[i];
            if (is_usable_animation(anim)) {
                pack_animation(anim);
            }
        }
    }

    void process_animations()
    {
        pack_ushort(count_usable_animations());
        pack_animations();
    }


    void process_scene()
    {
        fwrite(RMODEL_MAGIC, 1, strlen(RMODEL_MAGIC) + 1, m_fp);
        process_meshes();
        process_nodes();
        process_animations();
    }
};


int main(int argc, char **argv)
{
    if (argc != 2) {
        DIE("Usage: %s INPUT_FILE", argc < 1 ? "rmodel" : argv[0]);
    }

    unsigned int flags = aiProcess_FindDegenerates       //
                       | aiProcess_FindInvalidData       //
                       | aiProcess_JoinIdenticalVertices //
                       | aiProcess_LimitBoneWeights      //
                       | aiProcess_OptimizeGraph         //
                       | aiProcess_OptimizeMeshes        //
                       | aiProcess_PopulateArmatureData  //
                       | aiProcess_RemoveComponent       //
                       | aiProcess_SortByPType           //
                       | aiProcess_SplitByBoneCount      //
                       | aiProcess_SplitLargeMeshes      //
                       | aiProcess_ValidateDataStructure;

    Assimp::Importer importer;

    importer.SetPropertyInteger(AI_CONFIG_PP_LBW_MAX_WEIGHTS, NWEIGHTS);

    int components_to_remove = aiComponent_CAMERAS //
                             | aiComponent_COLORS  //
                             | aiComponent_LIGHTS  //
                             | aiComponent_MATERIALS;
    importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS, components_to_remove);

    importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE,
                                aiPrimitiveType_POINT | aiPrimitiveType_LINE);

    importer.SetPropertyInteger(AI_CONFIG_PP_SBBC_MAX_BONES, NBONES);

    importer.SetPropertyInteger(AI_CONFIG_PP_SLM_VERTEX_LIMIT, UINT16_MAX);
    importer.SetPropertyInteger(AI_CONFIG_PP_SLM_TRIANGLE_LIMIT, UINT16_MAX);

    const char *path = argv[1];
    DEBUG("Path '%s'", path);

    const aiScene *scene = importer.ReadFile(path, flags);
    if (!scene) {
        DIE("Error importing '%s': %s", path, importer.GetErrorString());
    }

    Context c{stdout, scene};
    c.process_scene();
    return 0;
}
