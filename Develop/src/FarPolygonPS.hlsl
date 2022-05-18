#include "DepthBufferSampleHeader.hlsli"

float4 FarPolygonPS(DepthSampleVSOut input) : SV_TARGET
{
    float2 uv = input.uv;
    return float4(1.f, 0.f, 0.f, 1.f);
    return float4(1 - uv.x, uv.y, 1.f, 1.f);
}
