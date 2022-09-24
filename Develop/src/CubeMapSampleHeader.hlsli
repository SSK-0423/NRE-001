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
    float3 reflect : TEXCOORD0;
    float2 uv : TEXCOORD1;
};

// ïΩçsåıåπ
struct DirectionalLight
{
    float3 color;
    float3 direction;
    float3 intensity;
};
