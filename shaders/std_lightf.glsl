#version 330 core

layout (location = 0) out vec4 frag;
layout (location = 1) out vec4 bright;

struct DirectionalLight {
    vec3 direction;
    vec4 color;
    float intensity;
};

struct PointLight {
    vec3 position;
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
uniform DirectionalLight directionalLight;
uniform PointLight[64] pointLights;

float bias(float x, float bias) {
    float k = pow(1 - bias, 3);
    return (x * k) / (x * k - x + 1);
}

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

vec3 calcDirLight(vec3 normal, vec3 albedo, vec3 viewDir, float specular) {
    vec3 lightDir = normalize(-directionalLight.direction);
    float diff = max(dot(lightDir, normal), 0);
    vec3 diffuse = directionalLight.color.rgb * diff * albedo.rgb;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
    vec3 specul = directionalLight.color.rgb * spec * specular;

    return (diffuse + specul) * directionalLight.intensity;
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

    // Directional Lighting
    vec3 ambient = ambientLight.rgb * vertexAlbedo.rgb;
    vec3 delight = calcDirLight(vertexNormal, vertexAlbedo, viewDir, vertexSpecular);

    float shadow = calcShadow(lightSpaceMatrix * vec4(vertexPosition, 1));

    vec3 lighting = ambient + (1 - shadow) * delight;

    // Point Lighting
    for (int i = 0; i < 64; i++) {
        PointLight pl = pointLights[i];
        float distance = length(pl.position - vertexPosition);
        if (distance < pl.intensity * 10) {
            vec3 lightDir = normalize(pl.position - vertexPosition);
            vec3 diffuse = max(dot(vertexNormal, lightDir), 0.0) * vertexAlbedo * pl.color.rgb;

            vec3 halfwayDir = normalize(lightDir + viewDir);
            float spec = pow(max(dot(vertexNormal, halfwayDir), 0.0), 16.0);
            vec3 specular = pl.color.rgb * spec * vertexSpecular;

            float attentuation = 1.0 / (1.0 + 0.4 * distance + 0.3 * distance * distance);
            diffuse *= attentuation * pl.intensity;
            specular *= attentuation * pl.intensity;
            lighting += diffuse + specular;
        }
    }

    vec3 lit = vertexLighting.r > 0 ? vertexAlbedo + vertexAlbedo * pow(vec3(vertexLighting).r, 2) * 16: lighting;

    frag = vec4(lit, 1);

    float lum = (0.299 * lit.r + 0.587 * lit.g + 0.114 * lit.b);

    bright = vec4(lit * bias(min(max(lum / 1.5, 0), 1), 0.5), 1);
}