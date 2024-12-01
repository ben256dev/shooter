#include <metal_stdlib>
using namespace metal;

struct VertexOut {
    float4 position [[position]];
};

constant float2 positions[3] = { float2(0.0, -0.5), float2(0.5, 0.5), float2(-0.5, 0.5) };

vertex VertexOut vertex_main(uint vertexID [[vertex_id]])
{
    VertexOut out;
    out.position = float4(positions[vertexID], 0.0, 1.0);
    return out;
}
