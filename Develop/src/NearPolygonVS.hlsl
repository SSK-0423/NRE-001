#include "DepthBufferSampleHeader.hlsli"

// コンスタントバッファー
cbuffer constBuff : register(b0)
{
    float4x4 rotation;
}

DepthSampleVSOut NearPolygonVS( float4 pos : POSITION, float2 uv : TEXCOORD )
{
    DepthSampleVSOut output;
    
    // 回転させる
    output.position = pos;
    output.uv = uv;
    
    return output;
}