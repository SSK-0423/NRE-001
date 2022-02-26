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

	result = _vertexBuffer.Create(_graphicsEngine.Device(), vertex);

	std::vector<UINT> index;
	index.push_back(0); index.push_back(1); index.push_back(2);

	result = _indexBuffer.Create(_graphicsEngine.Device(), index);

	RootSignatureData data;
	result = _rootSignature.Create(_graphicsEngine.Device(), data);

	// シェーダー
	result = _vertexShader.Create(L"../Develop/src/PolygonVertexShader.hlsl", "PolygonVS", "vs_5_0");
	result = _pixelShader.Create(L"../Develop/src/PolygonPixelShader.hlsl", "PolygonPS", "ps_5_0");

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

	result = _triangle.Create(_graphicsEngine.Device(), polygonData);

	vertex.resize(4);
	vertex[0] = { -1.f,-1.f,0.f };
	vertex[1] = { -1.f,1.f,0.f };
	vertex[2] = { 0.f,-1.f,0.f };
	vertex[3] = { 0.f,1.f,0.f };
	result = _vertexBuffer.Create(_graphicsEngine.Device(), vertex);

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
		_triangle.Draw(_graphicsEngine.GetRenderingContext());
		_square.Draw(_graphicsEngine.GetRenderingContext());
	}
	_graphicsEngine.EndDraw();
}

MYRESULT Dx12Application::InitTexture()
{
	ID3D12Device& device = _graphicsEngine.Device();
	// ファイル読み込み
	if (FAILED(LoadTextureFile(L"Ramen.JPG"))) { return MYRESULT::FAILED; }
	// リソース生成
	if (FAILED(CreateTextureResource(device))) { return MYRESULT::FAILED; }

	return MYRESULT::FAILED;
}

HRESULT Dx12Application::LoadTextureFile(const wchar_t* path)
{
	HRESULT result = DirectX::LoadFromWICFile(
		path, DirectX::WIC_FLAGS_NONE, &_metaData, _scratchImage);
	// テクスチャの生データ取得
	_image = _scratchImage.GetImage(0, 0, 0);

	return result;
}

HRESULT Dx12Application::CreateTextureResource(ID3D12Device& device)
{
	// バッファー生成
	CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_UPLOAD);
	// ディスクリプタヒープ生成
	// ビュー生成

	return S_OK;
}
