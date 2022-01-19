#version 330 core
out vec4 frag;

in vec2 vertexUv;

uniform sampler2D tex;

void main() {
    frag = texture(tex, vertexUv);
}