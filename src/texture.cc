#include "texture.hh"

#include "lodepng.hh"

#include <array>
#include <iostream>
#include <glm/glm.hpp>

static std::vector<unsigned char> loadImage(const std::string& path, uint& width, uint& height) {
    std::vector<unsigned char> data;
    lodepng::decode(data, width, height, path, LodePNGColorType::LCT_RGB, 8);
    return data;
}

Texture::Texture(const std::string& file, int rot, bool mirrorY) {
    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);
    glTextureParameteri(_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

    _data = loadImage(file, _width, _height);
    if (_width == _height) {
        if (rot == -1) {
            std::vector<uchar> copy = std::vector<uchar>(_data);
            for (size_t i = 0; i < _height; i++) {
                for (size_t j = 0; j < _width; j++) {
                    for (size_t t = 0; t < 3; t++) {
                        _data[j * 3 + t + i * _width * 3] =
                            copy[i * 3 + t + (_width - 1 - j) * _width * 3];
                    }
                }
            }
        }
        if (rot == 1) {
            std::vector<uchar> copy = std::vector<uchar>(_data);
            for (size_t i = 0; i < _height; i++) {
                for (size_t j = 0; j < _width; j++) {
                    for (size_t t = 0; t < 3; t++) {
                        _data[j * 3 + t + i * _width * 3] =
                            copy[(_width - 1 - i) * 3 + t + j * _width * 3];
                    }
                }
            }
        }
    }
    if (mirrorY) {
        for (size_t i = 0; i < _height / 2; i++) {
            for (size_t j = 0; j < _width; j++) {
                for (size_t t = 0; t < 3; t++) {
                    auto tmp = _data[j * 3 + t + i * _width * 3];
                    _data[j * 3 + t + i * _width * 3] =
                        _data[j * 3 + t + (_height - 1 - i) * _width * 3];
                    _data[j * 3 + t + (_height - 1 - i) * _width * 3] = tmp;
                }
            }
        }
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 _data.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

UniformTexture::UniformTexture(glm::vec3 color) {
    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);
    glTextureParameteri(_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    _data = {uchar(color.r * 255), uchar(color.g * 255), uchar(color.b * 255)};
    _width = 1;
    _height = 1;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 _data.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}