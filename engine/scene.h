//
// Created by harperkdavis on 1/22/2022.
//

#ifndef HKEDENGINE_SCENE_H
#define HKEDENGINE_SCENE_H

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "mesh.h"
#include "material.h"

#include "../dep/stb_image.h"
#include "shader.h"


using namespace std;

// Create a model matrix
glm::mat4 modelMatrix(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

struct DirectionalLight {
public:
    glm::vec3 direction;
    glm::vec4 color;
    float intensity;

    DirectionalLight(glm::vec3 direction, glm::vec4 color, float intensity) {
        this->direction = glm::normalize(direction);
        this->color = color;
        this->intensity = intensity;
    }
};

struct PointLight {
public:
    glm::vec3 position;
    glm::vec4 color;
    float intensity;

    PointLight() : PointLight(glm::vec3(0, 0, 0), glm::vec4(0, 0, 0, 0), 0) {}

    PointLight(glm::vec3 position, glm::vec4 color, float intensity) {
        this->position = position;
        this->color = color;
        this->intensity = intensity;
    }
};

// Camera class
class Camera {
public:

    glm::vec3 position;
    glm::vec3 rotation;
    float fov, nearPlane, farPlane;

    glm::mat4 viewMatrix() const;
    glm::mat4 projectionMatrix(int width, int height) const;

    Camera(glm::vec3 position, glm::vec3 rotation, float fov, float nearPlane, float farPlane);

};

class Thing {
public:

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    bool visible = true;

    Thing* parent = nullptr;

    Mesh mesh;
    Material& material;

    glm::mat4 getLocalModelMatrix() const;
    glm::mat4 getModelMatrix() const;

    void draw(Shader& shader) const;

    Thing(Mesh mesh, Material& material, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

};

// Class for a scene tree
class Scene {
public:

    Camera* camera;
    vector<Thing> things;

    glm::vec4 ambientLight = glm::vec4(0.05f, 0.05f, 0.05f, 1.0f);
    DirectionalLight dirLight;
    PointLight pointLights[64];

    Thing& add(Thing thing) {
        return things.emplace_back(thing);
    }

    void draw(Shader& shader);

    Scene();

};

#endif //HKEDENGINE_SCENE_H
