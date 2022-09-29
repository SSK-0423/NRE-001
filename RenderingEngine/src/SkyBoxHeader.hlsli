#include "ShaderCommon.hlsli"

struct VertexOut
{
    float4 position : SV_POSITION;
    float4 worldPos : TEXCOORD;
};