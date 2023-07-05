#version 460
#extension GL_ARB_shading_language_420pack : require
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_separate_shader_objects : require
#extension GL_ARB_explicit_uniform_location : require

layout(location = 0) uniform mat4 projMatrix;
layout(location = 1) uniform mat4 viewMatrix;
layout(location = 2) uniform mat4 modelMatrix;

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uvs;

layout(location = 0) out vec2 outUv;
layout(location = 1) out vec3 outNormal;

void main() {
    outUv = uvs;
    outNormal = normal;
    gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(pos, 1.0);
}