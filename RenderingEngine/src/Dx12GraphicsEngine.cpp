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

	// フェンス生成
	if (FAILED(CreateFence())) { return MYRESULT::FAILED; }

	if (CreateFrameRenderTarget() == MYRESULT::FAILED) { return MYRESULT::FAILED; }

	// レンダリングコンテキストの初期化
	_renderContext.Init(*_cmdList.Get());

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
	// EnumAdaptersの第一引数がUINTなので合わせる
	for (UINT i = 0; _dxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++) {
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
	swapchainDesc.BufferCount = DOUBLE_BUFFER;
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

HRESULT Dx12GraphicsEngine::CreateFence()
{
	HRESULT result = _device->CreateFence(
		_fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(_fence.ReleaseAndGetAddressOf()));
	if (FAILED(result)) {
		MessageBoxA(_hwnd, "フェンス生成失敗", "エラー", MB_OK | MB_ICONERROR);
	}

	return result;
}

ID3D12Device& Dx12GraphicsEngine::Device()
{
	return *_device.Get();
}

ID3D12GraphicsCommandList& Dx12GraphicsEngine::CmdList()
{
	return *_cmdList.Get();
}

ID3D12CommandAllocator& Dx12GraphicsEngine::CmdAllocator()
{
	return *_cmdAllocator.Get();
}

ID3D12CommandQueue& Dx12GraphicsEngine::CmdQueue()
{
	return *_cmdQueue.Get();
}

IDXGISwapChain4& Dx12GraphicsEngine::SwapChain()
{
	return *_swapchain.Get();
}

void Dx12GraphicsEngine::BeginDraw()
{
	// 描画対象のバッファーを示すインデックス取得
	auto bbIdx = _swapchain->GetCurrentBackBufferIndex();

	// 描画対象バッファーへ移動
	auto rtvHandle = _frameHeap.GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += bbIdx * _frameHeap.GetHandleIncrimentSize();

	// バリア処理
	_renderContext.TransitionResourceState(
		_frameBuffers[bbIdx].GetBuffer(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	// レンダーターゲットセット
	_renderContext.SetRenderTarget(&rtvHandle, nullptr);

	// 画面を指定色でクリア
	ColorRGBA color(0.f, 1.f, 1.f, 1.f);
	_renderContext.ClearRenderTarget(rtvHandle, color, 0, nullptr);
}

void Dx12GraphicsEngine::EndDraw()
{
	// 描画対象のバッファーを示すインデックス取得
	auto bbIdx = _swapchain->GetCurrentBackBufferIndex();

	// バリア処理
	_renderContext.TransitionResourceState(
		_frameBuffers[bbIdx].GetBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT);

	// 命令の受付終了
	_renderContext.Close();

	// コマンドリストの実行
	ID3D12CommandList* cmdlists[] = { _cmdList.Get() };
	_cmdQueue->ExecuteCommandLists(1, cmdlists);

	// CPUとGPUの同期
	_cmdQueue->Signal(_fence.Get(), ++_fenceVal);
	if (_fence->GetCompletedValue() != _fenceVal) {
		// イベントハンドル取得
		auto event = CreateEvent(nullptr, false, false, nullptr);

		_fence->SetEventOnCompletion(_fenceVal, event);

		// イベントが発生するまで待ち続ける
		WaitForSingleObject(event, INFINITE);

		// イベントハンドルを閉じる
		CloseHandle(event);
	}

	_cmdAllocator->Reset();	                        // キューをクリア
	_renderContext.Reset(*_cmdAllocator.Get());	    // コマンドを受け付けられる状態にする

	// フリップ
	_swapchain->Present(1, 0);
}

void Dx12GraphicsEngine::SetFrameRenderTarget()
{
	// 描画対象のバッファーを示すインデックス取得
	auto bbIdx = _swapchain->GetCurrentBackBufferIndex();

	// 描画対象バッファーへ移動
	auto rtvHandle = _frameHeap.GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += bbIdx * _frameHeap.GetHandleIncrimentSize();

	// バリア処理
	//_renderContext.TransitionResourceState(
	//	_frameBuffers[bbIdx].GetBuffer(),
	//	D3D12_RESOURCE_STATE_PRESENT,
	//	D3D12_RESOURCE_STATE_RENDER_TARGET);

	// レンダーターゲットセット
	_renderContext.SetRenderTarget(&rtvHandle, nullptr);

	// 画面を指定色でクリア
	ColorRGBA color(0.f, 1.f, 1.f, 1.f);
	_renderContext.ClearRenderTarget(rtvHandle, color, 0, nullptr);
}

MYRESULT Dx12GraphicsEngine::CreateFrameRenderTarget()
{
	MYRESULT result;

	// ディスクリプタヒープ生成
	_frameHeap.Create(*_device.Get());

	// バッファー生成してディスクリプタヒープに登録
	for (size_t idx = 0; idx < DOUBLE_BUFFER; idx++) {
		// 生成
		result = _frameBuffers[idx].Create(*_device.Get(), *_swapchain.Get(), idx);
		if (result == MYRESULT::FAILED) { return result; }
		// 登録
		_frameHeap.RegistDescriptor(*_device.Get(), _frameBuffers[idx]);
	}

	return MYRESULT::SUCCESS;
}

RenderingContext& Dx12GraphicsEngine::GetRenderingContext()
{
	return _renderContext;
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
