#include "SkyBoxHeader.hlsli"

VertexOut main(VertexInput input)
{
    VertexOut output;
    output.position = mul(worldViewProj, input.position);
    output.worldPos = mul(world, input.position);
    
    return output;
}