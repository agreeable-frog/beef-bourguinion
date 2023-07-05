#include "window.hh"

int Context::instanceCounter = 0;
bool Context::debugCallbacksSet = false;

void Context::registerDebugCallbacks() {
    if (!debugCallbacksSet) {
        glDebugMessageCallback(&Context::debugCallback, 0);
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glfwSetErrorCallback(&Context::glfwErrorCallback);
        debugCallbacksSet = true;
    }
}

void Context::debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                            const GLchar* message, const void* userparam) {
    std::cout << "DEBUG : source " << source << ", type " << type << ", id " << id << ", severity "
              << severity << '\n';
    std::cout << std::string(message) << '\n';
}

void Context::glfwErrorCallback(int i, const char* errStr) {
    std::cerr << "ERROR : " << errStr << '\n';
}

void Window::keyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods) {
    Window* windowInstance = static_cast<Window*>(glfwGetWindowUserPointer(pWindow));
    if (key != -1) {
        if (action == GLFW_PRESS) {
            windowInstance->keystates[key] = 1;
        } else if (action == GLFW_RELEASE) {
            windowInstance->keystates[key] = 0;
        } else if (action == GLFW_REPEAT) {
        }
    }
}
void Window::mousePosCallback(GLFWwindow* pWindow, double xpos, double ypos) {
    Window* windowInstance = static_cast<Window*>(glfwGetWindowUserPointer(pWindow));
    static int lastPos[2] = {(int)xpos, (int)ypos};
    int pos[2] = {(int)xpos, (int)ypos};
    windowInstance->mouseMove[0] += (pos[0] - lastPos[0]);
    windowInstance->mouseMove[1] += (pos[1] - lastPos[1]);
    lastPos[0] = pos[0];
    lastPos[1] = pos[1];
}
void Window::mouseButtonCallback(GLFWwindow* pWindow, int button, int action, int mods) {
    Window* windowInstance = static_cast<Window*>(glfwGetWindowUserPointer(pWindow));
    if (action == GLFW_PRESS) {
        windowInstance->mouseButtonStates[button] = 1;
    } else if (action == GLFW_RELEASE) {
        windowInstance->mouseButtonStates[button] = 0;
    } else if (action == GLFW_REPEAT) {
    }
}

Window::Window(const std::string& name, uint width, uint height, Window* parent) : _width(width), _height(height) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    std::cout << "hello?\n";
    _pWindow = glfwCreateWindow(width, height, name.c_str(), 0, parent ? parent->getHandle() : 0);
    std::cout << "goodbye?\n";
    if (_pWindow == nullptr) {
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwSetWindowUserPointer(_pWindow, this);
    glfwSetKeyCallback(_pWindow, &Window::keyCallback);
    glfwSetCursorPosCallback(_pWindow, &Window::mousePosCallback);
    glfwSetMouseButtonCallback(_pWindow, &Window::mouseButtonCallback);
    glfwMakeContextCurrent(_pWindow);
    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("GLEW init failed.");
    }
    glfwSwapInterval(1);
    Context::registerDebugCallbacks();
}