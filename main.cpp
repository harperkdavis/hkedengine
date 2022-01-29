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

#define STB_IMAGE_IMPLEMENTATION
#include "engine/dep/stb_image.h"

using namespace std;

const float MOUSE_SENSITIVITY = 0.04f;


// The main function
int main() {
    // Start the rendering pipeline!
    Pipeline::initialize();

    // Set the scene
    Camera playerCamera = Camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 80.0f, 0.1f, 8192.0f);

    // Create the main scene
    Scene scene = Scene();
    glm::vec3 dirLightDir = glm::normalize(glm::vec3(-0.2f, -0.6f, -0.7f));
    scene.dirLight.direction = dirLightDir;

    scene.camera = &playerCamera;
    scene.pointLights[0] = PointLight(glm::vec3(0, 1, 0), glm::vec4(1, 1, 1, 1), 1);
    scene.pointLights[1] = PointLight(glm::vec3(0, 4, -10), glm::vec4(1, 1, 1, 1), 4);
    Pipeline::scene = &scene;

    Material mat = Material(new Texture("../resources/test.png"));
    Material glowy = Material(new Texture("../resources/star.png"));
    glowy.emission = 1.0f;
    mat.specular = 0.2;

    scene.add(Thing(Mesh::cube(1.0f), &mat, glm::vec3(0, -10000, 0), glm::vec3(0, 0, 0), glm::vec3(10, 1, 10)));
    scene.add(Thing(Mesh::load("../resources/models/smallarea.obj"), &mat, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));
    scene.add(Thing(Mesh::load("../resources/models/uvsphere.obj"), &glowy, -dirLightDir * 2000.0f, glm::vec3(0, 0, 0), glm::vec3(200, 200, 200)));

    double deltaTime = 0;
    int frameCount = 0, lastSecond = 0;
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

        // Process player camera movement
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

        updateFrame -= glfwGetTime();

        double preRender = glfwGetTime();
        Pipeline::preRender();
        Pipeline::geometryPass();
        Pipeline::lightingPass();

        Input::updateInput();
        Pipeline::glUpdate();

        deltaTime = glfwGetTime() - startFrame;
    }

    Pipeline::shutdown();
    return 0;
}