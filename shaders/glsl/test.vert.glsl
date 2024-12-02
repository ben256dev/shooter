#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

layout(location = 0) out vec4 colorOut;

void main() {
    gl_Position = vec4(position, 1.0);
    colorOut = vec4(normal, 1.0);
}
