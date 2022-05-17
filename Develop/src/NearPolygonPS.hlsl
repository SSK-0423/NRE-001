#include "DepthBufferSampleHeader.hlsli"

float4 NearPolygonPS(DepthSampleVSOut input) : SV_Target
{
    return float4(0.f, 1.f, 0.f, 1.f);
}
