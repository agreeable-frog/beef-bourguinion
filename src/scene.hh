#pragma once

#include <vector>
#include <memory>

#include "light.hh"
#include "mesh.hh"
#include "object.hh"
#include "texture.hh"

class Scene {
public:
    Scene() {}
    void addMesh(std::shared_ptr<Mesh> pMesh) {
        pMesh->registerInBuffer(_vertexBuffer, _indexBuffer);
        _pMeshes.push_back(pMesh);
    }
    void addObject(std::shared_ptr<Object> pObject) {
        _pObjects.push_back(pObject);
    }
    const std::vector<Vertex>& getVertexBuffer() const {
        return _vertexBuffer;
    }
    const std::vector<uint32_t>& getIndexBuffer() const {
        return _indexBuffer;
    }
    const std::vector<std::shared_ptr<Object>> getObjects() const {
        return _pObjects;
    }
    void update(double deltaTime);
private:
    std::vector<std::shared_ptr<Mesh>> _pMeshes;
    std::vector<std::shared_ptr<Object>> _pObjects;
    std::vector<std::shared_ptr<Texture>> _pTextures;
    std::vector<std::shared_ptr<Light>> _pLights;
    std::vector<Vertex> _vertexBuffer;
    std::vector<uint32_t> _indexBuffer;
};