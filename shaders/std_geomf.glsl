#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec4 gLightingData;

in vec3 vertexPos;
in vec2 vertexUV;
in vec3 vertexNormal;

uniform sampler2D diffuse;
uniform float emission = 0;
uniform float specular = 0;

void main() {

    gPosition = vertexPos;
    gNormal = normalize(vertexNormal);

    gAlbedoSpec.rgb = texture(diffuse, vertexUV).rgb;
    gAlbedoSpec.a = specular;

    gLightingData = vec4(emission, 0, 0, 0);
}