#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
