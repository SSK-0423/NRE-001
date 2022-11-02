TextureCube texCube : register(t0);
Texture2D lightedMap : register(t1);

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
    return float4(texCube.Sample(smp, input.uv));
}