#include "DepthBufferSampleHeader.hlsli"

DepthSampleVSOut FarPolygonVS(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    DepthSampleVSOut output;
    
    output.position = mul(worldViewProj,pos);
    output.uv = uv;
    
    return output;
}