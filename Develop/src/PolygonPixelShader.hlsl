#include "PolygonShaderHeader.hlsli"

float4 PolygonPS(VSOut input) : SV_TARGET
{
    float4 texColor = tex.Sample(smp, input.uv);
    
    return texColor;
    return float4(normalize(input.svpos.xy), 1.f, 1.f);
}