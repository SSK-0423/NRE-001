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
    float3 position : SV_Position;
    float3 normal;
    float2 uv;
};