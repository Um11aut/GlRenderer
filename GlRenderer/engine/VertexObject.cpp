#include "VertexObject.h"
#include <spdlog/spdlog.h>

VertexObject VertexObject::create(const float vertices[], uint32_t vertices_count)
{
    uint32_t VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices_count * sizeof(float), vertices, GL_STATIC_DRAW);

    // Assuming each vertex has 3 components (x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    return VertexObject(M{
        .VAO = VAO,
        .VBO = VBO
        });
}

void VertexObject::invoke() const
{
    glBindVertexArray(m.VAO);
    // Assuming each face of the cube consists of 6 vertices (triangles)
    glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
}

void VertexObject::destroy() const
{
    glDeleteVertexArrays(1, &m.VAO);
    glDeleteBuffers(1, &m.VBO);
}

VertexObject::~VertexObject()
{
    // because of std::move, the destructor is being called more than once, so the 
    // buffer is being destroyed. Therefore I need to create a separate function for the destruction.
}

VertexObject::VertexObject(WithResultOf&& res) noexcept
{
	VertexObject other = std::move(res.func());

	m = std::move(other.m);
}
