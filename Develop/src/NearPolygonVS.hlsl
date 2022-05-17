#include "DepthBufferSampleHeader.hlsli"

DepthSampleVSOut NearPolygonVS( float4 pos : POSITION, float2 uv : TEXCOORD )
{
    DepthSampleVSOut output;
    
    // ‰ñ“]‚³‚¹‚é
    //pos = mul(rotationY, pos);
    output.position = mul(worldViewProj, pos);
    output.uv = uv;
    
    return output;
}