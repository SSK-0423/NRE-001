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
	triangleVertex.push_back({ { 0.f  ,0.7f		,0.f }	,{0.5f,0.f} });
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
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		}
	);
	polygonData._inputLayout.push_back
	(
		{
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
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
	// �R���X�^���g�o�b�t�@�\
	result = InitConstantBuffer();
	// �}���`�p�X�����_�����O
	result = InitOffscreenRender();
	// �}���`�����_�[�^�[�Q�b�g
	result = InitMultiRenderTarget();

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
	_constantBuffer.UpdateData((void*)&_polygonConstantBuffer);
}

void Dx12Application::Draw()
{
	//TextureMappingDraw();
	//MultiPassRenderingDraw();
	MultiRenderTargetDraw();
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
		XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
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

void Dx12Application::TextureMappingDraw()
{
	// 1�t���[���̕`�揈��
	_graphicsEngine.BeginDraw();
	{
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

MYRESULT Dx12Application::InitOffscreenRender()
{
	RenderTargetBufferData renderData(
		DXGI_FORMAT_R8G8B8A8_UNORM, _window->GetWindowSize().cx, _window->GetWindowSize().cy,
		{ 0.f,1.f,0.4f,1.f });
	ID3D12Device& device = _graphicsEngine.Device();

	OffScreenRenderData firstPassRenderData;
	firstPassRenderData.renderTargetData = renderData;
	firstPassRenderData.rootSignature = _rootSignature;
	firstPassRenderData.vertexShaderData = ShaderData(L"src/OffscreenVertexShader.hlsl", "OffscreenVS", "vs_5_0");
	firstPassRenderData.pixelShaderData = ShaderData(L"src/BackgroundPixelShader.hlsl", "BackgroundPS", "ps_5_0");
	firstPassRenderData.viewport = _viewport;
	firstPassRenderData.scissorRect = _scissorRect;

	// 1�p�X�ڐ���
	MYRESULT result = _firstPassRender.Create(device, firstPassRenderData);
	if (result == MYRESULT::FAILED) { return result; }

	// 2�p�X�ڐ���
	OffScreenRenderData secondPassRenderData = firstPassRenderData;
	secondPassRenderData.pixelShaderData = ShaderData(L"src/OffscreenPixelShader.hlsl", "OffscreenPS", "ps_5_0");

	result = _secondPassRender.Create(device, secondPassRenderData);
	if (result == MYRESULT::FAILED) { return result; }

	return MYRESULT::SUCCESS;
}

void Dx12Application::MultiPassRenderingDraw()
{
	// �����_�����O�R���e�L�X�g�e��
	RenderingContext& renderContext = _graphicsEngine.GetRenderingContext();

	// 1�t���[���̕`�揈��
	_graphicsEngine.BeginDraw();
	{
		// 1�p�X�ڂ̃����_�����O
		_firstPassRender.BeginRendering(renderContext);
		_firstPassRender.EndRendering(renderContext);

		// 2�p�X�ڂ̃����_�����O
		_secondPassRender.BeginRendering(renderContext);
		{
			// 1�p�X�ڂ̃����_�����O���ʂ�`��
			auto descHeap = _firstPassRender.GetDescriptorHeap();

			renderContext.SetDescriptorHeap(descHeap.GetDescriptorHeapAddress());
			renderContext.SetGraphicsRootDescriptorTable(0, descHeap.GetGPUDescriptorHandleForHeapStartSRV());

			_firstPassRender.Draw(renderContext);

			// 2�p�X�ڂŕ`�悵�����I�u�W�F�N�g��`��
			renderContext.SetDescriptorHeap(_polygonHeap.GetDescriptorHeapAddress());
			renderContext.SetGraphicsRootDescriptorTable(
				0, _polygonHeap.GetGPUDescriptorHandleForHeapStartSRV());
			renderContext.SetGraphicsRootDescriptorTable(
				1, _polygonHeap.GetGPUDescriptorHandleForHeapStartCBV());

			_square.Draw(renderContext);
		}
		_secondPassRender.EndRendering(renderContext);

		// �ŏI�p�X�̃����_�����O
		// �t���[�������_�[�^�[�Q�b�g�ɕύX
		_graphicsEngine.SetFrameRenderTarget(_viewport, _scissorRect);
		{
			// 2�p�X�ڂ܂ł̃����_�����O���ʂ�`��
			auto descHeap = _secondPassRender.GetDescriptorHeap();

			renderContext.SetDescriptorHeap(descHeap.GetDescriptorHeapAddress());
			renderContext.SetGraphicsRootDescriptorTable(0, descHeap.GetGPUDescriptorHandleForHeapStartSRV());
			renderContext.SetGraphicsRootSignature(_rootSignature);

			_secondPassRender.Draw(renderContext);
		}
	}
	_graphicsEngine.EndDraw();
}

MYRESULT Dx12Application::InitMultiRenderTarget()
{
	// �}���`�����_�[�^�[�Q�b�g�e�X�g�p�̃|���S������
	MYRESULT result = CreateMRTPolygon();
	if (result == MYRESULT::FAILED) { return result; }

	// �}���`�����_�[�^�[�Q�b�g����
	result = CreateMRTRenders();
	if (result == MYRESULT::FAILED) { return result; }

	// �}���`�����_�[�^�[�Q�b�g�p�̃f�B�X�N���v�^�q�[�v����
	result = CreateMRTDescriptorHeap();
	if (result == MYRESULT::FAILED) { return result; }

	return result;
}

MYRESULT Dx12Application::CreateMRTPolygon()
{
	ID3D12Device& device = _graphicsEngine.Device();

	// ���_�o�b�t�@�[
	std::vector<PolygonVertex> squareVertex(4);
	squareVertex[0] = { {-1.f,-1.f,0.f}	,{0.f,1.f} };
	squareVertex[1] = { {-1.f,1.f,0.f}	,{0.f,0.f} };
	squareVertex[2] = { {1.f,-1.f,0.f}	,{1.f,1.f} };
	squareVertex[3] = { {1.f,1.f,0.f}	,{1.f,0.f} };

	MYRESULT result = _mrtPolygonVB.Create(device, (void*)&squareVertex[0],
		SizeofVector<PolygonVertex>(squareVertex), sizeof(PolygonVertex));
	if (result == MYRESULT::FAILED) { return result; }

	// �C���f�b�N�X�o�b�t�@�[
	// TODO: index(6)
	std::vector<UINT> index;
	index.push_back(0); index.push_back(1); index.push_back(2);
	index.push_back(2); index.push_back(1); index.push_back(3);

	result = _mrtPolygonIB.Create(device, index);
	if (result == MYRESULT::FAILED) { return result; }

	// �V�F�[�_�[
	result = _mrtPolygonVS.Create(ShaderData(L"src/MRTPolygonVertexShader.hlsl", "MrtPolygonVS", "vs_5_0"));
	if (result == MYRESULT::FAILED) { return result; }
	result = _mrtPolygonPS.Create(ShaderData(L"src/MRTPolygonPixelShader.hlsl", "MrtPolygonPS", "ps_5_0"));
	if (result == MYRESULT::FAILED) { return result; }

	// �I�t�X�N���[���|���S������
	PolygonData polygonData;
	polygonData._vertexBuffer = _mrtPolygonVB;
	polygonData._indexBuffer = _mrtPolygonIB;
	polygonData._vertexShader = _mrtPolygonVS;
	polygonData._pixelShader = _mrtPolygonPS;
	polygonData._rootSignature = _rootSignature;
	polygonData._renderTargetNum = 2;				// �}���`�����_�[�^�[�Q�b�g
	polygonData._inputLayout.push_back(
		{
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		}
	);
	polygonData._inputLayout.push_back(
		{
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		}
	);

	result = _mrtPolygon.Create(device, polygonData);
	if (result == MYRESULT::FAILED) { return result; }

	return result;
}

MYRESULT Dx12Application::CreateMRTRenders()
{
	RenderTargetBufferData renderData(
		DXGI_FORMAT_R8G8B8A8_UNORM, _window->GetWindowSize().cx, _window->GetWindowSize().cy,
		{ 0.f,1.f,0.4f,1.f });
	ID3D12Device& device = _graphicsEngine.Device();

	OffScreenRenderData offRenderData;
	offRenderData.renderTargetData = renderData;
	offRenderData.rootSignature = _rootSignature;
	offRenderData.vertexShaderData = ShaderData(L"src/OffscreenVertexShader.hlsl", "OffscreenVS", "vs_5_0");
	offRenderData.pixelShaderData = ShaderData(L"src/OffscreenPixelShader.hlsl", "OffscreenPS", "ps_5_0");
	offRenderData.viewport = _viewport;
	offRenderData.scissorRect = _scissorRect;

	for (size_t idx = 0; idx < _countof(_mrtRenders); idx++)
	{
		MYRESULT result = _mrtRenders[idx].Create(device, offRenderData);
		if (result == MYRESULT::FAILED) { return result; }
	}

	// �Q�p�X�ڂ̃����_�[�^�[�Q�b�g����
	MYRESULT result = _mrtSecondRender.Create(device, offRenderData);
	if (result == MYRESULT::FAILED) { return result; }

	return MYRESULT::SUCCESS;
}

MYRESULT Dx12Application::CreateMRTDescriptorHeap()
{
	return _mrtDescriptorHeap.Create(_graphicsEngine.Device());
}

void Dx12Application::MultiRenderTargetDraw()
{
	// �����_�����O�R���e�L�X�g�p��
	RenderingContext& renderContext = _graphicsEngine.GetRenderingContext();

	// 1�t���[���̕`��
	_graphicsEngine.BeginDraw();
	{
		renderContext.SetGraphicsRootSignature(_rootSignature);
		renderContext.SetViewport(_viewport);
		renderContext.SetScissorRect(_scissorRect);

		// �I�t�X�N���[���Q���Ƀ����_�����O
		// 1�p�X��
		for (size_t idx = 0; idx < _countof(_mrtRenders); idx++) 
		{
			_mrtRenders[idx].BeginRendering(renderContext);
		}

		for (size_t idx = 0; idx < _countof(_mrtRenders); idx++)
		{
			_mrtRenders[idx].EndRendering(renderContext);
		}

		// �ŏI�p�X�ł��ꂼ����e�N�X�`���Ŏ󂯎���ĕ`��
		_graphicsEngine.SetFrameRenderTarget(_viewport, _scissorRect);
		{
			auto descHeap = _mrtDescriptorHeap;
			renderContext.SetDescriptorHeap(descHeap.GetDescriptorHeapAddress());
			renderContext.SetGraphicsRootDescriptorTable(0, descHeap.GetGPUDescriptorHandleForHeapStartSRV());
			renderContext.SetGraphicsRootSignature(_rootSignature);

			_mrtPolygon.Draw(renderContext);
		}
	}
	_graphicsEngine.EndDraw();
}
