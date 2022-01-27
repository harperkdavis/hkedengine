//
// Created by harperkdavis on 1/22/2022.
//

#include "scene.h"

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

// Generates a view matrix
glm::mat4 Camera::viewMatrix() const {
    return modelMatrix(position * -1.0f, rotation, glm::vec3(-1, -1, 1));
}

glm::mat4 Camera::rotationMatrix() const {
    return modelMatrix(glm::vec3(0), rotation, glm::vec3(-1, -1, 1));
}

// Generates a projection matrix
glm::mat4 Camera::projectionMatrix(int width, int height) const {
    return glm::perspective(fov, (float) width / height, nearPlane, farPlane);
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
void Thing::draw(Shader& shader) const {
    material->texture->use();

    shader.setMat4("model", getModelMatrix());

    shader.setVec4("color", material->color);
    shader.setFloat("specular", material->specular);
    shader.setFloat("emission", material->emission);

    mesh.draw();
}

// Thing maker invents the thing
Thing::Thing(Mesh mesh, Material* material, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
    this->mesh = mesh;
    this->material = material;
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
}

Thing::Thing() {

}

Scene::Scene() : dirLight(DirectionalLight(glm::vec3(-0.4f, -1.0f, -0.5f), glm::vec4(1, 1, 1, 1), 0.9f)) {
    auto* material = new Material(new Texture("../resources/skybox.png", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP_TO_EDGE));
    material->emission = 1.0f / 256.0f;
    const float farFactor = 0.57735026919f;
    skybox = Thing(Mesh::cubemap(4096.0f * farFactor), material, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
}

void Scene::draw(Shader& shader) {
    skybox.position = camera->position;
    skybox.draw(shader);
    for (const Thing& t : things) {
        if (t.visible && t.material != nullptr && !t.mesh.indices.empty()) {
            t.draw(shader);
        }
    }
}