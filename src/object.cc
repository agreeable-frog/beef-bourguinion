#include "object.hh"

#include <glm/gtx/transform.hpp>

Object::Object(std::shared_ptr<Mesh> pMesh, std::shared_ptr<Texture> pTexture, glm::vec3 pos,
               glm::vec3 rot, float scale)
    : _pMesh(pMesh), _pTexture(pTexture), _position(pos), _rot(rot), _scale(scale) {
}

glm::mat4 Object::getModelMatrix() {
    return glm::translate(_position) * glm::rotate(_rot[2], glm::vec3{0.0f, 0.0f, 1.0f}) *
           glm::rotate(_rot[1], glm::vec3{0.0f, 1.0f, 0.0f}) *
           glm::rotate(_rot[0], glm::vec3{1.0f, 0.0f, 0.0f}) *
           glm::scale(glm::vec3{_scale, _scale, _scale});
}