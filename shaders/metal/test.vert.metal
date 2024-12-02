#include <metal_stdlib>
using namespace metal;

struct VertexIn {
    float3 position [[attribute(0)]];
    float2 uv [[attribute(1)]];
    float3 normal [[attribute(2)]];
};

struct VertexOut {
    float4 position [[position]];
    float4 color;
};

vertex VertexOut vertex_main(VertexIn vertexIn [[stage_in]])
{
    VertexOut out;
    out.position = float4(vertexIn.position, 1.0);
    out.color = float4(vertexIn.position, 1.0);
    return out;
}
