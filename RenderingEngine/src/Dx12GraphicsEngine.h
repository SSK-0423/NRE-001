#pragma once
#include "EngineUtility.h"
#include "AppWindow.h"

#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include <wrl.h>

class Dx12GraphicsEngine
{
private:
	Dx12GraphicsEngine();
	~Dx12GraphicsEngine() = default;
	Dx12GraphicsEngine(const Dx12GraphicsEngine& inst) = delete;
	void operator=(const Dx12GraphicsEngine& inst) = delete;

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
	/// <returns>MYRESULT::SUCCESS: 成功 MYRESULT::FAILED: 失敗</returns>
	MYRESULT Init(const HWND& hwnd, const UINT& windowWidth, const UINT& windowHeight);
private:
	// DXGI関連
	Microsoft::WRL::ComPtr<IDXGIFactory6> _dxgiFactory = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> _swapchain = nullptr;

	// DirectX12初期化関連
	Microsoft::WRL::ComPtr<ID3D12Device> _device = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> _cmdAllocator = nullptr;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> _cmdList = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> _cmdQueue = nullptr;

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
		const HWND& hwnd, const UINT& windowWidth, const UINT& windowHeightconst,
		const Microsoft::WRL::ComPtr<IDXGIFactory6>& dxgiFactory);

public:

};

/// メモ
/// MiniEngineはグローバル変数でポインタを持っていたのでよろしくない
/// 同様のことをするならシングルトンの方が安全→デストラクタがprivateになっているのでどこかでdeleteされる心配がない
/// メンバ関数の引数にメンバ変数を指定する→関数の処理内容、意図が明確に伝わる、影響範囲が明確になる