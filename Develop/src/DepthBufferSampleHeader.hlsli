struct DepthSampleVSOut
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

// コンスタントバッファー
cbuffer constBuff : register(b0)
{
    matrix worldViewProj;
    matrix rotationY;
}

// テクスチャ
Texture2D<float4> renderTex : register(t0);
Texture2D<float> depthTex : register(t1);

// サンプラー
sampler smp : register(s0);