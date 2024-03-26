#include "VertexObject.h"
#include <spdlog/spdlog.h>

VertexObject VertexObject::create(VertexBufferCreateInfo info)
{
    uint32_t VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create and bind Vertex Buffer Object (VBO) for vertices
    uint32_t VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, info.vertices_count * sizeof(float), info.vertices.get().data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // Create and bind Vertex Buffer Object (VBO) for texture coordinates
    uint32_t textureVBO;
    glGenBuffers(1, &textureVBO);
    glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
    glBufferData(GL_ARRAY_BUFFER, info.texture_coords_count * sizeof(float), info.textureCoords.get().data(), GL_STATIC_DRAW);

    // Bind the texture coordinate buffer
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(1);

    // Create and bind Element Buffer Object (EBO) for indices
    uint32_t EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, info.indices_count * sizeof(uint32_t), info.indices.get().data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    return VertexObject(M{
        .VAO = VAO,
        .VBO = VBO,
        .EBO = EBO,
        .indices_count = info.indices_count
        });
}

void VertexObject::invoke() const
{
    glBindVertexArray(m.VAO);
    glDrawElements(GL_TRIANGLES, m.indices_count, GL_UNSIGNED_INT, nullptr);
}

void VertexObject::destroy() const
{
    glDeleteVertexArrays(1, &m.VAO);
    glDeleteBuffers(1, &m.VBO);
    glDeleteBuffers(1, &m.EBO);
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
