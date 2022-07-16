#include "MeshSampleHeader.hlsli"

cbuffer constBuff : register(b0)
{
    matrix world;
    matrix worldViewProj;
    float3 eye;
};

struct VSInput
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    output.pos = mul(worldViewProj, input.pos);
    
    float4 normal = float4(input.normal.xyz, 0.f);
    output.normal = mul(world, normal);
    
    output.uv = input.uv;
    
    output.ray = normalize(input.pos.xyz - eye);
    
    return output;
}