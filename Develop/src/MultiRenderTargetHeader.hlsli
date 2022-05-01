// サンプラー
sampler smp : register(s0);
// テクスチャ
Texture2D render1 : register(t0); // レンダーターゲット1
Texture2D render2 : register(t1); // レンダーターゲット2

struct MultiRenderOut
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
};

// １パス目のピクセルシェーダー用
struct MultiRenderFirstPixelOut
{
    float4 render1Color : SV_TARGET0;
    float4 render2Color : SV_TARGET1;
};

// 最終パスのシェーダー
struct MultiRenderiFinalVertex
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};