#include "OffscreenHeader.hlsli"

OffOut OffscreenVS(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    OffOut output;
    output.pos = pos;
    output.uv = uv;
    
    return output;
}