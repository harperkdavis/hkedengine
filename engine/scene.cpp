//
// Created by harperkdavis on 1/22/2022.
//

#include "scene.h"

#include <utility>

// Create a model matrix
glm::mat4 modelMatrix(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, scale);

    glm::mat4 rot = glm::mat4(1.0f);
    rot = glm::rotate(rot, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    rot = glm::rotate(rot, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    rot = glm::rotate(rot, glm::radians(rotation.z), glm::vec3(0, 0, 1));
    model *= rot;

    model = glm::translate(model, position / abs(scale));
    return model;
}

glm::vec3 rp3dToGlm(rp3d::Vector3 v) {
    return {v.x, v.y, v.z};
}

rp3d::Vector3 glmToRp3d(glm::vec3 v) {
    return {v.x, v.y, v.z};
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
    return glm::perspective(fov, (float) width / (float) height, nearPlane, farPlane);
}

// Camera constructor
Camera::Camera(glm::vec3 position, glm::vec3 rotation, float fov, float nearPlane, float farPlane) {
    this->position = position;
    this->rotation = rotation;
    this->fov = fov;
    this->nearPlane = nearPlane;
    this->farPlane = farPlane;
}

// Draws a thing
void Thing::draw(Shader& shader) const {
    material->texture->use();

    shader.setMat4("model", this->getModelMatrix());

    shader.setVec4("color", material->color);
    shader.setFloat("specular", material->specular);
    shader.setFloat("emission", material->emission);

    mesh.draw();
}

// Gets the raw model matrix of the thing
glm::mat4 StaticThing::getLocalModelMatrix() const {
    return modelMatrix(position, rotation, scale);
}

// Gets the model matrix transformed by parents
glm::mat4 StaticThing::getModelMatrix() const {
    if (parent == nullptr) {
        return getLocalModelMatrix();
    }
    return getLocalModelMatrix() * parent->getModelMatrix();
}

// Constructor for a Static Object
StaticThing::StaticThing(Mesh mesh, Material* material, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
    this->mesh = std::move(mesh);
    this->material = material;
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
}

// Physics thing constructor
PhysicsThing::PhysicsThing(rp3d::PhysicsWorld* world, rp3d::BodyType type, rp3d::CollisionShape* shape, Mesh mesh, Material* material, glm::vec3 position, glm::vec3 rotation) {

    this->mesh = std::move(mesh);
    this->material = material;

    rp3d::Vector3 rpPosition(position.x, position.y, position.z);
    rp3d::Quaternion rpRotation = rp3d::Quaternion::fromEulerAngles(rotation.x, rotation.y, rotation.z);
    rp3d::Transform transform(rpPosition, rpRotation);

    this->rigidbody = world->createRigidBody(transform);
    this->rigidbody->setType(type);

    this->rigidbody->addCollider(shape, rp3d::Transform::identity());

}

glm::mat4 PhysicsThing::getLocalModelMatrix() const {
    float matrix[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    this->getTransform().getOpenGLMatrix(matrix);
    return glm::make_mat4(matrix);
}

glm::mat4 PhysicsThing::getModelMatrix() const {
    return this->getLocalModelMatrix();
}

// Scene constructor // TODO CLEANUP
Scene::Scene() : directionalLight(DirectionalLight(glm::vec3(-0.4f, -1.0f, -0.5f), glm::vec4(1, 1, 1, 1), 0.8f)) {

    auto* material = new Material(new Texture("../resources/skybox_default.png", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP_TO_EDGE));
    material->emission = 1.0f / 256.0f;
    const float farFactor = 0.57735026919f;

    skybox = StaticThing(Mesh::cubemap(4096.0f * farFactor), material, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

    rp3d::PhysicsWorld::WorldSettings settings;
    settings.gravity = rp3d::Vector3(0, -12, 0);

    physicsWorld = physicsCommon.createPhysicsWorld(settings);
}

// Updates the physics
void Scene::update(double timestep) const {
    physicsWorld->update((float) timestep);
}


// Draws the scene! how cool is that!
void Scene::draw(Shader& shader) {
    skybox.setPosition(mainCamera->position);
    skybox.draw(shader);
    for (const Thing* t : things) {
        if (t->isVisible() && t->getMaterial() != nullptr && !t->getMesh().indices.empty()) {
            t->draw(shader);
        }
    }
}

PhysicsThing* Scene::addPhysicsBox(glm::vec3 size, int type, Material *material, glm::vec3 position, glm::vec3 rotation) {
    rp3d::BoxShape* boxShape = physicsCommon.createBoxShape(rp3d::Vector3(size.x, size.y, size.z));
    auto* physBox = new PhysicsThing(physicsWorld, type == 0 ? rp3d::BodyType::STATIC : (type == 1 ? rp3d::BodyType::KINEMATIC : rp3d::BodyType::DYNAMIC), boxShape, Mesh::rect(size.x, size.y, size.z), material, position, rotation);
    things.emplace_back(physBox);
    return physBox;
}

PhysicsThing* Scene::addPhysicsSphere(float radius, int type, Material *material, glm::vec3 position, glm::vec3 rotation) {
    rp3d::SphereShape* sphereShape = physicsCommon.createSphereShape(radius);
    auto* physSphere = new PhysicsThing(physicsWorld, type == 0 ? rp3d::BodyType::STATIC : (type == 1 ? rp3d::BodyType::KINEMATIC : rp3d::BodyType::DYNAMIC), sphereShape, Mesh::load("../resources/models/uvsphere.obj", glm::vec3(1) * radius), material, position, rotation);
    things.emplace_back(physSphere);
    return physSphere;
}
