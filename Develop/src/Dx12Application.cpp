#include "Dx12Application.h"
#include "Dx12ApplicationImpl.h"
#include "Dx12GraphicsEngine.h"

const LONG WINDOW_WIDTH = 720;
const LONG WINDOW_HEIGHT = 720;

Dx12Application::Dx12Application(Dx12ApplicationImpl& applicationImpl)
	: _graphicsEngine(Dx12GraphicsEngine::Instance()), _applicationImpl(applicationImpl)
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
	if (result == MYRESULT::FAILED) { return result; }

	// アプリケーション本体の初期化
	result = _applicationImpl.Init(_graphicsEngine, *_window.get());
	if (result == MYRESULT::FAILED) { return result; }

	return result;
}

void Dx12Application::Run()
{
	// ウィンドウが作成されていないならその時点で終了
	if (_window == nullptr) { return; }

	// ゲームループ
	while (_window->DispatchWindowMessage())
	{
		// いずれエンジン(フレームワーク側)に吸収させる？？？？
		_applicationImpl.Update(0.f);

		// 1フレームの描画
		_graphicsEngine.BeginDraw();
		{
			_applicationImpl.Draw(_graphicsEngine);
		}
		_graphicsEngine.EndDraw();
	}
}

void Dx12Application::End()
{
	_applicationImpl.Final();
}