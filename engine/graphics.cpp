//
// Created by harperkdavis on 1/18/2022.
//

#include "graphics.h"
#include "../dep/stb_image.h"

// Create a model matrix
glm::mat4 modelMatrix(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, scale);

    glm::mat4 rot = glm::mat4(1.0f);
    rot = glm::rotate(rot, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    rot = glm::rotate(rot, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    rot = glm::rotate(rot, glm::radians(rotation.z), glm::vec3(0, 0, 1));
    model *= rot;

    model = glm::translate(model, position);
    return model;
}

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
        Vertex(glm::vec3(size, -size, size), glm::vec3(-1, -1, 0), glm::vec2(0, 0)),
        Vertex(glm::vec3(size, -size, -size), glm::vec3(-1, -1, 0), glm::vec2(0, 1)),
        Vertex(glm::vec3(-size, -size, -size), glm::vec3(-1, -1, 0), glm::vec2(1, 1)),
        Vertex(glm::vec3(-size, -size, size), glm::vec3(-1, -1, 0), glm::vec2(1, 0)),

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
        Vertex(glm::vec3(xSize, -ySize, zSize), glm::vec3(-1, -1, 0), glm::vec2(0, 0)),
        Vertex(glm::vec3(xSize, -ySize, -zSize), glm::vec3(-1, -1, 0), glm::vec2(0, 1)),
        Vertex(glm::vec3(-xSize, -ySize, -zSize), glm::vec3(-1, -1, 0), glm::vec2(1, 1)),
        Vertex(glm::vec3(-xSize, -ySize, zSize), glm::vec3(-1, -1, 0), glm::vec2(1, 0)),
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

// Default texture
Texture::Texture() : Texture("../resources/default.png") {}

// Creates a texture by path
Texture::Texture(string path) {
    if (path.empty()) {
        path = "../resources/default.png";
    }
    data = stbi_load(&path[0], &width, &height, &channels, 0);

    load();
}

// Loads a texture
void Texture::load() {
    glGenTextures(1, &id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
}

// Binds a texture
void Texture::use() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id);
}

// Basic material with just a shader to its name
Material::Material(Shader& shader) : Material(shader, "", glm::vec4(1, 1, 1, 1)) {}

// Basic material with a texture
Material::Material(Shader& shader, string texture) : Material(shader, texture, glm::vec4(1, 1, 1, 1)) {}

// Material with texture and color
Material::Material(Shader& shader, string texture, glm::vec4 color) : shader(shader) {
    this->shader = shader;
    this->texture = Texture(texture);
    this->color = color;
}

// Generates a view matrix
glm::mat4 Camera::viewMatrix() const {
    return modelMatrix(position, rotation, glm::vec3(1, 1, 1));
}

// Generates a projection matrix
glm::mat4 Camera::projectionMatrix() const {
    return glm::perspective(fov, 1200.0f / 800.0f, nearPlane, farPlane);
}

// Camera constructor
Camera::Camera(glm::vec3 position, glm::vec3 rotation, float fov, float nearPlane, float farPlane) {
    this->position = position;
    this->rotation = rotation;
    this->fov = fov;
    this->nearPlane = nearPlane;
    this->farPlane = farPlane;
}

// Gets the raw model matrix of the thing
glm::mat4 Thing::getLocalModelMatrix() const {
    return modelMatrix(position, rotation, scale);
}

// Gets the model matrix transformed by parents
glm::mat4 Thing::getModelMatrix() const {
    if (parent == nullptr) {
        return getLocalModelMatrix();
    }
    return getLocalModelMatrix() * parent->getModelMatrix();
}

// Draws a thing
void Thing::draw() {
    if (Camera::mainCamera == nullptr) {
        return;
    }
    material.texture.use();
    material.shader.use();

    material.shader.setMat4("model", getModelMatrix());
    material.shader.setMat4("projection", Camera::mainCamera->projectionMatrix());
    material.shader.setMat4("view", Camera::mainCamera->viewMatrix());

    material.shader.setVec4("color", material.color);

    mesh.draw();
}

// Thing maker invents the thing
Thing::Thing(Mesh mesh, Material& material, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : material(material) {
    this->mesh = mesh;
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
}

Scene::Scene() : dirLight(DirectionalLight(glm::vec3(-0.2f, 1.0f, -0.7f), glm::vec4(1, 1, 1, 1), 1)) {}

void Scene::draw() {
    for (Thing t : things) {
        if (t.visible) {
            t.draw();
        }
    }
}
