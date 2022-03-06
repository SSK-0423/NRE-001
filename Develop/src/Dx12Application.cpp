#include "Dx12Application.h"
#include "Dx12GraphicsEngine.h"
#include "AppWindow.h"

#include <iostream>

const LONG WINDOW_WIDTH = 1280;
const LONG WINDOW_HEIGHT = 720;

using namespace DirectX;

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

	// �O�p�`�|���S��
	std::vector<PolygonVertex> triangleVertex;
	triangleVertex.push_back({ { -0.5f,-0.7f	,0.f }	,{0.f,1.f} });
	triangleVertex.push_back({ { 0.f  ,0.7f	,0.f }	,{0.5f,0.f} });
	triangleVertex.push_back({ { 0.5f ,-0.7f	,0.f}	,{1.f,1.f} });

	result = _vertexBuffer.Create(
		_graphicsEngine.Device(), (void*)&triangleVertex[0],
		SizeofVector<PolygonVertex>(triangleVertex), sizeof(PolygonVertex));

	std::vector<UINT> index;
	index.push_back(0); index.push_back(1); index.push_back(2);

	result = _indexBuffer.Create(_graphicsEngine.Device(), index);

	RootSignatureData data;
	result = _rootSignature.Create(_graphicsEngine.Device(), data);

	// �V�F�[�_�[
	result = _vertexShader.Create(L"src/PolygonVertexShader.hlsl", "PolygonVS", "vs_5_0");
	result = _pixelShader.Create(L"src/PolygonPixelShader.hlsl", "PolygonPS", "ps_5_0");

	// �|���S������
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
	polygonData._inputLayout.push_back
	(
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		}
	);

	result = _triangle.Create(_graphicsEngine.Device(), polygonData);

	// �l�p�`�|���S��
	std::vector<PolygonVertex> squareVertex(4);
	squareVertex[0] = { {-1.f,-1.f,0.f},{0.f,1.f} };
	squareVertex[1] = { {-1.f,1.f,0.f},{0.f,0.f} };
	squareVertex[2] = { {1.f,-1.f,0.f},{1.f,1.f} };
	squareVertex[3] = { {1.f,1.f,0.f},{1.f,0.f} };

	result = _vertexBuffer.Create(_graphicsEngine.Device(), (void*)&squareVertex[0],
		SizeofVector<PolygonVertex>(squareVertex), sizeof(PolygonVertex));

	index.push_back(2);	index.push_back(1); index.push_back(3);
	result = _indexBuffer.Create(_graphicsEngine.Device(), index);

	polygonData._vertexBuffer = _vertexBuffer;
	polygonData._indexBuffer = _indexBuffer;

	result = _square.Create(_graphicsEngine.Device(), polygonData);

	_viewport.TopLeftX = 0.f;
	_viewport.TopLeftY = 0.f;
	_viewport.Width = static_cast<FLOAT>(_window->GetWindowSize().cx);
	_viewport.Height = static_cast<FLOAT>(_window->GetWindowSize().cy);

	_scissorRect.left = 0;
	_scissorRect.top = 0;
	_scissorRect.right = _window->GetWindowSize().cx;
	_scissorRect.bottom = _window->GetWindowSize().cy;
	
	// �e�N�X�`���}�b�s���O
	result = InitTexture();
	result = InitConstantBuffer();

	// �}���`�p�X�����_�����O
	result = InitOffscreenRender();

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
	_polygonConstantBuffer._matrix = XMMatrixRotationX(_angle);
	_constantBuffer.UpdateData((void*)&_polygonConstantBuffer);
	_angle += 0.01f;
}

void Dx12Application::Draw()
{
	// 1�t���[���̕`�揈��
	_graphicsEngine.BeginDraw();
	{
		// ���f���Ȃǂ̕`��
		_graphicsEngine.GetRenderingContext().SetGraphicsRootSignature(_rootSignature);
		_graphicsEngine.GetRenderingContext().SetViewport(_viewport);
		_graphicsEngine.GetRenderingContext().SetScissorRect(_scissorRect);

		// �e�N�X�`���p�̐ݒ�
		_graphicsEngine.GetRenderingContext().SetDescriptorHeap(_polygonHeap.GetDescriptorHeapAddress());
		_graphicsEngine.GetRenderingContext().SetGraphicsRootDescriptorTable(
			0, _polygonHeap.GetGPUDescriptorHandleForHeapStartSRV());
		_graphicsEngine.GetRenderingContext().SetGraphicsRootDescriptorTable(
			1, _polygonHeap.GetGPUDescriptorHandleForHeapStartCBV());

		// �`��
		_triangle.Draw(_graphicsEngine.GetRenderingContext());
		_square.Draw(_graphicsEngine.GetRenderingContext());
	}
	_graphicsEngine.EndDraw();
}

MYRESULT Dx12Application::InitTexture()
{
	ID3D12Device& device = _graphicsEngine.Device();

	std::wstring path(L"textest.png");
	MYRESULT result = _texture.CreateTextureFromWIC(_graphicsEngine, path);
	if (result == MYRESULT::FAILED) { return result; }
	result = _polygonHeap.Create(device);
	if (result == MYRESULT::FAILED) { return result; }
	_polygonHeap.RegistShaderResource(device, _texture);

	return MYRESULT::SUCCESS;
}

MYRESULT Dx12Application::InitConstantBuffer()
{
	ID3D12Device& device = _graphicsEngine.Device();

	XMFLOAT3 eye(0, 0, -2);
	XMFLOAT3 target(0, 0, 0);
	XMFLOAT3 up(0, 1, 0);

	// ���[���h�s��
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	// �r���[�s��
	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(
		XMLoadFloat3(&eye),XMLoadFloat3(&target),XMLoadFloat3(&up));
	// �v���W�F�N�V�����s��
	DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(
		XM_PIDIV2, 
		static_cast<FLOAT>(_window->GetWindowSize().cx) / static_cast<FLOAT>(_window->GetWindowSize().cy),
		1.f, 10.f);
	
	_polygonConstantBuffer._worldViewProj = world * view * proj;
	_polygonConstantBuffer._matrix = XMMatrixRotationY(0.f);

	// �R���X�^���g�o�b�t�@�\����
	MYRESULT result = _constantBuffer.Create(
		device, (void*)&_polygonConstantBuffer, sizeof(_polygonConstantBuffer));
	if (result == MYRESULT::FAILED) { return result; }
	// �f�B�X�N���v�^�q�[�v�ɒǉ�
	_polygonHeap.RegistConstantBuffer(device, _constantBuffer);

	return MYRESULT::SUCCESS;
}

MYRESULT Dx12Application::InitOffscreenRender()
{
	RenderTargetBufferData renderData(
		DXGI_FORMAT_R8G8B8A8_UNORM,_window->GetWindowSize().cx,_window->GetWindowSize().cy);
	ID3D12Device& device = _graphicsEngine.Device();
	
	MYRESULT result = _offscreenRender.Create(device, renderData);

	return MYRESULT::SUCCESS;
}
