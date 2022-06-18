#pragma once

#include <array>

#include "FBXLoader.h"

#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RootSignature.h"
#include "GraphicsPipelineState.h"
#include "DescriptorHeapCBV_SRV_UAV.h"
#include "RenderingContext.h"

#include "EngineUtility.h"
/// <summary>
/// メッシュ生成用データ
/// </summary>
struct MeshData {
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

	/// <summary>
	/// カラーフォーマット配列を調べてレンダーターゲット数を返す
	/// </summary>
	/// <returns>レンダーターゲット数</returns>
	size_t GetRenderTargetNum() const;
};

/// <summary>
/// メッシュクラス
/// </summary>
class Mesh {
public:
	Mesh();
	~Mesh();

private:
	IMeshLoader* _loader = nullptr;
	FBXLoader* _fbxLoader = nullptr;

	VertexBuffer _vertexBuffer;
	MYRESULT CreateVertexBuffers(ID3D12Device& device);

	IndexBuffer _indexBuffer;
	MYRESULT CreateIndexBuffers(ID3D12Device& device);

	GraphicsPipelineState _graphicsPipelineState;
	MYRESULT CreateGraphicsPipelineState(ID3D12Device& device, MeshData& meshData);

	RootSignature _rootSignature;

	DescriptorHeapCBV_SRV_UAV* _descriptorHeap = nullptr;

public:
	/// <summary>
	/// メッシュ読み込み
	/// </summary>
	/// <param name="modelPath">ファイルパス</param>
	/// <returns></returns>
	MYRESULT LoadMesh(const char* modelPath, ID3D12Device& device, MeshData& meshData);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="renderContext"></param>
	void Draw(RenderingContext& renderContext);

	void SetDescriptorHeap(DescriptorHeapCBV_SRV_UAV& descriptorHeap);

};