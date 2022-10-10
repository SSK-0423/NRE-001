#pragma once

#include "NamelessEngine.h"

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
	virtual NamelessEngine::Utility::RESULT Init(
		NamelessEngine::Core::Dx12GraphicsEngine& graphicsEngine,
		NamelessEngine::Core::AppWindow& appWindow) = 0;

	/// <summary>
	/// �A�v���P�[�V�����̍X�V����
	/// </summary>
	virtual void Update(float deltaTime) = 0;

	/// <summary>
	/// �A�v���P�[�V�����̕`�揈��
	/// </summary>
	virtual void Draw(NamelessEngine::Core::Dx12GraphicsEngine& graphicsEngine) = 0;

	/// <summary>
	/// �A�v���P�[�V�����̏I������
	/// </summary>
	virtual void Final() = 0;
};