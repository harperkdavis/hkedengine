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

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);


    Mesh square = Mesh({Vertex(glm::vec3(-0.5, 0.5, 0), glm::vec2(0, 0), glm::vec3(0, 0, 0)),
                          Vertex(glm::vec3(-0.5, -0.5, 0), glm::vec2(0, 1), glm::vec3(0, 0, 0)),
                          Vertex(glm::vec3(0.5, -0.5, 0), glm::vec2(1, 1), glm::vec3(0, 0, 0)),
                          Vertex(glm::vec3(0.5, 0.5, 0), glm::vec2(1, 0), glm::vec3(0, 0, 0))},
                         {1, 0, 3, 1, 2, 3});

    Shader simpleShader = Shader("../shaders/simpleVertex.glsl", "../shaders/simpleFragment.glsl");

    int imgwidth, imgheight, imgnrChannels;
    unsigned char *imgdata = stbi_load("../textures/image.jpg", &imgwidth, &imgheight, &imgnrChannels, 0);

    unsigned int texture;
    glGenTextures(1, &texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgwidth, imgheight, 0, GL_RGB, GL_UNSIGNED_BYTE, imgdata);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(imgdata);

    // Game loop
    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float timeValue = glfwGetTime();
        simpleShader.setFloat("time", timeValue);
        simpleShader.use();

        square.draw(simpleShader);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
