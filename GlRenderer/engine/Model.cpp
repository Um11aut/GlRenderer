#include "Model.h"

std::vector<float> g_vertices;
std::vector<uint32_t> g_indices;
std::vector<float> g_texture_coords;

static void process_mesh(aiMesh* mesh, const aiScene* scene) {
    spdlog::info("Loading mesh with {} number of vertices", mesh->mNumVertices);

    for (size_t i = 0; i < mesh->mNumVertices; ++i) {
        aiVector3D vertex = mesh->mVertices[i];
        g_vertices.push_back(vertex.x);
        g_vertices.push_back(vertex.y);
        g_vertices.push_back(vertex.z);

        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            g_texture_coords.push_back(mesh->mTextureCoords[0][i].x);
            g_texture_coords.push_back(mesh->mTextureCoords[0][i].y);
        }
        else
            g_texture_coords.push_back(0.f), g_texture_coords.push_back(0.f);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            g_indices.push_back(face.mIndices[j]);
    }
}

static void process_node(aiNode* node, const aiScene* scene) {
    for (size_t i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        process_mesh(mesh, scene);
    }
    for (size_t i = 0; i < node->mNumChildren; i++)
    {
        process_node(node->mChildren[i], scene);
    }
}

Model Model::create(ModelCreateInfo info)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(info.path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        spdlog::error("Error using Assimp: {}", importer.GetErrorString());
    }
    else {
        process_node(scene->mRootNode, scene);
    }

    std::unique_ptr<Shader> shader = std::make_unique<Shader>(Shader::WithResultOf{ 
        []() {
            return Shader::compile(Shader::ShaderModule{
                .vertexPath = "resources/vertex.vert",
                .fragmentPath = "resources/fragment.frag"
            });
         }
    });

    std::unique_ptr<Texture> texture = std::make_unique<Texture>(Texture::WithResultOf{
        []() {
            return Texture::create("resources/textures/texture.jpg");
        }
    });

    spdlog::info("Model with {} size!", g_vertices.size());

    std::unique_ptr<VertexObject> vertex_object = std::make_unique<VertexObject>(VertexObject::WithResultOf{ 
        [&]() {
            return VertexObject::create({
                .vertices = g_vertices,
                .textureCoords = g_texture_coords,
                .indices = g_indices,
                .vertices_count = static_cast<uint32_t>(g_vertices.size()),
                .indices_count = static_cast<uint32_t>(g_indices.size()),
                .texture_coords_count = static_cast<uint32_t>(g_texture_coords.size())
            });
        }
    });

    std::shared_ptr<glm::mat4> model = std::make_shared<glm::mat4>(0.5f);

    auto& cam = info.camera.get();

    std::unique_ptr<UniformObject> uniform_object = std::make_unique<UniformObject>(UniformObject::WithResultOf{
        [&shader, &cam, &model]() {
            return UniformObject::create(UniformObject::Parameters{
                model,
                cam,
                shader->get_program(),
                0
            });
        }
    });

    shader->invoke();
    shader->set_uniform_i1("txt");

    return Model(M{
        .model_name = info.name,
        .texture = std::move(texture),
        .shader = std::move(shader),
        .vertex_object = std::move(vertex_object),
        .uniform_object = std::move(uniform_object),
        .model = model,
        .model_position = {0,0,0}
    });
}

Model::Model(WithResultOf&& res) {
    auto other = res.func();

    m = std::move(other.m);
}

void Model::invoke() const
{
    m.shader->invoke();

    m.uniform_object->invoke();
    m.uniform_object->update();
    
    m.texture->invoke();
    m.vertex_object->invoke();

    m.uniform_object->revoke();

    glUseProgram(0);
}

void Model::destroy() const
{
    m.vertex_object->destroy();
}

void Model::set_position(const glm::vec3& new_position)
{
    *m.model = glm::translate(glm::mat4(1.0f), new_position);
    m.model_position = new_position;
}