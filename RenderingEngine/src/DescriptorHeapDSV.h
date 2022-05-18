#pragma once

#include <d3d12.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")

#include "EngineUtility.h"
#include "DepthStencilBuffer.h"

class DescriptorHeapDSV
{
public:
	DescriptorHeapDSV() = default;
	~DescriptorHeapDSV() = default;

private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _dsvHeap = nullptr;		// ディスクリプタヒープ
	SIZE_T _handleIncrimentSize = 0;										// ハンドルのインクリメントサイズ
	UINT _registedDSVNum = 0;									            // ヒープに登録されたディスクリプタ数
	
	/// <summary>
	/// ディスクリプタヒープ生成
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <returns></returns>
	HRESULT CreateDescriptorHeap(ID3D12Device& device);

public:
	/// <summary>
	/// DSV用ディスクリプタヒープ生成 これを実行することでディスクリプタヒープが扱えるようになる
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <returns>MYRESULT::SUCCESS: 成功 MYRESULT::FAILED: 失敗</returns>
	MYRESULT Create(ID3D12Device& device);

	/// <summary>
	/// ディスクリプタヒープハンドルのインクリメントサイズ取得
	/// </summary>
	/// <returns></returns>
	SIZE_T GetHandleIncrimentSize() {
		return _handleIncrimentSize;
	}

	/// <summary>
	/// 登録済みのディスクリプタ数取得
	/// </summary>
	/// <returns></returns>
	UINT GetRegistedDescriptorNum() {
		return _registedDSVNum;
	}

	/// <summary>
	/// ディスクリプタ登録
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="buffer">デプスステンシルバッファー</param>
	void RegistDescriptor(ID3D12Device& device, DepthStencilBuffer& buffer);

	/// <summary>
	/// CPUのディスクリプタヒープの先頭ハンドルを取得
	/// </summary>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandleForHeapStart() {
		return _dsvHeap->GetCPUDescriptorHandleForHeapStart();
	}
};