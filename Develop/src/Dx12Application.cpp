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
	// �E�B���h�E������
	_window = std::make_shared<AppWindow>();
	AppWindowInitData initData(TEXT("�����_�����O�G���W���J��"), WINDOW_WIDTH, WINDOW_HEIGHT);
	_window->CreateAppWindow(initData);

	// �O���t�B�N�X�G���W��������
	SIZE wndSize = _window->GetWindowSize();
	MYRESULT result = _graphicsEngine.Init(_window->GetHwnd(), wndSize.cx, wndSize.cy);
	if (result == MYRESULT::FAILED) { return result; }

	// �A�v���P�[�V�����{�̂̏�����
	result = _applicationImpl.Init(_graphicsEngine, *_window.get());
	if (result == MYRESULT::FAILED) { return result; }

	return result;
}

void Dx12Application::Run()
{
	// �E�B���h�E���쐬����Ă��Ȃ��Ȃ炻�̎��_�ŏI��
	if (_window == nullptr) { return; }

	// �Q�[�����[�v
	while (_window->DispatchWindowMessage())
	{
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
	_applicationImpl.Final();
}