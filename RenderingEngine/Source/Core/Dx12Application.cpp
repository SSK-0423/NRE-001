#include "Dx12Application.h"
#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

const LONG WINDOW_WIDTH = 1980;
const LONG WINDOW_HEIGHT = 1080;

using namespace NamelessEngine::Core;
using namespace NamelessEngine::Utility;

Dx12Application::Dx12Application(Dx12ApplicationImpl& applicationImpl)
	: _graphicsEngine(Dx12GraphicsEngine::Instance()),
	_applicationImpl(applicationImpl), _input(Input::Instance())
{
}

Dx12Application::~Dx12Application()
{
}

RESULT Dx12Application::Init()
{
	// ウィンドウ初期化
	_window = std::make_unique<AppWindow>();
	AppWindowInitData initData(TEXT("Nameless Renderer"), WINDOW_WIDTH, WINDOW_HEIGHT);
	_window->CreateAppWindow(initData);

	// グラフィクスエンジン初期化
	SIZE wndSize = _window->GetWindowSize();
	RESULT result = _graphicsEngine.Init(_window->GetHwnd(), wndSize.cx, wndSize.cy);
	if (result == RESULT::FAILED) { return result; }

	// アプリケーション本体の初期化
	result = _applicationImpl.Init(_graphicsEngine, *_window);
	if (result == RESULT::FAILED) { return result; }

	// 入力システム初期化
	result = _input.Init(_window->GetHwnd());
	if (result == RESULT::FAILED) { return result; }

	// Imgui初期化
	if (ImGui::CreateContext() == nullptr) {
		return RESULT::FAILED;
	}
	if (!ImGui_ImplWin32_Init(_window->GetHwnd())) {
		return RESULT::FAILED;
	}
	ImGui_ImplDX12_Init(
		&_graphicsEngine.Device(),
		3,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		*_graphicsEngine.GetImguiDescriptorHeap().GetDescriptorHeapAddress(),
		_graphicsEngine.GetImguiDescriptorHeap().GetCPUDescriptorHandleForHeapStart(),
		_graphicsEngine.GetImguiDescriptorHeap().GetGPUDescriptorHandleForHeapStart());
	// Atlasフォントのリビルド？
	//ImGui::GetIO().Fonts->Build();
	//ImGui_ImplDX12_InvalidateDeviceObjects();
	return result;
}

void Dx12Application::Run()
{
	// ウィンドウが作成されていないならその時点で終了
	if (_window == nullptr) { return; }

	// ゲームループ
	while (_window->DispatchWindowMessage())
	{
		// キーマウス入力情報の更新
		_input.Update();

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
	ImGui_ImplDX12_Shutdown();
	_applicationImpl.Final();
}