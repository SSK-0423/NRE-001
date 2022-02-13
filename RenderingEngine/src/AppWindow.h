#pragma once
#include <Windows.h>
#include <tchar.h>

/// <summary>
/// �E�B���h�E�̐ݒ�
/// </summary>
struct AppWindowInitData
{
	const TCHAR* _windowName;	// �E�B���h�E��
	LONG _windowWidth;	        // �E�B���h�E��
	LONG _windowHeight;	        // �E�B���h�E��

	AppWindowInitData(const TCHAR* name = L"DX12�A�v���P�[�V����", LONG width = 1024, LONG heigh = 768);
};

/// <summary>
/// �E�B���h�E�N���X
/// </summary>
class AppWindow
{
public:
	AppWindow() = default;
	~AppWindow() = default;
	/// <summary>
	/// �E�B���h�E����
	/// </summary>
	/// <param name="initData">�E�B���h�E�ݒ�̍\����</param>
	void CreateAppWindow(AppWindowInitData initData);

	/// <summary>
	/// ���b�Z�[�W���f�B�X�p�b�`���ăA�v���𑱍s���邩���f����
	/// </summary>
	/// <returns>true: �A�v�����s false: �A�v���I��</returns>
	bool DispatchWindowMessage();

private:
	HWND _hwnd;
	WNDCLASSEX _wndClassEx;
public:
	/// <summary>
	/// �E�B���h�E�n���h���擾
	/// </summary>
	/// <returns>�E�B���h�E�n���h��</returns>
	HWND& GetHwnd() { return _hwnd; }

	/// <summary>
	/// �E�B���h�E�T�C�Y�擾
	/// </summary>
	/// <returns>����: �E�B���h�E�T�C�Y ���s: �c��0�̃T�C�Y��Ԃ�</returns>
	SIZE GetWindowSize();
};