#include "Dx12Application.h"
#include "Dx12GraphicsEngine.h"
#include "AppWindow.h"

#include <iostream>

const LONG WINDOW_WIDTH = 1280;
const LONG WINDOW_HEIGHT = 720;

Dx12Application::Dx12Application() : _graphicsEngine(Dx12GraphicsEngine::Instance())
{
}

Dx12Application::~Dx12Application()
{
}

MYRESULT Dx12Application::Init()
{
	// ウィンドウ初期化
	_window = std::make_shared<AppWindow>();
	AppWindowInitData initData(TEXT("レンダリングエンジン開発"), WINDOW_WIDTH, WINDOW_HEIGHT);
	_window->CreateAppWindow(initData);
	
	// グラフィクスエンジン初期化
	SIZE wndSize = _window->GetWindowSize();
	MYRESULT result = _graphicsEngine.Init(_window->GetHwnd(), wndSize.cx, wndSize.cy);
	return result;
}

void Dx12Application::Run()
{
	// ウィンドウが作成されていないならその時点で終了
	if (_window == nullptr) { return; }

	// ゲームループ
	while(_window->DispatchWindowMessage())
	{
	}
}

void Dx12Application::End()
{
}