#include "Dx12GraphicsEngine.h"
#include <vector>
#include <wrl.h>
#include <string>
#include <cassert>

using namespace Microsoft::WRL;

Dx12GraphicsEngine::Dx12GraphicsEngine()
	: _hwnd(0), _windowWidth(0), _windowHeight(0)
{
}

Dx12GraphicsEngine& Dx12GraphicsEngine::Instance()
{
	static Dx12GraphicsEngine inst;
	return inst;
}

MYRESULT Dx12GraphicsEngine::Init(
	const HWND& hwnd, const UINT& windowWidth, const UINT& windowHeight)
{
	assert(windowWidth > 0 && windowHeight > 0);

	// ウィンドウ関連の変数初期化
	_hwnd = hwnd;
	_windowWidth = windowWidth;
	_windowHeight = windowHeight;

	// デバッグレイヤー有効
#ifdef _DEBUG
	if (FAILED(EnableDebugLayer())) { return MYRESULT::FAILED; }
#endif // DEBUG

	// デバイスとファクトリー生成
	if (FAILED(CreateDeviceAndDXGIFactory())) { return MYRESULT::FAILED; }

	// コマンドアロケータ、リスト、キュー生成
	if (FAILED(CreateCommandX())) { return MYRESULT::FAILED; }

	// スワップチェーン生成(ダブルバッファリング用のバッファー生成)
	if (FAILED(CreateSwapChain(hwnd, windowWidth, windowHeight, _dxgiFactory))) { return MYRESULT::FAILED; }

	// フレームバッファのビュー生成
	if (FAILED(CreateFrameRTV())) { return MYRESULT::FAILED; }

	return MYRESULT::SUCCESS;
}

HRESULT Dx12GraphicsEngine::EnableDebugLayer()
{
	ID3D12Debug* debugLayer = nullptr;
	HRESULT result = D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer));
	debugLayer->EnableDebugLayer();
	debugLayer->Release();
	return result;
}

HRESULT Dx12GraphicsEngine::CreateDeviceAndDXGIFactory()
{
	// フィーチャーレベル列挙
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	// ファクトリー生成
	HRESULT result = CreateDXGIFactory2(
		DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(_dxgiFactory.ReleaseAndGetAddressOf()));
	if (FAILED(result)) {
		MessageBoxA(_hwnd, "ファクトリー生成失敗", "エラー", MB_OK | MB_ICONERROR);
		return result;
	}

	// アダプター列挙
	std::vector<ComPtr<IDXGIAdapter>> adapters;
	ComPtr<IDXGIAdapter> tmpAdapter = nullptr;
	for (size_t i = 0; _dxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++) {
		adapters.push_back(tmpAdapter);
	}

	// "NVIDIA"のGPUを探す
	for (auto adpt : adapters) {
		DXGI_ADAPTER_DESC adesc = {};
		adpt->GetDesc(&adesc);
		std::wstring strDesc = adesc.Description;
		if (strDesc.find(L"NVIDIA") != std::string::npos) {
			tmpAdapter = adpt;
			break;
		}
	}

	// Direct3Dデバイスの初期化
	D3D_FEATURE_LEVEL featureLevel;
	for (auto l : levels) {
		if (SUCCEEDED(
			D3D12CreateDevice(tmpAdapter.Get(), l, IID_PPV_ARGS(_device.ReleaseAndGetAddressOf())))) {
			featureLevel = l;
			break;
		}
	}

	return result;
}

HRESULT Dx12GraphicsEngine::CreateCommandX()
{
	// コマンドアロケータ生成
	HRESULT result = _device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(_cmdAllocator.ReleaseAndGetAddressOf()));
	if (FAILED(result)) {
		MessageBoxA(_hwnd, "コマンドアロケータ生成失敗", "エラー", MB_OK | MB_ICONERROR);
		return result;
	}

	// コマンドリスト生成
	result = _device->CreateCommandList(
		0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAllocator.Get(), nullptr,
		IID_PPV_ARGS(_cmdList.ReleaseAndGetAddressOf()));
	if (FAILED(result)) {
		MessageBoxA(_hwnd, "コマンドリスト生成失敗", "エラー", MB_OK | MB_ICONERROR);
		return result;
	}

	// コマンドキュー生成
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cmdQueueDesc.NodeMask = 0;
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	result = _device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(_cmdQueue.ReleaseAndGetAddressOf()));
	if (FAILED(result)) {
		MessageBoxA(_hwnd, "コマンドキュー生成失敗", "エラー", MB_OK | MB_ICONERROR);
	}

	return result;
}

HRESULT Dx12GraphicsEngine::CreateSwapChain(
	const HWND& hwnd, const UINT& windowWidth, const UINT& windowHeight,
	const ComPtr<IDXGIFactory6>& dxgiFactory)
{
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
	swapchainDesc.Width = windowWidth;
	swapchainDesc.Height = windowHeight;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.Stereo = false;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.SampleDesc.Quality = 0;
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapchainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapchainDesc.BufferCount = _frameBuffer.BUFFERCOUNT;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT result = _dxgiFactory->CreateSwapChainForHwnd(_cmdQueue.Get(),
		hwnd,
		&swapchainDesc,
		nullptr,
		nullptr,
		(IDXGISwapChain1**)_swapchain.ReleaseAndGetAddressOf());

	if (FAILED(result)) {
		MessageBoxA(_hwnd, "スワップチェーン生成失敗", "エラー", MB_OK | MB_ICONERROR);
	}

	return result;
}

ID3D12Device& Dx12GraphicsEngine::Device()
{
	return *_device.Get();
}

ID3D12CommandList& Dx12GraphicsEngine::CmdList()
{
	return *_cmdList.Get();
}

IDXGISwapChain4& Dx12GraphicsEngine::SwapChain()
{
	return *_swapchain.Get();
}

HRESULT Dx12GraphicsEngine::CreateFrameRTV()
{
	// ディスクリプタヒープ生成
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = _frameBuffer.BUFFERCOUNT;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	HRESULT result = _device->CreateDescriptorHeap(
		&heapDesc, IID_PPV_ARGS(_frameRtvHeap.ReleaseAndGetAddressOf()));
	if (FAILED(result)) {
		MessageBoxA(_hwnd, "スワップチェーンディスクリプタ取得失敗", "エラー", MB_OK | MB_ICONERROR);
		return result;
	}

	// レンダーターゲットビュー生成
	UINT IncSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	D3D12_CPU_DESCRIPTOR_HANDLE handle = _frameRtvHeap->GetCPUDescriptorHandleForHeapStart();

	for (size_t idx = 0; idx < static_cast<size_t>(_frameBuffer.BUFFERCOUNT); idx++)
	{
		// スワップチェーンで生成したバッファーをID3D12Resourceに結びつける
		auto& buffer = _frameBuffer._frameBuffers[idx];
		result = _swapchain->GetBuffer(idx, IID_PPV_ARGS(buffer.ReleaseAndGetAddressOf()));
		if (FAILED(result)) {
			MessageBoxA(_hwnd, "フレームバッファ取得失敗", "エラー", MB_OK | MB_ICONERROR);
			return result;
		}

		// ビュー生成
		_device->CreateRenderTargetView(buffer.Get(), nullptr, handle);

		// 次にビューを生成する位置へ移動
		handle.ptr += IncSize;
	}

	return result;
}

/// メモ
/// returnする場合としない場合がある→引数でリターンの有無を指定する？
/// 無理にインライン化して可読性下がる可能性もある→この処理は生成処理以外呼ぶことが少ないので、
/// インライン化するメリットが小さい
//inline HRESULT CheckFailed(const HRESULT& result, const char* message, const HWND& hwnd) {
//	if (FAILED(result)) {
//		MessageBoxA(hwnd, message, "エラー", MB_OK | MB_ICONERROR);
//		return result;
//	}
//}
