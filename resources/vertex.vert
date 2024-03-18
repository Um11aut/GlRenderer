#version 460 core

layout (location = 0) in vec3 position;

layout (std140, binding = 0) uniform MVP {
    mat4 model;
    mat4 view;
    mat4 proj;
};

void main()
{
    gl_Position = proj * view * model * vec4(position, 1.0);
}
