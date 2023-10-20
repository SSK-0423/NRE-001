#include "ToneMapping.hlsli"

Texture2D skyMap : register(t0);
Texture2D lightedMap : register(t1);
Texture2D depthMap : register(t2);

sampler smp : register(s0);

struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

VertexOutput VSMain(uint id : SV_VertexID)
{
    float x = (id >> 1) * 2.0 - 1.0; // -1, -1, 1, 1
    float y = 1.0 - (id & 1) * 2.0; //  1, -1, 1, -1
    
    VertexOutput output;
    output.position = float4(x, y, 0, 1.f);
    
    float u = (id >> 1);
    float v = (id & 1);
    output.uv = float2(u, v);
    
    return output;
}

float4 PSMain(VertexOutput input) : SV_Target
{
    uint blend = depthMap.Sample(smp, input.uv).r;
    float3 skyColor = skyMap.Sample(smp, input.uv).rgb;
    float3 lightedColor = lightedMap.Sample(smp, input.uv).rgb;
    
    //return float4(lightedColor, 1.f);
    
    //float depth = depthMap.Sample(smp, input.uv).r;
    //return float4(depth, depth, depth, 1.f);
    
    float3 outColor = skyColor * blend + lightedColor * (1 - blend);

    return float4(outColor, 1.f);
    //return float4(Reinhard(outColor), 1.f);
}