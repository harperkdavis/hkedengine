#version 330 core

layout (location = 0) out vec4 frag;
layout (location = 1) out vec4 bright;

struct DirectionalLight {
    vec3 direction;
    vec4 color;
    float intensity;
};

in vec2 vertexUV;

uniform vec3 viewPos;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

uniform vec4 ambientLight;
uniform DirectionalLight dirLight;

const float gamma = 1.2;

vec4 calcDirLight(vec3 normal, vec3 albedo) {
    vec3 lightDir = normalize(-dirLight.direction);

    float diff = max(dot(lightDir, normal), 0);

    vec3 ambient = ambientLight.rgb * albedo.rgb;
    vec3 diffuse = diff * albedo.rgb * dirLight.intensity;

    return vec4(diffuse + ambient, 1);
}

void main() {
    vec3 vertexNormal = normalize(texture(gNormal, vertexUV).xyz);
    vec3 vertexPosition = texture(gPosition, vertexUV).xyz;
    vec3 vertexAlbedo = texture(gAlbedoSpec, vertexUV).rgb;
    float vertexSpecular = texture(gAlbedoSpec, vertexUV).a;

    vec3 viewDir = normalize(viewPos - vertexPosition);

    vec3 itslit = calcDirLight(vertexNormal, vertexAlbedo).xyz;

    frag = vec4(itslit, 1);
    bright = vec4(pow(itslit.x, 5), pow(itslit.y, 5), pow(itslit.z, 5), 1);
}