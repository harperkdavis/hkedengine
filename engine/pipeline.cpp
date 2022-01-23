//
// Created by harperkdavis on 1/22/2022.
//

#include "pipeline.h"

// Initializes the rendering pipeline
void Pipeline::initialize() {
    // Create a window using GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_DECORATED, GL_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, &title[0], nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetKeyCallback(window, Input::inputKeyCallback);
    glfwSetCursorPosCallback(window, Input::inputCursorCallback);
    glfwSetMouseButtonCallback(window, Input::inputMouseCallback);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return;
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

    createFramebuffers();
    loadShaders();
}

// Shuts down the rendering pipeline
void Pipeline::shutdown() {
    glfwTerminate();
}

// Should the window close? IDK, but this function will tell you!
bool Pipeline::windowShouldClose() {
    return glfwWindowShouldClose(window);
}

// Sets the title of the window, how fancy
void Pipeline::setTitle(string gameTitle) {
    Pipeline::title = gameTitle;
    glfwSetWindowTitle(window, &title[0]);
}

// Creates the graphics pipeline framebuffers
void Pipeline::createFramebuffers() {
    // Create geometry buffer
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

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
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // HDR buffers
    glGenFramebuffers(1, &hdrBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrBuffer);

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

    glGenFramebuffers(1, &shadowBuffer);

    glGenTextures(1, &shadowMap);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_RESOLUTION, SHADOW_RESOLUTION, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

// Pre-render stage
void Pipeline::preRender() {
    // Clear
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, WIDTH, HEIGHT);

    geometryShader->use();
    geometryShader->setMat4("projection", scene->camera->projectionMatrix(WIDTH, HEIGHT));
    geometryShader->setMat4("view", scene->camera->viewMatrix());

}

glm::mat4 Pipeline::getLightSpaceMatrix() {
    float nearPlane = 1.0f, farPlane = 100.0f;
    glm::mat4 lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, nearPlane, farPlane);
    glm::vec3 dld = scene->dirLight.direction;
    glm::mat4 lightView = glm::lookAt(glm::vec3(-dld.x * 20, -dld.y * 20, -dld.z * 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    return lightProjection * lightView;
}

// Deferred rendering geometry pass
void Pipeline::geometryPass() {
    // Draw shadow map

    glViewport(0, 0, SHADOW_RESOLUTION, SHADOW_RESOLUTION);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);

    shadowShader->use();
    shadowShader->setMat4("lightSpaceMatrix", getLightSpaceMatrix());
    scene->draw(*shadowShader);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Geometry Pass
    glViewport(0, 0, WIDTH, HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_BACK);

    geometryShader->use();
    scene->draw(*geometryShader);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Deferred rendering lighting and effects
void Pipeline::lightingPass() {
    // Lighting pass

    glBindFramebuffer(GL_FRAMEBUFFER, hdrBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    lightingShader->use();

    lightingShader->setVec3("viewPos", scene->camera->position);
    lightingShader->setVec4("ambientLight", scene->ambientLight);
    lightingShader->setDirLight("dirLight", scene->dirLight.direction, scene->dirLight.color, scene->dirLight.intensity);
    lightingShader->setMat4("lightSpaceMatrix", getLightSpaceMatrix());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, gLightingData);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, shadowMap);

    renderQuad();


    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Bloom

    glBindFramebuffer(GL_FRAMEBUFFER, bloomBuffers[0]);
    glClear(GL_COLOR_BUFFER_BIT);

    bloomShader->use();
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
    hdrShader->use();

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
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, bloomTextures[6]);

    renderQuad();

}

// Does the glfw things you need to do to keep the window alive
void Pipeline::glUpdate() {
    glfwPollEvents();
    glfwSwapBuffers(window);

}

// Loads the shaders
void Pipeline::loadShaders() {
    // Load shaders
    geometryShader = new Shader("../shaders/std_geomv.glsl", "../shaders/std_geomf.glsl");
    lightingShader = new Shader("../shaders/stdv.glsl", "../shaders/std_lightf.glsl");
    bloomShader = new Shader("../shaders/stdv.glsl", "../shaders/std_bloomf.glsl");
    hdrShader = new Shader("../shaders/stdv.glsl", "../shaders/std_postf.glsl");
    shadowShader = new Shader("../shaders/std_shadowv.glsl", "../shaders/std_shadowf.glsl");

    // Setup default values
    lightingShader->use();
    lightingShader->setInt("gPosition", 0);
    lightingShader->setInt("gNormal", 1);
    lightingShader->setInt("gAlbedoSpec", 2);
    lightingShader->setInt("gLightingData", 3);
    lightingShader->setInt("shadowMap", 4);

    bloomShader->use();
    bloomShader->setInt("pass", 0);

    hdrShader->use();
    hdrShader->setInt("color", 0);
    hdrShader->setInt("bloom1", 1);
    hdrShader->setInt("bloom2", 2);
    hdrShader->setInt("bloom4", 3);
    hdrShader->setInt("bloom8", 4);
    hdrShader->setInt("bloom16", 5);
    hdrShader->setInt("bloom32", 6);
    hdrShader->setInt("bloom64", 7);

}

// Renders an on-screen quad
void Pipeline::renderQuad() {
    if (renderQuadVao == 0) {
        float quadVertices[] = {
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        glGenVertexArrays(1, &renderQuadVao);
        glGenBuffers(1, &renderQuadVbo);
        glBindVertexArray(renderQuadVao);
        glBindBuffer(GL_ARRAY_BUFFER, renderQuadVao);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(renderQuadVao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}