#pragma once

#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class AbstractCamera {
protected:
    AbstractCamera(glm::vec3 position, glm::vec3 axis, glm::vec3 forward)
        : _position(position),
          _axis(glm::normalize(axis)),
          _up(glm::normalize(axis)),
          _forward(glm::normalize(forward)) {
    }
    glm::vec3 _position;
    glm::vec3 _axis;
    glm::vec3 _up;
    glm::vec3 _forward;

public:
    void keyMove(const std::array<int, 512>& keystates, double deltaTime);
    void mouseDrag(const std::array<int, 16>& mouseButtonStates, const std::array<int, 2>& move,
                   double deltaTime);
    glm::mat4 getViewMatrix() {
        return glm::lookAt(_position, _position + _forward, _up);
    }
    virtual glm::mat4 getProjectionMatrix() = 0;
};

class SimplePerspectiveCamera : public AbstractCamera {
private:
    float _fovy;
    float _ratio;
    float _nearClip;
    float _farClip;

public:
    SimplePerspectiveCamera(glm::vec3 position, glm::vec3 axis, glm::vec3 forward, float fovy,
                            float ratio, float nearClip = 0.1f, float farClip = 30.0f)
        : AbstractCamera(position, axis, forward),
          _fovy(fovy),
          _ratio(ratio),
          _nearClip(nearClip),
          _farClip(farClip) {
    }
    glm::mat4 getProjectionMatrix() override {
        return glm::perspective(_fovy, _ratio, _nearClip, _farClip);
    }
};

class FrustumCamera : public AbstractCamera {
private:
    float _left;
    float _right;
    float _top;
    float _bottom;
    float _nearClip;
    float _farClip;

public:
    FrustumCamera(glm::vec3 position, glm::vec3 axis, glm::vec3 forward, float left, float right,
                  float top, float bottom, float nearClip = 0.1f, float farClip = 30.0f)
        : AbstractCamera(position, axis, forward),
          _left(left),
          _right(right),
          _top(top),
          _bottom(bottom),
          _nearClip(nearClip),
          _farClip(farClip) {
    }
    static FrustumCamera build(glm::vec3 position, glm::vec3 axis, glm::vec3 forward, float fovx,
                               float fovy, float headingOffset = 0.0f, float pitchOffset = 0.0f,
                               float nearClip = 0.1f, float farClip = 30.0f);
    glm::mat4 getProjectionMatrix() override {
        return glm::frustum(_left, _right, _bottom, _top, _nearClip, _farClip);
    }
};