#include "PolygonSampleHeader.hlsli"

PolygonSampleVSOut PolygonSampleVS(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    PolygonSampleVSOut data;
    data.position = pos;
    data.uv = uv;
    
    return data;
}