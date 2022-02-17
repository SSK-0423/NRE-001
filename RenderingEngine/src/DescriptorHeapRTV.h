#pragma once
#include <d3d12.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")

#include "EngineUtility.h"

/// <summary>
/// レンダーターゲットビュー用のディスクリプタヒープ
/// </summary>
class DescriptorHeapRTV
{
public:
	DescriptorHeapRTV() = default;
	~DescriptorHeapRTV() = default;

private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _rtvHeap = nullptr;	// ディスクリプタヒープ
	UINT _handleIncrimentSize = 0;	                                    // ハンドルのインクリメントサイズ

	/// <summary>
	/// ディスクリプタヒープ生成
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <returns></returns>
	HRESULT CreateDescriptorHeap(ID3D12Device& device);

public:
	/// <summary>
	/// RTV用ディスクリプタヒープ生成 これを実行することでディスクリプタヒープが扱えるようになる
	/// </summary>
	/// <param name="device"></param>
	/// <returns>MYRESULT::SUCCESS: 成功 MYRESULT::FAILED: 失敗</returns>
	MYRESULT Create(ID3D12Device& device);

	/// <summary>
	/// CPUのディスクリプタヒープの先頭ハンドルを取得
	/// </summary>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE* GetCPUDescriptorHandleForHeapStart() {
		return &_rtvHeap->GetCPUDescriptorHandleForHeapStart();
	}

	/// <summary>
	/// ディスクリプタヒープハンドルのインクリメントサイズ取得
	/// </summary>
	/// <returns></returns>
	UINT GetHandleIncrimentSize() {
		return _handleIncrimentSize;
	}
};