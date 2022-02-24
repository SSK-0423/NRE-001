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
	// �E�B���h�E������
	_window = std::make_shared<AppWindow>();
	AppWindowInitData initData(TEXT("�����_�����O�G���W���J��"), WINDOW_WIDTH, WINDOW_HEIGHT);
	_window->CreateAppWindow(initData);

	// �O���t�B�N�X�G���W��������
	SIZE wndSize = _window->GetWindowSize();
	MYRESULT result = _graphicsEngine.Init(_window->GetHwnd(), wndSize.cx, wndSize.cy);

	// �|���S���֘A������(�e�X�g�p)
	std::vector<DirectX::XMFLOAT3> vertex;
	vertex.push_back({ -0.5f,-0.7f,0.f });	// ����
	vertex.push_back({ 0.f,0.7f,0.f });	    // �^��
	vertex.push_back({ 0.5f,-0.7f,0.f });	// �E��

	result = _vertexBuffer.Create(_graphicsEngine.Device(), vertex);

	std::vector<UINT> index;
	index.push_back(0); index.push_back(1); index.push_back(2);
	index.push_back(2); index.push_back(1); index.push_back(3);

	result = _indexBuffer.Create(_graphicsEngine.Device(), index);

	RootSignatureData data;
	result = _rootSignature.Create(_graphicsEngine.Device(), data);

	// �V�F�[�_�[
	result = _vertexShader.Create(L"../Develop/src/PolygonVertexShader.hlsl", "PolygonVS", "vs_5_0");
	result = _pixelShader.Create(L"../Develop/src/PolygonPixelShader.hlsl", "PolygonPS", "ps_5_0");

	PolygonData polygonData;
	polygonData._vertexBuffer = _vertexBuffer;
	polygonData._indexBuffer = _indexBuffer;
	polygonData._vertexShader = _vertexShader;
	polygonData._pixelShader = _pixelShader;
	polygonData._rootSignature = _rootSignature;
	polygonData._inputLayout.push_back
	(
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		}
	);

	result = _triangle.Create(_graphicsEngine.Device(), polygonData);

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
		Update();
		Draw();
	}
}

void Dx12Application::End()
{
}

void Dx12Application::Update()
{
}

void Dx12Application::Draw()
{
	// 1�t���[���̕`�揈��
	_graphicsEngine.BeginDraw();
	{
		// ���f���Ȃǂ̕`��
	}
	_graphicsEngine.EndDraw();
}