#include <iostream>
#include <string>
#include <random>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/scene.h"
#include "engine/pipeline.h"
#include "engine/mesh.h"
#include "engine/material.h"
#include "engine/input.h"
#include "engine/ui.h"

#define STB_IMAGE_IMPLEMENTATION
#include "engine/dep/stb_image.h"

using namespace std;

const float MOUSE_SENSITIVITY = 0.04f;


// The main function
int main() {
    // Start the rendering pipeline!
    Pipeline::initialize();
    UI::initialize(Pipeline::WIDTH, Pipeline::HEIGHT);

    Font font = Font("../resources/fonts/helvetica.fnt", "../resources/fonts/helvetica_0.png");

    // Set the scene
    Camera playerCamera = Camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 80.0f, 0.1f, 8192.0f);

    // Create the main scene
    Scene scene = Scene();
    glm::vec3 dirLightDir = glm::normalize(glm::vec3(-0.2f, -0.6f, -0.7f));
    scene.directionalLight.direction = dirLightDir;

    scene.mainCamera = &playerCamera;
    scene.pointLights[0] = PointLight(glm::vec3(0, 1, 0), glm::vec4(1, 1, 1, 1), 1);
    scene.pointLights[1] = PointLight(glm::vec3(0, 4, -10), glm::vec4(1, 1, 1, 1), 4);
    Pipeline::scene = &scene;

    Material mat = Material(new Texture("../resources/test.png"));
    Material glowy = Material(new Texture("../resources/star.png"));
    glowy.emission = 1.0f;
    mat.specular = 0.2;

    scene.addPhysicsBox(glm::vec3(20, 1, 20), 0, &mat, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
    scene.addPhysicsSphere(1, 2, &mat, glm::vec3(4, 5, 0), glm::vec3(0, 0, 0));

    // sun
    scene.add(new StaticThing(Mesh::load("../resources/models/uvsphere.obj"), &glowy, -dirLightDir * 2000.0f, glm::vec3(0, 0, 0), glm::vec3(200, 200, 200)));

    Material crosshairMaterial = Material(new Texture("../resources/crosshair.png"));
    UiThing crosshair = UiThing(UI::uiCenter(glm::vec2(32.0f)), &crosshairMaterial, glm::vec3((float) Pipeline::WIDTH / 2, (float) Pipeline::HEIGHT / 2, 0.0f), glm::vec3(0), glm::vec3(1));

    UiThing text = UiThing(font.getTextMesh("Among us balls", 1.0f), font.fontMaterial, glm::vec3(400.0f, 400.0f, 0.0f), glm::vec3(0), glm::vec3(1));


    double deltaTime = 0;
    int frameCount = 0, lastSecond = 0;
    Shader shader = Shader("../shaders/quadv.glsl", "../shaders/quadf.glsl");
    // Game loop
    while(!Pipeline::windowShouldClose()) {
        double startFrame = glfwGetTime();
        // Update framerate
        if (floor(glfwGetTime()) > lastSecond) {
            lastSecond = floor(glfwGetTime());
            string winTitle = "HKED Engine 0.1.0-alpha.8 (FPS: " + to_string(frameCount) + ")";
            Pipeline::setTitle(winTitle);
            frameCount = 0;
        } else {
            frameCount++;
        }

        scene.update(deltaTime);

        // Process player mainCamera movement
        double updateFrame = glfwGetTime();

        playerCamera.rotation.x += Input::getAxisY() * MOUSE_SENSITIVITY;
        playerCamera.rotation.y += Input::getAxisX() * MOUSE_SENSITIVITY;

        if (playerCamera.rotation.x > 89) {
            playerCamera.rotation.x = 89;
        }
        if (playerCamera.rotation.x < -89) {
            playerCamera.rotation.x = -89;
        }


        float& rotY = playerCamera.rotation.y;
        float playerSpeed = 4.0f * (Input::key(GLFW_KEY_LEFT_SHIFT) ? 4.0f : (Input::key(GLFW_KEY_LEFT_ALT) ? 0.05f : 1.0f));

        // Handle basic player movement
        if (Input::key(GLFW_KEY_W)) {
            playerCamera.position -= glm::vec3(sin(-glm::radians(rotY)), 0, cos(-glm::radians(rotY))) * (float) deltaTime * playerSpeed;
        }
        if (Input::key(GLFW_KEY_S)) {
            playerCamera.position += glm::vec3(sin(-glm::radians(rotY)), 0, cos(-glm::radians(rotY))) * (float) deltaTime * playerSpeed;
        }
        if (Input::key(GLFW_KEY_D)) {
            playerCamera.position += glm::vec3(sin(-glm::radians(rotY) + M_PI / 2), 0, cos(-glm::radians(rotY) + M_PI / 2)) * (float) deltaTime * playerSpeed;
        }
        if (Input::key(GLFW_KEY_A)) {
            playerCamera.position -= glm::vec3(sin(-glm::radians(rotY) + M_PI / 2), 0, cos(-glm::radians(rotY) + M_PI / 2)) * (float) deltaTime * playerSpeed;
        }
        if (Input::key(GLFW_KEY_SPACE)) {
            playerCamera.position += glm::vec3(0, 1, 0) * (float) deltaTime * playerSpeed;
        }
        if (Input::key(GLFW_KEY_LEFT_CONTROL)) {
            playerCamera.position -= glm::vec3(0, 1, 0) * (float) deltaTime * playerSpeed;
        }

        if (Input::mouseDown(GLFW_MOUSE_BUTTON_1)) {
            scene.addPhysicsBox(glm::vec3(1, 1, 1), 2, &mat, playerCamera.position - glm::vec3(0, 2, 0), glm::vec3(0, 0, 0));
        }
        if (Input::mouseDown(GLFW_MOUSE_BUTTON_2)) {
            glm::vec4 forward = glm::vec4(0, 0, -1, 1);
            forward = forward * playerCamera.rotationMatrix();
            glm::vec3 force = glm::vec3(forward.x, forward.y, forward.z);
            PhysicsThing* thing = scene.addPhysicsSphere(0.5f, 2, &mat, playerCamera.position + force, glm::vec3(0, 0, 0));
            thing->setVelocity(force * 40.0f);
        }
        if (Input::mouse(GLFW_MOUSE_BUTTON_3)) {
            scene.addPhysicsBox(glm::vec3(0.5f, 0.5f, 0.5f), 2, &mat, playerCamera.position - glm::vec3(0, 2, 0), glm::vec3(0, 0, 0));
        }


        updateFrame -= glfwGetTime();

        double preRender = glfwGetTime();
        Pipeline::preRender();
        Pipeline::geometryPass();
        Pipeline::lightingPass();

        UI::startPass();

        crosshair.draw(*UI::uiShader);
        text.draw(*UI::uiShader);

        text.replaceMesh(font.getTextMesh("Current Time: " + to_string(glfwGetTime()), 1));

        UI::endPass();

        Input::updateInput();
        Pipeline::glUpdate();

        deltaTime = glfwGetTime() - startFrame;
    }

    Pipeline::shutdown();
    return 0;
}