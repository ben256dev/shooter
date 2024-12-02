#include <metal_stdlib>
using namespace metal;

struct VertexIn {
    float2 position [[attribute(0)]];
};

struct VertexOut {
    float4 position [[position]];
};

vertex VertexOut vertex_main(VertexIn vertexIn [[stage_in]])
{
    VertexOut out;
    out.position = float4(vertexIn.position, 0.0, 1.0);
    return out;
}
