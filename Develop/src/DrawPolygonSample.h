#pragma once

#include "Dx12ApplicationImpl.h"

#include "EngineUtility.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RootSignature.h"
#include "Polygon.h"

class DrawPolygonSample : public Dx12ApplicationImpl {
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
	MyFrameWork::Polygon _triangle;		        // 三角形ポリゴン
	CD3DX12_VIEWPORT _viewport;			        // ビューポート
	CD3DX12_RECT _scissorRect;			        // シザー矩形
};