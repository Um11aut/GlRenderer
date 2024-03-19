#include "UniformObject.h"

UniformObject UniformObject::create(Parameters&& p)
{
    std::shared_ptr<glm::mat4> proj = p.camera->get_proj();
    std::shared_ptr<glm::mat4> view = p.camera->get_view();

    GLuint ubo;
    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 3, nullptr, GL_DYNAMIC_DRAW); // Allocate space for MVP matrices
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Bind the buffer to the appropriate binding point (you should have the binding point defined in your shader)
    glBindBufferBase(GL_UNIFORM_BUFFER, p.binding, ubo); // Assuming binding point 0

    // Send the MVP matrices to the buffer
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), p.model.get()); // Model matrix
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), view.get()); // View matrix
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), proj.get()); // Projection matrix
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    GLuint blockIndex = glGetUniformBlockIndex(p.program, "MVP");
    glUniformBlockBinding(p.program, blockIndex, p.binding);

    return UniformObject(M{
        ubo,
        p.program,
        p.binding,
        {p.model, view, proj}
    });
}

void UniformObject::invoke()
{
    // Bind the buffer object
    glBindBufferBase(GL_UNIFORM_BUFFER, m.binding, m.UBO);
}

void UniformObject::update()
{
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), m.mvp.model.get()); // Model matrix
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), m.mvp.view.get()); // View matrix
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), m.mvp.proj.get()); // Projection matrix
}

void UniformObject::revoke()
{
    glBindBufferBase(GL_UNIFORM_BUFFER, m.binding, 0);
}

UniformObject::UniformObject(WithResultOf&& res) noexcept
{
    auto other = res.func();

    m = std::move(other.m);
}
