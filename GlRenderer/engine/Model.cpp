#include "Model.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Model Model::create(std::unique_ptr<Camera>& camera, const std::string& model_name) 
{
    const float vertices[] = {
        -1.0f,-1.0f,-1.0f, // triangle 1 : begin
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end
        1.0f, 1.0f,-1.0f, // triangle 2 : begin
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f, // triangle 2 : end
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };


    std::unique_ptr<Shader> shader = std::make_unique<Shader>(Shader::WithResultOf{ 
        []() {
            return Shader::compile(Shader::ShaderModule{
                .vertexPath = "resources/vertex.vert",
                .fragmentPath = "resources/fragment.frag"
            });
         }
    });

    std::unique_ptr<VertexObject> vertex_object = std::make_unique<VertexObject>(VertexObject::WithResultOf{ 
        [&vertices]() {
            return VertexObject::create(vertices, sizeof(vertices) / sizeof(vertices[0]));
        }
    });

    std::shared_ptr<glm::mat4> model = std::make_shared<glm::mat4>(1.f);

    std::unique_ptr<UniformObject> uniform_object = std::make_unique<UniformObject>(UniformObject::WithResultOf{
        [&shader, &camera, &model]() {
            return UniformObject::create(UniformObject::Parameters{
                model,
                camera,
                shader->get_program(),
                0
            });
        }
    });

    return Model(M{
        .model_name = model_name,
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
    m.uniform_object->invoke();
    
    m.uniform_object->update();
    m.shader->invoke();
    m.vertex_object->invoke();

    m.uniform_object->revoke();
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