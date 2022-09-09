#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <memory>

#define GLEW_STATIC
#include "../../../glew/glew.h"

#include <GLFW/glfw3.h>

class Window {
private:
    struct CallbackManager {
        static int width, height;
        
        static std::function<void(GLFWwindow*, int, int)> resizeFun;
        static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);

        static std::function<void(GLFWwindow*, double, double)> mouseFun;
        static void mouseCallback(GLFWwindow* window, double xpos, double ypos);

        static std::function<void(GLFWwindow*, int, int, int)> mouseButtonFun;
        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

        static std::function<void(GLFWwindow*, int, int, int, int)> keyFun;
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    };
private:
    GLFWwindow* window;
    std::string title;
public:
    Window(const std::string& _title, unsigned int _width, unsigned int _height);
    Window() = default;
    ~Window() = default;
private:
    void initGlew();
    void initWindow();
public:
    void update();

    inline void terminate() {
        glfwTerminate();
    }

    inline void setResizeFun(const std::function<void(GLFWwindow*, int, int)>& resizeFun) {
        CallbackManager::resizeFun = resizeFun;
    }

    inline void setMouseFun(const std::function<void(GLFWwindow*, double, double)>& mouseFun) {
        CallbackManager::mouseFun = mouseFun;
    }

    inline void setMouseButtonFun(const std::function<void(GLFWwindow*, int, int, int)>& mouseButtonFun) {
        CallbackManager::mouseButtonFun = mouseButtonFun;
    }

    inline void setKeyFun(const std::function<void(GLFWwindow*, int, int, int, int)>& keyFun) {
        CallbackManager::keyFun = keyFun;
    }    
public:
    inline GLFWwindow* getGLFWwindow() { return window; }
    inline void setGLFWwindow(GLFWwindow* window) { this->window = window; }

    inline bool windowShouldClose() const { return glfwWindowShouldClose(window); }

    inline unsigned int getWidth() const { return CallbackManager::width; }
    inline void setWidth(unsigned int width) { CallbackManager::width = width; }

    inline unsigned int getHeight() const { return CallbackManager::height; }
    inline void setHeight(unsigned int height) { CallbackManager::height = height; }
};