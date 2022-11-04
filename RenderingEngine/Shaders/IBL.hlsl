#include "ToneMapping.hlsli"

Texture2D colorMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D positionMap : register(t2);
Texture2D metalRoughReflectMap : register(t3);

Texture2D lightedMap : register(t4);
TextureCube specularLD : register(t5); // HDR
TextureCube diffuseLD : register(t6); // HDR
Texture2D dfgMap : register(t7);

sampler smp : register(s0);

static const int MIP_COUNT = 3;

cbuffer Parameter : register(b0)
{
    float3 eyePos;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

// ���t�l�X����~�b�v���x���v�Z
float RoughnessToMipLeval(float roughness, float mipCount)
{
    return (mipCount - 1.f) * roughness;
}

// �I�t�X�y�L�����[���l���������˕��������߂�
float3 GetSpecularDir(float3 N, float3 R, float roughness)
{
    float smoothness = saturate(1 - roughness);
    float lerpFactor = smoothness * (sqrt(smoothness) + roughness);
    
    return lerp(N, R, lerpFactor);
}

// �f�B�t���[�Y��IBL���擾
float3 IBLDiffuse(float3 N)
{
    return Reinhard(diffuseLD.Sample(smp, N).rgb);
}
// �X�y�L�����[��IBL���擾
float3 IBLSpecualr(float NV, float3 N, float3 R, float3 f0, float roughness, float mipCount)
{
    float a = roughness * roughness;
    float3 specDir = GetSpecularDir(N, R, roughness);
    
    float mipLevel = RoughnessToMipLeval(roughness, mipCount);
    float3 specLD = Reinhard(specularLD.SampleLevel(smp, specDir, mipLevel).rgb);
    
    // �e�N�X�`���͈̔͊O�Q�Ƃ�h��
    float2 DFG = dfgMap.SampleLevel(smp, float2(saturate(NV), roughness), 0.f);
    
    return float3(DFG, 0.f);
    return specLD * (f0 * DFG.x + DFG.y);
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
    float metallic = metalRoughReflectMap.Sample(smp, uv).r;
    float roughness = metalRoughReflectMap.Sample(smp, uv).g;
    float useReflection = metalRoughReflectMap.Sample(smp, uv).b;

    // BRDF�̌v�Z�ɕK�v�ȗv�f�v�Z
    float3 N = normalize(normalMap.Sample(smp, uv).rgb); // ���̏�̖@��
    float3 V = normalize(eyePos - pos); // ��������
    float3 R = normalize(-reflect(V, N)); // ���̔��˕���

    float3 kd = color * (1.f - metallic);
    float3 ks = color * metallic;
    
    float3 diffuse = IBLDiffuse(N) * kd;
    float3 specular = IBLSpecualr(dot(N, V), N, R, ks, roughness, MIP_COUNT);
    
    float3 outColor = specular; /*+ lightedMap.Sample(smp, input.uv).rgb*/;
    
    return float4(dfgMap.Sample(smp, input.uv).rgb, 1.f);
    return float4(outColor, 1.f);

}