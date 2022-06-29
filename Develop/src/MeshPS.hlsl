#include "MeshSampleHeader.hlsli"
#include "../../RenderingEngine/src/PhongReflectionModel.hlsli"

// �R���X�^���g�o�b�t�@�[
cbuffer material : register(b0)
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float shiness;
}

float4 main(VSOutput input) : SV_TARGET
{
    PhongReflectionModelData data;
    // ����
    data.ray = input.ray;
    // ���C�g�̋���
    data.intensity = float3(1.f, 1.f, 1.f);
    // �I�u�W�F�N�g�\�ʂ��猩�����C�g����
    float3 lightDir = float3(1.f, -1.f, -1.f);
    data.direction = normalize(-lightDir);
    // �@��
    data.normal = normalize(input.normal);
    
    // �}�e���A�������Z�b�g
    data.diffuse = diffuse.rgb;
    data.diffuseFactor = diffuse.a;
    data.specular = specular.rgb;
    data.specularFactor = specular.a;
    data.shiness = shiness;
    data.ambient = ambient.rgb;
    data.ambientFactor = ambient.a;
    
    return PhongReflectionModel(data);
}