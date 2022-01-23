//
// Created by harperkdavis on 1/22/2022.
//

#ifndef HKEDENGINE_PIPELINE_H
#define HKEDENGINE_PIPELINE_H

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "input.h"
#include "scene.h"

#include "../dep/stb_image.h"
#include "shader.h"

using namespace std;

class Pipeline {
public:
    inline static int WIDTH = 1200, HEIGHT = 800;
    inline static Scene* scene = nullptr;

    static void initialize();
    static void shutdown();

    static bool windowShouldClose();
    static void setTitle(string title);

    static void preRender();
    static void geometryPass();
    static void lightingPass();
    static void glUpdate();

private:
    inline static GLFWwindow* window;
    inline static string title = "HKED Engine 0.1.0-alpha.6";
    inline static unsigned int renderQuadVao = 0, renderQuadVbo = 0;

    inline static const unsigned int BLOOM_DEPTH = 7;

    inline static unsigned int gBuffer;
    inline static unsigned int gPosition, gNormal, gAlbedoSpec, gLightingData;
    inline static unsigned int rboDepth;

    inline static unsigned int hdrBuffer;
    inline static unsigned int colorBuffer, brightBuffer;

    inline static unsigned int bloomBuffers[BLOOM_DEPTH], bloomTextures[BLOOM_DEPTH];

    inline static Shader *geometryShader, *lightingShader, *bloomShader, *hdrShader;

    static void createFramebuffers();
    static void loadShaders();
    static void renderQuad();

};

#endif //HKEDENGINE_PIPELINE_H
