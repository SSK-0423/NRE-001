#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include "EngineUtility.h"

namespace NamelessEngine::DX12API
{
	struct RenderTargetBufferData
	{
		DXGI_FORMAT colorFormat;
		UINT width;
		UINT height;
		float clearColor[4];

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="colorFormat">カラーフォーマット デフォルト：DXGI_FORMAT_R8G8B8A8_UNORM</param>
		/// <param name="width">レンダーターゲット幅 デフォルト：1024</param>
		/// <param name="height">レンダーターゲット高 デフォルト：768</param>
		/// <param name="clearColor">初期化色 デフォルト：黒</param>
		RenderTargetBufferData(
			const DXGI_FORMAT& colorFormat = DXGI_FORMAT_R8G8B8A8_UNORM,
			const UINT& width = 1024, const UINT& height = 768,
			const float(&clearColor)[4] = { 1.f,1.f,1.f,1.f });
	};

	/// <summary>
	/// レンダーターゲット用のバッファークラス
	/// </summary>
	class RenderTargetBuffer
	{
	public:
		RenderTargetBuffer() = default;
		~RenderTargetBuffer() = default;

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> _rtvBuffer = nullptr;

	public:
		/// <summary>
		/// レンダーターゲットバッファー生成
		/// </summary>
		/// <param name="device">デバイス</param>
		/// <param name="data">レンダーターゲットの設定</param>
		/// <returns>Utility::MYRESULT::SUCCESS: 成功 Utility::MYRESULT::FAILED: 失敗</returns>
		Utility::MYRESULT Create(ID3D12Device& device, const RenderTargetBufferData& data);

		/// <summary>
		/// フレームバッファ用のレンダーターゲットバッファー生成
		/// </summary>
		/// <param name="device">デバイス</param>
		/// <param name="swapchain">スワップチェーン</param>
		/// <param name="index">生成するバッファのインデックス ダブルバッファリングなら0か1</param>
		/// <returns>Utility::MYRESULT::SUCCESS: 成功 Utility::MYRESULT::FAILED: 失敗</returns>
		Utility::MYRESULT Create(ID3D12Device& device, IDXGISwapChain3& swapchain, const size_t& index);

		/// <summary>
		/// バッファー取得
		/// </summary>
		/// <returns></returns>
		ID3D12Resource& GetBuffer() {
			return *_rtvBuffer.Get();
		}
	};
}