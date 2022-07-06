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
/// </summary>
class IMaterial {
public:
	
	/// <summary>
	/// ���b�V���Ƀ}�e���A���K�p
	/// </summary>
	/// <param name="mesh">FBX���b�V��</param>
	virtual void ApplyMaterial(FBXMesh& mesh) = 0;
};