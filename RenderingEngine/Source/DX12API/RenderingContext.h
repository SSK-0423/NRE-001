#pragma once
#include <d3d12.h>
#include "EngineUtility.h"

class DescriptorHeapRTV;
class DescriptorHeapDSV;
class DescriptorHeapCBV_SRV_UAV;
class VertexBuffer;
class IndexBuffer;
class RootSignature;
class GraphicsPipelineState;

namespace NamelessEngine::DX12API
{
	/// <summary>
	/// レンダリングコンテキストクラス このクラスを介して描画命令を発行する
	/// </summary>
	class RenderingContext {
	public:
		RenderingContext();
		~RenderingContext();

	private:
		ID3D12GraphicsCommandList* _cmdList = nullptr;

	public:
		/// <summary>
		/// レンダリングコンテキストを初期化する
		/// </summary>
		/// <param name="cmdList">グラフィクスエンジンのコマンドリスト</param>
		void Init(ID3D12GraphicsCommandList& cmdList);

		/// <summary>
		/// コマンドリストの命令受付終了
		/// </summary>
		void Close();

		/// <summary>
		/// コマンドリストとコマンドアロケータを初期状態にする
		/// </summary>
		/// <param name="cmdAlloc">セットしたコマンドリストを生成する際に指定したコマンドアロケータ</param>
		void Reset(ID3D12CommandAllocator& cmdAlloc);

		/// <summary>
		/// リソースの状態遷移
		/// </summary>
		/// <param name="resource">遷移対象のリソース</param>
		/// <param name="beforeState">現在の状態</param>
		/// <param name="afterState">次の状態</param>
		void TransitionResourceState(
			ID3D12Resource& resource, D3D12_RESOURCE_STATES presentState, D3D12_RESOURCE_STATES afterState);

		/// <summary>
		/// レンダーターゲットを１つセットする
		/// </summary>
		/// <param name="_rtvHandle">レンダーターゲットビューのハンドル</param>
		/// <param name="_dsvHandle">デプスステンシルビューのハンドル</param>
		void SetRenderTarget(
			const D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandle, const D3D12_CPU_DESCRIPTOR_HANDLE* dsvHandle);

		/// <summary>
		/// 複数のレンダーターゲットをセットする
		/// </summary>
		/// <param name="rtvNum">セットするレンダーターゲット数</param>
		/// <param name="rtvHandles">レンダーターゲットビューのハンドルの配列</param>
		/// <param name="dsvHandle">デプスステンシルビューのハンドル</param>
		void SetRenderTargets(
			const UINT& rtvNum, const D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[],
			const D3D12_CPU_DESCRIPTOR_HANDLE* dsvHandle);

		/// <summary>
		/// 指定色でレンダーターゲット初期化
		/// </summary>
		/// <param name="rtvHandle">レンダーターゲットビューのハンドル</param>
		/// <param name="color">初期化色</param>
		/// <param name="numRects">rectsの要素数</param>
		/// <param name="rects">D3D12_RECTの配列 nullptrでレンダーターゲットの全範囲初期化</param>
		void ClearRenderTarget(
			D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, const Utility::ColorRGBA& color,
			const UINT& numRects, const D3D12_RECT* rects);

		/// <summary>
		/// 指定色でレンダーターゲット初期化
		/// </summary>
		/// <param name="rtvHandle">レンダーターゲットビューのハンドル</param>
		/// <param name="color">初期化色</param>
		/// <param name="numRects">rectsの要素数</param>
		/// <param name="rects">D3D12_RECTの配列 nullptrでレンダーターゲットの全範囲初期化</param>
		void ClearRenderTarget(
			D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, const float* color,
			const UINT& numRects, const D3D12_RECT* rects);

		/// <summary>
		/// デプスステンシルの値初期化
		/// </summary>
		/// <param name="dsvHandle">デプスステンシルビューのハンドル</param>
		/// <param name="clearFlags">初期化先の指定</param>
		/// <param name="clearDepth">初期化深度値</param>
		/// <param name="clearStencil">初期化ステンシル値</param>
		/// <param name="numRects">rectsの要素数</param>
		/// <param name="rects">D3D12_RECTの配列 nullptrでレンダーターゲットの全範囲初期化</param>
		void ClearDepthStencilView(
			D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle, D3D12_CLEAR_FLAGS clearFlags,
			const FLOAT& clearDepth, const UINT8& clearStencil, const UINT& numRects, const D3D12_RECT* rects);

		/// <summary>
		/// ビューポートセット
		/// </summary>
		/// <param name="viewport">ビューポート</param>
		void SetViewport(const D3D12_VIEWPORT& viewport);

		/// <summary>
		/// ビューポートを複数セット
		/// </summary>
		/// <param name="viewportNum">ビューポート数</param>
		/// <param name="viewports">ビューポート配列の先頭</param>
		void SetViewports(const UINT& viewportNum, const D3D12_VIEWPORT& viewports);

		/// <summary>
		/// シザー矩形セット
		/// </summary>
		void SetScissorRect(const D3D12_RECT& scissorRect);

		/// <summary>
		/// シザー矩形を複数セット
		/// </summary>
		/// <param name="scissorRectNum">シザー矩形数</param>
		/// <param name="scissorRect">シザー矩形配列の先頭</param>
		void SetScissorRects(const UINT& scissorRectNum, const D3D12_RECT& scissorRect);

		/// <summary>
		/// 頂点バッファーをセット
		/// </summary>
		/// <param name="startSlot">スロットの開始位置</param>
		/// <param name="vertexBuffer">頂点バッファー</param>
		void SetVertexBuffer(const UINT& startSlot, const VertexBuffer& vertexBuffer);

		/// <summary>
		/// インデックスバッファーセット
		/// </summary>
		/// <param name="indexBuffer">インデックスバッファー</param>
		void SetIndexBuffer(const IndexBuffer& indexBuffer);

		/// <summary>
		/// パイプラインステートセット
		/// </summary>
		/// <param name="pipelineState">グラフィックスパイプラインステート</param>
		void SetPipelineState(GraphicsPipelineState& pipelineState);

		/// <summary>
		/// ルートシグネチャセット
		/// </summary>
		/// <param name="rootSignature">ルートシグネチャ</param>
		void SetGraphicsRootSignature(RootSignature& rootSignature);

		/// <summary>
		/// プリミティブトポロジー設定
		/// </summary>
		/// <param name="primitiveTopology"></param>
		void SetPrimitiveTopology(const D3D12_PRIMITIVE_TOPOLOGY& primitiveTopology);

		/// <summary>
		/// ディスクリプタヒープをセットする
		/// </summary>
		/// <param name="descriptorHeap">ディスクリプタヒープのダブルポインタ</param>
		void SetDescriptorHeap(ID3D12DescriptorHeap* const* descriptorHeap);

		/// <summary>
		/// ディスクリプタヒープをセットする
		/// </summary>
		/// <param name="descriptorHeap">ディスクリプタヒープクラス</param>
		void SetDescriptorHeap(DescriptorHeapCBV_SRV_UAV& descriptorHeap);

		/// <summary>
		/// 複数のディスクリプタヒープをセットする
		/// </summary>
		/// <param name="descriptorHeapNum">セットするディスクリプタヒープ数</param>
		/// <param name="descriptorHeaps">ディスクリプタヒープのダブルポインタ</param>
		void SetDescriptorHeaps(const UINT& descriptorHeapNum, ID3D12DescriptorHeap* const* descriptorHeaps);

		/// <summary>
		/// ディスクリプタテーブルセット
		/// </summary>
		/// <param name="rootPrameterIndex">セットするルートパラメータのインデックス</param>
		/// <param name="handle">セット済みのディスクリプタヒープのGPUハンドル</param>
		void SetGraphicsRootDescriptorTable(const UINT& rootPrameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE handle);

		/// <summary>
		/// インスタンス描画
		/// </summary>
		/// <param name="vertexNumPerInstance">1インスタンス当たりの頂点数</param>
		/// <param name="instanceNum">描画するインスタンス数</param>
		/// <param name="startVertexLocation">頂点データのオフセット デフォルト:0</param>
		/// <param name="startInstanceLocation">インスタンスのオフセット デフォルト:0</param>
		void DrawInstanced(
			const UINT& vertexNumPerInstance, const UINT& instanceNum,
			const UINT& startVertexLocation = 0, const UINT& startInstanceLocation = 0);

		/// <summary>
		/// インスタンス描画
		/// </summary>
		/// <param name="indexNumPerInstance">1インスタンス当たりのインデックス数</param>
		/// <param name="instanceNum">描画するインスタンス数</param>
		/// <param name="startIndexLocation">頂点データのオフセット デフォルト:0</param>
		/// <param name="baseVertexLocation">頂点バッファから頂点を読み込む前に、各インデックスに追加される値
		///									デフォルト:0</param>
		/// <param name="startInstanceLocation">インスタンスのオフセット デフォルト:0</param>
		void DrawIndexedInstanced(
			const UINT& indexNumPerInstance, const UINT& instanceNum,
			const UINT startIndexLocation = 0, const INT& baseVertexLocation = 0,
			const UINT& startInstanceLocation = 0);

		/// <summary>
		/// テクスチャ領域のコピー
		/// </summary>
		/// <param name="src">GPU上のコピー元アドレス</param>
		/// <param name="dst">GPU上のコピー先アドレス</param>
		/// <param name="dstX">コピー先領域のオフセット(X) デフォルト値:0</param>
		/// <param name="dstY">コピー先領域のオフセット(Y) デフォルト値:0</param>
		/// <param name="dstZ">コピー先領域のオフセット(Z) デフォルト値:0</param>
		/// <param name="srcBox">コピー元領域ボックス デフォルト値:nullptr</param>
		void CopyTextureRegion(
			const D3D12_TEXTURE_COPY_LOCATION& src, const D3D12_TEXTURE_COPY_LOCATION& dst,
			const UINT& dstX = 0, const UINT& dstY = 0, const UINT& dstZ = 0, D3D12_BOX* srcBox = nullptr);
	};
}