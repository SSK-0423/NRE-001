#pragma once

#include <array>

#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RootSignature.h"
#include "GraphicsPipelineState.h"

#include "DescriptorHeapCBV_SRV_UAV.h"
#include "ConstantBuffer.h"
#include "Texture.h"

#include "RenderingContext.h"

#include "FBXLoader.h"

#include "Dx12GraphicsEngine.h"

#include "EngineUtility.h"

namespace NamelessEngine::Graphics
{
	class FBXLoader;
	
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

		DX12API::Shader vertexShader;	                            // 頂点シェーダー
		DX12API::Shader pixelShader;									// ピクセルシェーダー
		DX12API::RootSignatureData rootSignatureData;				// ルートシグネチャ
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

		DX12API::VertexBuffer* _vertexBuffer = nullptr;
		Utility::RESULT CreateVertexBuffer(ID3D12Device& device, FBXMeshData& meshData);

		DX12API::IndexBuffer* _indexBuffer = nullptr;
		Utility::RESULT CreateIndexBuffer(ID3D12Device& device, FBXMeshData& meshData);

		DX12API::GraphicsPipelineState* _graphicsPipelineState = nullptr;
		Utility::RESULT CreateGraphicsPipelineState(ID3D12Device& device, FBXMeshCreateData& meshData);

		DX12API::RootSignature* _rootSignature = nullptr;

		std::vector<FBXMesh*> _childs;

		// ディスクリプタヒープ
		DX12API::DescriptorHeapCBV_SRV_UAV* _descriptorHeap = nullptr;
		Utility::RESULT CreateDescriptorHeap(ID3D12Device& device);

		/// <summary>
		/// マテリアルセット
		/// </summary>
		/// <param name="material"></param>
		void SetMaterial(IMaterial& material);

		/// <summary>
		///	親子共通の描画処理
		/// </summary>
		void CommonDraw(DX12API::RenderingContext& renderContext);

		/// <summary>
		/// 階層メッシュ対応の描画(予定)
		/// </summary>
		/// <param name="renderContext"></param>
		/// <param name="isRootMesh"></param>
		void Draw(DX12API::RenderingContext& renderContext, bool isRootMesh);

		Utility::RESULT CreateAsChild(Core::Dx12GraphicsEngine& graphicsEngine, FBXMeshData& meshData);

	public:
		Utility::RESULT LoadFBX(Core::Dx12GraphicsEngine& device, FBXMeshCreateData& meshCreateData);

		void Draw(DX12API::RenderingContext& renderContext);

		void SetConstantBuffer(
			ID3D12Device& device, DX12API::ConstantBuffer& constantBuffer,
			const int& registerNo = DX12API::DescriptorHeapCBV_SRV_UAV::NEXT_REGISTER);

		void SetTexture(
			ID3D12Device& device, DX12API::Texture& texture,
			const int& registerNo = DX12API::DescriptorHeapCBV_SRV_UAV::NEXT_REGISTER);

	};
}