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