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
	// �E�B���h�E������
	_window = std::make_unique<AppWindow>();
	AppWindowInitData initData(TEXT("Nameless Renderer"), WINDOW_WIDTH, WINDOW_HEIGHT);
	_window->CreateAppWindow(initData);

	// �O���t�B�N�X�G���W��������
	SIZE wndSize = _window->GetWindowSize();
	RESULT result = _graphicsEngine.Init(_window->GetHwnd(), wndSize.cx, wndSize.cy);
	if (result == RESULT::FAILED) { return result; }

	// �A�v���P�[�V�����{�̂̏�����
	result = _applicationImpl.Init(_graphicsEngine, *_window);
	if (result == RESULT::FAILED) { return result; }

	// ���̓V�X�e��������
	result = _input.Init(_window->GetHwnd());
	if (result == RESULT::FAILED) { return result; }

	// Imgui������
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
	// Atlas�t�H���g�̃��r���h�H
	//ImGui::GetIO().Fonts->Build();
	//ImGui_ImplDX12_InvalidateDeviceObjects();
	return result;
}

void Dx12Application::Run()
{
	// �E�B���h�E���쐬����Ă��Ȃ��Ȃ炻�̎��_�ŏI��
	if (_window == nullptr) { return; }

	// �Q�[�����[�v
	while (_window->DispatchWindowMessage())
	{
		// �L�[�}�E�X���͏��̍X�V
		_input.Update();

		// ������G���W��(�t���[�����[�N��)�ɋz��������H�H�H�H
		_applicationImpl.Update(0.f);

		// 1�t���[���̕`��
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