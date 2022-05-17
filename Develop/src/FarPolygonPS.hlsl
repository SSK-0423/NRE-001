#include "DepthBufferSampleHeader.hlsli"

float4 FarPolygonPS(DepthSampleVSOut input) : SV_TARGET
{
    return float4(1.f, 0.f, 0.f, 1.f);
}
