#include "OffscreenHeader.hlsli"

OffOut main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    OffOut output;
    output.svpos = pos;
    output.uv = uv;
    
    return output;
}