#pragma once
#include <d3d12.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")

#include "EngineUtility.h"

namespace NamelessEngine::DX12API
{
	/// <summary>
	/// コンスタントバッファ―クラス
	/// </summary>
	class ConstantBuffer {
	public:
		ConstantBuffer() = default;
		~ConstantBuffer() = default;

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> _constantBuffer = nullptr;	// バッファー
		BYTE* _mappedData = nullptr;	                                    // データのマップ先
		UINT _bufferSize = 0;	                                            // バッファーサイズ

		/// <summary>
		/// コンスタントバッファ―生成
		/// </summary>
		/// <param name="device">デバイス</param>
		/// <param name="bufferSize">バッファーサイズ
		/// <returns></returns>
		HRESULT CreateConstantBuffer(ID3D12Device& device, const UINT& bufferSize);

		/// <summary>
		/// マップ処理
		/// </summary>
		/// <param name="data">デバイス</param>
		/// <param name="bufferSize">バッファーサイズ</param>
		/// <returns></returns>
		HRESULT MapConstantBuffer(void* data, const UINT& bufferSize);

	public:
		/// <summary>
		/// コンスタントバッファ―生成
		/// </summary>
		/// <param name="device">デバイス</param>
		/// <param name="data">バッファーに入れるデータ</param>
		/// <param name="bufferSize">バッファーサイズ</param>
		/// <returns>Utility::MYRESULT::SUCCESS: 成功 Utility::MYRESULT::FAILED: 失敗</returns>
		Utility::MYRESULT Create(ID3D12Device& device, void* data, const UINT& bufferSize);

		/// <summary>
		/// バッファーのGPU上のバーチャルアドレス取得
		/// </summary>
		/// <returns>バッファーのGPU上のバーチャルアドレス</returns>
		D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() {
			return _constantBuffer->GetGPUVirtualAddress();
		}

		/// <summary>
		/// バッファー取得
		/// </summary>
		/// <returns>バッファ―</returns>
		ID3D12Resource& GetBuffer() {
			return *_constantBuffer.Get();
		}

		/// <summary>
		/// バッファーサイズ取得
		/// </summary>
		/// <returns>バッファーサイズ</returns>
		UINT& GetBufferSize() {
			return _bufferSize;
		}

		/// <summary>
		/// データ更新 バッファー生成時の指定サイズ分のデータをコピーする
		/// </summary>
		/// <param name="data">データ</param>
		void UpdateData(void* data) {
			std::memcpy((void*)_mappedData, data, static_cast<size_t>(_bufferSize));
		}
	};
}