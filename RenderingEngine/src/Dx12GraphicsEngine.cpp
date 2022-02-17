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

	// �E�B���h�E�֘A�̕ϐ�������
	_hwnd = hwnd;
	_windowWidth = windowWidth;
	_windowHeight = windowHeight;

	// �f�o�b�O���C���[�L��
#ifdef _DEBUG
	if (FAILED(EnableDebugLayer())) { return MYRESULT::FAILED; }
#endif // DEBUG

	// �f�o�C�X�ƃt�@�N�g���[����
	if (FAILED(CreateDeviceAndDXGIFactory())) { return MYRESULT::FAILED; }

	// �R�}���h�A���P�[�^�A���X�g�A�L���[����
	if (FAILED(CreateCommandX())) { return MYRESULT::FAILED; }

	// �X���b�v�`�F�[������(�_�u���o�b�t�@�����O�p�̃o�b�t�@�[����)
	if (FAILED(CreateSwapChain(hwnd, windowWidth, windowHeight, _dxgiFactory))) { return MYRESULT::FAILED; }

	// �t�F���X����
	if (FAILED(CreateFence())) { return MYRESULT::FAILED; }

	// �t���[���o�b�t�@�̃r���[����
	RenderTargetInitData rtInitData;
	rtInitData._width = windowWidth;
	rtInitData._height = windowHeight;

	if (_frameBuffer.Create(*_device.Get(), *_swapchain.Get(), rtInitData) == MYRESULT::FAILED) {
		return MYRESULT::FAILED;
	}

	// �����_�����O�R���e�L�X�g�̏�����
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
	// �t�B�[�`���[���x����
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	// �t�@�N�g���[����
	HRESULT result = CreateDXGIFactory2(
		DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(_dxgiFactory.ReleaseAndGetAddressOf()));
	if (FAILED(result)) {
		MessageBoxA(_hwnd, "�t�@�N�g���[�������s", "�G���[", MB_OK | MB_ICONERROR);
		return result;
	}

	// �A�_�v�^�[��
	std::vector<ComPtr<IDXGIAdapter>> adapters;
	ComPtr<IDXGIAdapter> tmpAdapter = nullptr;
	// EnumAdapters�̑�������UINT�Ȃ̂ō��킹��
	for (UINT i = 0; _dxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++) {
		adapters.push_back(tmpAdapter);
	}

	// "NVIDIA"��GPU��T��
	for (auto adpt : adapters) {
		DXGI_ADAPTER_DESC adesc = {};
		adpt->GetDesc(&adesc);
		std::wstring strDesc = adesc.Description;
		if (strDesc.find(L"NVIDIA") != std::string::npos) {
			tmpAdapter = adpt;
			break;
		}
	}

	// Direct3D�f�o�C�X�̏�����
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
	// �R�}���h�A���P�[�^����
	HRESULT result = _device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(_cmdAllocator.ReleaseAndGetAddressOf()));
	if (FAILED(result)) {
		MessageBoxA(_hwnd, "�R�}���h�A���P�[�^�������s", "�G���[", MB_OK | MB_ICONERROR);
		return result;
	}

	// �R�}���h���X�g����
	result = _device->CreateCommandList(
		0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAllocator.Get(), nullptr,
		IID_PPV_ARGS(_cmdList.ReleaseAndGetAddressOf()));
	if (FAILED(result)) {
		MessageBoxA(_hwnd, "�R�}���h���X�g�������s", "�G���[", MB_OK | MB_ICONERROR);
		return result;
	}

	// �R�}���h�L���[����
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cmdQueueDesc.NodeMask = 0;
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	result = _device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(_cmdQueue.ReleaseAndGetAddressOf()));
	if (FAILED(result)) {
		MessageBoxA(_hwnd, "�R�}���h�L���[�������s", "�G���[", MB_OK | MB_ICONERROR);
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
		MessageBoxA(_hwnd, "�X���b�v�`�F�[���������s", "�G���[", MB_OK | MB_ICONERROR);
	}

	return result;
}

HRESULT Dx12GraphicsEngine::CreateFence()
{
	HRESULT result = _device->CreateFence(
		_fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(_fence.ReleaseAndGetAddressOf()));
	if (FAILED(result)) {
		MessageBoxA(_hwnd, "�t�F���X�������s", "�G���[", MB_OK | MB_ICONERROR);
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

IDXGISwapChain4& Dx12GraphicsEngine::SwapChain()
{
	return *_swapchain.Get();
}

void Dx12GraphicsEngine::BeginDraw()
{
//	// �`��Ώۂ̃o�b�t�@�[�������C���f�b�N�X�擾
//	auto bbIdx = _swapchain->GetCurrentBackBufferIndex();
//
//	// �`��Ώۃo�b�t�@�[�ֈړ�
//	auto rtvHandle = _frameRtvHeap->GetCPUDescriptorHandleForHeapStart();
//	rtvHandle.ptr +=
//		bbIdx * _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
//
//	// �o���A����
//	_renderContext.TransitionResourceState(
//		*_frameBuffer._frameBuffers[bbIdx].Get(),
//		D3D12_RESOURCE_STATE_PRESENT,
//		D3D12_RESOURCE_STATE_RENDER_TARGET);
//
//	// �����_�[�^�[�Q�b�g�Z�b�g
//	_cmdList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);
//
//	// ��ʂ��w��F�ŃN���A
//	float clearColor[] = { 0.f,1.f,1.f,1.f };
//	_cmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
}

void Dx12GraphicsEngine::EndDraw()
{
	//// �`��Ώۂ̃o�b�t�@�[�������C���f�b�N�X�擾
	//auto bbIdx = _swapchain->GetCurrentBackBufferIndex();

	//// �o���A����
	//_renderContext.TransitionResourceState(
	//	*_frameBuffer._frameBuffers[bbIdx].Get(),
	//	D3D12_RESOURCE_STATE_RENDER_TARGET,
	//	D3D12_RESOURCE_STATE_PRESENT);

	//// ���߂̎�t�I��
	//_renderContext.Close();

	//// �R�}���h���X�g�̎��s
	//ID3D12CommandList* cmdlists[] = { _cmdList.Get() };
	//_cmdQueue->ExecuteCommandLists(1, cmdlists);

	//// CPU��GPU�̓���
	//_cmdQueue->Signal(_fence.Get(), ++_fenceVal);
	//if (_fence->GetCompletedValue() != _fenceVal) {
	//	// �C�x���g�n���h���擾
	//	auto event = CreateEvent(nullptr, false, false, nullptr);

	//	_fence->SetEventOnCompletion(_fenceVal, event);

	//	// �C�x���g����������܂ő҂�������
	//	WaitForSingleObject(event, INFINITE);

	//	// �C�x���g�n���h�������
	//	CloseHandle(event);
	//}

	//_cmdAllocator->Reset();	                        // �L���[���N���A
	//_renderContext.Reset(*_cmdAllocator.Get());	    // �R�}���h���󂯕t�������Ԃɂ���

	//// �t���b�v
	//_swapchain->Present(1, 0);
}

RenderingContext& Dx12GraphicsEngine::GetRenderingContext()
{
	return _renderContext;
}

/// ����
/// return����ꍇ�Ƃ��Ȃ��ꍇ�����遨�����Ń��^�[���̗L�����w�肷��H
/// �����ɃC�����C�������ĉǐ�������\�������遨���̏����͐��������ȊO�ĂԂ��Ƃ����Ȃ��̂ŁA
/// �C�����C�������郁���b�g��������
//inline HRESULT CheckFailed(const HRESULT& result, const char* message, const HWND& hwnd) {
//	if (FAILED(result)) {
//		MessageBoxA(hwnd, message, "�G���[", MB_OK | MB_ICONERROR);
//		return result;
//	}
//}
