#pragma once
#include "EngineUtility.h"

class Dx12GraphicsEngine
{
public:
	Dx12GraphicsEngine();
	~Dx12GraphicsEngine();

	/// <summary>
	/// DirectX12�̏�����
	/// </summary>
	/// <returns>�����FMYRESULT::SUCCESS ���s�FMYRESULT::FAILED</returns>
	MYRESULT Init();
};

/// ����
/// MiniEngine�̓O���[�o���ϐ��Ń|�C���^�������Ă����̂ł�낵���Ȃ�
/// ���l�̂��Ƃ�����Ȃ�V���O���g���̕������S���f�X�g���N�^��private�ɂȂ��Ă���̂łǂ�����delete�����S�z���Ȃ�