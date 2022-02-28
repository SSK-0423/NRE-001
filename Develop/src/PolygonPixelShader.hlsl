#include "PolygonShaderHeader.hlsli"

// サンプラー
sampler smp : register(s0);
// テクスチャ
Texture2D tex : register(t0);

float4 PolygonPS(VSOut input) : SV_TARGET
{
    float4 texColor = tex.Sample(smp, input.uv);
    
    return texColor;
}