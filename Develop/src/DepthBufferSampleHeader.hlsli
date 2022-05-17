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