#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/graphics.h"

#define STB_IMAGE_IMPLEMENTATION
#include "dep/stb_image.h"

#define MAJOR_VERSION 0
#define MINOR_VERSION 1
#define PATCH_VERSION 0
#define DEV_VERSION 0
#define DEV_PHASE "alpha"

const int WIDTH = 1200;
const int HEIGHT = 800;

using namespace std;

// Get a string indicating the current version
string GET_VERSION() {
    return to_string(MAJOR_VERSION) + "." + to_string(MINOR_VERSION) + "." + to_string(PATCH_VERSION) + "-" + DEV_PHASE + "." + to_string(DEV_VERSION);
}

// The main function
int main() {

    // Create a window using GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    string title = "HKED Engine " + GET_VERSION();

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, &title[0], nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);


    Shader simpleShader = Shader("../shaders/simplev.glsl", "../shaders/simplef.glsl");

    Material mat = Material(simpleShader, "../resources/image.jpg");
    Thing square = Thing(Mesh::cube(0.5f), mat, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

    int framecount = 0, second = 0;
    // Game loop
    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float timeValue = glfwGetTime();
        if (timeValue > second + 1) {
            std::cout << "FPS: " << framecount << std::endl;
            second = floor(timeValue);
            framecount = 0;
        } else {
            framecount++;
        }

        square.draw();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
