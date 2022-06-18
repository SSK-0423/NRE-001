#include "DepthBufferSampleHeader.hlsli"

DepthSampleVSOut main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    DepthSampleVSOut output;
    output.position = pos;
    output.uv = uv;
    
    return output;
}