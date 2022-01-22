#version 330 core
out vec4 frag;

in vec2 vertexUV;

uniform sampler2D color;

uniform sampler2D bloom1;
uniform sampler2D bloom2;
uniform sampler2D bloom4;
uniform sampler2D bloom8;
uniform sampler2D bloom16;
uniform sampler2D bloom32;

void main() {

    vec3 col = texture(color, vertexUV).rgb;
    vec3 bl1 = texture(bloom1, vertexUV).rgb;
    vec3 bl2 = texture(bloom2, vertexUV).rgb;
    vec3 bl4 = texture(bloom4, vertexUV).rgb;
    vec3 bl8 = texture(bloom8, vertexUV).rgb;
    vec3 bl16 = texture(bloom16, vertexUV).rgb;
    vec3 bl32 = texture(bloom32, vertexUV).rgb;

    frag = vec4(col + bl1 + bl2 + bl4 + bl8 + bl16 + bl32, 1);
}