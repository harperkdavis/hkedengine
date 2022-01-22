#version 330 core
out vec4 frag;

in vec2 vertexUV;

uniform sampler2D pass;

const float blurSum = 484;
float[49] blur = float[49](
    1, 2, 4, 8, 4, 2, 1,
    2, 4, 8, 16, 8, 4, 2,
    4, 8, 16, 32, 16, 8, 4,
    8, 16, 32, 64, 32, 16, 8,
    4, 8, 16, 32, 16, 8, 4,
    2, 4, 8, 16, 8, 4, 2,
    1, 2, 4, 8, 4, 2, 1
);

void main() {
    vec2 pixelSize = (1.0 / textureSize(pass, 0)) * 2;

    vec3 sum = vec3(0);
    for (int x = -3; x <= 3; x++) {
        for (int y = -3; y <= 3; y++) {
            sum += texture(pass, vertexUV + vec2(pixelSize.x * x, pixelSize.y * y)).xyz * blur[(x + 3) + (y + 3) * 7];
        }
    }
    sum /= blurSum;
    frag = vec4(sum, 1);
}