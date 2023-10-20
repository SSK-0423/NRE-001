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
    matrix v =
    {
        //float4(view._11_12_13, 0.f),
        //float4(view._21_22_23, 0.f),
        //float4(view._31_32_33, 0.f),
        float4(view._11_21_31, 0.f),
        float4(view._12_22_32, 0.f),
        float4(view._13_23_33, 0.f),
        float4(0.f, 0.f, 0.f, 1.f)
    };
    
    matrix worldViewProj = mul(viewProj, world);
    VertexOutput output;
    output.position = mul(worldViewProj, float4(input.position, 1.f));
    output.uv = mul(world, float4(input.position, 1.f));
    
    return output;
}

float4 PSMain(VertexOutput input) : SV_Target
{
    //float3 skyColor = texCube.SampleLevel(smp, input.uv, 0).rgb;
    //if (skyColor.r > 1.f || skyColor.g > 1.f || skyColor.b > 1.f)
    //{
    //    return float4(0.f, 0.f, 0.f, 1.f);
    //}
    //return float4(Reinhard(skyColor), 1.f);
    return float4(Reinhard(texCube.Sample(smp, input.uv).rgb), 1.f);
}