#include "DepthBufferSampleHeader.hlsli"

float4 SpritePS(DepthSampleVSOut input) : SV_TARGET
{
    //float depth = depthTex.Sample(smp, input.uv);
    //return float4(depth, depth, depth, 1.f);
    return renderTex.Sample(smp, input.uv);
    return float4(1.f, 1.f, 1.f, 1.f);
}