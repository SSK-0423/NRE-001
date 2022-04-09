#include "AppWindow.h"
#include <cassert>

AppWindowInitData::AppWindowInitData(const TCHAR* name, LONG width, LONG height)
	: windowName(name)
{
	assert(width > 0 && height > 0);
	windowWidth = width;
	windowHeight = height;
}

/// <summary>
/// �E�B���h�E�v���V�[�W��
/// </summary>
/// <param name="hwnd"></param>
/// <param name="msg"></param>
/// <param name="wparam"></param>
/// <param name="lparam"></param>
/// <returns></returns>
LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// �E�B���h�E���j�����ꂽ��Ă΂��
	if (msg == WM_DESTROY) {
		PostQuitMessage(0);	// OS�ɃA�v���̏I����ʒm
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void AppWindow::CreateAppWindow(AppWindowInitData initData)
{
	_wndClassEx.cbSize = sizeof(WNDCLASSEX);
	_wndClassEx.lpfnWndProc = (WNDPROC)WindowProcedure;		// �R�[���o�b�N�֐��w��
	_wndClassEx.lpszClassName = _T("DirectX12App");
	_wndClassEx.hInstance = GetModuleHandle(nullptr);

	RegisterClassEx(&_wndClassEx);

	RECT rect = { 0,0,initData.windowWidth,initData.windowHeight };

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	//�E�B���h�E�I�u�W�F�N�g�̐���
	_hwnd = CreateWindow(_wndClassEx.lpszClassName,
		initData.windowName,	// �^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW,	// �^�C�g���o�[�Ƌ��E��������E�B���h�E
		CW_USEDEFAULT,			// �\��X���W��OS�ɂ��C��
		CW_USEDEFAULT,			// �\��Y���W��OS�ɂ��C��
		rect.right - rect.left,	// �E�B���h�E��
		rect.bottom - rect.top,	// �E�B���h�E��
		nullptr,	            // �e�E�B���h�E�n���h��
		nullptr,	            // ���j���[�n���h��
		_wndClassEx.hInstance,	// �Ăяo���A�v���P�[�V�����n���h��
		nullptr);	            // �ǉ��p�����[�^

	ShowWindow(_hwnd, SW_SHOW);
}

bool AppWindow::DispatchWindowMessage()
{
	MSG msg = { 0 };
	// �E�B���h�E����̃��b�Z�[�W���󂯎��
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.message != WM_QUIT;
}

SIZE AppWindow::GetWindowSize()
{
	RECT rect;
	BOOL result = GetClientRect(_hwnd, &rect);	// ���j���[�o�[���������`��\�̈�̃T�C�Y���擾
	if (!result) {
		MessageBoxA(_hwnd, "�E�B���h�E�T�C�Y�擾���s", "�G���[", MB_OK | MB_ICONERROR);
		return {};
	}

	SIZE size;
	size.cx = rect.right - rect.left;
	size.cy = rect.bottom - rect.top;

	return size;
}

