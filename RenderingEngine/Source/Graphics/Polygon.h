#pragma once

#include <array>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "GraphicsPipelineState.h"
#include "RootSignature.h"
#include "RenderingContext.h"
#include "Shader.h"
#include "DescriptorHeapCBV_SRV_UAV.h"

#include "EngineUtility.h"

/// <summary>
/// ポリゴン生成用データ
/// </summary>
struct PolygonData
{
	Shader vertexShader;	                            // 頂点シェーダー
	Shader pixelShader;									// ピクセルシェーダー
	VertexBuffer vertexBuffer;	                        // 頂点バッファー
	IndexBuffer indexBuffer;	                        // インデックスバッファー
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
/// ポリゴンの頂点データ
/// </summary>
struct PolygonVertex {
	DirectX::XMFLOAT3 vertex;
	DirectX::XMFLOAT2 uv;
};

namespace MyFrameWork
{
	class Polygon
	{
	public:
		Polygon() = default;
		~Polygon() = default;

	private:
		VertexBuffer _vertexBuffer;
		IndexBuffer _indexBuffer;
		GraphicsPipelineState _graphicsPipelineState;
		RootSignature _rootSignature;
		DescriptorHeapCBV_SRV_UAV* _descriptorHeap = nullptr;

		/// <summary>
		/// ポリゴン表示用のパイプラインステート生成
		/// </summary>
		/// <returns></returns>
		MYRESULT CreateGraphicsPipelineState(ID3D12Device& device, const PolygonData& data);

	public:
		/// <summary>
		/// ポリゴン生成
		/// </summary>
		/// <param name="device">デバイス</param>
		/// <param name="data">ポリゴンデータ</param>
		/// <returns></returns>
		MYRESULT Create(ID3D12Device& device, const PolygonData& data);

		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="renderContext">レンダリングコンテキスト</param>
		void Draw(RenderingContext& renderContext);

		void SetDescriptorHeap(DescriptorHeapCBV_SRV_UAV& descriptorHeap);
	};
}

/// メモ
/// 1.レンダリングコンテキスト
/// 2.GraphicsPipelineState
/// 3.頂点バッファー
/// 4.インデックスバッファー()