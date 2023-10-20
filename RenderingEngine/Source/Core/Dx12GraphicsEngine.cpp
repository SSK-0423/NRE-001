#include "Dx12GraphicsEngine.h"

#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

#include <DirectXMath.h>

#include <vector>
#include <wrl.h>
#include <string>
#include <cassert>
#include <dshow.h>

using namespace Microsoft::WRL;

namespace NamelessEngine::Core
{
	Dx12GraphicsEngine::Dx12GraphicsEngine()
		: _hwnd(0), _windowWidth(0), _windowHeight(0)
	{
	}

	Dx12GraphicsEngine& Dx12GraphicsEngine::Instance()
	{
		static Dx12GraphicsEngine inst;
		return inst;
	}

	Utility::RESULT Dx12GraphicsEngine::Init(
		const HWND& hwnd, const UINT& windowWidth, const UINT& windowHeight)
	{
		assert(windowWidth > 0 && windowHeight > 0);

		// ウィンドウ関連の変数初期化
		_hwnd = hwnd;
		_windowWidth = windowWidth;
		_windowHeight = windowHeight;

		// デバッグレイヤー有効
#ifdef _DEBUG
		if (FAILED(EnableDebugLayer())) { return Utility::RESULT::FAILED; }
#endif // DEBUG

		// デバイスとファクトリー生成
		if (FAILED(CreateDeviceAndDXGIFactory())) { return Utility::RESULT::FAILED; }

		// コマンドアロケータ、リスト、キュー生成
		if (FAILED(CreateCommandX())) { return Utility::RESULT::FAILED; }

		// スワップチェーン生成(ダブルバッファリング用のバッファー生成)
		if (FAILED(CreateSwapChain(hwnd, windowWidth, windowHeight, _dxgiFactory))) { return Utility::RESULT::FAILED; }

		// フェンス生成
		if (FAILED(CreateFence())) { return Utility::RESULT::FAILED; }

		// フレームバッファ―(最終レンダリング先)生成
		if (CreateFrameRenderTarget() == Utility::RESULT::FAILED) { return Utility::RESULT::FAILED; }

		// Imgui用のディスクリプタヒープ生成
		if (CreateImguiDescriptorHeap() == Utility::RESULT::FAILED) { return Utility::RESULT::FAILED; }

		// レンダリングコンテキストの初期化
		_renderContext.Init(*_cmdList.Get());

		return Utility::RESULT::SUCCESS;
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
		//result = _dxgiFactory.Get()->QueryInterface(IID_PPV_ARGS(_dxgiFactory.ReleaseAndGetAddressOf()));
		//if (FAILED(result)) {
		//	MessageBoxA(_hwnd, "ファクトリー7生成失敗", "エラー", MB_OK | MB_ICONERROR);
		//	return result;
		//}

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
		const ComPtr<IDXGIFactory4>& dxgiFactory)
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

	IDXGISwapChain3& Dx12GraphicsEngine::SwapChain()
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

		// 深度バッファー
		auto dsvHandle = _dsvHeap.GetCPUDescriptorHandleForHeapStart();

		// バリア処理
		_renderContext.TransitionResourceState(
			_frameBuffers[bbIdx].GetBuffer(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET);

		// レンダーターゲットセット
		_renderContext.SetRenderTarget(&rtvHandle, &dsvHandle);

		// 画面を指定色でクリア
		DirectX::XMFLOAT4 color(0.f, 1.f, 1.f, 1.f);
		_renderContext.ClearRenderTarget(rtvHandle, color, 0, nullptr);

		// デプスステンシルバッファをクリア
		_renderContext.ClearDepthStencilView(
			dsvHandle, D3D12_CLEAR_FLAG_DEPTH,
			depthStencilBufferData.clearDepth, depthStencilBufferData.clearStencil, 0, nullptr);

		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		// Imgui描画前準備
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void Dx12GraphicsEngine::EndDraw()
	{
		// Imguiの描画
		_renderContext.SetDescriptorHeap(_imguiHeap);
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), _cmdList.Get());

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

	void Dx12GraphicsEngine::SetFrameRenderTarget(const CD3DX12_VIEWPORT& viewport, const CD3DX12_RECT& scissorRect)
	{
		// 描画対象のバッファーを示すインデックス取得
		auto bbIdx = _swapchain->GetCurrentBackBufferIndex();

		// 描画対象バッファーへ移動
		auto rtvHandle = _frameHeap.GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += bbIdx * _frameHeap.GetHandleIncrimentSize();

		// 深度バッファー
		auto dsvHandle = _dsvHeap.GetCPUDescriptorHandleForHeapStart();

		// レンダーターゲットセット
		_renderContext.SetRenderTarget(&rtvHandle, &dsvHandle);

		// 画面を指定色でクリア
		DirectX::XMFLOAT4 color(0.f, 1.f, 1.f, 1.f);
		_renderContext.ClearRenderTarget(rtvHandle, color, 0, nullptr);

		// デプスステンシルバッファをクリア
		_renderContext.ClearDepthStencilView(
			dsvHandle, D3D12_CLEAR_FLAG_DEPTH,
			depthStencilBufferData.clearDepth, depthStencilBufferData.clearStencil, 0, nullptr);

		// ビューポートとシザー矩形セット
		_renderContext.SetViewport(viewport);
		_renderContext.SetScissorRect(scissorRect);
	}

	Utility::RESULT Dx12GraphicsEngine::CreateFrameRenderTarget()
	{
		Utility::RESULT result;

		// ディスクリプタヒープ生成
		_frameHeap.Create(*_device.Get());

		// バッファー生成してディスクリプタヒープに登録
		for (size_t idx = 0; idx < DOUBLE_BUFFER; idx++) {
			// 生成
			result = _frameBuffers[idx].Create(*_device.Get(), *_swapchain.Get(), idx);
			if (result == Utility::RESULT::FAILED) { return result; }
			// 登録
			_frameHeap.RegistDescriptor(*_device.Get(), _frameBuffers[idx], DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
		}

		// デプスステンシルバッファー生成
		depthStencilBufferData.width = _windowWidth;
		depthStencilBufferData.height = _windowHeight;
		result = _depthStencilBuffer.Create(*_device.Get(), depthStencilBufferData);
		if (result == Utility::RESULT::FAILED) { return result; }

		// デプスステンシル用ディスクリプタヒープ生成
		result = _dsvHeap.Create(*_device.Get());
		if (result == Utility::RESULT::FAILED) { return result; }

		// デプスステンシルビュー生成
		_dsvHeap.RegistDescriptor(*_device.Get(), _depthStencilBuffer);

		return Utility::RESULT::SUCCESS;
	}

	Utility::RESULT Dx12GraphicsEngine::CreateImguiDescriptorHeap()
	{
		return _imguiHeap.Create(*_device.Get());
	}

	DX12API::RenderingContext& Dx12GraphicsEngine::GetRenderingContext()
	{
		return _renderContext;
	}
	DX12API::DescriptorHeapCBV_SRV_UAV& Dx12GraphicsEngine::GetImguiDescriptorHeap()
	{
		return _imguiHeap;
	}
}