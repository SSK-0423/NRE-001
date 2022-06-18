#include "TextureMapSampleHeader.hlsli"

float4 main(TexMapOut input) : SV_TARGET
{
    float4 texColor = tex.Sample(smp, input.uv);
    return texColor;
}