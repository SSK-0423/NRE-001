#pragma once
#include <d3d12.h>

class DescriptorHeapRTV;
class DescriptorHeapDSV;
class VertexBuffer;
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
	/// 頂点バッファーをセット
	/// </summary>
	/// <param name="startSlot">スロットの開始位置</param>
	/// <param name="vertexBuffer">頂点バッファー</param>
	void SetVertexBuffer(UINT startSlot, const VertexBuffer& vertexBuffer);

};