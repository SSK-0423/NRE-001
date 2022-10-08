#pragma once

#include <Windows.h>
#include <memory>

#include "Dx12ApplicationImpl.h"

/// <summary>
/// �����_�����O�J�����̃e�X�g�p�N���X
/// </summary>
class Dx12Application
{
public:
	Dx12Application(Dx12ApplicationImpl& applicationImpl);
	~Dx12Application();

	/// <summary>
	/// �A�v���P�[�V�����̏�����
	/// </summary>
	/// <returns>�����FMYRESULT::SUCCESS ���s�FMYRESULT::FAILED</returns>
	NamelessEngine::Utility::MYRESULT Init();

	/// <summary>
	/// ���C�����[�v
	/// </summary>
	void Run();

	/// <summary>
	/// �I������
	/// </summary>
	void End();

private:
	NamelessEngine::Core::Dx12GraphicsEngine& _graphicsEngine; // �`��̊�b������S������G���W��
	std::shared_ptr<NamelessEngine::Core::AppWindow> _window = nullptr; // �A�v���P�[�V�����̃E�B���h�E
	Dx12ApplicationImpl& _applicationImpl;				// �A�v���P�[�V�����̖{��
};