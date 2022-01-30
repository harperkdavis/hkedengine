//
// Created by harperkdavis on 1/29/2022.
//

#ifndef HKEDENGINE_UI_H
#define HKEDENGINE_UI_H


#include <bits/stdc++.h>
#include <string>
#include <map>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>

#include "mesh.h"
#include "material.h"

#include "dep/stb_image.h"
#include "shader.h"

using namespace std;

struct Character {
    glm::ivec2 position;
    glm::ivec2 size;
    glm::ivec2 bearing;
    int advance;
    std::map<char, int> kerning;
};

class Font {
public:

    Material* fontMaterial;
    std::map<char, Character> characters;

    int lineHeight = 0;
    int baseLine = 0;
    glm::vec2 fontScale;

    Mesh getTextMesh(string text, float scale);

    Font(string fntPath, string imagePath);

private:

    std::map<string, int> readLine(string line);
};

class UiThing {
public:

    Mesh mesh;
    Material* material;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    glm::mat4 getModelMatrix() const;

    void replaceMesh(Mesh newMesh);

    void draw(Shader& shader) const;

    UiThing(Mesh mesh, Material* material, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

};

class UI {
public:

    static Mesh uiCenter(glm::vec2 size);
    static inline glm::mat4 projection = glm::mat4(1);
    static inline Shader* uiShader;

    static void initialize(int width, int height);
    static void startPass();
    static void endPass();

};

#endif //HKEDENGINE_UI_H
