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
	// ウィンドウ初期化
	_window = std::make_shared<AppWindow>();
	AppWindowInitData initData(TEXT("レンダリングエンジン開発"), WINDOW_WIDTH, WINDOW_HEIGHT);
	_window->CreateAppWindow(initData);

	// グラフィクスエンジン初期化
	SIZE wndSize = _window->GetWindowSize();
	MYRESULT result = _graphicsEngine.Init(_window->GetHwnd(), wndSize.cx, wndSize.cy);

	// 三角形ポリゴン
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

	// 四角形ポリゴン
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

	// テクスチャマッピング
	result = InitTexture();
	// コンスタントバッファ―
	result = InitConstantBuffer();
	// マルチパスレンダリング
	result = InitOffscreenRender();

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
	_constantBuffer.UpdateData((void*)&_polygonConstantBuffer);
}

void Dx12Application::Draw()
{
	//TextureMappingDraw();
	MultiPassRenderingDraw();
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

	// ワールド行列
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	// ビュー行列
	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(
		XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	// プロジェクション行列
	DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(
		XM_PIDIV2,
		static_cast<FLOAT>(_window->GetWindowSize().cx) / static_cast<FLOAT>(_window->GetWindowSize().cy),
		1.f, 10.f);

	_polygonConstantBuffer._worldViewProj = world * view * proj;
	_polygonConstantBuffer._matrix = XMMatrixRotationY(0.f);

	// コンスタントバッファ―生成
	MYRESULT result = _constantBuffer.Create(
		device, (void*)&_polygonConstantBuffer, sizeof(_polygonConstantBuffer));
	if (result == MYRESULT::FAILED) { return result; }
	// ディスクリプタヒープに追加
	_polygonHeap.RegistConstantBuffer(device, _constantBuffer);

	return MYRESULT::SUCCESS;
}

void Dx12Application::TextureMappingDraw()
{
	// 1フレームの描画処理
	_graphicsEngine.BeginDraw();
	{
		_graphicsEngine.GetRenderingContext().SetGraphicsRootSignature(_rootSignature);
		_graphicsEngine.GetRenderingContext().SetViewport(_viewport);
		_graphicsEngine.GetRenderingContext().SetScissorRect(_scissorRect);

		// テクスチャ用の設定
		_graphicsEngine.GetRenderingContext().SetDescriptorHeap(_polygonHeap.GetDescriptorHeapAddress());
		_graphicsEngine.GetRenderingContext().SetGraphicsRootDescriptorTable(
			0, _polygonHeap.GetGPUDescriptorHandleForHeapStartSRV());
		_graphicsEngine.GetRenderingContext().SetGraphicsRootDescriptorTable(
			1, _polygonHeap.GetGPUDescriptorHandleForHeapStartCBV());

		// 描画
		_triangle.Draw(_graphicsEngine.GetRenderingContext());
		_square.Draw(_graphicsEngine.GetRenderingContext());
	}
	_graphicsEngine.EndDraw();
}

MYRESULT Dx12Application::InitOffscreenRender()
{
	RenderTargetBufferData renderData(
		DXGI_FORMAT_R8G8B8A8_UNORM, _window->GetWindowSize().cx, _window->GetWindowSize().cy,
		{ 0.f,1.f,0.f,1.f });
	ID3D12Device& device = _graphicsEngine.Device();

	// オフスクリーンレンダーターゲット生成
	MYRESULT result = _offscreenRender.Create(device, renderData);
	if (result == MYRESULT::FAILED) { return result; }

	// オフスクリーンレンダーターゲットヒープ生成
	result = _offscreenRTVHeap.Create(device);
	if (result == MYRESULT::FAILED) { return result; }

	// オフスクリーンレンダーターゲットビュー生成
	_offscreenRTVHeap.RegistDescriptor(device, _offscreenRender);

	// オフスクリーンテクスチャバッファー生成
	_offscreenTexture.CreateTextureFromRenderTarget(_offscreenRender);
	if (result == MYRESULT::FAILED) { return result; }

	// オフスクリーンテクスチャヒープ生成
	result = _offscreenHeap.Create(device);
	if (result == MYRESULT::FAILED) { return result; }

	// テクスチャとして登録
	_offscreenHeap.RegistShaderResource(device, _offscreenTexture);

	// 頂点バッファー
	std::vector<PolygonVertex> squareVertex(4);
	squareVertex[0] = { {-1.f,-1.f,0.f}	,{0.f,1.f} };
	squareVertex[1] = { {-1.f,1.f,0.f}	,{0.f,0.f} };
	squareVertex[2] = { {1.f,-1.f,0.f}	,{1.f,1.f} };
	squareVertex[3] = { {1.f,1.f,0.f}	,{1.f,0.f} };
	
	result = _offscreenPolygonVB.Create(device, (void*)&squareVertex[0],
		SizeofVector<PolygonVertex>(squareVertex), sizeof(PolygonVertex));
	if (result == MYRESULT::FAILED) { return result; }

	// インデックスバッファー
	std::vector<UINT> index;
	index.push_back(0); index.push_back(1); index.push_back(2);
	index.push_back(2); index.push_back(1); index.push_back(3);
	
	result = _offscreenPolygonIB.Create(device, index);
	if (result == MYRESULT::FAILED) { return result; }

	// シェーダー
	result = _offscreenVS.Create(L"src/OffscreenVertexShader.hlsl", "OffscreenVS", "vs_5_0");
	if (result == MYRESULT::FAILED) { return result; }
	result = _offscreenPS.Create(L"src/OffscreenPixelShader.hlsl", "OffscreenPS", "ps_5_0");
	if (result == MYRESULT::FAILED) { return result; }

	// オフスクリーンポリゴン生成
	PolygonData polygonData;
	polygonData._vertexBuffer = _offscreenPolygonVB;
	polygonData._indexBuffer = _offscreenPolygonIB;
	polygonData._vertexShader = _offscreenVS;
	polygonData._pixelShader = _offscreenPS;
	polygonData._rootSignature = _rootSignature;
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

	result = _offscreenPolygon.Create(device, polygonData);
	if (result == MYRESULT::FAILED) { return result; }

	return MYRESULT::SUCCESS;
}

void Dx12Application::MultiPassRenderingDraw()
{
	// レンダリングコンテキスト容易
	RenderingContext& renderContext = _graphicsEngine.GetRenderingContext();

	// 1フレームの描画処理
	_graphicsEngine.BeginDraw();
	{
		// _firstPassRender.BeginDraw()
		// バリア処理
		renderContext.TransitionResourceState(
			_offscreenRender.GetBuffer(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET);

		// オフスクリーンのレンダーターゲット設定
		auto offRtvHandle = _offscreenRTVHeap.GetCPUDescriptorHandleForHeapStart();
		renderContext.SetRenderTarget(&offRtvHandle, nullptr);

		// 画面を指定色でクリア
		ColorRGBA color(0.f, 1.f, 0.f, 1.f);
		renderContext.ClearRenderTarget(offRtvHandle, color, 0, nullptr);

		// ルートシグネチャセット
		renderContext.SetGraphicsRootSignature(_rootSignature);

		// ビューポートとシザー矩形セット
		renderContext.SetViewport(_viewport);
		renderContext.SetScissorRect(_scissorRect);

		// テクスチャ用の設定
		renderContext.SetDescriptorHeap(_polygonHeap.GetDescriptorHeapAddress());
		renderContext.SetGraphicsRootDescriptorTable(
			0, _polygonHeap.GetGPUDescriptorHandleForHeapStartSRV());
		renderContext.SetGraphicsRootDescriptorTable(
			1, _polygonHeap.GetGPUDescriptorHandleForHeapStartCBV());

		// 描画
		_square.Draw(renderContext);

		// _firstPassRender.NextPassRendering()

		// レンダーターゲット→テクスチャへ
		renderContext.TransitionResourceState(
			_offscreenRender.GetBuffer(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

		// フレームレンダーターゲットに変更
		_graphicsEngine.SetFrameRenderTarget();

		renderContext.SetDescriptorHeap(_offscreenHeap.GetDescriptorHeapAddress());
		renderContext.SetGraphicsRootDescriptorTable(
			0, _offscreenHeap.GetGPUDescriptorHandleForHeapStartSRV());

		renderContext.SetGraphicsRootSignature(_rootSignature);

		// ビューポートとシザー矩形セット
		renderContext.SetViewport(_viewport);
		renderContext.SetScissorRect(_scissorRect);

		_offscreenPolygon.Draw(renderContext);

		// テクスチャ→待機状態
		renderContext.TransitionResourceState(
			_offscreenRender.GetBuffer(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_PRESENT);
		// _firstPassRender.EndDraw()
	}
	_graphicsEngine.EndDraw();
}