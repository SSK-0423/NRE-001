#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include "EngineUtility.h"

struct RenderTargetInitData
{
	DXGI_FORMAT _colorFormat;
	DXGI_FORMAT _depthFormat;
	UINT _width;
	UINT _height;
	float _clearColor[4];
	float _clearDepth;

	RenderTargetInitData();
	RenderTargetInitData(const DXGI_FORMAT& colorFormat, const DXGI_FORMAT& depthFormat, const UINT& width,
		const UINT& height, const float(&clearColor)[4], const float& clearDepth);
};

/// <summary>
/// レンダーターゲットクラス
/// </summary>
class RenderTarget
{
public:
	RenderTarget() = default;
	~RenderTarget() = default;

	/// <summary>
	/// レンダーターゲットリソースを生成
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="initData">生成するレンダーターゲット、デプスステンシルのパラメータ構造体</param>
	/// <param name="createDepth">true: デプスステンシル生成 デフォルトはfalse</param>
	/// <returns>MYRESULT::SUCCESS: 成功 MYRESULT::FAILED: 失敗</returns>
	MYRESULT Create(ID3D12Device& device, const RenderTargetInitData& initData, bool createDepth = false);

	/// <summary>
	/// レンダーターゲットリソースを生成
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="swapchain">スワップチェーン</param>
	/// <param name="bufferIndex">生成するバッファのインデックス ダブルバッファリングなら0か1</param>
	/// <param name="createDepth">true: デプスステンシル生成 デフォルトはfalse</param>
	/// <returns>MYRESULT::SUCCESS: 成功 MYRESULT::FAILED: 失敗</returns>
	MYRESULT Create(
		ID3D12Device& device, IDXGISwapChain4& swapchain, const RenderTargetInitData& initData, 
		size_t bufferIndex, bool createDepth = false);

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> _renderTargetBuffer = nullptr;	    // レンダーターゲットのバッファー
	Microsoft::WRL::ComPtr<ID3D12Resource> _depthBuffer = nullptr;				// デプスステンシルのバッファー

	/// <summary>
	/// レンダーターゲットのバッファー生成
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="format">フォーマット</param>
	/// <param name="width">レンダーターゲット幅</param>
	/// <param name="height">レンダーターゲット高</param>
	/// <param name="clearColor">初期化色</param>
	/// <param name="rtNum">生成するリソース数</param>
	/// <returns></returns>
	HRESULT CreateRenderTargetBuffer(
		ID3D12Device& device, const DXGI_FORMAT& format, const UINT& width, const UINT& height,
		const float(&clearColor)[4]);

	/// <summary>
	/// スワップチェーンを用いてレンダーターゲットのバッファー生成
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="swapchain">スワップチェーン</param>
	/// <param name="bufferIndex">生成するバッファのインデックス ダブルバッファリングなら0か1</param>
	/// <returns></returns>
	HRESULT CreateRenderTargetBuffer(ID3D12Device& device, IDXGISwapChain4& swapchain, size_t bufferIndex);

	/// <summary>
	/// デプスステンシルのバッファー生成
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="width">デプスステンシル幅</param>
	/// <param name="height">デプスステンシル高</param>
	/// <returns></returns>
	HRESULT CreateDepthBuffer(
		ID3D12Device& device, const DXGI_FORMAT& format, const UINT& width, const UINT& height,
		const float& clearDepth);
};

/// メモ
/// 現在：マルチレンダーターゲットやダブルバッファリングに対応するために、レンダーターゲットクラスが複数のレンダーターゲットを持つことができる
/// バッファー、ディスクリプタヒープ、ビューを所持
/// メリット
///		レンダーターゲット
/// ディスクリプタヒープとビューを別クラスにしてレンダーターゲットクラス：レンダーターゲット = 1:1にした方が分かりやすいのでは
/// メリット
///		MRTの際に、法線用、色情報用など個別に変数名を付けることができる→可読性向上
///		マルチパスレンダリングの際には、バッファーをSRVとしても生成する必要があるので、RenderTargetクラスでRTV
/// デメリット
///		MRTの際にレンダーターゲットクラスの配列を用意する必要がある
///		レンダーターゲットビューの用意が面倒になる RenderTargetクラス生成→RTVディスクリプタヒープクラスを生成して登録
///		OMSetRenderTargetやClearRenderTargetViewなど実際に扱うのはRTVディスクリプタヒープクラスになる
///		→RenderTargetという名前は不適切なのでは
///		→TextureクラスとRTVDescriptorHeapクラスをラップしたRenderTargetクラスにする
