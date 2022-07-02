#include "TextureMapSampleHeader.hlsli"

TexMapOut main( float4 pos : POSITION, float2 uv : TEXCOORD )
{
    TexMapOut output;
    output.pos = pos;
    output.uv = uv;
    
    return output;
}