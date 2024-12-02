#version 450

layout(location = 0) out vec4 outColor;

layout(binding = 3) uniform UniformBufferObject {
   float color;
} ubo;

void main() {
    outColor = vec4(ubo.color, 0.0, 0.0, 1.0);
}
