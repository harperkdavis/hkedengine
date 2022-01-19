//
// Created by harperkdavis on 1/18/2022.
//
#include <glm/glm.hpp>
#include <vector>
#include "shader.h"

#ifndef HKEDENGINE_MESH_H
#define HKEDENGINE_MESH_H

using namespace std;

// Class for storing vertex information
struct Vertex {

    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;

    Vertex(glm::vec3 position, glm::vec2 uv, glm::vec3 normal) {
        this->position = position;
        this->uv = uv;
        this->normal = normal;
    }

};

// Class for storing mesh information
class Mesh {
public:

    vector<Vertex> vertices;
    vector<unsigned int> indices;

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices);
    void draw(Shader &shader) const;

private:

    unsigned int vao, vbo, ibo;
    void load();

};

#endif //HKEDENGINE_MESH_H
