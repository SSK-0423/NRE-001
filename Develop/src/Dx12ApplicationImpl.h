#pragma once

#include "EngineUtility.h"


class Dx12GraphicsEngine;
class AppWindow;

/// <summary>
/// DirectX12�̃A�v���P�[�V�����̃C���^�[�t�F�[�X
/// </summary>
class Dx12ApplicationImpl {
public:
	/// <summary>
	/// �A�v���P�[�V�����̏�����
	/// </summary>
	/// <param name="graphicsEngine">�O���t�B�b�N�X�G���W��</param>
	/// <param name="appWindow">�E�B���h�E</param>
	/// <returns></returns>
	virtual MYRESULT Init(Dx12GraphicsEngine& graphicsEngine, AppWindow& appWindow) = 0;

	/// <summary>
	/// �A�v���P�[�V�����̍X�V����
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// �A�v���P�[�V�����̕`�揈��
	/// </summary>
	virtual void Draw(Dx12GraphicsEngine& graphicsEngine) = 0;

	/// <summary>
	/// �A�v���P�[�V�����̏I������
	/// </summary>
	virtual void Final() = 0;
};