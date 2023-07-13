#include "camera.hh"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

void AbstractCamera::keyMove(const std::array<int, 512>& keystates, double deltaTime) {
    float speed = 1.5f;
    if (keystates[GLFW_KEY_W]) {
        _position += _forward * speed * (float)deltaTime;
    }
    if (keystates[GLFW_KEY_S]) {
        _position += -_forward * speed * (float)deltaTime;
    }
    if (keystates[GLFW_KEY_A]) {
        _position += glm::cross(_axis, _forward) * speed * (float)deltaTime;
    }
    if (keystates[GLFW_KEY_D]) {
        _position += -glm::cross(_axis, _forward) * speed * (float)deltaTime;
    }
    if (keystates[GLFW_KEY_SPACE]) {
        _position += _axis * speed * (float)deltaTime;
    }
    if (keystates[GLFW_KEY_LEFT_SHIFT]) {
        _position += -_axis * speed * (float)deltaTime;
    }
}

void AbstractCamera::mouseDrag(const std::array<int, 16>& mouseButtonStates,
                               const std::array<int, 2>& move, double deltaTime) {
    float speed = -0.1f;
    if (mouseButtonStates[GLFW_MOUSE_BUTTON_1]) {
        _forward = glm::rotate(glm::mat4(1.0f), (float)(move[0] * deltaTime) * speed, _axis) *
                   glm::vec4(_forward, 1.0f);
        _up = glm::rotate(glm::mat4(1.0f), (float)(move[0] * deltaTime) * speed, _axis) *
              glm::vec4(_up, 1.0f);

        auto right = -glm::cross(_up, _forward);

        _forward = glm::rotate(glm::mat4(1.0f), (float)(move[1] * deltaTime) * speed, right) *
                   glm::vec4(_forward, 1.0f);
        _up = glm::rotate(glm::mat4(1.0f), (float)(move[1] * deltaTime) * speed, right) *
              glm::vec4(_up, 1.0f);
    }
}

FrustumCamera FrustumCamera::build(glm::vec3 position, glm::vec3 axis, glm::vec3 forward,
                                   float fovx, float fovy, float headingOffset, float pitchOffset,
                                   float nearClip, float farClip) {
    float width = 2 * nearClip * std::tan(fovx / 2);
    float height = 2 * nearClip * std::tan(fovy / 2);
    float left = -width / 2;
    float right = width / 2;
    float top = height / 2;
    float bot = -height / 2;
    float yOffset = 0.0f;
    if (pitchOffset < 0)
        yOffset = std::tan(pitchOffset + fovy / 2) * nearClip - height / 2;
    else
        yOffset = std::tan(pitchOffset - fovy / 2) * nearClip + height / 2;
    float xOffset = 0.0f;
    if (headingOffset < 0)
        xOffset = std::tan(headingOffset + fovx / 2) * nearClip - width / 2;
    else
        xOffset = std::tan(headingOffset - fovx / 2) * nearClip + width / 2;
    return FrustumCamera(position, axis, forward, left - xOffset, right - xOffset, top + yOffset,
                         bot + yOffset, nearClip, farClip);
}

PanoramicCamera::PanoramicCamera(glm::vec3 position, glm::vec3 axis, glm::vec3 forward, float fovy,
                                 int nSplit, float nearClip, float farClip)
    : _position(position),
      _axis(axis),
      _forward(forward),
      _fovy(fovy),
      _nSplit(nSplit),
      _nearClip(nearClip),
      _farClip(farClip) {
    for (int i = 0; i < _nSplit; i++) {
        auto cam1 = FrustumCamera::build(_position, _axis, _forward, 2 * (float)M_PI / _nSplit,
                                        _fovy, 0.0f, 0.0f, _nearClip, _farClip);
        cam1.rotateHeading((2 * M_PI * (_nSplit / 2 - i)) / (float)_nSplit);
        _internalCameras.push_back(cam1);
    }
}