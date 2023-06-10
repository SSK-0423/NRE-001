#include "BRDF.hlsli"

Texture2D colorMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D positionMap : register(t2);
Texture2D metalRoughReflectMap : register(t3);
Texture2D occlusionMap : register(t4);
Texture2D emissiveMap : register(t5);
Texture2D depthMap : register(t6);

sampler smp : register(s0);

cbuffer Parameter : register(b0)
{
    float3 eyePos : packoffset(c0);
    int brdfModel : packoffset(c0.w);
};

cbuffer LightBuffer : register(b1)
{
    float3 dLightColor : packoffset(c0);
    float dLightintensity : packoffset(c0.w);
    float3 dLightdirection : packoffset(c1);
};

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
    
    //directionalLight.direction = normalize(float3(1, 1, -1));
    //directionalLight.intensity = 5.0f;
    //directionalLight.color = float3(1, 1, 1);
    
    float2 uv = input.uv;
    float3 color = colorMap.Sample(smp, uv).rgb;
    float3 normal = normalMap.Sample(smp, uv).rgb * 2.f - 1.f;
    float3 pos = positionMap.Sample(smp, uv).rgb;
    float metallic = metalRoughReflectMap.Sample(smp, uv).b;
    float roughness = metalRoughReflectMap.Sample(smp, uv).g;
    float useReflection = metalRoughReflectMap.Sample(smp, uv).r;

    // BRDFの計算に必要な要素計算
    float3 N = normalize(normal); // 物体上の法線
    float3 L = normalize(normalize(dLightdirection)); // 光の入射方向
    float3 V = normalize(eyePos - pos); // 視線方向
    float3 R = normalize(-reflect(L, N)); // 光の反射方向
    // マイクロサーフェース上の法線
    // ライトベクトルと視線ベクトルの中間ベクトル(ハーフベクトル)
    float3 H = normalize(V + L);
    
    float3 diffuseColor = saturate(normalizeLambert(color) * (1.f - metallic));
    
    float3 specularColor = float3(0.f, 0.f, 0.f);
    if (brdfModel == COOK_TORRANCE)
    {
        specularColor = saturate(CookTorrance(color, metallic, roughness, uv, N, H, V, L));
    }
    else if (brdfModel == GGX_MODEL)
    {
        specularColor = saturate(GGXModel(color, metallic, roughness, uv, N, H, V, L));
    }
    // Li(x,ω) * BRDF * cosθ
    float3 outColor = dLightColor * (diffuseColor + specularColor) * dot(N, L) * dLightintensity;
    
    //return float4((normal + 1.f)/2.f, 1.f);
    
    return float4(outColor, 1.f);
}
    //float NH = saturate(dot(N, H)); //   
    //float NV = saturate(dot(N, V)); //  
    //float NL = saturate(dot(N, L)); //  入射方向のcos
    //float VH = saturate(dot(V, H)); //  出射方向のcos