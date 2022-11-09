#pragma once
#include <d3d12.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")

#include "EngineUtility.h"

namespace NamelessEngine::DX12API
{
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

	struct SamplerData
	{
		D3D12_FILTER samplerFilter;			// サンプラーフィルター
		D3D12_TEXTURE_ADDRESS_MODE addressU;	// X方向のテクスチャマッピング設定
		D3D12_TEXTURE_ADDRESS_MODE addressV;   // Y方向のテクスチャマッピング設定
		D3D12_TEXTURE_ADDRESS_MODE addressW;   // Z方向のテクスチャマッピング設定

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="samplerFilter">サンプラ－フィルター設定</param>
		/// <param name="addressU">X方向のテクスチャマッピング設定</param>
		/// <param name="addressV">Y方向のテクスチャマッピング設定</param>
		/// <param name="addressW">Z方向のテクスチャマッピング設定</param>
		SamplerData(
			D3D12_FILTER samplerFilter = D3D12_FILTER_ANISOTROPIC,
			D3D12_TEXTURE_ADDRESS_MODE addressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE addressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE addressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP);
	};

	struct DescriptorRangeData
	{
		UINT srvDescriptorNum;	                // SRV数
		UINT cbvDescriptorNum;	                // CBV数
		UINT uavDescriptorNum;	                // UAV数
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="srvDescriptorNum">SRV数</param>
		/// <param name="cbvDescriptorNum">CBV数</param>
		/// <param name="uavDescriptorNum">UAV数</param>
		DescriptorRangeData(UINT srvDescriptorNum = 1, UINT cbvDescriptorNum = 1, UINT uavDescriptorNum = 1);
	};

	/// <summary>
	/// ルートシグネチャクラス生成用構造体
	/// </summary>
	struct RootSignatureData
	{
		SamplerData _samplerData;				// サンプラーの設定
		DescriptorRangeData _descRangeData;		// ディスクリプタレンジの設定

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="samplerFilter">サンプラ－フィルター設定</param>
		/// <param name="addressU">X方向のテクスチャマッピング設定</param>
		/// <param name="addressV">Y方向のテクスチャマッピング設定</param>
		/// <param name="addressW">Z方向のテクスチャマッピング設定</param>
		/// <param name="srvDescriptorNum">SRV数</param>
		/// <param name="cbvDescriptorNum">CBV数</param>
		/// <param name="uavDescriptorNum">UAV数</param>
		RootSignatureData(
			D3D12_FILTER samplerFilter = D3D12_FILTER_ANISOTROPIC,
			D3D12_TEXTURE_ADDRESS_MODE addressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE addressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE addressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			UINT srvDescriptorNum = 1, UINT cbvDescriptorNum = 1, UINT uavDescriptorNum = 1);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="samplerData">サンプラーデータ</param>
		/// <param name="descRangeData">ディスクリプタレンジデータ</param>
		RootSignatureData(SamplerData samplerData, DescriptorRangeData descRangeData);
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
		/// <param name="descRangeData">ディスクリプタレンジの設定</param>
		/// <param name="samplerDescs">サンプラー</param>
		/// <param name="samplerNum">サンプラー数</param>
		/// <returns></returns>
		HRESULT CreateRootSignature(
			ID3D12Device& device, const DescriptorRangeData& descRangeData,
			const D3D12_STATIC_SAMPLER_DESC& samplerDescs, UINT samplerNum);

	public:

		/// <summary>
		/// ルートシグネチャ生成
		/// </summary>
		/// <param name="device">デバイス</param>
		/// <param name="data">ルートシグネチャの設定</param>
		/// <returns></returns>
		Utility::RESULT Create(ID3D12Device& device, const RootSignatureData& data);

		/// <summary>
		/// ルートシグネチャ生成
		/// </summary>
		/// <param name="device"></param>
		/// <param name="data"></param>
		/// <param name="samplerDescs"></param>
		/// <param name="samplerNum"></param>
		/// <returns></returns>
		Utility::RESULT Create(
			ID3D12Device& device, const DescriptorRangeData& descRangeData,
			const D3D12_STATIC_SAMPLER_DESC& samplerDescs, UINT samplerNum);

		/// <summary>
		/// ルートシグネチャ取得
		/// </summary>
		/// <returns>ルートシグネチャ</returns>
		ID3D12RootSignature& GetRootSignature() const {
			return *_rootSignature.Get();
		}

	};
}