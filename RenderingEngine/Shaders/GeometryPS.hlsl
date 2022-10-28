#include "ShaderCommon.hlsli"

struct DirectionalLight
{
    float3 color;
    float3 direction;
};

cbuffer Material : register(b2)
{
    float4 baseColor;
    float metallic;
    float roughness;
};

float4 main(VertexOutput input) : SV_TARGET
{
    DirectionalLight light;
    light.color = float3(1, 1, 1);
    light.direction = normalize(float3(1, -1, 1));
    
    float3 diffuse = float3(0.69, 0.69, 0.69);
    float3 normal = normalize(input.normal);
    
    float3 diffuseColor = light.color * diffuse * saturate(dot(-light.direction, normal));
    
    float3 outColor = diffuseColor;
    outColor = baseColor.rgb;
    
    return float4(outColor, 1.f);
}