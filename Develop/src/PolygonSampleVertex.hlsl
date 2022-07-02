#include "PolygonSampleHeader.hlsli"

PolygonSampleVSOut main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    PolygonSampleVSOut data;
    data.position = pos;
    data.uv = uv;
    
    return data;
}