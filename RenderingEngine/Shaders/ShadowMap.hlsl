Texture2D baseColorMap : register(t0);
Texture2D metalRoughMap : register(t1);
Texture2D normalMap : register(t2);
Texture2D occlusionMap : register(t3);
Texture2D emissiveMap : register(t4);

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

cbuffer LightUniform : register(b2)
{
    matrix lightViewProj;
};

struct VertexInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float4 tangent : TANGENT;
    float2 uv : TEXCOORD;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

VertexOutput VSMain(VertexInput input)
{
    VertexOutput output;
    
    matrix worldViewProj = mul(viewProj, world);
    output.position = mul(worldViewProj, float4(input.position, 1.f));
    output.uv = input.uv;
    
    return output;
}

float4 PSMain(VertexOutput input) : SV_Target
{
    return baseColorMap.Sample(smp, input.uv);
}