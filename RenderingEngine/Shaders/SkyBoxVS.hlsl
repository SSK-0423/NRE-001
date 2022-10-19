#include "SkyBoxHeader.hlsli"

VertexOut main(VertexInput input)
{
    VertexOut output;
    matrix worldViewProj = mul(proj,mul(view, world));
    output.position = mul(worldViewProj, input.position);
    output.worldPos = mul(world, input.position);
    
    return output;
}