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
	/// <returns>MYRESULT::SUCCESS: ���� MYRESULT::FAILED: ���s</returns>
	MYRESULT Init(const HWND& hwnd, const UINT& windowWidth, const UINT& windowHeight);
private:
	// DXGI�֘A
	Microsoft::WRL::ComPtr<IDXGIFactory6> _dxgiFactory = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> _swapchain = nullptr;

	// DirectX12�������֘A
	Microsoft::WRL::ComPtr<ID3D12Device> _device = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> _cmdAllocator = nullptr;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> _cmdList = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> _cmdQueue = nullptr;

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
		const HWND& hwnd, const UINT& windowWidth, const UINT& windowHeightconst,
		const Microsoft::WRL::ComPtr<IDXGIFactory6>& dxgiFactory);

public:

};

/// ����
/// MiniEngine�̓O���[�o���ϐ��Ń|�C���^�������Ă����̂ł�낵���Ȃ�
/// ���l�̂��Ƃ�����Ȃ�V���O���g���̕������S���f�X�g���N�^��private�ɂȂ��Ă���̂łǂ�����delete�����S�z���Ȃ�
/// �����o�֐��̈����Ƀ����o�ϐ����w�肷�遨�֐��̏������e�A�Ӑ}�����m�ɓ`���A�e���͈͂����m�ɂȂ�