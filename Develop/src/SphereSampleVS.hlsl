#include "SphereSampleHeader.hlsli"

struct VSInput
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    output.pos = input.pos;
    output.normal = input.normal;
    output.uv = input.uv;
    
    return output;
}