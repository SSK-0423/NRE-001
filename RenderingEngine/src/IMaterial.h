#pragma once

class FBXMesh;

enum class MATERIAL_TYPE
{
	PHONG,		// �t�H���̔��˃��f��
	PBR,		// �����x�[�X�����_�����O
	TYPE_NUM
};

/// <summary>
/// �}�e���A���N���X�̃C���^�[�t�F�[�X
/// �d�l:
/// �E���b�V���N���X�ɃZ�b�g�������
/// �EPhongMaterial��PBRMaterial�͕ʎd�l
/// �EIMaterial�N���X�̃Z�b�g�֐��Ń��b�V���Ƀ}�e���A����K�p����
/// �}�e���A���̒�`
/// �E�I�u�W�F�N�g�̎��������肷�����
/// �E�V�F�[�f�B���O�֘A�̐��l
/// �E�e�N�X�`�����Y��
/// </summary>
class IMaterial {
protected:
	static const size_t _dummyTextureWidth = 16;
	static const size_t _dummyTextureHeight = 16;

public:
	
	/// <summary>
	/// ���b�V���Ƀ}�e���A���K�p
	/// </summary>
	/// <param name="mesh">FBX���b�V��</param>
	virtual void ApplyMaterial(FBXMesh& mesh) = 0;
};