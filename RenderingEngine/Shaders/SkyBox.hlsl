#include "ToneMapping.hlsli"

TextureCube texCube : register(t0);

sampler smp : register(s0);

cbuffer Uniforms : register(b0)
{
    matrix view;
    matrix proj;
    matrix viewProj;
    float3 eyePos;
};

cbuffer Transform : register(b1)
{
    matrix world;
};

struct VertexInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float3 uv : TEXCOORD0;
};

VertexOutput VSMain(VertexInput input)
{
    matrix worldViewProj = mul(viewProj, world);
    VertexOutput output;
    output.position = mul(worldViewProj, float4(input.position, 1.f));
    output.uv = mul(world, float4(input.position, 1.f));
    
    return output;
}

float4 PSMain(VertexOutput input) : SV_Target
{
    float3 skyColor = texCube.Sample(smp, input.uv).rgb;
    //if (skyColor.r > 1.f || skyColor.g > 1.f || skyColor.b > 1.f)
    //{
    //    return float4(0.f, 0.f, 0.f, 1.f);
    //}
    return float4(Reinhard(skyColor), 1.f);
    return float4(texCube.Sample(smp, input.uv));
}