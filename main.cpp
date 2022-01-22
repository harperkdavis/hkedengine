#include <iostream>
#include <string>
#include <random>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/graphics.h"
#include "engine/input.h"

#define STB_IMAGE_IMPLEMENTATION
#include "dep/stb_image.h"

const int WIDTH = 1200;
const int HEIGHT = 800;

const unsigned int BLOOM_DEPTH = 6;

const int SSAO_KERNEL_SIZE = 64;

using namespace std;

const float MOUSE_SENSITIVITY = 0.04f;

// Linear intERPolation function (aka the most beautiful function of all time)
float lerp(float a, float b, float f) {
    return a + f * (b - a);
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
// Render an on screen quad
void renderQuad() {
    if (quadVAO == 0) {
        float quadVertices[] = {
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

const string title = "HKED Engine 0.1.0-alpha.5";


// The main function
int main() {

    // Create a window using GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, &title[0], nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    glfwSetKeyCallback(window, Input::inputKeyCallback);
    glfwSetCursorPosCallback(window, Input::inputCursorCallback);
    glfwSetMouseButtonCallback(window, Input::inputMouseCallback);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    stbi_set_flip_vertically_on_load(true);

    // Finish up window
    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_DEPTH_TEST);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    // Enable face culling
    glEnable(GL_CULL_FACE);

    // Setup framebuffers
    unsigned int gBuffer;
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    unsigned int gPosition, gNormal, gAlbedoSpec, gLightingData;

    // Position
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

    // Normal
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

    // Albedo / Specular
    glGenTextures(1, &gAlbedoSpec);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);

    // Lighting Data
    glGenTextures(1, &gLightingData);
    glBindTexture(GL_TEXTURE_2D, gLightingData);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gLightingData, 0);

    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned int gAttachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
    glDrawBuffers(4, gAttachments);

    // Depth
    unsigned int rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // HDR buffers
    unsigned int hdrBuffer;
    glGenFramebuffers(1, &hdrBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrBuffer);
    unsigned int colorBuffer, brightBuffer;

    // Regular color buffer
    glGenTextures(1, &colorBuffer);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);

    // Bright color buffer
    glGenTextures(1, &brightBuffer);
    glBindTexture(GL_TEXTURE_2D, brightBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, brightBuffer, 0);

    unsigned int hAttachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, hAttachments);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Framebuffer not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Bloom framebuffers

    unsigned int bloomBuffers[BLOOM_DEPTH], bloomTextures[BLOOM_DEPTH];
    glGenFramebuffers(BLOOM_DEPTH, bloomBuffers);
    glGenTextures(BLOOM_DEPTH, bloomTextures);
    glActiveTexture(GL_TEXTURE0);
    for (unsigned int i = 0; i < BLOOM_DEPTH; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, bloomBuffers[i]);
        glBindTexture(GL_TEXTURE_2D, bloomTextures[i]);

        double factor = pow(2, i);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int) (WIDTH / factor), (int) (HEIGHT / factor), 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bloomTextures[i], 0);
    }

    // Set the scene

    Camera playerCamera = Camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 80.0f, 0.1f, 1000.0f);
    Camera::mainCamera = &playerCamera;

    // Load shaders
    Shader geometryShader = Shader("../shaders/std_geomv.glsl", "../shaders/std_geomf.glsl");
    Shader lightingShader = Shader("../shaders/stdv.glsl", "../shaders/std_lightf.glsl");
    Shader bloomShader = Shader("../shaders/stdv.glsl", "../shaders/std_bloomf.glsl");
    Shader hdrShader = Shader("../shaders/stdv.glsl", "../shaders/std_postf.glsl");

    // Setup default values
    lightingShader.use();
    lightingShader.setInt("gPosition", 0);
    lightingShader.setInt("gNormal", 1);
    lightingShader.setInt("gAlbedoSpec", 2);
    lightingShader.setInt("gLightingData", 3);

    bloomShader.use();
    bloomShader.setInt("pass", 0);

    hdrShader.use();
    hdrShader.setInt("color", 0);
    hdrShader.setInt("bloom1", 1);
    hdrShader.setInt("bloom2", 2);
    hdrShader.setInt("bloom4", 3);
    hdrShader.setInt("bloom8", 4);
    hdrShader.setInt("bloom16", 5);
    hdrShader.setInt("bloom32", 6);

    // Create the main scene
    Scene scene = Scene();

    Material mat = Material(geometryShader, "../resources/test.png");
    // mat.emission = 0.5f;
    scene.add(Thing(Mesh::cube(1.0f), mat, glm::vec3(0, -1, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));
    scene.add(Thing(Mesh::cube(2.0f), mat, glm::vec3(4, -1, 3), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));
    Thing& square = scene.add(Thing(Mesh::cube(0.5f), mat, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));

    double deltaTime = 0;
    int frameCount = 0, lastSecond = 0;
    // Game loop
    while(!glfwWindowShouldClose(window)) {
        double startFrame = glfwGetTime();

        // Update framerate
        if (floor(glfwGetTime()) > lastSecond) {
            lastSecond = floor(glfwGetTime());
            string winTitle = title + " (FPS: " + to_string(frameCount) + ")";
            glfwSetWindowTitle(window, &winTitle[0]);
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

        // Clear
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, WIDTH, HEIGHT);

        // Geometry Pass
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene.draw();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        glBindFramebuffer(GL_FRAMEBUFFER, hdrBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Lighting Pass
        lightingShader.use();

        lightingShader.setVec3("viewPos", playerCamera.position);
        lightingShader.setVec4("ambientLight", scene.ambientLight);
        lightingShader.setDirLight("dirLight", scene.dirLight.direction, scene.dirLight.color, scene.dirLight.intensity);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, gLightingData);
        renderQuad();


        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Bloom

        glBindFramebuffer(GL_FRAMEBUFFER, bloomBuffers[0]);
        glClear(GL_COLOR_BUFFER_BIT);

        bloomShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, brightBuffer);

        renderQuad();

        for (int i = 1; i < BLOOM_DEPTH; i++) {
            double factor = pow(2, i);

            glBindFramebuffer(GL_FRAMEBUFFER, bloomBuffers[i]);
            glViewport(0, 0, (int) (WIDTH / factor), (int) (HEIGHT / factor));

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, bloomTextures[i - 1]);

            renderQuad();
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Final post-processing
        glViewport(0, 0, WIDTH, HEIGHT);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        hdrShader.use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorBuffer);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, bloomTextures[0]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, bloomTextures[1]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, bloomTextures[2]);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, bloomTextures[3]);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, bloomTextures[4]);
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, bloomTextures[5]);

        renderQuad();

        Input::updateInput();
        glfwPollEvents();
        glfwSwapBuffers(window);

        deltaTime = glfwGetTime() - startFrame;
    }

    glfwTerminate();
    return 0;
}