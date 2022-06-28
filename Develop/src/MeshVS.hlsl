#include "MeshSampleHeader.hlsli"

// コンスタントバッファー
cbuffer material : register(b0)
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float alpha;
}

cbuffer constBuff : register(b1)
{
    matrix world;
    matrix worldViewProj;
    float3 eye;
};

struct VSInput
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    output.pos = mul(worldViewProj, input.pos);
    
    float4 normal = float4(input.normal.xyz, 0.f);
    output.normal = mul(world, normal);
    
    output.ray = normalize(input.pos.xyz - eye);
    return output;
}