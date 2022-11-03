#include "BRDF.hlsli"

Texture2D colorMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D positionMap : register(t2);
Texture2D metalRoughReflectMap : register(t3);
Texture2D depthMap : register(t4);

TextureCube texCube : register(t5);

sampler smp : register(s0);

cbuffer Parameter : register(b0)
{
    float3 eyePos;
};

struct DirectionalLight
{
    float3 color;
    float3 direction;
    float intensity;
};

struct PointLight
{
    float3 pos;
    float3 color;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

// 拡散反射
float3 lambert(float3 lightDir, float3 lightColor, float3 normal, float3 diffuseColor)
{
    return diffuseColor * saturate(dot(lightDir, normal)) * lightColor;
}

// 鏡面反射
float3 phongSpecular(float3 lightDir, float3 lightColor, float3 normal, float3 specularColor, float3 eyeDir, float alpha)
{
    float3 ref = -reflect(lightDir, normal);
    ref = normalize(ref);
    
    // 視線ベクトルと反射ベクトルのなす角のn乗
    return specularColor * pow(saturate(dot(eyeDir, ref)), alpha) * lightColor;
}

float3 ambient(float ambientIntensity, float3 ambientColor)
{
    return ambientIntensity * ambientColor;
}

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
    float divY = 6;
    float step = 1.f / divY;
    
    // デバッグ情報表示
    // カラー
    //if (input.uv.x < step)
    //{
    //    if (input.uv.y < step)
    //    {
    //        float3 color = colorMap.Sample(smp, input.uv * divY).rgb;
    //        if (color.r == 0 && color.g == 0 && color.b == 0)
    //        {
    //            return float4(0.5, 0.5, 0.5, 1.f);
    //        }
    //        return float4(color, 1.f);
    //    }
    //    // 法線
    //    if (input.uv.y < step * 2 && input.uv.y > step)
    //    {
    //        float3 color = normalMap.Sample(smp, input.uv * divY).rgb;
    //        if (color.r == 0 && color.g == 0 && color.b == 0)
    //        {
    //            return float4(0.5, 0.5, 0.5, 1.f);
    //        }
    //        return float4(color, 1.f);
    //    }
    //    // 位置
    //    if (input.uv.y < step * 3 && input.uv.y > step * 2)
    //    {
    //        float3 color = positionMap.Sample(smp, input.uv * divY).rgb;
    //        if (color.r == 0 && color.g == 0 && color.b == 0)
    //        {
    //            return float4(0.5, 0.5, 0.5, 1.f);
    //        }
    //        return float4(color, 1.f);
    //    }
    //    // メタリック・ラフネス...
    //    if (input.uv.y < step * 4 && input.uv.y > step * 3)
    //    {
    //        float color = metalRoughReflectMap.Sample(smp, input.uv * divY).r;
    //        return float4(color, color, color, 1.f);
    //    }
    //    if (input.uv.y < step * 5 && input.uv.y > step * 4)
    //    {
    //        float color = metalRoughReflectMap.Sample(smp, input.uv * divY).g;
    //        return float4(color, color, color, 1.f);
    //    }
    //    if (input.uv.y < step * 6 && input.uv.y > step * 5)
    //    {
    //        float color = depthMap.Sample(smp, input.uv * divY).r;
    //        return float4(color, color, color, 1.f);
    //    }
    //}
    
    DirectionalLight light;
    light.direction = normalize(float3(1, 1, -1));
    light.intensity = 5.0f;
    light.color = float3(1, 1, 1);
    
    PointLight pointLight;
    pointLight.pos = float3(0, 50, 100);
    pointLight.color = float3(1, 1, 1);
    
    float2 uv = input.uv;
    float3 color = colorMap.Sample(smp, uv).rgb;
    float3 normal = normalMap.Sample(smp, uv).rgb;
    float3 pos = positionMap.Sample(smp, uv).rgb;
    float metallic = metalRoughReflectMap.Sample(smp, uv).r;
    float roughness = metalRoughReflectMap.Sample(smp, uv).g;
    float useReflection = metalRoughReflectMap.Sample(smp, uv).b;

    // BRDFの計算に必要な要素計算
    float3 N = normalize(normalMap.Sample(smp, uv).rgb); // 物体上の法線
    float3 L = normalize(light.direction); // 光の入射方向
    float3 V = normalize(eyePos - pos); // 視線方向
    float3 R = normalize(reflect(L, N)); // 光の反射方向
    // マイクロサーフェース上の法線
    // ライトベクトルと視線ベクトルの中間ベクトル(ハーフベクトル)
    float3 H = normalize(V + L);

    float3 cubeUV = -reflect(V, normal);
    float3 skyLight = texCube.Sample(smp, cubeUV);
    
    float3 diffuseColor = saturate(normalizeLambert(color) * (1.f - metallic));
    float3 specularColor = saturate(CookTorrance(color, metallic, roughness, uv, N, H, V, L));
    // Li(x,ω) * BRDF * cosθ
    float3 outColor = light.color.rgb * (diffuseColor + specularColor) * dot(N, L) * light.intensity;
    
    return float4(outColor, 1.f);
}
    //float NH = saturate(dot(N, H)); //   
    //float NV = saturate(dot(N, V)); //  
    //float NL = saturate(dot(N, L)); //  入射方向のcos
    //float VH = saturate(dot(V, H)); //  出射方向のcos