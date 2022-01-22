//
// Created by harpe on 1/21/2022.
//

#include <string>
#include <vector>
#include <map>
#include <GLFW/glfw3.h>
#include <algorithm>

#ifndef HKEDENGINE_INPUT_H
#define HKEDENGINE_INPUT_H

using namespace std;

// Class for all things input
class Input {
public:
    // Callback for keyboard inputs
    static void inputKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
            return;
        }
        if (action == GLFW_PRESS) {
            keysList.emplace_back(key);
            keysDownList.emplace_back(key);
        } else if (action == GLFW_RELEASE) {
            auto it = find(keysList.begin(), keysList.end(), key);
            if (it != keysList.end()) {
                swap(*it, keysList.back());
                keysList.pop_back();
            }
            keysUpList.emplace_back(key);
        }
    }

    // Callback for mouse button inputs
    static void inputMouseCallback(GLFWwindow *window, int button, int action, int mods) {
        if (action == GLFW_PRESS) {
            mouseList.emplace_back(button);
            mouseDownList.emplace_back(button);
        } else if (action == GLFW_RELEASE) {
            auto it = find(mouseList.begin(), mouseList.end(), button);
            if (it != mouseList.end()) {
                swap(*it, mouseList.back());
                mouseList.pop_back();
            }
            mouseUpList.emplace_back(button);
        }
    }

    // Callback for cursor changes
    static void inputCursorCallback(GLFWwindow *window, double xpos, double ypos) {
        mouseX = xpos;
        mouseY = ypos;
    }

    static void updateInput() {
        keysDownList.clear();
        keysUpList.clear();

        mouseDownList.clear();
        mouseUpList.clear();

        pmouseX = mouseX;
        pmouseY = mouseY;
    }

    static bool key(const int &key) {
        auto it = find(keysList.begin(), keysList.end(), key);
        if (it != keysList.end()) {
            return true;
        }
        return false;
    }

    static bool keyDown(const int &key) {
        auto it = find(keysDownList.begin(), keysDownList.end(), key);
        if (it != keysDownList.end()) {
            return true;
        }
        return false;
    }

    static bool keyUp(const int &key) {
        auto it = find(keysUpList.begin(), keysUpList.end(), key);
        if (it != keysUpList.end()) {
            return true;
        }
        return false;
    }

    static bool mouse(const int &button) {
        auto it = find(mouseList.begin(), mouseList.end(), button);
        if (it != mouseList.end()) {
            return true;
        }
        return false;
    }

    static bool mouseDown(const int &button) {
        auto it = find(mouseDownList.begin(), mouseDownList.end(), button);
        if (it != mouseDownList.end()) {
            return true;
        }
        return false;
    }

    static bool mouseUp(const int &button) {
        auto it = find(mouseDownList.begin(), mouseDownList.end(), button);
        if (it != mouseDownList.end()) {
            return true;
        }
        return false;
    }

    static vector<int> keys() {
        return keysList;
    }

    static double getMouseX() {
        return mouseX;
    }

    static double getMouseY() {
        return mouseY;
    }

    static double getPmouseX() {
        return pmouseX;
    }

    static double getPmouseY() {
        return pmouseY;
    }

    static double getAxisX() {
        return mouseX - pmouseX;
    }

    static double getAxisY() {
        return mouseY - pmouseY;
    }
private:
    inline static vector<int> keysList;
    inline static vector<int> keysDownList;
    inline static vector<int> keysUpList;

    inline static vector<int> mouseList;
    inline static vector<int> mouseDownList;
    inline static vector<int> mouseUpList;

    inline static double mouseX = 0;
    inline static double mouseY = 0;
    inline static double pmouseX = 0;
    inline static double pmouseY = 0;

    inline static double scrollX = 0;
    inline static double scrollY = 0;

};

#endif //HKEDENGINE_INPUT_H
