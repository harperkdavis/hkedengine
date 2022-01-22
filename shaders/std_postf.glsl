#version 330 core
out vec4 frag;

in vec2 vertexUV;

uniform sampler2D color;
uniform sampler2D bright;

void main() {
    frag = texture(color, vertexUV);
}