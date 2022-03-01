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
		_graphicsEngine.GetRenderingContext().SetDescriptorHeap(_textureHeap.GetAddressOf());
		_graphicsEngine.GetRenderingContext().SetGraphicsRootDescriptorTable(
			0, _textureHeap->GetGPUDescriptorHandleForHeapStart());

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
	_texture.CreateTextureFromWIC(_graphicsEngine, path);
	if (FAILED(CreateTextureHeap(device))) { return MYRESULT::FAILED; }
	CreateTextureSRV(device);

	//// ファイル読み込み
	//if (FAILED(LoadTextureFile(L"textest.png"))) { return MYRESULT::FAILED; }
	//// リソース生成
	//if (FAILED(CreateTextureResource(device))) { return MYRESULT::FAILED; }
	//// マップ
	//if (FAILED(MapTexture())) { return MYRESULT::FAILED; }
	//// 中間バッファーの内容をテクスチャバッファーにコピー
	//if (FAILED(CopyTexture(device, _graphicsEngine))) { return MYRESULT::FAILED; }
	//// ディスクリプタヒープ生成
	//if (FAILED(CreateTextureHeap(device))) { return MYRESULT::FAILED; }
	//// ビュー生成
	//CreateTextureSRV(device);

	return MYRESULT::SUCCESS;
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
	// アップロード用の中間バッファー生成
	CD3DX12_HEAP_PROPERTIES uploadHeapProp(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC uploadResDesc = CD3DX12_RESOURCE_DESC::Buffer(
		AlignmentedSize(_image->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT) * _image->height);

	HRESULT result = device.CreateCommittedResource(
		&uploadHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&uploadResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(_uploadBuffer.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { return result; }

	// アップロード先のバッファー生成
	CD3DX12_HEAP_PROPERTIES texHeapProp(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_RESOURCE_DESC texResDesc;
	texResDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(_metaData.dimension);
	texResDesc.Alignment = 0;
	texResDesc.Width = _metaData.width;
	texResDesc.Height = _metaData.height;
	texResDesc.DepthOrArraySize = _metaData.arraySize;
	texResDesc.MipLevels = _metaData.mipLevels;
	texResDesc.Format = _metaData.format;
	texResDesc.SampleDesc.Count = 1;
	texResDesc.SampleDesc.Quality = 0;
	texResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	result = device.CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texResDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(_textureBuffer.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { return result; }

	return result;
}

HRESULT Dx12Application::MapTexture()
{
	// マップ
	uint8_t* texMap = nullptr;
	HRESULT result = _uploadBuffer->Map(0, nullptr, (void**)&texMap);
	if (FAILED(result)) { return result; }

	// データコピー
	uint8_t* srcAddress = _image->pixels;;
	size_t alignRowPitch = AlignmentedSize(_image->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);

	for (size_t y = 0; y < _image->height; y++) {
		std::copy_n(srcAddress, alignRowPitch, texMap);
		srcAddress += _image->rowPitch;
		texMap += alignRowPitch;
	}
	_uploadBuffer->Unmap(0, nullptr);

	return result;
}

HRESULT Dx12Application::CopyTexture(
	ID3D12Device& device, Dx12GraphicsEngine& graphicsEngine)
{
	RenderingContext& renderContext = graphicsEngine.GetRenderingContext();
	ID3D12CommandAllocator& cmdAllocator = graphicsEngine.CmdAllocator();
	ID3D12CommandQueue& cmdQueue = graphicsEngine.CmdQueue();

	// コピー元
	D3D12_TEXTURE_COPY_LOCATION src = {};
	src.pResource = _uploadBuffer.Get();
	src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	src.PlacedFootprint.Offset = 0;
	src.PlacedFootprint.Footprint.Width = _metaData.width;
	src.PlacedFootprint.Footprint.Height = _metaData.height;
	src.PlacedFootprint.Footprint.Depth = _metaData.depth;
	src.PlacedFootprint.Footprint.RowPitch =
		AlignmentedSize(_image->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);
	src.PlacedFootprint.Footprint.Format = _image->format;

	// コピー先
	D3D12_TEXTURE_COPY_LOCATION dst = {};
	dst.pResource = _textureBuffer.Get();
	dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dst.SubresourceIndex = 0;

	// フェンス生成
	Microsoft::WRL::ComPtr<ID3D12Fence> fence = nullptr;
	UINT fenceVal = 0;
	HRESULT result = device.CreateFence(
		fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { return result; }

	// 中間バッファーの内容をコピー先バッファーへコピー
	{
		renderContext.CopyTextureRegion(src, dst);
		renderContext.TransitionResourceState(
			*_textureBuffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_PRESENT);

		renderContext.Close();

		// コマンドリスト実行
		ID3D12CommandList* cmdLists[] = { &graphicsEngine.CmdList() };
		cmdQueue.ExecuteCommandLists(_countof(cmdLists), cmdLists);
		cmdQueue.Signal(fence.Get(), ++fenceVal);
		if (fence->GetCompletedValue() != fenceVal) {
			// イベントハンドル取得
			auto event = CreateEvent(nullptr, false, false, nullptr);

			fence->SetEventOnCompletion(fenceVal, event);

			// イベントが発生するまで待ち続ける
			WaitForSingleObject(event, INFINITE);

			// イベントハンドルを閉じる
			CloseHandle(event);
		}

		cmdAllocator.Reset();
		renderContext.Reset(cmdAllocator);
	}

	return result;
}

HRESULT Dx12Application::CreateTextureHeap(ID3D12Device& device)
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.NumDescriptors = 1;
	heapDesc.NodeMask = 0;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	HRESULT result = device.CreateDescriptorHeap(
		&heapDesc, IID_PPV_ARGS(_textureHeap.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { return result; }

	return result;
}

void Dx12Application::CreateTextureSRV(ID3D12Device& device)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = _texture.GetImage().format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = _texture.GetTexMetadata().mipLevels;

	device.CreateShaderResourceView(
		&_texture.GetBuffer(), &srvDesc, _textureHeap->GetCPUDescriptorHandleForHeapStart());

	//device.CreateShaderResourceView(
	//	_textureBuffer.Get(), &srvDesc, _textureHeap->GetCPUDescriptorHandleForHeapStart());
}
