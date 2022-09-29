#include "CubeMapSampleHeader.hlsli"

struct VSInput
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    output.pos = mul(worldViewProj, input.pos);
    output.worldPos = mul(world, input.pos);
    
    float4 normal = float4(input.normal.xyz, 0.f);
    output.normal = mul(world, normal);
    output.uv = input.uv;
    
    float3 eyePos = eye;
    float3 eyeDir = normalize(output.worldPos.xyz - eyePos);
    
    output.reflect = reflect(eyeDir, normalize(output.normal));
    output.reflect = normalize(output.reflect);
    
    return output;
}