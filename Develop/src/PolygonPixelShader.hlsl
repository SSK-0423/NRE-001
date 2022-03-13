#include "PolygonShaderHeader.hlsli"

float4 PolygonPS(VSOut input) : SV_TARGET
{
    //float4 texColor = tex.Sample(smp, input.uv);
    
    //return texColor;
    float4 pos = input.svpos;
    
    return float4(pos.x, pos.y, pos.z, 1.f);
}