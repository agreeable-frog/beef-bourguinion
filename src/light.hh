#pragma once

#include <glm/glm.hpp>

class Light {
public:
    const glm::vec3& getPos() const {
        return _pos;
    }
private:
    glm::vec3 _pos;
    glm::vec3 _color;
    float _intensity;
};