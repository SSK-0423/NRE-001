cbuffer Uniforms : register(b0)
{
    matrix view;
    matrix proj;
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
    float3 eyePosition : POSITION;
    float3 worldPosition : POSITION1;
};

struct PixelOutput
{
    float4 color : SV_Target0;
    float4 normal : SV_Target1;
    float4 metallicRoughness : SV_Target2;
};

VertexOutput VSMain(VertexInput input)
{
    VertexOutput output;
    matrix worldViewProj = mul(proj, mul(view, world));
    output.position = mul(worldViewProj, float4(input.position, 1.f));
    output.normal = mul(world, float4(input.normal, 0.f));
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
    output.metallicRoughness = float4(metallic, roughness, 0.f, 0.f);
    
    return output;
}