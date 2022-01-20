#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 vertexPos;
in vec2 vertexUV;
in vec3 vertexNormal;

uniform sampler2D diffuse;

void main() {

    gPosition = vertexPos;
    gNormal = normalize(vertexNormal);

    gAlbedoSpec.rgb = texture(diffuse, vertexUV).rgb;
    gAlbedoSpec.a = 0;

}