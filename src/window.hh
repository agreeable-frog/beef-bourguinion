#pragma once

#include <array>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <iostream>
#include <memory>

class Context {
public:
    static void registerDebugCallbacks();
    Context() {
        if (!instanceCounter) {
            glfwInit();
        }
        instanceCounter++;
    }
    ~Context() {
        instanceCounter--;
        if (instanceCounter == 0) {
            glfwTerminate();
        }
    }

private:
    static int instanceCounter;
    static bool debugCallbacksSet;

    static void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                              GLsizei length, const GLchar* message, const void* userparam);
    static void glfwErrorCallback(int i, const char* errStr);
};

class Window {
private:
    Context _context;
    GLFWwindow* _pWindow;
    static void keyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods);
    static void mousePosCallback(GLFWwindow* pWindow, double xpos, double ypos);
    static void mouseButtonCallback(GLFWwindow* pWindow, int button, int action, int mods);

protected:
    size_t _width;
    size_t _height;

public:
    std::array<int, 512> keystates = {0};
    std::array<int, 2> mouseMove = {0}; // x,y from upper-left
    std::array<int, 16> mouseButtonStates = {0};
    Window(const std::string& name, uint width, uint height, Window* parent = 0, bool visible = true);
    size_t width() const {
        return _width;
    }
    size_t height() const {
        return _height;
    }
    GLFWwindow* getHandle() const {
        return _pWindow;
    }
    operator GLFWwindow*() const {
        return _pWindow;
    }
    void resize(size_t width, size_t height) {
        glfwSetWindowSize(*this, width, height);
        _width = width;
        _height = height;
    }
    ~Window() {
        glfwDestroyWindow(_pWindow);
    }
};

class RenderWindow : public Window {
public:
    RenderWindow(const std::string& name, uint width, uint height);
    GLuint fbo() const {
        return _fbo;
    }
    GLuint renderBuffer() const {
        return _renderBuffer;
    }
private:
    GLuint _renderBuffer;
    GLuint _depthBuffer;
    GLuint _fbo;
};

class SubWindow : public Window {
public:
    SubWindow(const std::string& name, uint width, uint height, RenderWindow* window);
    GLuint fbo() const {
        return _fbo;
    }
private:
    GLuint _fbo;
};