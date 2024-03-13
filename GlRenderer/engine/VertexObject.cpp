#include "VertexObject.h"
#include <spdlog/spdlog.h>

VertexObject VertexObject::create(float v[])
{
    uint32_t VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), v, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
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
}

void VertexObject::destroy() const
{
    glDeleteBuffers(1, &m.VBO);
}

VertexObject::~VertexObject()
{
    // because of std::move the destructor is being called more than once, so the 
    // buffer is being destroyed. Therefore I need to create a separate function for the destruction if array.
}



VertexObject::VertexObject(WithResultOf&& res) noexcept
{
	VertexObject other = std::move(res.func());

	m = std::move(other.m);
}
