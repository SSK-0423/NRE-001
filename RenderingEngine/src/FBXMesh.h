#pragma once

#include <array>

#include "FBXLoader.h"

#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RootSignature.h"
#include "GraphicsPipelineState.h"

#include "DescriptorHeapCBV_SRV_UAV.h"
#include "ConstantBuffer.h"
#include "Texture.h"

#include "RenderingContext.h"

#include "EngineUtility.h"

/// <summary>
/// メッシュ生成用データ
/// </summary>
struct FBXMeshCreateData {
	/// <summary>
	/// PBRの場合 baseColorName,metallicName,roughnessNam,normalName,occlusionNameの設定が必要
	/// </summary>
	MATERIAL_TYPE materialType;

	const char* modelPath;								// 3Dモデルへのファイルパス

	/// <summary>
	/// 3Dモデルのテクスチャがあるフォルダへのパス
	/// 3Dモデル毎にフォルダを分けて管理するためこのパスが必要
	/// wstringなのはテクスチャロード関数対応のため
	/// </summary>
	std::wstring textureFolderPath;

	/// FBXSDKでPBRマテリアルを読み込む方法が分からない(対応してないっぽい)ので
	/// PBRテクスチャのファイル名を直接書いてもらう
	std::wstring baseColorName;
	std::wstring metallicName;
	std::wstring roughnessName;
	std::wstring normalName;
	std::wstring occlusionName;

	Shader vertexShader;	                            // 頂点シェーダー
	Shader pixelShader;									// ピクセルシェーダー
	RootSignatureData rootSignatureData;				// ルートシグネチャ
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;	// 頂点レイアウト
	std::array<DXGI_FORMAT, D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT> colorFormats = {
	DXGI_FORMAT_R8G8B8A8_UNORM,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_UNKNOWN,
	};	// レンダーターゲットのカラーフォーマット

	size_t GetRenderTargetNum() const;
};

/// <summary>
/// メッシュクラス
/// </summary>
class FBXMesh {
public:
	FBXMesh();
	~FBXMesh();

private:

	std::vector<FBXMeshData> _meshDataList;		// メッシュ情報のリスト

	VertexBuffer* _vertexBuffer = nullptr;
	MYRESULT CreateVertexBuffer(ID3D12Device& device, FBXMeshData& meshData);

	IndexBuffer* _indexBuffer = nullptr;
	MYRESULT CreateIndexBuffer(ID3D12Device& device, FBXMeshData& meshData);

	GraphicsPipelineState* _graphicsPipelineState = nullptr;
	MYRESULT CreateGraphicsPipelineState(ID3D12Device& device, FBXMeshCreateData& meshData);

	RootSignature* _rootSignature = nullptr;

	std::vector<FBXMesh*> _childs;

	// ディスクリプタヒープ
	DescriptorHeapCBV_SRV_UAV* _descriptorHeap = nullptr;
	MYRESULT CreateDescriptorHeap(ID3D12Device& device);

	/// <summary>
	/// マテリアルセット
	/// </summary>
	/// <param name="material"></param>
	void SetMaterial(IMaterial& material);

	/// <summary>
	///	親子共通の描画処理
	/// </summary>
	void CommonDraw(RenderingContext& renderContext);

	/// <summary>
	/// 階層メッシュ対応の描画(予定)
	/// </summary>
	/// <param name="renderContext"></param>
	/// <param name="isRootMesh"></param>
	void Draw(RenderingContext& renderContext, bool isRootMesh);

	MYRESULT CreateAsChild(Dx12GraphicsEngine& graphicsEngine, FBXMeshData& meshData);

public:
	MYRESULT LoadFBX(Dx12GraphicsEngine& device, FBXMeshCreateData& meshCreateData);

	void Draw(RenderingContext& renderContext);

	void SetConstantBuffer(ID3D12Device& device, ConstantBuffer& constantBuffer);

	void SetTexture(ID3D12Device& device, Texture& texture);

};