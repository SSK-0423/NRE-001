#pragma once
#include "EngineUtility.h"

class Dx12GraphicsEngine
{
private:
	Dx12GraphicsEngine();
	~Dx12GraphicsEngine();
	Dx12GraphicsEngine(const Dx12GraphicsEngine& inst) = delete;
	void operator=(const Dx12GraphicsEngine& inst) = delete;

public:
	/// <summary>
	/// �N���X�̃V���O���g���C���X�^���X�擾
	/// </summary>
	/// <returns>�V���O���g���C���X�^���X</returns>
	static Dx12GraphicsEngine& Instance();

	/// <summary>
	/// DirectX12�̏�����
	/// </summary>
	/// <returns>MYRESULT::SUCCESS: ���� MYRESULT::FAILED: ���s</returns>
	MYRESULT Init();
};

/// ����
/// MiniEngine�̓O���[�o���ϐ��Ń|�C���^�������Ă����̂ł�낵���Ȃ�
/// ���l�̂��Ƃ�����Ȃ�V���O���g���̕������S���f�X�g���N�^��private�ɂȂ��Ă���̂łǂ�����delete�����S�z���Ȃ�