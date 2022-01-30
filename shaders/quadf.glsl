#version 330 core

in vec2 vertexUV;

out vec4 frag;

uniform sampler2D color;

void main() {
    frag = vec4(1, 0, 0, 1); // texture(color, vertexUV);
}