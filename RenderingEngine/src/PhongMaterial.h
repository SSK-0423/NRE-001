#pragma once
#include <string>
#include <DirectXMath.h>

#include "IMaterial.h"
#include "ConstantBuffer.h"
#include "Texture.h"

/// <summary>
/// ���b�V���̃}�e���A���N���X�@�����_�����O�G���W�����C���[
/// �d�l:
/// �E���b�V���N���X�ɃZ�b�g�������
/// �EPhongMaterial��PBRMaterial�͕ʎd�l
/// �EIMaterial�N���X�̃Z�b�g�֐��Ń��b�V���Ƀ}�e���A����K�p����
/// �}�e���A���̒�`
/// �E�I�u�W�F�N�g�̎��������肷�����
/// �E�V�F�[�f�B���O�֘A�̐��l
/// �E�e�N�X�`�����Y��
/// </summary>
class PhongMaterial : public IMaterial {
public:
	PhongMaterial();
	~PhongMaterial();
	PhongMaterial(const PhongMaterial& inst);

private:
	std::wstring _textureName;
	std::wstring _textureFolderPath;

	struct PhongMaterialBuff
	{
		DirectX::XMFLOAT4 ambient;
		DirectX::XMFLOAT4 diffuse;
		DirectX::XMFLOAT4 specular;
		float shininess;
	};
	PhongMaterialBuff _material;

	ConstantBuffer _materialCBuffer;
	MYRESULT CreateMaterialConstantBuffer(ID3D12Device& device);

	Texture _texture;
	MYRESULT CreateTexture(Dx12GraphicsEngine& graphicsEngine);

public:
	
	MYRESULT Commit();

	void ApplyMaterial(FBXMesh& mesh) override;

	inline void SetAmbient(float r, float g, float b, float factor)
	{
		DirectX::XMStoreFloat4(&_material.ambient, DirectX::XMVectorSet(r, g, b, factor));
	}

	inline void SetDiffuse(float r, float g, float b, float factor)
	{
		DirectX::XMStoreFloat4(&_material.diffuse, DirectX::XMVectorSet(r, g, b, factor));
	}

	inline void SetSpecular(float r, float g, float b, float factor)
	{
		DirectX::XMStoreFloat4(&_material.specular, DirectX::XMVectorSet(r, g, b, factor));
	}

	inline void SetShininess(float s)
	{
		_material.shininess = s;
	}

	inline void SetTextureName(std::wstring name)
	{
		_textureName = name;
	}

	inline void SetTextureFolderPath(std::wstring path)
	{
		_textureFolderPath = path;
	}
};