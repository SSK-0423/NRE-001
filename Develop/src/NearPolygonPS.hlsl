#include "DepthBufferSampleHeader.hlsli"

float4 NearPolygonPS(DepthSampleVSOut input) : SV_Target
{
    float2 uv = input.uv;
    return float4(uv.x, 1.f - uv.y, 1.f, 1.f);
}
