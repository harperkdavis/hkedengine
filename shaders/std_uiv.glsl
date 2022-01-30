#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out vec2 vertexUV;

uniform mat4 projection;
uniform mat4 model;

void main() {
    vertexUV = uv;

    gl_Position = projection * model * vec4(position, 1.0);
}