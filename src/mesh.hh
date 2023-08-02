#pragma once

#include <glm/glm.hpp>
#include <vector>

struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 uv;
};

class Mesh {
protected:
    Mesh() {
        static int counter = 0;
        _uid = counter++;
    }
    int _uid;
    std::vector<Vertex> _vertices;
    std::vector<uint32_t> _indices;
    size_t _indexOffset;

public:
    size_t getIndexOffset() const {
        return _indexOffset;
    }
    size_t getIndexSize() const {
        return _indices.size();
    }
    void registerInBuffer(std::vector<Vertex>& vertexBuffer, std::vector<uint32_t>& indexBuffer);
};

class CubeMesh : public Mesh {
public:
    CubeMesh();
};

class SphereMesh : public Mesh {
public:
    SphereMesh(int nRings, int nSegments);
};

class SkySphereMesh : public Mesh {
public:
    SkySphereMesh(int nRings, int nSegments);
};

class PlaneMesh : public Mesh {
public:
    PlaneMesh(glm::vec3 up, glm::vec3 dir, float ratio);
};