#include "TextureMappingSample.h"

#include <iostream>

MYRESULT TextureMappingSample::Init(Dx12GraphicsEngine& graphicsEngine, AppWindow& window)
{
	// ポリゴン初期化
	MYRESULT result = InitPolygon(graphicsEngine, window);
	if (result == MYRESULT::FAILED) { return result; }

	// テクスチャ初期化
	result = InitTexture(graphicsEngine);
	if (result == MYRESULT::FAILED) { return result; }

	// ビューポートセット
	_viewport = CD3DX12_VIEWPORT(0.f, 0.f,
		static_cast<FLOAT>(window.GetWindowSize().cx),
		static_cast<FLOAT>(window.GetWindowSize().cy));

	// シザー矩形セット
	_scissorRect = CD3DX12_RECT(0, 0, window.GetWindowSize().cx, window.GetWindowSize().cy);

	return result;
}

void TextureMappingSample::Update(float deltaTime)
{
}

void TextureMappingSample::Draw(Dx12GraphicsEngine& graphicsEngine)
{
	// レンダリングコンテキスト取得
	RenderingContext& renderContext = graphicsEngine.GetRenderingContext();

	renderContext.SetViewport(_viewport);
	renderContext.SetScissorRect(_scissorRect);

	// ポリゴン描画
	_square.Draw(renderContext);
}

void TextureMappingSample::Final()
{
}

MYRESULT TextureMappingSample::InitPolygon(Dx12GraphicsEngine& graphicsEngine, AppWindow& window)
{
	// 四角形ポリゴン
	std::vector<PolygonVertex> squareVertex(4);
	squareVertex[0] = { {-1.f,-1.f,0.f},{0.f,2.f} };
	squareVertex[1] = { {-1.f,1.f,0.f},{0.f,0.f} };
	squareVertex[2] = { {1.f,-1.f,0.f},{2.f,2.f} };
	squareVertex[3] = { {1.f,1.f,0.f},{2.f,0.f} };

	// 頂点バッファー生成
	MYRESULT result = _vertexBuffer.Create(graphicsEngine.Device(), (void*)&squareVertex[0],
		SizeofVector<PolygonVertex>(squareVertex), sizeof(PolygonVertex));

	// 四角形ポリゴンのインデックスデータ用意
	std::vector<UINT> index(6);
	index[0] = 0; index[1] = 1; index[2] = 2;
	index[3] = 2; index[4] = 1; index[5] = 3;

	// インデックスバッファー生成
	result = _indexBuffer.Create(graphicsEngine.Device(), index);

	// ルートシグネチャ生成
	RootSignatureData rootSignatureData;
	rootSignatureData._samplerData.addressU = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
	rootSignatureData._samplerData.addressV = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;

	// シェーダー
	result = _vertexShader.Create(L"src/TextureMapSampleVertex.hlsl", "main", "vs_5_0");
	result = _pixelShader.Create(L"src/TextureMapSamplePixel.hlsl", "main", "ps_5_0");

	// ポリゴン生成
	PolygonData polygonData;
	polygonData.vertexBuffer = _vertexBuffer;
	polygonData.indexBuffer = _indexBuffer;
	polygonData.vertexShader = _vertexShader;
	polygonData.pixelShader = _pixelShader;
	polygonData.rootSignatureData = rootSignatureData;
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

	result = _square.Create(graphicsEngine.Device(), polygonData);

	return result;
}

MYRESULT TextureMappingSample::InitTexture(Dx12GraphicsEngine& graphicsEngine)
{
	ID3D12Device& device = graphicsEngine.Device();

	std::vector<ColorRGBA> _texData(512 * 512);
	for (auto& color : _texData) {
		color = ColorRGBA(0.f, 1.f, 1.f, 1.f);
	}

	// テクスチャファイルパス
	std::wstring path(L"Ramen.JPG");

	// テクスチャ生成
	//MYRESULT result = _texture.CreateTextureFromWIC(graphicsEngine, path);
	MYRESULT result = _texture.CreateTextureFromRGBAData(graphicsEngine, _texData, 512, 512, DXGI_FORMAT_R32G32B32A32_FLOAT);
	if (result == MYRESULT::FAILED) { return result; }

	// テクスチャ用ヒープ生成
	result = _textureHeap.Create(device);
	if (result == MYRESULT::FAILED) { return result; }

	// テクスチャをヒープに登録
	_textureHeap.RegistShaderResource(device, _texture);

	// ポリゴンのディスクリプタヒープセット
	_square.SetDescriptorHeap(_textureHeap);

	return result;
}