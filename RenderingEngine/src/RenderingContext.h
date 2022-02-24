#pragma once
#include <d3d12.h>

class DescriptorHeapRTV;
class DescriptorHeapDSV;
class VertexBuffer;
class IndexBuffer;
class RootSignature;
class GraphicsPipelineState;
struct ColorRGBA;

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
		UINT rtvNum, const D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[],
		const D3D12_CPU_DESCRIPTOR_HANDLE* dsvHandle);

	/// <summary>
	/// 指定色でレンダーターゲット初期化
	/// </summary>
	/// <param name="rtvHandle">レンダーターゲットビューのハンドル</param>
	/// <param name="color">初期化色</param>
	/// <param name="numRects">rectsの要素数</param>
	/// <param name="rects">D3D12_RECTの配列 nullptrでレンダーターゲットの全範囲初期化</param>
	void ClearRenderTarget(
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, const ColorRGBA& color, UINT numRects, const D3D12_RECT* rects);

	/// <summary>
	/// 指定色でレンダーターゲット初期化
	/// </summary>
	/// <param name="rtvHandle">レンダーターゲットビューのハンドル</param>
	/// <param name="color">初期化色</param>
	/// <param name="numRects">rectsの要素数</param>
	/// <param name="rects">D3D12_RECTの配列 nullptrでレンダーターゲットの全範囲初期化</param>
	void ClearRenderTarget(
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, const float* color, UINT numRects, const D3D12_RECT* rects);

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
	void SetViewports(UINT viewportNum, const D3D12_VIEWPORT& viewports);

	/// <summary>
	/// シザー矩形セット
	/// </summary>
	void SetScissorRect(const D3D12_RECT& scissorRect);

	/// <summary>
	/// シザー矩形を複数セット
	/// </summary>
	/// <param name="scissorRectNum">シザー矩形数</param>
	/// <param name="scissorRect">シザー矩形配列の先頭</param>
	void SetScissorRects(UINT scissorRectNum, const D3D12_RECT& scissorRect);

	/// <summary>
	/// 頂点バッファーをセット
	/// </summary>
	/// <param name="startSlot">スロットの開始位置</param>
	/// <param name="vertexBuffer">頂点バッファー</param>
	void SetVertexBuffer(UINT startSlot, const VertexBuffer& vertexBuffer);

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
	void SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY primitiveTopology);

	/// <summary>
	/// インスタンス描画
	/// </summary>
	/// <param name="vertexNumPerInstance">1インスタンス当たりの頂点数</param>
	/// <param name="instanceNum">描画するインスタンス数</param>
	/// <param name="startVertexLocation">頂点データのオフセット デフォルト:0</param>
	/// <param name="startInstanceLocation">インスタンスのオフセット デフォルト:0</param>
	void DrawInstanced(
		UINT vertexNumPerInstance, UINT instanceNum,
		UINT startVertexLocation = 0, UINT startInstanceLocation = 0);

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
		UINT indexNumPerInstance, UINT instanceNum,
		UINT startIndexLocation = 0, INT baseVertexLocation = 0, UINT startInstanceLocation = 0);
};