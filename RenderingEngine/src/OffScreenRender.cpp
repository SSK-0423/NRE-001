#include "OffScreenRender.h"
#include "RenderingContext.h"
#include "ShaderResourceViewDesc.h"

MYRESULT OffScreenRender::CreateOffScreenPolygon(
	ID3D12Device& device, const OffScreenRenderData& offScreenRenderData)
{
	// 頂点バッファー
	std::vector<PolygonVertex> squareVertex(4);
	squareVertex[0] = { {-1.f,-1.f,0.f}	,{0.f,1.f} };
	squareVertex[1] = { {-1.f,1.f,0.f}	,{0.f,0.f} };
	squareVertex[2] = { {1.f,-1.f,0.f}	,{1.f,1.f} };
	squareVertex[3] = { {1.f,1.f,0.f}	,{1.f,0.f} };

	MYRESULT result = _offscreenPolygonVB.Create(device, (void*)&squareVertex[0],
		SizeofVector<PolygonVertex>(squareVertex), sizeof(PolygonVertex));
	if (result == MYRESULT::FAILED) { return result; }

	// インデックスバッファー
	// TODO: index(6)
	std::vector<UINT> index;
	index.push_back(0); index.push_back(1); index.push_back(2);
	index.push_back(2); index.push_back(1); index.push_back(3);

	result = _offscreenPolygonIB.Create(device, index);
	if (result == MYRESULT::FAILED) { return result; }

	// シェーダー
	result = _offscreenVS.Create(offScreenRenderData.vertexShaderData);
	if (result == MYRESULT::FAILED) { return result; }
	result = _offscreenPS.Create(offScreenRenderData.pixelShaderData);
	if (result == MYRESULT::FAILED) { return result; }

	// オフスクリーンポリゴン生成
	PolygonData polygonData;
	polygonData.vertexBuffer = _offscreenPolygonVB;
	polygonData.indexBuffer = _offscreenPolygonIB;
	polygonData.vertexShader = _offscreenVS;
	polygonData.pixelShader = _offscreenPS;
	//polygonData.rootSignature = offScreenRenderData.rootSignature;
	polygonData.inputLayout.push_back(
		{
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		}
	);
	polygonData.inputLayout.push_back(
		{
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		}
	);

	result = _offscreenPolygon.Create(device, polygonData);
	if (result == MYRESULT::FAILED) { return result; }

	return result;
}

MYRESULT OffScreenRender::Create(ID3D12Device& device, const OffScreenRenderData& offScreenRenderData)
{
	_offScreenRenderData = offScreenRenderData;

	// オフスクリーンレンダーターゲット生成
	MYRESULT result = _offscreenRender.Create(device, offScreenRenderData.renderTargetData);
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
	ShaderResourceViewDesc desc(_offscreenTexture);
	_offscreenHeap.RegistShaderResource(device, _offscreenTexture, desc);

	// オフスクリーンポリゴン生成
	result = CreateOffScreenPolygon(device, offScreenRenderData);
	if (result == MYRESULT::FAILED) { return result; }

	return MYRESULT::SUCCESS;
}

void OffScreenRender::BeginRendering(RenderingContext& renderContext)
{
	// バリア処理
	renderContext.TransitionResourceState(
		_offscreenRender.GetBuffer(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	// オフスクリーンのレンダーターゲット設定
	auto offRtvHandle = _offscreenRTVHeap.GetCPUDescriptorHandleForHeapStart();
	renderContext.SetRenderTarget(&offRtvHandle, nullptr);

	// 画面を指定色でクリア
	renderContext.ClearRenderTarget(offRtvHandle, _offScreenRenderData.renderTargetData.clearColor, 0, nullptr);

	// ルートシグネチャセット
	renderContext.SetGraphicsRootSignature(_offScreenRenderData.rootSignature);

	// ビューポートとシザー矩形セット
	renderContext.SetViewport(_offScreenRenderData.viewport);
	renderContext.SetScissorRect(_offScreenRenderData.scissorRect);
}

void OffScreenRender::EndRendering(RenderingContext& renderContext)
{
	// 次のパスでレンダリング結果をテクスチャとして利用できるようにする
	renderContext.TransitionResourceState(
		_offscreenRender.GetBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

void OffScreenRender::Draw(RenderingContext& renderContext)
{
	_offscreenPolygon.Draw(renderContext);
}
