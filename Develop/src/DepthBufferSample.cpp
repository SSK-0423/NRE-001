#include "DepthBufferSample.h"
#include "Dx12GraphicsEngine.h"

using namespace DirectX;

MYRESULT DepthBufferSample::Init(Dx12GraphicsEngine& graphicsEngine, AppWindow& window)
{
	MYRESULT result = CreateNearPolygon(graphicsEngine);
	if (result == MYRESULT::FAILED) { return result; }

	result = CreateFarPolygon(graphicsEngine);
	if (result == MYRESULT::FAILED) { return result; }

	result = SetConstantBuffer(graphicsEngine, window);
	if (result == MYRESULT::FAILED) { return result; }

	// ビューポートセット
	_viewport = CD3DX12_VIEWPORT(0.f, 0.f,
		static_cast<FLOAT>(window.GetWindowSize().cx),
		static_cast<FLOAT>(window.GetWindowSize().cy));

	// シザー矩形セット
	_scissorRect = CD3DX12_RECT(0, 0, window.GetWindowSize().cx, window.GetWindowSize().cy);

	RenderTargetData renderTargetData;
	renderTargetData.renderTargetBufferData
		= RenderTargetBufferData(DXGI_FORMAT_R8G8B8A8_UNORM, 1280, 720, { 0.f,1.f,1.f,1.f });
	renderTargetData.depthStencilBufferData.width = window.GetWindowSize().cx;
	renderTargetData.depthStencilBufferData.height = window.GetWindowSize().cy;
	
	result = _firstRender.Create(graphicsEngine.Device(), renderTargetData);
	if (result == MYRESULT::FAILED) { return result; }

	SpriteData data;
	data.vertexShaderData = ShaderData(L"src/SpriteVS.hlsl", "SpriteVS", "vs_5_0");
	data.pixelShaderData = ShaderData(L"src/SpritePS.hlsl", "SpritePS", "ps_5_0");
	data.textures[0] = &_firstRender.GetRenderTargetTexture();
	data.textures[1] = &_firstRender.GetDepthStencilTexture();
	data.rootSignatureData._descRangeData.srvDescriptorNum = 2;

	result = _firstRenderingSprite.Create(graphicsEngine, data);
	if (result == MYRESULT::FAILED) { return result; }


	return result;
}

void DepthBufferSample::Update(float deltaTime)
{
	angle += 0.01f;
	_nearCBuffData.rotation = XMMatrixRotationY(angle);
	_nearCBuffer.UpdateData((void*)&_nearCBuffData);
}

void DepthBufferSample::Draw(Dx12GraphicsEngine& graphicsEngine)
{
	// レンダリングコンテキスト取得
	RenderingContext& renderContext = graphicsEngine.GetRenderingContext();

	_firstRender.BeginRendering(renderContext, _viewport, _scissorRect);
	{
		// ポリゴン描画
		_nearPolygon.Draw(renderContext);
		_farPolygon.Draw(renderContext);
	}
	_firstRender.EndRendering(renderContext);

	graphicsEngine.SetFrameRenderTarget(_viewport, _scissorRect);
	{
		_firstRenderingSprite.Draw(renderContext);
	}
}

void DepthBufferSample::Final()
{
}

MYRESULT DepthBufferSample::CreateNearPolygon(Dx12GraphicsEngine& graphicsEngine)
{
	// ポリゴンの頂点データ用意
	std::vector<PolygonVertex> triangleVertex;
	triangleVertex.push_back({ { -2.f,-0.4f	,-0.f }	,{0.f,1.f} });
	triangleVertex.push_back({ { -2.f ,0.4f,-0.f }	,{0.f,0.f} });
	triangleVertex.push_back({ { 2.f  ,-0.4f,-0.f}	,{1.f,1.f} });
	triangleVertex.push_back({ { 2.f  ,0.4f	,-0.f}	,{1.f,0.f} });

	// 頂点バッファー生成
	MYRESULT result = _vertexBuffer.Create(
		graphicsEngine.Device(), (void*)&triangleVertex[0],
		SizeofVector<PolygonVertex>(triangleVertex), sizeof(PolygonVertex));

	// ポリゴンのインデックスデータ用意
	std::vector<UINT> index;
	index.push_back(0); index.push_back(1); index.push_back(2);
	index.push_back(2); index.push_back(1); index.push_back(3);

	// インデックスバッファー生成
	result = _indexBuffer.Create(graphicsEngine.Device(), index);

	// シェーダー
	result = _vertexShader.Create(L"src/NearPolygonVS.hlsl", "NearPolygonVS", "vs_5_0");
	result = _pixelShader.Create(L"src/NearPolygonPS.hlsl", "NearPolygonPS", "ps_5_0");
	// ポリゴン生成
	PolygonData polygonData;
	polygonData.vertexBuffer = _vertexBuffer;
	polygonData.indexBuffer = _indexBuffer;
	polygonData.vertexShader = _vertexShader;
	polygonData.pixelShader = _pixelShader;
	polygonData.rootSignatureData = _rootSignatureData;
	polygonData.inputLayout.push_back
	(
		{
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		}
	);
	polygonData.inputLayout.push_back
	(
		{
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		}
	);

	result = _nearPolygon.Create(graphicsEngine.Device(), polygonData);

	return result;
}

MYRESULT DepthBufferSample::CreateFarPolygon(Dx12GraphicsEngine& graphicsEngine)
{
	// ポリゴンの頂点データ用意
	std::vector<PolygonVertex> triangleVertex;
	triangleVertex.push_back({ { -1.f,-1.f	,0.f }	,{0.f,1.f} });
	triangleVertex.push_back({ { -1.f  ,1.f,0.f }	,{0.f,0.f} });
	triangleVertex.push_back({ { 1.f ,-1.f	,0.f}	,{1.f,1.f} });
	triangleVertex.push_back({ { 1.f ,1.f	,0.f}	,{1.f,0.f} });

	// 頂点バッファー生成
	MYRESULT result = _vertexBuffer.Create(
		graphicsEngine.Device(), (void*)&triangleVertex[0],
		SizeofVector<PolygonVertex>(triangleVertex), sizeof(PolygonVertex));

	// ポリゴンのインデックスデータ用意
	std::vector<UINT> index;
	index.push_back(0); index.push_back(1); index.push_back(2);
	index.push_back(2); index.push_back(1); index.push_back(3);

	// インデックスバッファー生成
	result = _indexBuffer.Create(graphicsEngine.Device(), index);

	// シェーダー
	result = _vertexShader.Create(L"src/FarPolygonVS.hlsl", "FarPolygonVS", "vs_5_0");
	result = _pixelShader.Create(L"src/FarPolygonPS.hlsl", "FarPolygonPS", "ps_5_0");

	// ポリゴン生成
	PolygonData polygonData;
	polygonData.vertexBuffer = _vertexBuffer;
	polygonData.indexBuffer = _indexBuffer;
	polygonData.vertexShader = _vertexShader;
	polygonData.pixelShader = _pixelShader;
	polygonData.rootSignatureData = _rootSignatureData;
	polygonData.inputLayout.push_back
	(
		{
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		}
	);
	polygonData.inputLayout.push_back
	(
		{
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		}
	);

	result = _farPolygon.Create(graphicsEngine.Device(), polygonData);

	return result;
}

MYRESULT DepthBufferSample::SetConstantBuffer(Dx12GraphicsEngine& graphicsEngine, AppWindow& window)
{
	// 変換行列用意
	XMMATRIX worldViewProj = XMMatrixIdentity();

	XMFLOAT3 eye(0, 0, -3);
	XMFLOAT3 target(0, 0, 0);
	XMFLOAT3 up(0, 1, 0);

	worldViewProj *= XMMatrixLookAtLH(
		XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	worldViewProj *= XMMatrixPerspectiveFovLH(
		XM_PIDIV2,
		static_cast<float>(window.GetWindowSize().cx) / static_cast<float>(window.GetWindowSize().cy),
		1.f,
		10.f);

	_nearCBuffData.worldViewProj = worldViewProj;
	_nearCBuffData.rotation = DirectX::XMMatrixRotationY(30);

	MYRESULT result = _nearCBuffer.Create(graphicsEngine.Device(), &_nearCBuffData, sizeof(NearConstBuff));
	if (result == MYRESULT::FAILED) { return MYRESULT::FAILED; }

	result = _nearHeap.Create(graphicsEngine.Device());
	if (result == MYRESULT::FAILED) { return MYRESULT::FAILED; }

	_nearHeap.RegistConstantBuffer(graphicsEngine.Device(), _nearCBuffer);

	_nearPolygon.SetDescriptorHeap(_nearHeap);
	_farPolygon.SetDescriptorHeap(_nearHeap);

	return result;
}
