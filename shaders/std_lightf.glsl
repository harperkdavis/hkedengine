#version 330 core

struct DirectionalLight {
    vec3 direction;
    vec4 color;
    float intensity;
};

out vec4 frag;

in vec2 vertexUV;

uniform vec3 viewPos;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

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
    vec3 vertexPosition = texture(gPosition, vertexUV).xyz;
    vec3 vertexAlbedo = texture(gAlbedoSpec, vertexUV).rgb;
    float vertexSpecular = texture(gAlbedoSpec, vertexUV).a;

    vec3 viewDir = normalize(viewPos - vertexPosition);

    vec4 color = calcDirLight(vertexNormal, vertexAlbedo);
    frag = color * color * color;
}