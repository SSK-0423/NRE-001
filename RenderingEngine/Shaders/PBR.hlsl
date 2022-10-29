#include "BRDF.hlsli"

Texture2D colorMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D positionMap : register(t2);
Texture2D metalRoughReflectMap : register(t3);

TextureCube texCube : register(t4);

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

struct VertexOut
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

VertexOut VSMain(uint id : SV_VertexID)
{
    float x = (id >> 1) * 2.0 - 1.0; // -1, -1, 1, 1
    float y = 1.0 - (id & 1) * 2.0; //  1, -1, 1, -1
    
    VertexOut output;
    output.position = float4(x, y, 0, 1.f);
    
    float u = (id >> 1);
    float v = (id & 1);
    output.uv = float2(u, v);
    
    return output;
}

float4 PSMain(VertexOut input) : SV_Target
{
    DirectionalLight light;
    light.color = float3(1, 1, 1);
    light.direction = float3(-10, -10, -10);
    light.intensity = 10.f;
    
    float2 uv = input.uv;
    float3 color = colorMap.Sample(smp, uv).rgb;
    float3 normal = normalMap.Sample(smp, uv).rgb;
    float metallic = metalRoughReflectMap.Sample(smp, uv).r;
    float roughness = metalRoughReflectMap.Sample(smp, uv).g;
    float useReflectin = metalRoughReflectMap.Sample(smp, uv).b;
    
    //// BRDF�̌v�Z�ɕK�v�ȗv�f�v�Z
    //float3 N = normalize(normalMap.Sample(smp, uv).rgb); // ���̏�̖@��
    //float3 L = normalize(light.direction); // ���̓��˕���
    //float3 V = normalize(eye - input.pos.xyz); // ��������
    //float3 R = normalize(reflect(L, N)); // ���̔��˕���
    //// �}�C�N���T�[�t�F�[�X��̖@��
    //// ���C�g�x�N�g���Ǝ����x�N�g���̒��ԃx�N�g��(�n�[�t�x�N�g��)
    //float3 H = normalize(V + L);
    
    //float NH = saturate(dot(N, H)); //   
    //float NV = saturate(dot(N, V)); //  
    //float NL = saturate(dot(N, L)); //  ���˕�����cos
    //float VH = saturate(dot(V, H)); //  �o�˕�����cos
    
    
    float3 diffuseColor = normalizeLambert(color);
    //float3 specularColor = CookTorrance();
    
    float3 outColor = light.color * (diffuseColor);
    
    return float4(outColor.rgb, 1.f);
}