#pragma once

#include "Dx12ApplicationImpl.h"

#include "EngineUtility.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RootSignature.h"
#include "Polygon.h"

#include "DepthStencilBuffer.h"
#include "DescriptorHeapDSV.h"
#include "DescriptorHeapCBV_SRV_UAV.h"
#include "ConstantBuffer.h"

#include "RenderTarget.h"
#include "Sprite.h"

#include <DirectXMath.h>

class DepthBufferSample : public Dx12ApplicationImpl
{
	struct NearConstBuff
	{
		DirectX::XMMATRIX worldViewProj;
		DirectX::XMMATRIX rotation;
	};

public:
	MYRESULT Init(Dx12GraphicsEngine& graphicsEngine, AppWindow& window) override;
	void Update(float deltaTime) override;
	void Draw(Dx12GraphicsEngine& graphicsEngine) override;
	void Final() override;

private:
	VertexBuffer _vertexBuffer;			        // 頂点バッファー
	IndexBuffer _indexBuffer;			        // インデックスバッファー
	Shader _vertexShader;				        // 頂点シェーダー
	Shader _pixelShader;				        // ピクセルシェーダー
	RootSignatureData _rootSignatureData;		// ルートシグネチャ
	MyFrameWork::Polygon _nearPolygon;		    // 手前ポリゴン
	MyFrameWork::Polygon _farPolygon;		    // 奥ポリゴン
	CD3DX12_VIEWPORT _viewport;			        // ビューポート
	CD3DX12_RECT _scissorRect;			        // シザー矩形

	float angle = 0.f;
	NearConstBuff _nearCBuffData;
	DescriptorHeapCBV_SRV_UAV _nearHeap;
	ConstantBuffer _nearCBuffer;

	RenderTarget _firstRender;
	Sprite _firstRenderingSprite;

	MYRESULT CreateNearPolygon(Dx12GraphicsEngine& graphicsEngine);
	MYRESULT CreateFarPolygon(Dx12GraphicsEngine& graphicsEngine);
	MYRESULT SetConstantBuffer(Dx12GraphicsEngine& graphicsEngine, AppWindow& window);
};