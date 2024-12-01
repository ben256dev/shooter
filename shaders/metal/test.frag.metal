#include <metal_stdlib>
using namespace metal;

struct FragmentOut {
    float4 color [[color(0)]];
};

fragment FragmentOut fragment_main()
{
    FragmentOut out;
    out.color = float4(1.0, 0.0, 0.0, 1.0); // Red color
    return out;
}
