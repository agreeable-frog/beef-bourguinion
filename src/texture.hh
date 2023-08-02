#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

typedef unsigned char uchar;

class Texture {
public:
    Texture() = default;
    Texture(const std::string& file, int rot = 0);
    ~Texture() {
        glDeleteTextures(1, &_id);
    }
    GLuint getId() const {
        return _id;
    }
protected:
    uint _width;
    uint _height;
    std::vector<uchar> _data;
    GLuint _id;
};

class UniformTexture : public Texture {
public:
    UniformTexture(glm::vec3 color);
};