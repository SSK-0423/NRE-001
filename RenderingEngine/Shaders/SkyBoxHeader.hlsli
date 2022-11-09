#include "ShaderCommon.hlsli"

TextureCube texCube : register(t0);
SamplerState smp : register(s0);

struct VertexOut
{
    float4 position : SV_POSITION;
    float4 worldPos : TEXCOORD;
};