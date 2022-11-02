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

cbuffer Material : register(b2)
{
    float4 baseColor;
    float metallic;
    float roughness;
    bool useReflection;
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
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float3 reflect : TEXCOORD1;
    float3 eyePosition : POSITION;
    float3 worldPosition : POSITION1;
};

struct PixelOutput
{
    float4 color : SV_Target0;
    float4 normal : SV_Target1;
    float4 pos : SV_Target2;
    float4 metalRough : SV_Target3;
};

VertexOutput VSMain(VertexInput input)
{
    VertexOutput output;
    matrix worldViewProj = mul(viewProj, world);
    output.position = mul(worldViewProj, float4(input.position, 1.f));
    output.normal = normalize(mul(world, float4(input.normal, 0.f)));
    output.uv = input.uv;
    output.eyePosition = eyePos;
    output.worldPosition = mul(world, float4(input.position, 1.f));
    
    return output;
}

PixelOutput PSMain(VertexOutput input)
{
    PixelOutput output;
    output.color = baseColor;
    output.normal = float4(input.normal, 1.f);
    output.metalRough = float4(metallic, roughness, useReflection, 0.f);
    output.pos = float4(input.worldPosition, 1.f);
    return output;
}