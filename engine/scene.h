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

#include <spdlog/spdlog.h>
#include <reactphysics3d/reactphysics3d.h>

#include "mesh.h"
#include "material.h"

#include "dep/stb_image.h"
#include "shader.h"


using namespace std;

// Converts a rp3d vector to a glm vector
glm::vec3 rp3dToGlm(rp3d::Vector3 v);

// Converts a glm vector to a rp3d vector
rp3d::Vector3 glmToRp3d(glm::vec3 v);

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

    [[nodiscard]] glm::mat4 viewMatrix() const;
    [[nodiscard]] glm::mat4 rotationMatrix() const;
    [[nodiscard]] glm::mat4 projectionMatrix(int width, int height) const;

    Camera(glm::vec3 position, glm::vec3 rotation, float fov, float nearPlane, float farPlane);

};

class Thing {
public:

    virtual glm::vec3 getPosition() = 0;
    virtual glm::vec3 getRotation() = 0;
    virtual glm::vec3 getScale() = 0;

    virtual void setPosition(glm::vec3 position) = 0;
    virtual void setRotation(glm::vec3 rotation) = 0;
    virtual void setScale(glm::vec3 scale) = 0;

    [[nodiscard]] bool isVisible() const {
        return visible;
    }
    void setVisible(bool vis) {
        this->visible = vis;
    }

    [[nodiscard]] Material* getMaterial() const {
        return material;
    }

    [[nodiscard]] Mesh getMesh() const {
        return mesh;
    }

    [[nodiscard]] virtual glm::mat4 getLocalModelMatrix() const = 0;
    [[nodiscard]] virtual glm::mat4 getModelMatrix() const = 0;

    void draw(Shader& shader) const;

protected:

    Mesh mesh;
    Material* material = nullptr;

    bool visible = true;

    vector<Thing> children;

    Thing* parent = nullptr;
};

class StaticThing : public Thing {
public:
    glm::vec3 getPosition() override {
        return position;
    }
    glm::vec3 getRotation() override {
        return rotation;
    }
    glm::vec3 getScale() override {
        return scale;
    }

    void setPosition(glm::vec3 pos) override {
        this->position = pos;
    }
    void setRotation(glm::vec3 rot) override {
        this->rotation = rot;
    }
    void setScale(glm::vec3 sca) override {
        this->scale = sca;
    }

    [[nodiscard]] glm::mat4 getLocalModelMatrix() const override;
    [[nodiscard]] glm::mat4 getModelMatrix() const override;

    StaticThing(Mesh mesh, Material* material, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    StaticThing() = default;

private:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

class PhysicsThing : public Thing {
public:

    [[nodiscard]] rp3d::Transform getTransform() const {
        return rigidbody->getTransform();
    }

    glm::vec3 getPosition() override {
        return rp3dToGlm(getTransform().getPosition());
    }

    glm::vec3 getRotation() override {
        return rp3dToGlm(getTransform().getOrientation().getVectorV());
    }

    glm::vec3 getScale() override {
        return {1, 1, 1};
    }

    void setPosition(glm::vec3 pos) override {
        getTransform().setPosition(glmToRp3d(pos));
    }

    void setRotation(glm::vec3 rot) override{
        getTransform().setOrientation(rp3d::Quaternion::fromEulerAngles(glmToRp3d(rot)));
    }

    void setScale(glm::vec3 sca) override {
        // lmao doesn't do sh*t (get f*ked loser, imagine actually trying to scale a physics thing! haha, classic loser mistake :joy: I am literally dying on the floor right now. please call an ambulance. im starting to see the light. is this it?)
    }

    [[nodiscard]] glm::mat4 getLocalModelMatrix() const override;
    [[nodiscard]] glm::mat4 getModelMatrix() const override;

    glm::vec3 getVelocity() {
        return rp3dToGlm(rigidbody->getLinearVelocity());
    }

    void setVelocity(glm::vec3 velocity) {
        rigidbody->setLinearVelocity(glmToRp3d(velocity));
    }

    glm::vec3 getAngularVelocity() {
        return rp3dToGlm(rigidbody->getAngularVelocity());
    }

    void setAngularVelocity(glm::vec3 velocity) {
        rigidbody->setAngularVelocity(glmToRp3d(velocity));
    }

    void addForce(glm::vec3 force) {
        rigidbody->applyLocalForceAtCenterOfMass(glmToRp3d(force));
    }

    PhysicsThing(rp3d::PhysicsWorld* world, rp3d::BodyType type, rp3d::CollisionShape* shape, Mesh mesh, Material* material, glm::vec3 position, glm::vec3 rotation);

protected:
    rp3d::RigidBody* rigidbody;
};

// Class for a scene tree
class Scene {
public:

    Camera* mainCamera;

    StaticThing skybox;
    vector<Thing*> things;

    glm::vec4 ambientLight = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    DirectionalLight directionalLight;
    PointLight pointLights[64];

    rp3d::PhysicsCommon physicsCommon;
    rp3d::PhysicsWorld* physicsWorld;

    Thing* add(Thing* thing) {
        things.emplace_back(thing);
        return thing;
    }

    PhysicsThing* addPhysicsBox(glm::vec3 size, int type, Material* material, glm::vec3 position, glm::vec3 rotation);
    PhysicsThing* addPhysicsSphere(float radius, int type, Material* material, glm::vec3 position, glm::vec3 rotation);

    void update(double timestep) const;
    void draw(Shader& shader);

    Scene();

};

#endif //HKEDENGINE_SCENE_H
