#version 330 core
out vec4 frag;

in vec2 vertexUV;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

void main() {
    frag = vec4(texture(gAlbedoSpec, vertexUV).rgb, 1);
}