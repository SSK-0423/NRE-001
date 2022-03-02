#include "PolygonShaderHeader.hlsli"

VSOut PolygonVS( float4 pos : POSITION, float2 uv : TEXCOORD )
{
    VSOut data;
    data.svpos = pos;
    data.uv = uv;
    
    return data;
}