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

	// ポリゴン関連初期化(テスト用)
	std::vector<DirectX::XMFLOAT3> vertex;
	vertex.push_back({ -0.5f,-0.7f,0.f });	// 左下
	vertex.push_back({ 0.f,0.7f,0.f });	    // 真ん中
	vertex.push_back({ 0.5f,-0.7f,0.f });	// 右下

	std::vector<PolygonVertex> polygonVertex;
	polygonVertex.push_back({ { -0.5f,-0.7f	,0.f }	,{0.f,1.f} });
	polygonVertex.push_back({ { 0.f  ,0.7f	,0.f }	,{0.5f,0.f} });
	polygonVertex.push_back({ { 0.5f ,-0.7f	,0.f}	,{1.f,1.f} });

	//result = _vertexBuffer.Create(_graphicsEngine.Device(), vertex);
	result = _vertexBuffer.Create(
		_graphicsEngine.Device(), (void*)&polygonVertex[0],
		SizeofVector<PolygonVertex>(polygonVertex), sizeof(PolygonVertex));

	std::vector<UINT> index;
	index.push_back(0); index.push_back(1); index.push_back(2);

	result = _indexBuffer.Create(_graphicsEngine.Device(), index);

	RootSignatureData data;
	result = _rootSignature.Create(_graphicsEngine.Device(), data);

	// シェーダー
	result = _vertexShader.Create(L"src/PolygonVertexShader.hlsl", "PolygonVS", "vs_5_0");
	result = _pixelShader.Create(L"src/PolygonPixelShader.hlsl", "PolygonPS", "ps_5_0");

	// ポリゴン生成
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

	// 四角形ポリゴン
	//vertex.resize(4);
	//vertex[0] = { -1.f,-1.f,0.f };
	//vertex[1] = { -1.f,1.f,0.f };
	//vertex[2] = { 0.f,-1.f,0.f };
	//vertex[3] = { 0.f,1.f,0.f };
	//result = _vertexBuffer.Create(_graphicsEngine.Device(), vertex);

	//index.push_back(2);	index.push_back(1); index.push_back(3);
	//result = _indexBuffer.Create(_graphicsEngine.Device(), index);

	//polygonData._vertexBuffer = _vertexBuffer;
	//polygonData._indexBuffer = _indexBuffer;

	//result = _square.Create(_graphicsEngine.Device(), polygonData);

	_viewport.TopLeftX = 0.f;
	_viewport.TopLeftY = 0.f;
	_viewport.Width = static_cast<FLOAT>(_window->GetWindowSize().cx);
	_viewport.Height = static_cast<FLOAT>(_window->GetWindowSize().cy);

	_scissorRect.left = 0;
	_scissorRect.top = 0;
	_scissorRect.right = _window->GetWindowSize().cx;
	_scissorRect.bottom = _window->GetWindowSize().cy;

	result = InitTexture();

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
	// 1フレームの描画処理
	_graphicsEngine.BeginDraw();
	{
		// モデルなどの描画
		_graphicsEngine.GetRenderingContext().SetGraphicsRootSignature(_rootSignature);
		_graphicsEngine.GetRenderingContext().SetViewport(_viewport);
		_graphicsEngine.GetRenderingContext().SetScissorRect(_scissorRect);

		// テクスチャ用の設定
		_graphicsEngine.GetRenderingContext().SetDescriptorHeap(_textureHeap.GetDescriptorHeapAddress());
		_graphicsEngine.GetRenderingContext().SetGraphicsRootDescriptorTable(
			0, _textureHeap.GetGPUDescriptorHandleForHeapStartSRV());

		// 描画
		_triangle.Draw(_graphicsEngine.GetRenderingContext());
		//_square.Draw(_graphicsEngine.GetRenderingContext());
	}
	_graphicsEngine.EndDraw();
}

MYRESULT Dx12Application::InitTexture()
{
	ID3D12Device& device = _graphicsEngine.Device();

	std::wstring path(L"textest.png");
	MYRESULT result = _texture.CreateTextureFromWIC(_graphicsEngine, path);
	if (result == MYRESULT::FAILED) { return result; }
	result = _textureHeap.Create(device);
	if (result == MYRESULT::FAILED) { return result; }
	_textureHeap.RegistShaderResource(device, _texture);

	return MYRESULT::SUCCESS;
}