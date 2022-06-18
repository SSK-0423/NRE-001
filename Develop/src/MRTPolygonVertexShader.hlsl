#include "MRTHeader.hlsli"

VertexOutput main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    VertexOutput output;
    output.svpos = pos;
    output.uv = uv;
    
    return output;
}