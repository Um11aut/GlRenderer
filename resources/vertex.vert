#version 460 core

layout (location = 0) in vec3 position;
layout(location = 1) in vec2 texture_coord;

out vec2 texCoord;

layout (std140, binding = 0) uniform MVP {
    mat4 model;
    mat4 view;
    mat4 proj;
};

void main()
{
    gl_Position = proj * view * model * vec4(position, 1.0);
    texCoord = texture_coord;
}
