struct TexMapOut
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

// サンプラー
sampler smp : register(s0);

// テクスチャ
Texture2D tex : register(t0);