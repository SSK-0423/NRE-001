// コンスタントバッファー
cbuffer cbuff : register(b0)
{
    matrix worldViewProj;
    matrix mat;
};
// サンプラー
sampler smp : register(s0);
// テクスチャ
Texture2D tex : register(t0);

struct OffOut
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
};