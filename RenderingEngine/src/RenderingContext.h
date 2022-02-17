#pragma once
#include <d3d12.h>

class RenderTarget;

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
	/// レンダーターゲットセット
	/// </summary>
	/// <param name="renderTarget"></param>
	/// <param name="descriptorContinuous"></param>
	void SetRenderTargets(RenderTarget& renderTarget,BOOL descriptorContinuous);

};