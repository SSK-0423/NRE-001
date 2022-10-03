#include "SkyBoxHeader.hlsli"

struct VertexInput
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

VertexOut main(VertexInput input)
{
    VertexOut output;
    output.position = mul(worldViewProj, input.position);
    output.worldPos = mul(world, input.position);
    
    return output;
}