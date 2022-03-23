#include "MRTHeader.hlsli"

PixelOutput MrtPS(VertexOutput input)
{
    PixelOutput output;
    output.outColor1 = float4(1.f, 1.f, 0.f, 1.f);
    output.outColor2 = float4(0.f, 1.f, 1.f, 1.f);
    
    return output;
}