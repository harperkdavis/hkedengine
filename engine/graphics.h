//
// Created by harperkdavis on 1/18/2022.
//
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "shader.h"

#ifndef HKEDENGINE_MESH_H
#define HKEDENGINE_MESH_H

using namespace std;

// Create a model matrix
glm::mat4 modelMatrix(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

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
    static Mesh rect(float sizex, float sizey, float sizez);

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

// Class for storing texture information
class Texture {
public:

    int width, height, channels;
    unsigned char* data;

    void use() const;

    Texture();
    explicit Texture(string path);

    unsigned int id;
private:

    void load();
};

class Material {
public:

    Shader& shader;
    Texture texture;
    glm::vec4 color;

    Material(Shader& shader);
    Material(Shader& shader, string texture);
    Material(Shader& shader, string texture, glm::vec4 color);
};

class Camera {
public:
    inline static Camera* mainCamera = nullptr;
    glm::vec3 position;
    glm::vec3 rotation;
    float fov, nearPlane, farPlane;

    glm::mat4 viewMatrix() const;
    glm::mat4 projectionMatrix() const;

    Camera(glm::vec3 position, glm::vec3 rotation, float fov, float nearPlane, float farPlane);

};

class Thing {
public:

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    Thing* parent = nullptr;

    Mesh mesh;
    Material& material;

    glm::mat4 getLocalModelMatrix() const;
    glm::mat4 getModelMatrix() const;

    void draw();

    Thing(Mesh mesh, Material& material, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

};

#endif //HKEDENGINE_MESH_H
