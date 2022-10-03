#pragma once
#include "EngineUtility.h"
#include "AppWindow.h"
#include "RenderingContext.h"
#include "RenderTargetBuffer.h"
#include "DescriptorHeapRTV.h"
#include "DepthStencilBuffer.h"
#include "DescriptorHeapDSV.h"

#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dx12.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include <wrl.h>

namespace NamelessEngine::Core
{
	class Dx12GraphicsEngine
	{
	private:
		Dx12GraphicsEngine();
		~Dx12GraphicsEngine() = default;
		Dx12GraphicsEngine(const Dx12GraphicsEngine& inst) = delete;
		void operator=(const Dx12GraphicsEngine& inst) = delete;

		static constexpr UINT DOUBLE_BUFFER = 2;	// ダブルバッファリング

	public:
		/// <summary>
		/// クラスのシングルトンインスタンス取得
		/// </summary>
		/// <returns>シングルトンインスタンス</returns>
		static Dx12GraphicsEngine& Instance();

		/// <summary>
		/// DirectXで描画を行うための初期化処理
		/// </summary>
		/// <param name="hwnd">ウィンドウハンドル</param>
		/// <param name="windowWidth">ウィンドウ幅</param>
		/// <param name="windowHeight">ウィンドウ高</param>
		/// <param name="dxgiFactory">ファクトリー</param>
		/// <returns>Utility::MYRESULT::SUCCESS: 成功 Utility::MYRESULT::FAILED: 失敗</returns>
		Utility::MYRESULT Init(const HWND& hwnd, const UINT& windowWidth, const UINT& windowHeight);

	private:
		// DXGI関連
		Microsoft::WRL::ComPtr<IDXGIFactory4> _dxgiFactory = nullptr;
		Microsoft::WRL::ComPtr<IDXGISwapChain3> _swapchain = nullptr;

		// DirectX12初期化関連
		Microsoft::WRL::ComPtr<ID3D12Device> _device = nullptr;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> _cmdAllocator = nullptr;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> _cmdList = nullptr;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> _cmdQueue = nullptr;

		// フェンス関連
		Microsoft::WRL::ComPtr<ID3D12Fence> _fence = nullptr;
		UINT _fenceVal = 0;

		// ウィンドウ関連
		HWND _hwnd;
		UINT _windowWidth;
		UINT _windowHeight;

		/// <summary>
		/// デバッグレイヤーを有効にする
		/// </summary>
		/// <returns></returns>
		HRESULT EnableDebugLayer();
		/// <summary>
		/// デバイスとファクトリー生成
		/// </summary>
		/// <returns></returns>
		HRESULT CreateDeviceAndDXGIFactory();
		/// <summary>
		/// コマンドアロケータ、リスト、キュー生成
		/// </summary>
		/// <returns></returns>
		HRESULT CreateCommandX();
		/// <summary>
		/// スワップチェーン生成
		/// </summary>
		/// <param name="hwnd">ウィンドウハンドル</param>
		/// <param name="windowWidth">ウィンドウ幅</param>
		/// <param name="windowHeightconst">ウィンドウ高</param>
		/// <param name="dxgiFactory">ファクトリー</param>
		/// <returns></returns>
		HRESULT CreateSwapChain(
			const HWND& hwnd, const UINT& windowWidth, const UINT& windowHeight,
			const Microsoft::WRL::ComPtr<IDXGIFactory4>& dxgiFactory);
		/// <summary>
		/// フェンス生成
		/// </summary>
		/// <returns></returns>
		HRESULT CreateFence();

	public:
		/// <summary>
		/// デバイス取得
		/// </summary>
		/// <returns></returns>
		ID3D12Device& Device();
		/// <summary>
		/// コマンドリスト取得
		/// </summary>
		/// <returns></returns>
		ID3D12GraphicsCommandList& CmdList();
		/// <summary>
		/// コマンドアロケータ取得
		/// </summary>
		/// <returns></returns>
		ID3D12CommandAllocator& CmdAllocator();
		/// <summary>
		/// コマンドキュー取得
		/// </summary>
		/// <returns></returns>
		ID3D12CommandQueue& CmdQueue();
		/// <summary>
		/// スワップチェーン取得
		/// </summary>
		/// <returns></returns>
		IDXGISwapChain3& SwapChain();

		/// <summary>
		/// 1フレームの描画開始
		/// </summary>
		void BeginDraw();
		/// <summary>
		/// 1フレームの描画終了
		/// </summary>
		void EndDraw();
		/// <summary>
		/// フレームレンダーターゲットセット
		/// </summary>
		void SetFrameRenderTarget(const CD3DX12_VIEWPORT& viewport, const CD3DX12_RECT& scissorRect);

		// 開発用
	private:
		DX12API::RenderingContext _renderContext;	            // レンダリングコンテキスト
		DX12API::RenderTargetBuffer _frameBuffers[2];	        // フレームバッファ
		DX12API::DescriptorHeapRTV _frameHeap;	                // フレームバッファ用ディスクリプタヒープ	

		DX12API::DepthStencilBufferData depthStencilBufferData;	// デプスステンシルバッファーの設定
		DX12API::DepthStencilBuffer _depthStencilBuffer;		    // デプスステンシルバッファー
		DX12API::DescriptorHeapDSV _dsvHeap;					    // デプスステンシル用ヒープ

		/// <summary>
		/// フレームバッファ用のレンダーターゲット生成
		/// </summary>
		/// <returns></returns>
		Utility::MYRESULT CreateFrameRenderTarget();

	public:
		/// <summary>
		/// レンダリングコンテキスト取得
		/// </summary>
		/// <returns></returns>
		DX12API::RenderingContext& GetRenderingContext();
	};
}

/// メモ
/// MiniEngineはグローバル変数でポインタを持っていたのでよろしくない
/// 同様のことをするならシングルトンの方が安全→デストラクタがprivateになっているのでどこかでdeleteされる心配がない
/// メンバ関数の引数にメンバ変数を指定する→関数の処理内容、意図が明確に伝わる、影響範囲が明確になる