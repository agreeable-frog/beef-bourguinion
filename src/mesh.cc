#include "mesh.hh"

#include <algorithm>
#include <iostream>

void Mesh::registerInBuffer(std::vector<Vertex>& vertexBuffer, std::vector<uint32_t>& indexBuffer) {
    _indexOffset = indexBuffer.size();
    auto vertexBufferSize = vertexBuffer.size();

    vertexBuffer.insert(vertexBuffer.end(), _vertices.begin(), _vertices.end());

    std::vector<uint32_t> indicesOffsetted = std::vector<uint32_t>(_indices.size());
    std::transform(_indices.begin(), _indices.end(), indicesOffsetted.begin(),
                   [&, this](uint32_t i) { return i + vertexBufferSize; });
    indexBuffer.insert(indexBuffer.end(), indicesOffsetted.begin(), indicesOffsetted.end());
}

CubeMesh::CubeMesh() : Mesh() {
    _vertices.push_back(Vertex{{-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}});
    _vertices.push_back(Vertex{{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}});
    _vertices.push_back(Vertex{{1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}});
    _vertices.push_back(Vertex{{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}});
    std::vector<uint32_t> indexFace1 = {0, 1, 3, 1, 2, 3};
    _indices.insert(_indices.end(), indexFace1.begin(), indexFace1.end());

    _vertices.push_back(Vertex{{-1.0f, -1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}});
    _vertices.push_back(Vertex{{-1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}});
    _vertices.push_back(Vertex{{1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}});
    _vertices.push_back(Vertex{{1.0f, -1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}});
    std::vector<uint32_t> indexFace2 = {4, 5, 7, 5, 6, 7};
    _indices.insert(_indices.end(), indexFace2.begin(), indexFace2.end());

    _vertices.push_back(Vertex{{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}});
    _vertices.push_back(Vertex{{-1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}});
    _vertices.push_back(Vertex{{1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}});
    _vertices.push_back(Vertex{{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}});
    std::vector<uint32_t> indexFace3 = {8, 9, 11, 9, 10, 11};
    _indices.insert(_indices.end(), indexFace3.begin(), indexFace3.end());

    _vertices.push_back(Vertex{{-1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}});
    _vertices.push_back(Vertex{{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}});
    _vertices.push_back(Vertex{{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}});
    _vertices.push_back(Vertex{{1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}});
    std::vector<uint32_t> indexFace4 = {12, 13, 15, 13, 14, 15};
    _indices.insert(_indices.end(), indexFace4.begin(), indexFace4.end());

    _vertices.push_back(Vertex{{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}});
    _vertices.push_back(Vertex{{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}});
    _vertices.push_back(Vertex{{1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}});
    _vertices.push_back(Vertex{{1.0f, 1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}});
    std::vector<uint32_t> indexFace5 = {16, 17, 19, 17, 18, 19};
    _indices.insert(_indices.end(), indexFace5.begin(), indexFace5.end());

    _vertices.push_back(Vertex{{-1.0f, 1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}});
    _vertices.push_back(Vertex{{-1.0f, -1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}});
    _vertices.push_back(Vertex{{-1.0f, -1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}});
    _vertices.push_back(Vertex{{-1.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}});
    std::vector<uint32_t> indexFace6 = {20, 21, 23, 21, 22, 23};
    _indices.insert(_indices.end(), indexFace6.begin(), indexFace6.end());
}

SphereMesh::SphereMesh(int nRings, int nSegments) : Mesh() {
    for (size_t j = 0; j < nSegments; j++) {
        float theta = 2 * M_PI * float(j + 1) / float(nSegments);
        _vertices.push_back(Vertex{glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f},
                                   glm::vec2{0.0f, theta / (2 * M_PI)}});
    }
    for (size_t i = 0; i < nRings; i++) {
        float phi = M_PI * float(i + 1) / float(nRings);
        for (size_t j = 0; j < nSegments; j++) {
            float theta = 2 * M_PI * float(j + 1) / float(nSegments);
            float x = std::sin(phi) * std::cos(theta);
            float y = std::cos(phi);
            float z = std::sin(phi) * std::sin(theta);
            _vertices.push_back(Vertex{glm::vec3{x, y, z}, glm::normalize(glm::vec3{x, y, z}),
                                       glm::vec2{phi / M_PI, theta / (2 * M_PI)}});
        }
        auto v = _vertices[_vertices.size() - 1];
        v.uv.y = 0.0f;
        _vertices.push_back(v);
    }

    for (size_t i = 0; i < nRings + 1; i++) {
        for (size_t j = 0; j < nSegments; j++) {
            size_t k = i * nSegments + j;
            size_t knext = i * nSegments + (j + 1);

            _indices.push_back(k);
            _indices.push_back(knext + nSegments);
            _indices.push_back(k + nSegments);

            _indices.push_back(knext + nSegments);
            _indices.push_back(k);
            _indices.push_back(knext);
        }
    }
}

SkySphereMesh::SkySphereMesh(int nRings, int nSegments) : Mesh() {
    for (size_t j = 0; j < nSegments; j++) {
        float theta = 2 * M_PI * float(j + 1) / float(nSegments);
        _vertices.push_back(Vertex{glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f},
                                   glm::vec2{0.0f, theta / (2 * M_PI)}});
    }
    for (size_t i = 0; i < nRings; i++) {
        float phi = M_PI * float(i + 1) / float(nRings);
        for (size_t j = 0; j < nSegments; j++) {
            float theta = 2 * M_PI * float(j + 1) / float(nSegments);
            float x = std::sin(phi) * std::cos(theta);
            float y = std::cos(phi);
            float z = std::sin(phi) * std::sin(theta);
            _vertices.push_back(Vertex{glm::vec3{x, y, z}, glm::normalize(glm::vec3{x, y, z}),
                                       glm::vec2{phi / M_PI, theta / (2 * M_PI)}});
        }
        auto v = _vertices[_vertices.size() - 1];
        v.uv.y = 0.0f;
        _vertices.push_back(v);
    }

    for (size_t i = 0; i < nRings + 1; i++) {
        for (size_t j = 0; j < nSegments; j++) {
            size_t k = i * nSegments + j;
            size_t knext = i * nSegments + (j + 1);

            _indices.push_back(k);
            _indices.push_back(k + nSegments);
            _indices.push_back(knext + nSegments);

            _indices.push_back(knext + nSegments);
            _indices.push_back(knext);
            _indices.push_back(k);
        }
    }
}

PlaneMesh::PlaneMesh(glm::vec3 up, glm::vec3 dir, float ratio) {
    up = glm::normalize(up);
    dir = glm::normalize(dir - glm::dot(dir, up) / glm::length(up) * up); // Gram–Schmidt
    glm::vec3 dir2 = glm::cross(up, dir);
    _vertices.push_back(Vertex{glm::vec3{-dir + dir2}, up, {0.0f, 0.0f}});
    _vertices.push_back(Vertex{glm::vec3{-dir - dir2}, up, {1.0f, 0.0f}});
    _vertices.push_back(Vertex{glm::vec3{dir - dir2}, up, {1.0f, 1.0f}});
    _vertices.push_back(Vertex{glm::vec3{dir + dir2}, up, {0.0f, 1.0f}});
    std::vector<uint32_t> index = {0, 1, 2, 0, 2, 3};
    _indices.insert(_indices.end(), index.begin(), index.end());
}