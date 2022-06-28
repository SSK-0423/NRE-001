#include "MeshSampleHeader.hlsli"

// �R���X�^���g�o�b�t�@�[
cbuffer material : register(b0)
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float alpha;
}

float4 PhongModelWithMaterial(VSOutput input)
{
    // ���C�g
    float3 intensity = float3(1.f, 1.f, 1.f);
    float3 lightPos = float3(0.f, 5.f, -2.f);
    
    // �I�u�W�F�N�g�\�ʂ��猩�����C�g����
    float3 direction = normalize(lightPos - input.pos.xyz);
    
    // �@��
    float3 normal = normalize(input.normal);
    
	// �t�H���̔��˃��f��
    // �g�U����
    float3 lambertColor = intensity * diffuse.rgb * dot(direction, normal);
    
	// ���ʔ���
    float shiness = 8.f;
    float3 refLight = normalize(reflect(direction, normal));
    // ���ςȂ̂Ō��������킹��
    float3 specularColor = intensity * specular.rgb * pow(saturate(dot(refLight, -input.ray)), shiness);
    
    
    // ����
    float3 Ia = float3(1.f, 1.f, 1.f);
    float3 ambientColor = Ia * ambient.rgb;
    
    float4 color = float4(lambertColor + specularColor + ambientColor, 1.f);
    return color;
}

float4 PhongModelWithoutMaterial(VSOutput input)
{
    // ���C�g
    float3 intensity = float3(1.f, 1.f, 1.f);
    float3 lightPos = float3(0.f, 5.f, -2.f);
    
    // �I�u�W�F�N�g�\�ʂ��猩�����C�g����
    float3 direction = normalize(lightPos - input.pos.xyz);
    
    // �@��
    float3 normal = normalize(input.normal);
    
	// �t�H���̔��˃��f��
    // �g�U����
    float3 diffuse = float3(0.5f, 0.5f, 0.5f);
    float3 lambertColor = intensity * diffuse * dot(direction, normal);
    
	// ���ʔ���
    float3 specular = float3(0.5f, 0.5f, 0.5f);
    float shiness = 8.f;
    float3 refLight = normalize(reflect(direction, normal));
    // ���ςȂ̂Ō��������킹��
    float3 specularColor = intensity * specular * pow(saturate(dot(refLight, -input.ray)), shiness);
    
    
    // ����
    float3 ambient = float3(0.3f, 0.3f, 0.3f);
    float3 Ia = float3(1.f, 1.f, 1.f);
    float3 ambientColor = Ia * ambient;
    
    float4 color = float4(lambertColor + specularColor + ambientColor, 1.f);
    return color;
}
float4 main(VSOutput input) : SV_TARGET
{
    return PhongModelWithMaterial(input);
}