//
// Created by harperkdavis on 1/22/2022.
//

#include "mesh.h"

// Simple cube mesh
Mesh Mesh::cube(float size) {
    return Mesh({
        // Back
        Vertex(glm::vec3(size, -size, -size), glm::vec3(0, 0, -1), glm::vec2(0, 0)),
        Vertex(glm::vec3(size, size, -size), glm::vec3(0, 0, -1), glm::vec2(0, 1)),
        Vertex(glm::vec3(-size, size, -size), glm::vec3(0, 0, -1), glm::vec2(1, 1)),
        Vertex(glm::vec3(-size, -size, -size), glm::vec3(0, 0, -1), glm::vec2(1, 0)),

        // Front
        Vertex(glm::vec3(-size, -size, size), glm::vec3(0, 0, 1), glm::vec2(0, 0)),
        Vertex(glm::vec3(-size, size, size), glm::vec3(0, 0, 1), glm::vec2(0, 1)),
        Vertex(glm::vec3(size, size, size), glm::vec3(0, 0, 1), glm::vec2(1, 1)),
        Vertex(glm::vec3(size, -size, size), glm::vec3(0, 0, 1), glm::vec2(1, 0)),

        // Left
        Vertex(glm::vec3(-size, -size, -size), glm::vec3(-1, 0, 0), glm::vec2(0, 0)),
        Vertex(glm::vec3(-size, size, -size), glm::vec3(-1, 0, 0), glm::vec2(0, 1)),
        Vertex(glm::vec3(-size, size, size), glm::vec3(-1, 0, 0), glm::vec2(1, 1)),
        Vertex(glm::vec3(-size, -size, size), glm::vec3(-1, 0, 0), glm::vec2(1, 0)),

        // Right
        Vertex(glm::vec3(size, -size, size), glm::vec3(1, 0, 0), glm::vec2(0, 0)),
        Vertex(glm::vec3(size, size, size), glm::vec3(1, 0, 0), glm::vec2(0, 1)),
        Vertex(glm::vec3(size, size, -size), glm::vec3(1, 0, 0), glm::vec2(1, 1)),
        Vertex(glm::vec3(size, -size, -size), glm::vec3(1, 0, 0), glm::vec2(1, 0)),

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

// Simple cube mesh
Mesh Mesh::cubemap(float size) {
    const float ONE_THIRD = 1.0f / 3.0f;
    const float ONE_FOURTH = 1.0f / 4.0f;
    return Mesh({
        // Back
        Vertex(glm::vec3(size, -size, -size), glm::vec3(0, 0, -1), glm::vec2(3 * ONE_FOURTH, 1 * ONE_THIRD)),
        Vertex(glm::vec3(size, size, -size), glm::vec3(0, 0, -1), glm::vec2(3 * ONE_FOURTH, 2 * ONE_THIRD)),
        Vertex(glm::vec3(-size, size, -size), glm::vec3(0, 0, -1), glm::vec2(4 * ONE_FOURTH, 2 * ONE_THIRD)),
        Vertex(glm::vec3(-size, -size, -size), glm::vec3(0, 0, -1), glm::vec2(4 * ONE_FOURTH, 1 * ONE_THIRD)),

        // Front
        Vertex(glm::vec3(-size, -size, size), glm::vec3(0, 0, 1), glm::vec2(1 * ONE_FOURTH, 1 * ONE_THIRD)),
        Vertex(glm::vec3(-size, size, size), glm::vec3(0, 0, 1), glm::vec2(1 * ONE_FOURTH, 2 * ONE_THIRD)),
        Vertex(glm::vec3(size, size, size), glm::vec3(0, 0, 1), glm::vec2(2 * ONE_FOURTH, 2 * ONE_THIRD)),
        Vertex(glm::vec3(size, -size, size), glm::vec3(0, 0, 1), glm::vec2(2 * ONE_FOURTH, 1 * ONE_THIRD)),

        // Left
        Vertex(glm::vec3(-size, -size, -size), glm::vec3(-1, 0, 0), glm::vec2(0 * ONE_FOURTH, 1 * ONE_THIRD)),
        Vertex(glm::vec3(-size, size, -size), glm::vec3(-1, 0, 0), glm::vec2(0 * ONE_FOURTH, 2 * ONE_THIRD)),
        Vertex(glm::vec3(-size, size, size), glm::vec3(-1, 0, 0), glm::vec2(1 * ONE_FOURTH, 2 * ONE_THIRD)),
        Vertex(glm::vec3(-size, -size, size), glm::vec3(-1, 0, 0), glm::vec2(1 * ONE_FOURTH, 1 * ONE_THIRD)),

        // Right
        Vertex(glm::vec3(size, -size, size), glm::vec3(1, 0, 0), glm::vec2(2 * ONE_FOURTH, 1 * ONE_THIRD)),
        Vertex(glm::vec3(size, size, size), glm::vec3(1, 0, 0), glm::vec2(2 * ONE_FOURTH, 2 * ONE_THIRD)),
        Vertex(glm::vec3(size, size, -size), glm::vec3(1, 0, 0), glm::vec2(3 * ONE_FOURTH, 2 * ONE_THIRD)),
        Vertex(glm::vec3(size, -size, -size), glm::vec3(1, 0, 0), glm::vec2(3 * ONE_FOURTH, 1 * ONE_THIRD)),

        // Top
        Vertex(glm::vec3(size, size, -size), glm::vec3(0, 1, 0), glm::vec2(2 * ONE_FOURTH, 3 * ONE_THIRD)),
        Vertex(glm::vec3(size, size, size), glm::vec3(0, 1, 0), glm::vec2(2 * ONE_FOURTH, 2 * ONE_THIRD)),
        Vertex(glm::vec3(-size, size, size), glm::vec3(0, 1, 0), glm::vec2(1 * ONE_FOURTH, 2 * ONE_THIRD)),
        Vertex(glm::vec3(-size, size, -size), glm::vec3(0, 1, 0), glm::vec2(1 * ONE_FOURTH, 3 * ONE_THIRD)),

        // Bottom
        Vertex(glm::vec3(size, -size, size), glm::vec3(0, -1, 0), glm::vec2(2 * ONE_FOURTH, 1 * ONE_THIRD)),
        Vertex(glm::vec3(size, -size, -size), glm::vec3(0, -1, 0), glm::vec2(2 * ONE_FOURTH, 0 * ONE_THIRD)),
        Vertex(glm::vec3(-size, -size, -size), glm::vec3(0, -1, 0), glm::vec2(1 * ONE_FOURTH, 0 * ONE_THIRD)),
        Vertex(glm::vec3(-size, -size, size), glm::vec3(0, -1, 0), glm::vec2(1 * ONE_FOURTH, 1 * ONE_THIRD)),

    }, {

        1, 3, 0,
        1, 2, 3,

        5, 7, 4,
        5, 6, 7,

        9, 11, 8,
        9, 10, 11,

        13, 15, 12,
        13, 14, 15,

        17, 19, 16,
        17, 18, 19,

        21, 23, 20,
        21, 22, 23,

    });
}

// Simple rect mesh
Mesh Mesh::rect(float xSize, float ySize, float zSize) {
    return Mesh({
        // Back
        Vertex(glm::vec3(xSize, -ySize, -zSize), glm::vec3(0, 0, -1), glm::vec2(0, 0)),
        Vertex(glm::vec3(xSize, ySize, -zSize), glm::vec3(0, 0, -1), glm::vec2(0, 1)),
        Vertex(glm::vec3(-xSize, ySize, -zSize), glm::vec3(0, 0, -1), glm::vec2(1, 1)),
        Vertex(glm::vec3(-xSize, -ySize, -zSize), glm::vec3(0, 0, -1), glm::vec2(1, 0)),

        // Front
        Vertex(glm::vec3(-xSize, -ySize, zSize), glm::vec3(0, 0, 1), glm::vec2(0, 0)),
        Vertex(glm::vec3(-xSize, ySize, zSize), glm::vec3(0, 0, 1), glm::vec2(0, 1)),
        Vertex(glm::vec3(xSize, ySize, zSize), glm::vec3(0, 0, 1), glm::vec2(1, 1)),
        Vertex(glm::vec3(xSize, -ySize, zSize), glm::vec3(0, 0, 1), glm::vec2(1, 0)),

        // Left
        Vertex(glm::vec3(-xSize, -ySize, -zSize), glm::vec3(-1, 0, 0), glm::vec2(0, 0)),
        Vertex(glm::vec3(-xSize, ySize, -zSize), glm::vec3(-1, 0, 0), glm::vec2(0, 1)),
        Vertex(glm::vec3(-xSize, ySize, zSize), glm::vec3(-1, 0, 0), glm::vec2(1, 1)),
        Vertex(glm::vec3(-xSize, -ySize, zSize), glm::vec3(-1, 0, 0), glm::vec2(1, 0)),

        // Right
        Vertex(glm::vec3(xSize, -ySize, zSize), glm::vec3(1, 0, 0), glm::vec2(0, 0)),
        Vertex(glm::vec3(xSize, ySize, zSize), glm::vec3(1, 0, 0), glm::vec2(0, 1)),
        Vertex(glm::vec3(xSize, ySize, -zSize), glm::vec3(1, 0, 0), glm::vec2(1, 1)),
        Vertex(glm::vec3(xSize, -ySize, -zSize), glm::vec3(1, 0, 0), glm::vec2(1, 0)),

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

// Load models using assimp
Mesh Mesh::load(string path) {
    return Mesh::load(path, glm::vec3(1, 1, 1));
}

// Load models using assimp
Mesh Mesh::load(string path, glm::vec3 scale) {
    objl::Loader loader;

    loader.LoadFile(path);

    vector<Vertex> vertices;
    vector<unsigned int> indices;

    for (objl::Vertex v : loader.LoadedMeshes[0].Vertices) {
        glm::vec3 position = glm::vec3(v.Position.X * scale.x, v.Position.Y * scale.y, v.Position.Z * scale.z);
        glm::vec3 normal = glm::vec3(v.Normal.X, v.Normal.Y, v.Normal.Z);
        glm::vec2 uv = glm::vec2(v.TextureCoordinate.X, v.TextureCoordinate.Y);
        Vertex newVertex = Vertex(position, normal, uv);

        vertices.emplace_back(newVertex);
    }

    for (unsigned int i : loader.LoadedMeshes[0].Indices) {
        indices.emplace_back(i);
    }

    return Mesh(vertices, indices);
}

// Mesh constructor
Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices) {
    this->vertices = std::move(vertices);
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
