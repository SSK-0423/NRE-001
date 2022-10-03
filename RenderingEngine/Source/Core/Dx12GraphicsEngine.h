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

		static constexpr UINT DOUBLE_BUFFER = 2;	// �_�u���o�b�t�@�����O

	public:
		/// <summary>
		/// �N���X�̃V���O���g���C���X�^���X�擾
		/// </summary>
		/// <returns>�V���O���g���C���X�^���X</returns>
		static Dx12GraphicsEngine& Instance();

		/// <summary>
		/// DirectX�ŕ`����s�����߂̏���������
		/// </summary>
		/// <param name="hwnd">�E�B���h�E�n���h��</param>
		/// <param name="windowWidth">�E�B���h�E��</param>
		/// <param name="windowHeight">�E�B���h�E��</param>
		/// <param name="dxgiFactory">�t�@�N�g���[</param>
		/// <returns>Utility::MYRESULT::SUCCESS: ���� Utility::MYRESULT::FAILED: ���s</returns>
		Utility::MYRESULT Init(const HWND& hwnd, const UINT& windowWidth, const UINT& windowHeight);

	private:
		// DXGI�֘A
		Microsoft::WRL::ComPtr<IDXGIFactory4> _dxgiFactory = nullptr;
		Microsoft::WRL::ComPtr<IDXGISwapChain3> _swapchain = nullptr;

		// DirectX12�������֘A
		Microsoft::WRL::ComPtr<ID3D12Device> _device = nullptr;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> _cmdAllocator = nullptr;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> _cmdList = nullptr;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> _cmdQueue = nullptr;

		// �t�F���X�֘A
		Microsoft::WRL::ComPtr<ID3D12Fence> _fence = nullptr;
		UINT _fenceVal = 0;

		// �E�B���h�E�֘A
		HWND _hwnd;
		UINT _windowWidth;
		UINT _windowHeight;

		/// <summary>
		/// �f�o�b�O���C���[��L���ɂ���
		/// </summary>
		/// <returns></returns>
		HRESULT EnableDebugLayer();
		/// <summary>
		/// �f�o�C�X�ƃt�@�N�g���[����
		/// </summary>
		/// <returns></returns>
		HRESULT CreateDeviceAndDXGIFactory();
		/// <summary>
		/// �R�}���h�A���P�[�^�A���X�g�A�L���[����
		/// </summary>
		/// <returns></returns>
		HRESULT CreateCommandX();
		/// <summary>
		/// �X���b�v�`�F�[������
		/// </summary>
		/// <param name="hwnd">�E�B���h�E�n���h��</param>
		/// <param name="windowWidth">�E�B���h�E��</param>
		/// <param name="windowHeightconst">�E�B���h�E��</param>
		/// <param name="dxgiFactory">�t�@�N�g���[</param>
		/// <returns></returns>
		HRESULT CreateSwapChain(
			const HWND& hwnd, const UINT& windowWidth, const UINT& windowHeight,
			const Microsoft::WRL::ComPtr<IDXGIFactory4>& dxgiFactory);
		/// <summary>
		/// �t�F���X����
		/// </summary>
		/// <returns></returns>
		HRESULT CreateFence();

	public:
		/// <summary>
		/// �f�o�C�X�擾
		/// </summary>
		/// <returns></returns>
		ID3D12Device& Device();
		/// <summary>
		/// �R�}���h���X�g�擾
		/// </summary>
		/// <returns></returns>
		ID3D12GraphicsCommandList& CmdList();
		/// <summary>
		/// �R�}���h�A���P�[�^�擾
		/// </summary>
		/// <returns></returns>
		ID3D12CommandAllocator& CmdAllocator();
		/// <summary>
		/// �R�}���h�L���[�擾
		/// </summary>
		/// <returns></returns>
		ID3D12CommandQueue& CmdQueue();
		/// <summary>
		/// �X���b�v�`�F�[���擾
		/// </summary>
		/// <returns></returns>
		IDXGISwapChain3& SwapChain();

		/// <summary>
		/// 1�t���[���̕`��J�n
		/// </summary>
		void BeginDraw();
		/// <summary>
		/// 1�t���[���̕`��I��
		/// </summary>
		void EndDraw();
		/// <summary>
		/// �t���[�������_�[�^�[�Q�b�g�Z�b�g
		/// </summary>
		void SetFrameRenderTarget(const CD3DX12_VIEWPORT& viewport, const CD3DX12_RECT& scissorRect);

		// �J���p
	private:
		DX12API::RenderingContext _renderContext;	            // �����_�����O�R���e�L�X�g
		DX12API::RenderTargetBuffer _frameBuffers[2];	        // �t���[���o�b�t�@
		DX12API::DescriptorHeapRTV _frameHeap;	                // �t���[���o�b�t�@�p�f�B�X�N���v�^�q�[�v	

		DX12API::DepthStencilBufferData depthStencilBufferData;	// �f�v�X�X�e���V���o�b�t�@�[�̐ݒ�
		DX12API::DepthStencilBuffer _depthStencilBuffer;		    // �f�v�X�X�e���V���o�b�t�@�[
		DX12API::DescriptorHeapDSV _dsvHeap;					    // �f�v�X�X�e���V���p�q�[�v

		/// <summary>
		/// �t���[���o�b�t�@�p�̃����_�[�^�[�Q�b�g����
		/// </summary>
		/// <returns></returns>
		Utility::MYRESULT CreateFrameRenderTarget();

	public:
		/// <summary>
		/// �����_�����O�R���e�L�X�g�擾
		/// </summary>
		/// <returns></returns>
		DX12API::RenderingContext& GetRenderingContext();
	};
}

/// ����
/// MiniEngine�̓O���[�o���ϐ��Ń|�C���^�������Ă����̂ł�낵���Ȃ�
/// ���l�̂��Ƃ�����Ȃ�V���O���g���̕������S���f�X�g���N�^��private�ɂȂ��Ă���̂łǂ�����delete�����S�z���Ȃ�
/// �����o�֐��̈����Ƀ����o�ϐ����w�肷�遨�֐��̏������e�A�Ӑ}�����m�ɓ`���A�e���͈͂����m�ɂȂ�