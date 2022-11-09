#include "SkyBoxHeader.hlsli"

float4 main(VertexOut input) : SV_TARGET
{
    return texCube.Sample(smp, input.worldPos.xyz);
}