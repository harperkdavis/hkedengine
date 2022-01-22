#version 330 core

in vec2 vertexUV;

out vec4 frag;

uniform sampler2D color;

void main() {
    frag = texture(color, vertexUV);
}