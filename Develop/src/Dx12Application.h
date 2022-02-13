#pragma once

#include <Windows.h>
#include <memory>

#include "EngineUtility.h"

class Dx12GraphicsEngine;

/// <summary>
/// �����_�����O�J�����̃e�X�g�p�N���X
/// </summary>
class Dx12Application
{
public:
	Dx12Application();
	~Dx12Application();

	/// <summary>
	/// �A�v���P�[�V�����̏�����
	/// </summary>
	/// <returns>�����FMYRESULT::SUCCESS ���s�FMYRESULT::FAILED</returns>
	MYRESULT Init();

	/// <summary>
	/// ���C�����[�v
	/// </summary>
	void Run();

	/// <summary>
	/// �I������
	/// </summary>
	void End();

private:
	Dx12GraphicsEngine* _graphicsEngine = nullptr;
};

/// ����
/// DirectX�̏������ɃE�B���h�E�n���h���K�v(HWND)
/// ���V���O���g�������ăE�B���h�E�T�C�Y�擾�ł���悤�ɂ�����
/// ���F��ȃN���X����Appication�N���X���Ăяo���H�ڂɂȂ�
/// ���A�v���P�[�V�����N���X�̓A�v���P�[�V�����ŗL�̏������L�q���邽�߂̃N���X
///	���V�X�e�����̃N���X���A�v���P�[�V�����N���X�Ɗ֌W�������Ƃ͔�����
/// ��main.cpp�ł̂݌Ăяo���悤�Ȑ݌v�ɂ��遨�V���O���g��������K�v�Ȃ�