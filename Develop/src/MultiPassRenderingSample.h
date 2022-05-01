#pragma once
#include "Dx12ApplicationImpl.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RootSignature.h"
#include "Polygon.h"

#include "RenderTarget.h"
#include "Sprite.h"

class MultiPassRendering : public Dx12ApplicationImpl {
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


private:
	RenderTarget _renderTarget;			// レンダーターゲット
	CD3DX12_VIEWPORT _viewport;			// ビューポート
	CD3DX12_RECT _scissorRect;			// シザー矩形

	Sprite _sprite;						// スプライト
	Sprite _myself;						// 1パス目に描画するスプライト(レンダーターゲット2つ)

	Texture _texture;					// １パス目に描画するテクスチャ
};