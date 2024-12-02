#include <metal_stdlib>
using namespace metal;

struct VertexOut {
    float4 position [[position]];
    float4 color;
};

struct FragmentOut {
    float4 color [[color(0)]];
};

fragment FragmentOut fragment_main(VertexOut in [[stage_in]])
{
    FragmentOut out;
    out.color = in.color; // Red color
    return out;
}
