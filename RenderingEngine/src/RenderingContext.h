#pragma once
#include <d3d12.h>

/// <summary>
/// レンダリングコンテキストクラス このクラスを介して描画命令を発行する
/// </summary>
class RenderingContext {
public:
	RenderingContext();
	~RenderingContext();
private:
	ID3D12GraphicsCommandList* _cmdList;
public:
	/// <summary>
	/// レンダリングコンテキストを初期化する
	/// </summary>
	/// <param name="cmdList">グラフィクスエンジンのコマンドリスト</param>
	void Init(ID3D12GraphicsCommandList& cmdList);

	/// <summary>
	/// コマンドを再び受け付けられる状態にする
	/// </summary>
	/// <param name="cmdAlloc"></param>
	/// <param name="pipeline"></param>
	void Reset(ID3D12CommandAllocator* cmdAlloc, ID3D12PipelineState* pipeline);

};