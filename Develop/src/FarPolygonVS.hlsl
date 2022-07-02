#include "DepthBufferSampleHeader.hlsli"

DepthSampleVSOut main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    DepthSampleVSOut output;
    
    output.position = mul(worldViewProj, mul(rotationY, pos));
    output.position = mul(worldViewProj, pos);
    output.uv = uv;
    
    return output;
}