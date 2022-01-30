#version 330 core
out vec4 frag;

in vec2 vertexUV;

uniform sampler2D tex;
uniform vec4 color;

void main() {
    frag = texture(tex, vertexUV) * color;
}