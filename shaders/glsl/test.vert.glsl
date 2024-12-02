#version 450

vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
);

layout(set=0, binding=0) uniform UniformBufferObject {
   mat4 model;
   mat4 view;
   mat4 projection;
} ubo;

void main() {
    gl_Position = projection * view * model * vec4(positions[gl_VertexIndex], 0.0, 1.0);
}
