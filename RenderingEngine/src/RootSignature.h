#pragma once
#include <d3d12.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")

#include "EngineUtility.h"

/// <summary>
/// ディスクリプタの種類
/// </summary>
enum DESCRIPTORTYPE
{
	DESCRIPTORTYPE_SRV,
	DESCRIPTORTYPE_CBV,
	DESCRIPTORTYPE_UAV,
	DESCRIPTORTYPE_NUM
};

struct RootSignatureData
{
	D3D12_TEXTURE_ADDRESS_MODE _addressU;	// X方向のテクスチャマッピング設定
	D3D12_TEXTURE_ADDRESS_MODE _addressV;   // Y方向のテクスチャマッピング設定
	D3D12_TEXTURE_ADDRESS_MODE _addressW;   // Z方向のテクスチャマッピング設定
	UINT _srvDescriptorNum;	                // SRV数
	UINT _cbvDescriptorNum;	                // CBV数
	UINT _uavDescriptorNum;	                // UAV数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="addressU">X方向のテクスチャマッピング設定</param>
	/// <param name="addressV">Y方向のテクスチャマッピング設定</param>
	/// <param name="addressW">Z方向のテクスチャマッピング設定</param>
	/// <param name="srvDescriptorNum">SRV数</param>
	/// <param name="cbvDescriptorNum">CBV数</param>
	/// <param name="uavDescriptorNum">UAV数</param>
	RootSignatureData(
		D3D12_TEXTURE_ADDRESS_MODE addressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE addressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE addressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		UINT srvDescriptorNum = 1, UINT cbvDescriptorNum = 1, UINT uavDescriptorNum = 1);
};

class RootSignature {
public:
	RootSignature() = default;
	~RootSignature() = default;
private:
	Microsoft::WRL::ComPtr<ID3D12RootSignature> _rootSignature = nullptr;	// ルートシグネチャ

	/// <summary>
	/// ルートシグネチャ生成
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="data">ルートシグネチャの設定</param>
	/// <returns></returns>
	HRESULT CreateRootSignature(ID3D12Device& device, const RootSignatureData& data);
public:

	/// <summary>
	/// ルートシグネチャ生成
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="data">ルートシグネチャの設定</param>
	/// <returns></returns>
	MYRESULT Create(ID3D12Device& device, const RootSignatureData& data);
};