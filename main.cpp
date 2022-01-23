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
#include "dep/stb_image.h"

const int WIDTH = 1200;
const int HEIGHT = 800;

const int SSAO_KERNEL_SIZE = 64;

using namespace std;

const float MOUSE_SENSITIVITY = 0.04f;


// The main function
int main() {

    // Start the rendering pipeline!
    Pipeline::initialize();

    // Set the scene
    Camera playerCamera = Camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 80.0f, 0.1f, 1000.0f);

    // Create the main scene
    Scene scene = Scene();
    scene.camera = &playerCamera;

    Pipeline::scene = &scene;

    Material mat = Material(new Texture("../resources/test.png"));
    Material mat2 = Material(new Texture("../resources/test.png"));
    mat2.emission = 0.5f;
    scene.add(Thing(Mesh::cube(1.0f), mat, glm::vec3(0, -1, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));
    scene.add(Thing(Mesh::cube(2.0f), mat2, glm::vec3(4, -1, 3), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));
    Thing& square = scene.add(Thing(Mesh::cube(0.5f), mat, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));

    double deltaTime = 0;
    int frameCount = 0, lastSecond = 0;
    // Game loop
    while(!Pipeline::windowShouldClose()) {
        double startFrame = glfwGetTime();

        // Update framerate
        if (floor(glfwGetTime()) > lastSecond) {
            lastSecond = floor(glfwGetTime());
            string winTitle = "HKED Engine 0.1.0-alpha.6 (FPS: " + to_string(frameCount) + ")";
            Pipeline::setTitle(winTitle);
            frameCount = 0;
        } else {
            frameCount++;
        }

        // Process player camera movement

        playerCamera.rotation.x += -Input::getAxisY() * MOUSE_SENSITIVITY;
        playerCamera.rotation.y += -Input::getAxisX() * MOUSE_SENSITIVITY;

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
            playerCamera.position += glm::vec3(sin(-glm::radians(rotY)), 0, cos(-glm::radians(rotY))) * (float) deltaTime * playerSpeed;
        }
        if (Input::key(GLFW_KEY_S)) {
            playerCamera.position -= glm::vec3(sin(-glm::radians(rotY)), 0, cos(-glm::radians(rotY))) * (float) deltaTime * playerSpeed;
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

        // vstd::cout << playerCamera.position.x << ", " << playerCamera.position.y << ", " << playerCamera.position.z << std::endl;


        square.rotation.y = glfwGetTime() * 64;

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