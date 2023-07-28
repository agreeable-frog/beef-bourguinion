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

Window::Window(const std::string& name, uint width, uint height, Window* parent, bool visible, bool resizable)
    : _width(width), _height(height) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_VISIBLE, visible ? GLFW_TRUE : GLFW_FALSE);
    _pWindow = glfwCreateWindow(width, height, name.c_str(), 0, parent ? parent->getHandle() : 0);
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

RenderWindow::RenderWindow(const std::string& name, uint width, uint height)
    : Window(name, width, height, 0, false) {
    glCreateRenderbuffers(1, &_renderBuffer);
    glCreateRenderbuffers(1, &_depthBuffer);
    glNamedRenderbufferStorage(_renderBuffer, GL_RGBA, _width, _height);
    glNamedRenderbufferStorage(_depthBuffer, GL_DEPTH_COMPONENT, _width, _height);
    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
    glNamedFramebufferRenderbuffer(_fbo, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _renderBuffer);
    glNamedFramebufferRenderbuffer(_fbo, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
}

void SubWindow::resizeCallback(GLFWwindow* pWindow, int width, int height) {
    Window* windowInstance = static_cast<Window*>(glfwGetWindowUserPointer(pWindow));
    windowInstance->sizeChanged(width, height);
}

SubWindow::SubWindow(const std::string& name, uint width, uint height, RenderWindow* window)
    : Window(name, width, height, window, true, true) {
    glfwSetWindowSizeCallback(_pWindow, &SubWindow::resizeCallback);
    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _fbo);
    glEnable(GL_RENDERBUFFER);
    glBindRenderbuffer(GL_RENDERBUFFER, window->renderBuffer());
    glNamedFramebufferRenderbuffer(_fbo, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER,
                                   window->renderBuffer());
}

void RenderWindow::resize(uint width, uint height) {
    glfwSetWindowSize(_pWindow, width, height);
    sizeChanged(width, height);
    glNamedRenderbufferStorage(_renderBuffer, GL_RGBA, _width, _height);
    glNamedRenderbufferStorage(_depthBuffer, GL_DEPTH_COMPONENT, _width, _height);
}