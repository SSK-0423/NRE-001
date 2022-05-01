#pragma once

#include "Dx12ApplicationImpl.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RootSignature.h"
#include "Polygon.h"

#include "DescriptorHeapCBV_SRV_UAV.h"
#include "Texture.h"

class TextureMappingSample : public Dx12ApplicationImpl {
public:
	MYRESULT Init(Dx12GraphicsEngine& graphicsEngine, AppWindow& window) override;
	void Update(float deltaTime) override;
	void Draw(Dx12GraphicsEngine& graphicsEngine) override;
	void Final() override;

// ポリゴン関連
private:
	VertexBuffer _vertexBuffer;			// 頂点バッファー
	IndexBuffer _indexBuffer;			// インデックスバッファー
	Shader _vertexShader;				// 頂点シェーダー
	Shader _pixelShader;				// ピクセルシェーダー
	MyFrameWork::Polygon _square;		// 四角形ポリゴン

	/// <summary>
	/// ポリゴン初期化
	/// </summary>
	/// <returns></returns>
	MYRESULT InitPolygon(Dx12GraphicsEngine& graphicsEngine, AppWindow& window);	            // ポリゴン初期化

// テクスチャ関連
private:
	DescriptorHeapCBV_SRV_UAV _textureHeap;		// テクスチャ用ヒープ
	Texture _texture;							// テクスチャ

	/// <summary>
	/// テクスチャ初期化
	/// </summary>
	/// <returns></returns>
	MYRESULT InitTexture(Dx12GraphicsEngine& graphicsEngine);

	CD3DX12_VIEWPORT _viewport;			// ビューポート
	CD3DX12_RECT _scissorRect;			// シザー矩形
};