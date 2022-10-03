#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include "EngineUtility.h"

namespace NamelessEngine::DX12API
{
	struct DepthStencilBufferData
	{
		DXGI_FORMAT depthFormat;
		UINT width;
		UINT height;
		float clearDepth;
		UINT8 clearStencil;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="colorFormat">深度フォーマット デフォルト：DXGI_FORMAT_D32_FLOAT</param>
		/// <param name="width">レンダーターゲット幅 デフォルト：1024</param>
		/// <param name="height">レンダーターゲット高 デフォルト：768</param>
		/// <param name="clearColor">初期化深度 デフォルト：1.f</param>
		DepthStencilBufferData(
			const DXGI_FORMAT& depthFormat = DXGI_FORMAT_D32_FLOAT,
			const UINT& width = 1024, const UINT& height = 768,
			const float& clearDepth = 1.f, const UINT8& clearStencil = 0);
	};

	/// <summary>
	/// デプスステンシルバッファークラス
	/// </summary>
	class DepthStencilBuffer
	{
	public:
		DepthStencilBuffer() = default;
		~DepthStencilBuffer() = default;

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> _dsvBuffer = nullptr;

		DepthStencilBufferData _depthStencilBufferData;

	public:
		/// <summary>
		/// デプスステンシルバッファー生成
		/// </summary>
		/// <param name="device">デバイス</param>
		/// <param name="data">デプスステンシルバッファーの設定</param>
		/// <returns>MYRESULT::SUCCESS: 成功 MYRESULT::FAILED: 失敗</returns>
		Utility::MYRESULT Create(ID3D12Device& device, const DepthStencilBufferData& data);

		/// <summary>
		/// バッファー取得
		/// </summary>
		/// <returns></returns>
		ID3D12Resource& GetBuffer() {
			return *_dsvBuffer.Get();
		}

		/// <summary>
		/// デプスステンシルバッファーの設定構造体を取得
		/// </summary>
		/// <returns></returns>
		DepthStencilBufferData GetDepthStencilBufferData()
		{
			return _depthStencilBufferData;
		}
	};
}