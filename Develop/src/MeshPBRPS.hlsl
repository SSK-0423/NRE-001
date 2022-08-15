#include "MeshSampleHeader.hlsli"
#include "../../RenderingEngine/src/PBR.hlsli"

float4 main(VSOutput input) : SV_TARGET
{
    return baseColor.Sample(smp, input.uv);
}