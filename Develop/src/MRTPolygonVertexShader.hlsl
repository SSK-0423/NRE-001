#include "MRTHeader.hlsli"

VertexOutput MrtPolygonVS(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    VertexOutput output;
    output.svpos = pos;
    output.uv = uv;
    
    return output;
}