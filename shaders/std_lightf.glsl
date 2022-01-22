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
uniform sampler2D gLightingData;

uniform vec4 ambientLight;
uniform DirectionalLight dirLight;

vec4 calcDirLight(vec3 normal, vec3 albedo) {
    vec3 lightDir = normalize(-dirLight.direction);

    float diff = max(dot(lightDir, normal), 0);

    vec3 ambient = ambientLight.rgb * albedo.rgb;
    vec3 diffuse = diff * albedo.rgb * dirLight.intensity;

    return vec4(diffuse + ambient, 1);
}

void main() {
    vec3 vertexNormal = normalize(texture(gNormal, vertexUV).xyz);
    if (vertexNormal == vec3(0)) discard;
    vec3 vertexPosition = texture(gPosition, vertexUV).xyz;
    vec3 vertexAlbedo = texture(gAlbedoSpec, vertexUV).rgb;
    float vertexSpecular = texture(gAlbedoSpec, vertexUV).a;

    vec4 vertexLighting = texture(gLightingData, vertexUV);

    vec3 viewDir = normalize(viewPos - vertexPosition);

    vec3 lit = vertexLighting.r > 0 ? vertexAlbedo + vec3(vertexLighting).r * 16 : calcDirLight(vertexNormal, vertexAlbedo).xyz;

    frag = vec4(lit, 1);

    float lum = (0.2126 * lit.r + 0.7152 * lit.g + 0.0722 * lit.b);

    bright = vec4(vertexAlbedo * pow(exp(min(lum, 2) - 2), 2), 1);
}