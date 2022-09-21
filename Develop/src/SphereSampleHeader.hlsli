cbuffer constBuff : register(b0)
{
    matrix world;
    matrix worldViewProj;
    float3 eye;
};

struct VSOutput
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

// ïΩçsåıåπ
struct Light
{
    float3 direction;
};
