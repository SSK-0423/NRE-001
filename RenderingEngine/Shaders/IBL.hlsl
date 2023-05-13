#include "ToneMapping.hlsli"

Texture2D colorMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D positionMap : register(t2);
Texture2D metalRoughReflectMap : register(t3);
Texture2D occlusionMap : register(t4);
Texture2D emissiveMap : register(t5);

Texture2D lightedMap : register(t6);
Texture2D dfgMap : register(t7);
TextureCube specularLD : register(t8); // HDR
TextureCube diffuseLD : register(t9); // HDR

sampler smp : register(s0);

static const int MIP_COUNT = 11;
static const float EPSILON = 0.0001f;


cbuffer Parameter : register(b0)
{
    float3 eyePos : packoffset(c0);
    float lightIntensity : packoffset(c0.w);
    bool isIblOnly : packoffset(c1);
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

// ラフネスからミップレベル計算
float RoughnessToMipLeval(float roughness, float mipCount)
{
    return (mipCount - 1.f) * roughness;
}

// オフスペキュラーを考慮した反射方向を求める
float3 GetSpecularDir(float3 N, float3 R, float roughness)
{
    float smoothness = saturate(1.f - roughness);
    float lerpFactor = smoothness * (sqrt(smoothness) + roughness);
    
    return lerp(N, R, lerpFactor);
}

// ディフューズのIBLを取得
float3 IBLDiffuse(float3 N)
{
    return Reinhard(diffuseLD.Sample(smp, N).rgb);
}
// スペキュラーのIBLを取得
float3 IBLSpecualr(float NV, float3 N, float3 R, float3 f0, float roughness, float mipCount)
{
    float a = pow(roughness, 2.f);
    float3 specDir = GetSpecularDir(N, R, a);
    
    float mipLevel = RoughnessToMipLeval(roughness, mipCount);
    float3 specLD = Reinhard(specularLD.SampleLevel(smp, specDir, mipLevel).rgb);
    
    float2 dfgUV = float2(min(roughness, 1.f - EPSILON), NV);
    // テクスチャの範囲外参照を防ぐ
    float2 DFG = dfgMap.SampleLevel(smp, dfgUV, 0.f).rg;
    
    return specLD * (f0 * DFG.r + DFG.g);
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
    float2 uv = input.uv;
    float3 color = colorMap.Sample(smp, uv).rgb;
    float3 normal = normalMap.Sample(smp, uv).rgb;
    float3 pos = positionMap.Sample(smp, uv).rgb;
    float metallic = metalRoughReflectMap.Sample(smp, uv).g;
    float roughness = max(metalRoughReflectMap.Sample(smp, uv).b, EPSILON);

    // BRDFの計算に必要な要素計算
    float3 N = normalize(normalMap.Sample(smp, uv).rgb); // 物体上の法線
    float3 V = normalize(pos - eyePos); // 視線方向
    float3 R = normalize(reflect(V, N)); // 光の反射方向

    float3 kd = color * (1.f - metallic);
    float3 ks = color * metallic;
    
    //return float4(isIblOnly, isIblOnly, isIblOnly,1.f);
    
    float3 diffuse = IBLDiffuse(N) * kd;
    // フレネル使った方が良い
    
    float3 specular = IBLSpecualr(saturate(dot(N, V)), N, R, ks, roughness, MIP_COUNT);
    
    float3 outColor = (diffuse + specular) * lightIntensity + lightedMap.Sample(smp, input.uv).rgb * (1.f - isIblOnly);
    outColor *= occlusionMap.Sample(smp, uv).r;
    outColor += emissiveMap.Sample(smp, uv).rgb;
    
    return float4(outColor, 1.f);

}