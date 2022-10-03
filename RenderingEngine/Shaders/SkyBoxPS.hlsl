#include "SkyBoxHeader.hlsli"

TextureCube texCube : register(t0);
SamplerState smp : register(s0);

float4 main(VertexOut input) : SV_TARGET
{
    return texCube.Sample(smp, input.worldPos.xyz);
}