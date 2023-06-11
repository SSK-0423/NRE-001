#include "BRDF.hlsli"

Texture2D colorMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D positionMap : register(t2);
Texture2D occMetalRoughShadowFactMap : register(t3);
Texture2D emissiveMap : register(t4);
Texture2D depthMap : register(t5);

sampler smp : register(s0);

cbuffer Parameter : register(b0)
{
    float3 eyePos : packoffset(c0);
    int brdfModel : packoffset(c0.w);
};

cbuffer LightBuffer : register(b1)
{
    float3 dLightdirection : packoffset(c0);
    float dLightintensity : packoffset(c0.w);
    float4 dLightColor : packoffset(c1);
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
    float2 uv = input.uv;
    float3 color = colorMap.Sample(smp, uv).rgb;
    float3 normal = normalMap.Sample(smp, uv).rgb * 2.f - 1.f;
    float3 pos = positionMap.Sample(smp, uv).rgb;
    
    float occlusion = occMetalRoughShadowFactMap.Sample(smp, uv).r;
    float roughness = occMetalRoughShadowFactMap.Sample(smp, uv).g;
    float metallic = occMetalRoughShadowFactMap.Sample(smp, uv).b;
    float shadowFactor = occMetalRoughShadowFactMap.Sample(smp, uv).a;
    
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
    float3 outColor = dLightColor.rgb * (diffuseColor + specularColor) * dot(N, L) * dLightintensity * occlusion;
    
    //return float4((normal + 1.f)/2.f, 1.f);
    
    //float depth = depthMap.Sample(smp, uv).r;
    //return float4(depth, depth, depth, 1.f);
    
    return float4(outColor, 1.f);
}
    //float NH = saturate(dot(N, H)); //   
    //float NV = saturate(dot(N, V)); //  
    //float NL = saturate(dot(N, L)); //  入射方向のcos
    //float VH = saturate(dot(V, H)); //  出射方向のcos