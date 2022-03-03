#include "PolygonShaderHeader.hlsli"

VSOut PolygonVS( float4 pos : POSITION, float2 uv : TEXCOORD )
{
    VSOut data;
    data.svpos = mul(worldViewProj, mul(mat, pos));
    data.uv = uv;
    
    return data;
}