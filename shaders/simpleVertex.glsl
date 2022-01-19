#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out vec2 vertexUv;

uniform float time;

const float PI = 3.14159265358979;

void main() {
    gl_Position = vec4(position.x, position.y + sin(time * 2 * (60.0 / 128.0) * PI + position.y * 0.8 + position.x * 0.4) * 0.2, position.z, 1.0);

    vertexUv = uv;
}