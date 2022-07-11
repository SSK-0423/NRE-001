#pragma once
#include <string>
#include <unordered_map>

#include "IMaterial.h"
#include "Texture.h"

/// <summary>
/// PBR�Ή��}�e���A���N���X
/// </summary>
class PBRMaterial : public IMaterial {
public:
	PBRMaterial();
	~PBRMaterial();

private:
	std::wstring _textureFolderPath = L"";

	enum class TEXTURETYPE
	{
		BASE_COLOR = 0,
		METALLIC,
		ROUGHNESS,
		NORMAL,
		OCCULUSION,	// �A���r�G���g�I�N���[�W����
		TYPE_NUM
	};

	std::unordered_map<TEXTURETYPE, std::wstring> _pbrTexturePaths;
	std::unordered_map<TEXTURETYPE, Texture*> _pbrTextures;

	Texture _dummyTexture;	// �e�N�X�`�����w�肳��Ă��Ȃ��ꍇ�̃_�~�[�p�e�N�X�`��

	/// <summary>
	/// �e�N�X�`�����w�肳��Ă��Ȃ��ꍇ��
	/// �_�~�[�ƂȂ�^�����ȃe�N�X�`������
	/// </summary>
	/// <returns></returns>
	MYRESULT CreateDammyTexture(Dx12GraphicsEngine& graphicsEngine);

	MYRESULT LoadPBRTextures(Dx12GraphicsEngine& graphicsEngine);

public:
	/// <summary>
	/// �}�e���A���𗘗p�\�ȏ�Ԃɂ���
	/// </summary>
	/// <returns></returns>
	MYRESULT Commit();

	void ApplyMaterial(FBXMesh& mesh) override;

	/// <summary>
	/// �ǂݍ��݂����e�N�X�`�����u���Ă���t�H���_�ւ̃p�X
	/// </summary>
	/// <param name="folderPath"></param>
	void SetTextureFolderPath(std::wstring folderPath);

	/// <summary>
	/// �ǂݍ��݂����e�N�X�`���̃t�@�C����(�g���q�t��)�Z�b�g
	/// </summary>
	/// <param name="type">�e�N�X�`���̎��(�x�[�X�J���[�E���^���b�N�E���t�l�X�Ȃ�)</param>
	/// <param name="path"></param>
	void SetTextureName(TEXTURETYPE type, std::wstring path);
};