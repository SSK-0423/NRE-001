#include "Sprite.h"

#include "Dx12GraphicsEngine.h"
#include "RenderingContext.h"
#include "Texture.h"
#include "ShaderResourceViewDesc.h"

MYRESULT Sprite::CreatePolygon(ID3D12Device& device, SpriteData& spriteData)
{
	// 四角形ポリゴン
	std::vector<PolygonVertex> squareVertex(4);
	squareVertex[0] = { {-1.f,-1.f,0.f},{0.f,1.f} };
	squareVertex[1] = { {-1.f,1.f,0.f},{0.f,0.f} };
	squareVertex[2] = { {1.f,-1.f,0.f},{1.f,1.f} };
	squareVertex[3] = { {1.f,1.f,0.f},{1.f,0.f} };

	// 頂点バッファー生成
	MYRESULT result = _vertexBuffer.Create(device, (void*)&squareVertex[0],
		SizeofVector<PolygonVertex>(squareVertex), sizeof(PolygonVertex));

	// 四角形ポリゴンのインデックスデータ用意
	std::vector<UINT> index(6);
	index[0] = 0; index[1] = 1; index[2] = 2;
	index[3] = 2; index[4] = 1; index[5] = 3;

	// インデックスバッファー生成
	result = _indexBuffer.Create(device, index);

	// シェーダー
	result = _vertexShader.Create(spriteData.vertexShaderData);
	if (result == MYRESULT::FAILED) { return result; }
	result = _pixelShader.Create(spriteData.pixelShaderData);
	if (result == MYRESULT::FAILED) { return result; }

	// ポリゴン生成
	PolygonData polygonData;
	polygonData.vertexBuffer = _vertexBuffer;
	polygonData.indexBuffer = _indexBuffer;
	polygonData.vertexShader = _vertexShader;
	polygonData.pixelShader = _pixelShader;
	polygonData.colorFormats = spriteData.colorFormats;
	polygonData.rootSignatureData = spriteData.rootSignatureData;
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

	result = _polygon.Create(device, polygonData);
	if (result == MYRESULT::FAILED) { return result; }

	return result;
}

MYRESULT Sprite::CreateTextureResource(
	Dx12GraphicsEngine& graphicsEngine, ID3D12Device& device, SpriteData& spriteData)
{
	// テクスチャ用ヒープ生成
	MYRESULT result = _textureHeap.Create(device);
	if (result == MYRESULT::FAILED) { return result; }

	// テクスチャリソース生成
	// ファイルパスからテクスチャを生成する
	if (spriteData.texturePaths[0] != nullptr) {
		for (auto path : spriteData.texturePaths) {
			if (path == nullptr) { break; }
			// 保留
			result = MYRESULT::FAILED;
		}
	}

	// 既存のテクスチャを使用する
	else if (spriteData.textures[0] != nullptr) {
		for (auto tex : spriteData.textures) {
			if (tex == nullptr) { break; }
			ShaderResourceViewDesc desc(*tex);
			_textureHeap.RegistShaderResource(device, *tex, desc);
		}
		_polygon.SetDescriptorHeap(_textureHeap);
	}

	return result;
}

MYRESULT Sprite::Create(Dx12GraphicsEngine& graphicsEngine, SpriteData& spriteData)
{
	ID3D12Device& device = graphicsEngine.Device();

	// ポリゴン生成
	MYRESULT result = CreatePolygon(device, spriteData);
	if (result == MYRESULT::FAILED) { return result; }

	// テクスチャリソース生成
	result = CreateTextureResource(graphicsEngine, device, spriteData);
	if (result == MYRESULT::FAILED) { return result; }

	return result;
}

void Sprite::Draw(RenderingContext& renderContext)
{
	_polygon.Draw(renderContext);
}
