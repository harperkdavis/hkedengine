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
uniform sampler2D shadowMap;

uniform mat4 lightSpaceMatrix;

uniform vec4 ambientLight;
uniform DirectionalLight dirLight;

float calcShadow(vec4 lightSpace) {

    vec3 projCoords = lightSpace.xyz / lightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x){
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - 0.001 > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    if(projCoords.z > 1.0)
    shadow = 0.0;

    return shadow;
}


vec3 calcDirLight(vec3 normal, vec3 albedo) {
    vec3 lightDir = normalize(-dirLight.direction);
    float diff = max(dot(lightDir, normal), 0);
    vec3 diffuse = diff * albedo.rgb * dirLight.intensity;

    return diffuse;
}

void main() {
    // Define fragment information
    vec3 vertexNormal = normalize(texture(gNormal, vertexUV).xyz);
    if (vertexNormal == vec3(0)) discard;

    vec3 vertexPosition = texture(gPosition, vertexUV).xyz;
    vec3 vertexAlbedo = texture(gAlbedoSpec, vertexUV).rgb;
    float vertexSpecular = texture(gAlbedoSpec, vertexUV).a;

    vec4 vertexLighting = texture(gLightingData, vertexUV);
    vec3 viewDir = normalize(viewPos - vertexPosition);

    // Calculate lighting

    vec3 ambient = ambientLight.rgb * vertexAlbedo.rgb;
    vec3 diffuse = calcDirLight(vertexNormal, vertexAlbedo);

    float shadow = calcShadow(lightSpaceMatrix * vec4(vertexPosition, 1));

    vec3 lit = vertexLighting.r > 0 ? vertexAlbedo + vec3(vertexLighting).r * 16 : ambient + (1 - shadow) * diffuse;

    frag = vec4(lit, 1);

    float lum = (0.2126 * lit.r + 0.7152 * lit.g + 0.0722 * lit.b);

    bright = vec4(lit * pow(exp(min(lum, 2) - 2), 2), 1);
}