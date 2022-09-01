#include "PBRSampleHeader.hlsli"
#include "../../RenderingEngine/src/PBR.hlsli"

struct DemoLight
{
};

float4 main(VSOutput input) : SV_TARGET
{
    float3 lightPos = float3(100, 1000, 0);
    float3 lightColor = float3(3, 3, 3);
    
    float3 N = normalize(input.normal);             // ���̏�̖@��
    float3 L = normalize(lightPos - input.pos.xyz); // ���̓��˕���
    float3 V = normalize(eye - input.pos.xyz);      // ��������
    float3 R = normalize(reflect(L, N));            // ���̔��˕���
    // �}�C�N���T�[�t�F�[�X��̖@��
    // ���C�g�x�N�g���Ǝ����x�N�g���̒��ԃx�N�g��(�n�[�t�x�N�g��)
    float3 H = normalize(V + L);
    
    float NH = saturate(dot(N, H)); //   
    float NV = saturate(dot(N, V)); //  
    float NL = saturate(dot(N, L)); //  ���˕�����cos
    float VH = saturate(dot(V, H)); //  �o�˕�����cos
    
    PBRInput pbrInput;
    pbrInput.NH = NH;
    pbrInput.NV = NV;
    pbrInput.NL = NL;
    pbrInput.VH = VH;
    pbrInput.uv = input.uv;
    pbrInput.lightColor = lightColor;
    
    return PBR(pbrInput);
}

/*
CookTorrance���f����������
�}�C�N���T�[�t�F�[�X�̖@���x�N�g�� : ���˕����x�N�g���Əo�˕����x�N�g���̒��Ԃɂ���n�[�t�x�N�g��
*/