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
/// ウィンドウプロシージャ
/// </summary>
/// <param name="hwnd"></param>
/// <param name="msg"></param>
/// <param name="wparam"></param>
/// <param name="lparam"></param>
/// <returns></returns>
LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// ウィンドウが破棄されたら呼ばれる
	if (msg == WM_DESTROY) {
		PostQuitMessage(0);	// OSにアプリの終了を通知
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void AppWindow::CreateAppWindow(AppWindowInitData initData)
{
	_wndClassEx.cbSize = sizeof(WNDCLASSEX);
	_wndClassEx.lpfnWndProc = (WNDPROC)WindowProcedure;		// コールバック関数指定
	_wndClassEx.lpszClassName = _T("DirectX12App");
	_wndClassEx.hInstance = GetModuleHandle(nullptr);

	RegisterClassEx(&_wndClassEx);

	RECT rect = { 0,0,initData.windowWidth,initData.windowHeight };

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	//ウィンドウオブジェクトの生成
	_hwnd = CreateWindow(_wndClassEx.lpszClassName,
		initData.windowName,	// タイトルバーの文字
		WS_OVERLAPPEDWINDOW,	// タイトルバーと境界線があるウィンドウ
		CW_USEDEFAULT,			// 表示X座標はOSにお任せ
		CW_USEDEFAULT,			// 表示Y座標はOSにお任せ
		rect.right - rect.left,	// ウィンドウ幅
		rect.bottom - rect.top,	// ウィンドウ高
		nullptr,	            // 親ウィンドウハンドル
		nullptr,	            // メニューハンドル
		_wndClassEx.hInstance,	// 呼び出しアプリケーションハンドル
		nullptr);	            // 追加パラメータ

	ShowWindow(_hwnd, SW_SHOW);
}

bool AppWindow::DispatchWindowMessage()
{
	MSG msg = { 0 };
	// ウィンドウからのメッセージを受け取る
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.message != WM_QUIT;
}

SIZE AppWindow::GetWindowSize()
{
	RECT rect;
	BOOL result = GetClientRect(_hwnd, &rect);	// メニューバーを除いた描画可能領域のサイズを取得
	if (!result) {
		MessageBoxA(_hwnd, "ウィンドウサイズ取得失敗", "エラー", MB_OK | MB_ICONERROR);
		return {};
	}

	SIZE size;
	size.cx = rect.right - rect.left;
	size.cy = rect.bottom - rect.top;

	return size;
}

