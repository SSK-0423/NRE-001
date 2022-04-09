// 頂点シェーダーの出力
struct VertexOutput
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
};

// ピクセルシェーダーの出力
struct PixelOutput
{
    float4 outColor1 : SV_TARGET0;
    float4 outColor2 : SV_TARGET1;
};