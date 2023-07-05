#version 460
#extension GL_ARB_shading_language_420pack : require
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_separate_shader_objects : require

layout(location = 0) uniform mat4 projMatrix;
layout(location = 1) uniform mat4 viewMatrix;
layout(location = 2) uniform mat4 modelMatrix;

uniform sampler2D tex;

layout(location = 0) in vec2 uvs;
layout(location = 1) in vec3 normal;

layout(location = 0) out vec4 fragColor;

void main() {
    fragColor = vec4(texture(tex, uvs));
}
