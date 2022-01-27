//
// Created by harperkdavis on 1/22/2022.
//

#ifndef HKEDENGINE_MATERIAL_H
#define HKEDENGINE_MATERIAL_H

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "dep/stb_image.h"
#include "shader.h"

using namespace std;

// Class for storing texture information
class Texture {
public:

    int width, height, channels;
    unsigned char* data;

    void use() const;

    Texture();
    explicit Texture(string path);
    Texture(string path, int minFiltering, int magFiltering);
    Texture(string path, int minFiltering, int magFiltering, int edgeMode);

    unsigned int id;
private:

    void load(int minFiltering, int magFiltering, int edgeMode);
};

// Class for storing material information
class Material {
public:

    Texture* texture = nullptr;
    glm::vec4 color;
    float specular = 0.2f;
    float emission = 0.0f;

    Material();
    explicit Material(Texture* texture);
    Material(Texture* texture, glm::vec4 color);
};

#endif //HKEDENGINE_MATERIAL_H
