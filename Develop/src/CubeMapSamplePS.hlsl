#include "CubeMapSampleHeader.hlsli"

TextureCube texCube : register(t0);
SamplerState smp : register(s0);


// 拡散反射
float3 lambert(DirectionalLight light, float3 normal, float3 diffuseColor)
{
    return diffuseColor * dot(light.direction, normal) * light.color;
}

// 鏡面反射
float3 phongSpecular(DirectionalLight light, float3 normal, float3 specularColor, float3 eyeDir, float alpha)
{
    float3 specular = float3(0, 0, 0);
    float3 ref = reflect(light.direction, normal);
    ref = normalize(ref);
    
    // 視線ベクトルと反射ベクトルのなす角のn乗
    return specularColor * pow(saturate(dot(eyeDir, ref)), alpha) * light.color;
}

float3 ambient(float ambientIntensity, float3 ambientColor)
{
    return ambientIntensity * ambientColor;
}

float4 main(VSOutput input) : SV_TARGET
{
    DirectionalLight light;
    light.direction = normalize(float3(1, 1, -1));
    light.intensity = 1.0f;
    light.color = float3(1, 1, 1);
    
    float3 normal = normalize(input.normal);
    float3 eyePos = float3(0, 0, 0);
    float3 eyeDir = input.pos.xyz - eyePos;
    eyeDir = normalize(eyeDir);
    
    float3 diffuse = lambert(light, normal, float3(0.69, 0.69, 0.69));
    float3 specular = phongSpecular(light, normal, float3(0.3, 0.3, 0.3), eyeDir, 2);
    float3 ambientColor = ambient(0.1, float3(0.01, 0.01, 0.01));
    
    float3 outColor = light.intensity * (diffuse + specular) + ambientColor;
    
    // 環境マッピング
    return texCube.Sample(smp, input.reflect);

    return texCube.Sample(smp, input.worldPos.xyz);
    
    return float4(outColor, 1.f);
}