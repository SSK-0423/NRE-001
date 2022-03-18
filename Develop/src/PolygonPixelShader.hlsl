#include "PolygonShaderHeader.hlsli"

float4 PolygonPS(VSOut input) : SV_TARGET
{
    float4 texColor = tex.Sample(smp, input.uv);
    
    return texColor;
}