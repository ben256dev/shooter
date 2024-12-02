#version 450

layout(location = 0) in vec3 position;

layout(location = 0) out vec4 colorOut;

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
} ubo;

void main() {
    gl_Position = ubo.projection * ubo.view * ubo.model * vec4(position, 1.0);
    colorOut = vec4(position, 1.0); // Example: pass the position as color
}
