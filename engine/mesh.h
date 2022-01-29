//
// Created by harperkdavis on 1/22/2022.
//

#ifndef HKEDENGINE_MESH_H
#define HKEDENGINE_MESH_H

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "dep/obj_loader.h"

#include "dep/stb_image.h"
#include "shader.h"

using namespace std;

// Class for storing vertex information
struct Vertex {

    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;

    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv) {
        this->position = position;
        this->normal = glm::normalize(normal);
        this->uv = uv;
    }

};

// Class for storing mesh information
class Mesh {
public:
    static Mesh cube(float size);
    static Mesh cubemap(float size);
    static Mesh rect(float xSize, float ySize, float zSize);

    static Mesh load(string path);
    static Mesh load(string path, glm::vec3 scale);

    vector<Vertex> vertices;
    vector<unsigned int> indices;

    void draw() const;
    void draw(int count) const;

    Mesh();
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices);

private:

    unsigned int vao, vbo, ibo;
    void load();

};

#endif //HKEDENGINE_MESH_H
