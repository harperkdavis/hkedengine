//
// Created by harperkdavis on 1/22/2022.
//

#include "mesh.h"

// Simple cube mesh
Mesh Mesh::cube(float size) {
    return Mesh({
        // Back
        Vertex(glm::vec3(-size, size, -size), glm::vec3(0, 0, -1), glm::vec2(0, 0)),
        Vertex(glm::vec3(-size, -size, -size), glm::vec3(0, 0, -1), glm::vec2(0, 1)),
        Vertex(glm::vec3(size, -size, -size), glm::vec3(0, 0, -1), glm::vec2(1, 1)),
        Vertex(glm::vec3(size, size, -size), glm::vec3(0, 0, -1), glm::vec2(1, 0)),

        // Front
        Vertex(glm::vec3(size, size, size), glm::vec3(0, 0, 1), glm::vec2(0, 0)),
        Vertex(glm::vec3(size, -size, size), glm::vec3(0, 0, 1), glm::vec2(0, 1)),
        Vertex(glm::vec3(-size, -size, size), glm::vec3(0, 0, 1), glm::vec2(1, 1)),
        Vertex(glm::vec3(-size, size, size), glm::vec3(0, 0, 1), glm::vec2(1, 0)),

        // Left
        Vertex(glm::vec3(-size, size, size), glm::vec3(-1, 0, 0), glm::vec2(0, 0)),
        Vertex(glm::vec3(-size, -size, size), glm::vec3(-1, 0, 0), glm::vec2(0, 1)),
        Vertex(glm::vec3(-size, -size, -size), glm::vec3(-1, 0, 0), glm::vec2(1, 1)),
        Vertex(glm::vec3(-size, size, -size), glm::vec3(-1, 0, 0), glm::vec2(1, 0)),

        // Right
        Vertex(glm::vec3(size, size, -size), glm::vec3(1, 0, 0), glm::vec2(0, 0)),
        Vertex(glm::vec3(size, -size, -size), glm::vec3(1, 0, 0), glm::vec2(0, 1)),
        Vertex(glm::vec3(size, -size, size), glm::vec3(1, 0, 0), glm::vec2(1, 1)),
        Vertex(glm::vec3(size, size, size), glm::vec3(1, 0, 0), glm::vec2(1, 0)),

        // Top
        Vertex(glm::vec3(size, size, -size), glm::vec3(0, 1, 0), glm::vec2(0, 0)),
        Vertex(glm::vec3(size, size, size), glm::vec3(0, 1, 0), glm::vec2(0, 1)),
        Vertex(glm::vec3(-size, size, size), glm::vec3(0, 1, 0), glm::vec2(1, 1)),
        Vertex(glm::vec3(-size, size, -size), glm::vec3(0, 1, 0), glm::vec2(1, 0)),

        // Bottom
        Vertex(glm::vec3(size, -size, size), glm::vec3(0, -1, 0), glm::vec2(0, 0)),
        Vertex(glm::vec3(size, -size, -size), glm::vec3(0, -1, 0), glm::vec2(0, 1)),
        Vertex(glm::vec3(-size, -size, -size), glm::vec3(0, -1, 0), glm::vec2(1, 1)),
        Vertex(glm::vec3(-size, -size, size), glm::vec3(0, -1, 0), glm::vec2(1, 0)),

    }, {

        1, 0, 3,
        1, 3, 2,

        5, 4, 7,
        5, 7, 6,

        9, 8, 11,
        9, 11, 10,

        13, 12, 15,
        13, 15, 14,

        17, 16, 19,
        17, 19, 18,

        21, 20, 23,
        21, 23, 22,

    });
}

// Simple rect mesh
Mesh Mesh::rect(float xSize, float ySize, float zSize) {
    return Mesh({
        // Back
        Vertex(glm::vec3(-xSize, ySize, -zSize), glm::vec3(0, 0, -1), glm::vec2(0, 0)),
        Vertex(glm::vec3(-xSize, -ySize, -zSize), glm::vec3(0, 0, -1), glm::vec2(0, 1)),
        Vertex(glm::vec3(xSize, -ySize, -zSize), glm::vec3(0, 0, -1), glm::vec2(1, 1)),
        Vertex(glm::vec3(xSize, ySize, -zSize), glm::vec3(0, 0, -1), glm::vec2(1, 0)),

        // Front
        Vertex(glm::vec3(xSize, ySize, zSize), glm::vec3(0, 0, 1), glm::vec2(0, 0)),
        Vertex(glm::vec3(xSize, -ySize, zSize), glm::vec3(0, 0, 1), glm::vec2(0, 1)),
        Vertex(glm::vec3(-xSize, -ySize, zSize), glm::vec3(0, 0, 1), glm::vec2(1, 1)),
        Vertex(glm::vec3(-xSize, ySize, zSize), glm::vec3(0, 0, 1), glm::vec2(1, 0)),

        // Left
        Vertex(glm::vec3(-xSize, ySize, zSize), glm::vec3(-1, 0, 0), glm::vec2(0, 0)),
        Vertex(glm::vec3(-xSize, -ySize, zSize), glm::vec3(-1, 0, 0), glm::vec2(0, 1)),
        Vertex(glm::vec3(-xSize, -ySize, -zSize), glm::vec3(-1, 0, 0), glm::vec2(1, 1)),
        Vertex(glm::vec3(-xSize, ySize, -zSize), glm::vec3(-1, 0, 0), glm::vec2(1, 0)),

        // Right
        Vertex(glm::vec3(xSize, ySize, -zSize), glm::vec3(1, 0, 0), glm::vec2(0, 0)),
        Vertex(glm::vec3(xSize, -ySize, -zSize), glm::vec3(1, 0, 0), glm::vec2(0, 1)),
        Vertex(glm::vec3(xSize, -ySize, zSize), glm::vec3(1, 0, 0), glm::vec2(1, 1)),
        Vertex(glm::vec3(xSize, ySize, zSize), glm::vec3(1, 0, 0), glm::vec2(1, 0)),

        // Top
        Vertex(glm::vec3(xSize, ySize, -zSize), glm::vec3(0, 1, 0), glm::vec2(0, 0)),
        Vertex(glm::vec3(xSize, ySize, zSize), glm::vec3(0, 1, 0), glm::vec2(0, 1)),
        Vertex(glm::vec3(-xSize, ySize, zSize), glm::vec3(0, 1, 0), glm::vec2(1, 1)),
        Vertex(glm::vec3(-xSize, ySize, -zSize), glm::vec3(0, 1, 0), glm::vec2(1, 0)),

        // Bottom
        Vertex(glm::vec3(xSize, -ySize, zSize), glm::vec3(0, -1, 0), glm::vec2(0, 0)),
        Vertex(glm::vec3(xSize, -ySize, -zSize), glm::vec3(0, -1, 0), glm::vec2(0, 1)),
        Vertex(glm::vec3(-xSize, -ySize, -zSize), glm::vec3(0, -1, 0), glm::vec2(1, 1)),
        Vertex(glm::vec3(-xSize, -ySize, zSize), glm::vec3(0, -1, 0), glm::vec2(1, 0)),
    }, {
        1, 0, 3,
        1, 3, 2,

        5, 4, 7,
        5, 7, 6,

        9, 8, 11,
        9, 11, 10,

        13, 12, 15,
        13, 15, 14,

        17, 16, 19,
        17, 19, 18,

        21, 20, 23,
        21, 23, 22,

    });
}

// Mesh constructor
Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices) {
    this->vertices = vertices;
    this->indices = indices;

    load();
}

// Load mesh
void Mesh::load() {

    // Generate buffers
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    // Bind buffers
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Generate vertex data
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // Generate index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // Create vertex attribute arrays
    // Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

    // Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // UVs
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    glBindVertexArray(0);
}

// Draw the mesh
void Mesh::draw() const {
    draw(1);
}

// Draw instanced meshes
void Mesh::draw(int count) const {
    glBindVertexArray(vao);
    glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr, count);
    glBindVertexArray(0);
}

// Default mesh constructor
Mesh::Mesh() : Mesh({}, {}) {}