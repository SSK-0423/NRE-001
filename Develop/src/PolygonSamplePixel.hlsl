#include "PolygonSampleHeader.hlsli"

float4 PolygonSamplePS(PolygonSampleVSOut input) : SV_TARGET
{
    float2 uv = input.uv;
    float4 color = float4(1.f - (uv.x + uv.y), uv.xy, 1.f);
    return color;
}