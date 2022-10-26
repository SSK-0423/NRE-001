#include "ShaderCommon.hlsli"

VertexOutput main(VertexInput input)
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