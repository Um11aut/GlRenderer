#include "UniformObject.h"

UniformObject UniformObject::create(const uint32_t& program, const uint32_t& binding)
{
	glm::mat4 view = glm::lookAt(
		glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	glm::mat4 proj = glm::perspective(glm::radians(45.f), (float)800 / (float)600, .1f, 100.f);
	glm::mat4 model(1.0f);

    GLuint ubo;
    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 3, nullptr, GL_DYNAMIC_DRAW); // Allocate space for MVP matrices
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Bind the buffer to the appropriate binding point (you should have the binding point defined in your shader)
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo); // Assuming binding point 0

    // Send the MVP matrices to the buffer
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(model)); // Model matrix
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view)); // View matrix
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(proj)); // Projection matrix
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    GLuint blockIndex = glGetUniformBlockIndex(program, "MVP");
    glUniformBlockBinding(program, blockIndex, binding);

    return UniformObject(M{
        ubo,
        program,
        {model, view, proj}
    });
}

void UniformObject::invoke() const
{
    // Calculate rotation angle based on time or any other parameter
    static float angle = 0.0f;
    angle += 0.01f; // Adjust rotation speed as needed

    // Update model matrix with rotation transformation
    glm::mat4 model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));

    // Bind the buffer object
    glBindBuffer(GL_UNIFORM_BUFFER, m.UBO);

    // Update the model matrix data in the buffer object
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(model));

    // Unbind the buffer object
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

UniformObject::UniformObject(WithResultOf&& res) noexcept
{
    auto other = res.func();

    m = std::move(other.m);
}
