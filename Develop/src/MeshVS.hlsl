// コンスタントバッファー
cbuffer constBuff : register(b0)
{
    matrix worldViewProj;
};

struct VSInput
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
};

float4 main(VSInput input) : SV_POSITION
{
    return mul(worldViewProj, input.pos);
}