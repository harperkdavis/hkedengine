#version 330 core

in vec2 vertexUV;

out vec4 frag;

uniform sampler2D tex;
uniform vec4 color;

void main() {
    frag = texture(tex, vertexUV) * color;
}