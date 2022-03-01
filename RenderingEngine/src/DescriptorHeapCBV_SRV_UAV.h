#pragma once

#include <d3d12.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")

#include "EngineUtility.h"

class Texture;
class ConstantBuffer;

class DescriptorHeapCBV_SRV_UAV
{
public:
	DescriptorHeapCBV_SRV_UAV() = default;
	~DescriptorHeapCBV_SRV_UAV() = default;

private:
	static constexpr UINT MAX_CBV_DESCRIPTOR_NUM = 8;	// 定数バッファーの最大数
	static constexpr UINT MAX_SRV_DESCRIPTOR_NUM = 8;	// シェーダーリソースの最大数
	static constexpr UINT MAX_UAV_DESCRIPTOR_NUM = 8;	// アンオーダーアクセスの最大数

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _descriptorHeap = nullptr;

	UINT _handleIncrimentSize = 0;
	UINT _registedCBVNum = 0;
	UINT _registedSRVNum = 0;
	UINT _registedUAVNum = 0;

	/// <summary>
	/// ディスクリプタヒープ生成
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <returns></returns>
	HRESULT CreateDescriptorHeap(ID3D12Device& device);

public:
	MYRESULT Create(ID3D12Device& device);

	/// <summary>
	/// CPUのディスクリプタヒープの先頭ハンドルを取得
	/// </summary>
	/// <returns>CPUのディスクリプタヒープの先頭ハンドル</returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandleForHeapStart() {
		return _descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	}

	/// <summary>
	/// ディスクリプタヒープハンドルのインクリメントサイズ取得
	/// </summary>
	/// <returns>インクリメントサイズ</returns>
	UINT GetHandleIncrimentSize() {
		return _handleIncrimentSize;
	}

	/// <summary>
	/// シェーダーリソース(テクスチャ)登録
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="texture">テクスチャ</param>
	void RegistShaderResource(ID3D12Device& device, Texture& texture);

	/// <summary>
	/// コンスタントバッファ―登録
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="constantBuffer">コンスタントバッファ―</param>
	void RegistConstantBuffer(ID3D12Device& device, ConstantBuffer& constantBuffer);
};