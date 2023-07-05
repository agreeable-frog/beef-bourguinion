#pragma once

#include <functional>
#include <glm/glm.hpp>
#include <memory>

#include "mesh.hh"
#include "window.hh"
#include "texture.hh"

class Object {
public:
    Object(std::shared_ptr<Mesh> pMesh, std::shared_ptr<Texture> pTexture = nullptr,
           glm::vec3 pos = {0.0f, 0.0f, 0.0f}, glm::vec3 rot = {0.0f, 0.0f, 0.0f},
           float scale = 1.0f);
    glm::mat4 getModelMatrix();
    std::shared_ptr<Mesh> getMesh() const {
        return _pMesh;
    }
    std::shared_ptr<Texture> getTexture() const {
        return _pTexture;
    }
    void setPosition(glm::vec3 pos) {
        _position = pos;
    }
    void setMoveFunction(std::function<void(Object*, double)> func) {
        _moveFunction = func;
    }
    void move(double t) {
        if (_moveFunction)
            _moveFunction(this, t);
    }
private:
    std::shared_ptr<Mesh> _pMesh;
    std::shared_ptr<Texture> _pTexture;
    glm::vec3 _position;
    glm::vec3 _rot;
    float _scale;
    std::function<void(Object*, double)> _moveFunction = 0;
};