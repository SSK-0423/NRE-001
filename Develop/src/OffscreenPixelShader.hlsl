#include "OffscreenHeader.hlsli"

float4 OffscreenPS(OffOut input) : SV_TARGET
{
    float4 texColor = tex.Sample(smp, input.uv);
    float4 output = 1.f - texColor;
    
    return texColor;
}