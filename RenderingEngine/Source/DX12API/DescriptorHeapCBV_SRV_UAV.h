#pragma once

#include <d3d12.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")

#include "EngineUtility.h"


namespace NamelessEngine::DX12API
{
	class Texture;
	class ConstantBuffer;
	class UnorderedAccessResource;

	struct ShaderResourceViewDesc;

	class DescriptorHeapCBV_SRV_UAV
	{
	public:
		DescriptorHeapCBV_SRV_UAV() = default;
		~DescriptorHeapCBV_SRV_UAV() = default;
		static constexpr int NEXT_REGISTER = -1;

	private:
		static constexpr UINT _MAX_CBV_DESCRIPTOR_NUM = 64;	// 定数バッファーの最大数
		static constexpr UINT _MAX_SRV_DESCRIPTOR_NUM = 64;	// シェーダーリソースの最大数
		static constexpr UINT _MAX_UAV_DESCRIPTOR_NUM = 64;	// アンオーダーアクセスの最大数

		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _descriptorHeap = nullptr;

		SIZE_T _handleIncrimentSize = 0;
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
		/// <summary>
		/// ディスクリプタヒープ生成
		/// </summary>
		/// <param name="device">デバイス</param>
		/// <returns></returns>
		Utility::RESULT Create(ID3D12Device& device);

		/// <summary>
		/// GPUのディスクリプタヒープの先頭ハンドルを取得
		/// </summary>
		/// <returns></returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandleForHeapStart() {
			return _descriptorHeap->GetGPUDescriptorHandleForHeapStart();
		}

		/// <summary>
		/// CPUのディスクリプタヒープの先頭ハンドル取得
		/// </summary>
		/// <returns></returns>
		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandleForHeapStart() {
			return _descriptorHeap->GetCPUDescriptorHandleForHeapStart();
		}

		/// <summary>
		/// GPUのCBV部分のディスクリプタヒープの先頭ハンドルを取得
		/// </summary>
		/// <returns>GPUのCBV部分のディスクリプタヒープの先頭ハンドル</returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandleForHeapStartCBV() {
			return _descriptorHeap->GetGPUDescriptorHandleForHeapStart();
		}

		/// <summary>
		/// GPUのSRV部分のディスクリプタヒープの先頭ハンドルを取得
		/// </summary>
		/// <returns>GPUのSRV部分のディスクリプタヒープの先頭ハンドル</returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandleForHeapStartSRV() {
			auto handle = _descriptorHeap->GetGPUDescriptorHandleForHeapStart();
			handle.ptr += _handleIncrimentSize * _MAX_CBV_DESCRIPTOR_NUM;

			return handle;
		}

		D3D12_GPU_DESCRIPTOR_HANDLE GetSRVHandle(int index) {
			auto handle = _descriptorHeap->GetGPUDescriptorHandleForHeapStart();
			handle.ptr += _handleIncrimentSize * _MAX_CBV_DESCRIPTOR_NUM;
			handle.ptr += _handleIncrimentSize * index;
			return handle;
		}

		/// <summary>
		/// GPUのUAV部分のディスクリプタヒープの先頭ハンドルを取得
		/// </summary>
		/// <returns>GPUのUAV部分のディスクリプタヒープの先頭ハンドル</returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandleForHeapStartUAV() {
			auto handle = _descriptorHeap->GetGPUDescriptorHandleForHeapStart();
			handle.ptr += _handleIncrimentSize * (_MAX_CBV_DESCRIPTOR_NUM + _MAX_UAV_DESCRIPTOR_NUM);

			return handle;
		}

		/// <summary>
		/// ディスクリプタヒープハンドルのインクリメントサイズ取得
		/// </summary>
		/// <returns>インクリメントサイズ</returns>
		SIZE_T GetHandleIncrimentSize() {
			return _handleIncrimentSize;
		}

		/// <summary>
		/// コンスタントバッファ―登録
		/// </summary>
		/// <param name="device">デバイス</param>
		/// <param name="constantBuffer">コンスタントバッファ―</param>
		/// <param name="registerNo">
		/// 登録するレジスタ番号 デフォルト値:NEXT_REGISTER(-1) 現在登録されているリソース数の次のレジスタを使用
		/// </param>
		void RegistConstantBuffer(
			ID3D12Device& device, ConstantBuffer& constantBuffer, const int& registerNo = NEXT_REGISTER);

		/// <summary>
		/// シェーダーリソース(テクスチャ)登録
		/// </summary>
		/// <param name="device">デバイス</param>
		/// <param name="texture">テクスチャ</param>
		/// <param name="registerNo">
		/// 登録するレジスタ番号 デフォルト値:NEXT_REGISTER(-1) 現在登録されているリソース数の次のレジスタを使用
		/// </param>
		void RegistShaderResource(
			ID3D12Device& device, Texture& texture, ShaderResourceViewDesc& desc, const int& registerNo = NEXT_REGISTER);

		/// <summary>
		/// アンオーダーアクセスリソース登録
		/// </summary>
		/// <param name="device">デバイス</param>
		/// <param name="unorderedAccessResource">アンオーダーアクセスリソース</param>
		/// <param name="registerNo">
		/// 登録するレジスタ番号 デフォルト値:NEXT_REGISTER(-1) 現在登録されているリソース数の次のレジスタを使用
		/// </param>
		void RegistUnorderedAccessResource(
			ID3D12Device& device, UnorderedAccessResource& unorderedAccessResource,
			const int& registerNo = NEXT_REGISTER);

		/// <summary>
		/// コンスタントバッファーが登録されているか
		/// </summary>
		/// <returns></returns>
		bool IsRegistedConstantBuffer() { return _registedCBVNum > 0; }

		/// <summary>
		/// シェーダーリソースが登録されているか
		/// </summary>
		/// <returns></returns>
		bool IsRegistedShaderResource() { return _registedSRVNum > 0; }

		/// <summary>
		/// アンオーダーアクセスリソースが登録されているか
		/// </summary>
		/// <returns></returns>
		bool IsRegistedUnorderedAccessResource() { return _registedUAVNum > 0; }

		/// <summary>
		/// ディスクリプタヒープのアドレス取得
		/// </summary>
		/// <returns>ディスクリプタヒープのアドレス</returns>
		ID3D12DescriptorHeap** GetDescriptorHeapAddress() {
			return _descriptorHeap.GetAddressOf();
		}
	};
}