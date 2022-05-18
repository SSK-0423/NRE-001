#include "DrawPolygonSample.h"
#include "Dx12GraphicsEngine.h"
#include "AppWindow.h"

MYRESULT DrawPolygonSample::Init(Dx12GraphicsEngine& graphicsEngine, AppWindow& window)
{
	// 三角形ポリゴンの頂点データ用意
	std::vector<PolygonVertex> triangleVertex;
	triangleVertex.push_back({ { -0.5f,-0.7f	,0.f }	,{0.f,1.f} });
	triangleVertex.push_back({ { 0.f  ,0.7f		,0.f }	,{0.5f,0.f} });
	triangleVertex.push_back({ { 0.5f ,-0.7f	,0.f}	,{1.f,1.f} });

	// 頂点バッファー生成
	MYRESULT result = _vertexBuffer.Create(
		graphicsEngine.Device(), (void*)&triangleVertex[0],
		SizeofVector<PolygonVertex>(triangleVertex), sizeof(PolygonVertex));

	// 三角形ポリゴンのインデックスデータ用意
	std::vector<UINT> index;
	index.push_back(0); index.push_back(1); index.push_back(2);

	// インデックスバッファー生成
	result = _indexBuffer.Create(graphicsEngine.Device(), index);

	// シェーダー
	result = _vertexShader.Create(L"src/PolygonSampleVertex.hlsl", "PolygonSampleVS", "vs_5_0");
	result = _pixelShader.Create(L"src/PolygonSamplePixel.hlsl", "PolygonSamplePS", "ps_5_0");

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

	result = _triangle.Create(graphicsEngine.Device(), polygonData);

	// ビューポートセット
	_viewport = CD3DX12_VIEWPORT(0.f, 0.f,
		static_cast<FLOAT>(window.GetWindowSize().cx),
		static_cast<FLOAT>(window.GetWindowSize().cy));

	// シザー矩形セット
	_scissorRect = CD3DX12_RECT(0, 0, window.GetWindowSize().cx, window.GetWindowSize().cy);

	return result;
}

void DrawPolygonSample::Update(float deltaTime)
{
}

void DrawPolygonSample::Draw(Dx12GraphicsEngine& graphicsEngine)
{
	// レンダリングコンテキスト取得
	RenderingContext& renderContext = graphicsEngine.GetRenderingContext();

	renderContext.SetViewport(_viewport);
	renderContext.SetScissorRect(_scissorRect);

	// ポリゴン描画
	_triangle.Draw(renderContext);
}

void DrawPolygonSample::Final()
{
}
