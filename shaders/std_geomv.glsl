#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out vec3 vertexPos;
out vec2 vertexUV;
out vec3 vertexNormal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main() {

    vec4 worldSpace = model * vec4(position, 1.0);
    vertexPos = worldSpace.xyz;

    vertexUV = uv;
    vertexNormal = normal * transpose(inverse(mat3(model)));

    gl_Position = projection * view * worldSpace;

}