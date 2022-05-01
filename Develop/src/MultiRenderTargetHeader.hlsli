// サンプラー
sampler smp : register(s0);
// テクスチャ
Texture2D render1 : register(t0);   // レンダーターゲット1
Texture2D render2 : register(t1);   // レンダーターゲット2


struct MultiRenderOut
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
};