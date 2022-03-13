#include "OffscreenHeader.hlsli"

float4 OffscreenPS(OffOut input) : SV_TARGET
{
    float4 texColor = tex.Sample(smp, input.uv);
    
    return texColor;
}