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
/// �����_�[�^�[�Q�b�g�N���X
/// </summary>
class RenderTarget
{
public:
	RenderTarget() = default;
	~RenderTarget() = default;

	/// <summary>
	/// �����_�[�^�[�Q�b�g���\�[�X�𐶐�
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="initData">�������郌���_�[�^�[�Q�b�g�A�f�v�X�X�e���V���̃p�����[�^�\����</param>
	/// <param name="createDepth">true: �f�v�X�X�e���V������ �f�t�H���g��false</param>
	/// <returns>MYRESULT::SUCCESS: ���� MYRESULT::FAILED: ���s</returns>
	MYRESULT Create(ID3D12Device& device, const RenderTargetInitData& initData, bool createDepth = false);

	/// <summary>
	/// �����_�[�^�[�Q�b�g���\�[�X�𐶐�
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="swapchain">�X���b�v�`�F�[��</param>
	/// <param name="bufferIndex">��������o�b�t�@�̃C���f�b�N�X �_�u���o�b�t�@�����O�Ȃ�0��1</param>
	/// <param name="createDepth">true: �f�v�X�X�e���V������ �f�t�H���g��false</param>
	/// <returns>MYRESULT::SUCCESS: ���� MYRESULT::FAILED: ���s</returns>
	MYRESULT Create(
		ID3D12Device& device, IDXGISwapChain4& swapchain, const RenderTargetInitData& initData, 
		size_t bufferIndex, bool createDepth = false);

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> _renderTargetBuffer = nullptr;	    // �����_�[�^�[�Q�b�g�̃o�b�t�@�[
	Microsoft::WRL::ComPtr<ID3D12Resource> _depthBuffer = nullptr;				// �f�v�X�X�e���V���̃o�b�t�@�[

	/// <summary>
	/// �����_�[�^�[�Q�b�g�̃o�b�t�@�[����
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="format">�t�H�[�}�b�g</param>
	/// <param name="width">�����_�[�^�[�Q�b�g��</param>
	/// <param name="height">�����_�[�^�[�Q�b�g��</param>
	/// <param name="clearColor">�������F</param>
	/// <param name="rtNum">�������郊�\�[�X��</param>
	/// <returns></returns>
	HRESULT CreateRenderTargetBuffer(
		ID3D12Device& device, const DXGI_FORMAT& format, const UINT& width, const UINT& height,
		const float(&clearColor)[4]);

	/// <summary>
	/// �X���b�v�`�F�[����p���ă����_�[�^�[�Q�b�g�̃o�b�t�@�[����
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="swapchain">�X���b�v�`�F�[��</param>
	/// <param name="bufferIndex">��������o�b�t�@�̃C���f�b�N�X �_�u���o�b�t�@�����O�Ȃ�0��1</param>
	/// <returns></returns>
	HRESULT CreateRenderTargetBuffer(ID3D12Device& device, IDXGISwapChain4& swapchain, size_t bufferIndex);

	/// <summary>
	/// �f�v�X�X�e���V���̃o�b�t�@�[����
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="width">�f�v�X�X�e���V����</param>
	/// <param name="height">�f�v�X�X�e���V����</param>
	/// <returns></returns>
	HRESULT CreateDepthBuffer(
		ID3D12Device& device, const DXGI_FORMAT& format, const UINT& width, const UINT& height,
		const float& clearDepth);
};

/// ����
/// ���݁F�}���`�����_�[�^�[�Q�b�g��_�u���o�b�t�@�����O�ɑΉ����邽�߂ɁA�����_�[�^�[�Q�b�g�N���X�������̃����_�[�^�[�Q�b�g�������Ƃ��ł���
/// �o�b�t�@�[�A�f�B�X�N���v�^�q�[�v�A�r���[������
/// �����b�g
///		�����_�[�^�[�Q�b�g
/// �f�B�X�N���v�^�q�[�v�ƃr���[��ʃN���X�ɂ��ă����_�[�^�[�Q�b�g�N���X�F�����_�[�^�[�Q�b�g = 1:1�ɂ�������������₷���̂ł�
/// �����b�g
///		MRT�̍ۂɁA�@���p�A�F���p�Ȃǌʂɕϐ�����t���邱�Ƃ��ł��遨�ǐ�����
///		�}���`�p�X�����_�����O�̍ۂɂ́A�o�b�t�@�[��SRV�Ƃ��Ă���������K�v������̂ŁARenderTarget�N���X��RTV
/// �f�����b�g
///		MRT�̍ۂɃ����_�[�^�[�Q�b�g�N���X�̔z���p�ӂ���K�v������
///		�����_�[�^�[�Q�b�g�r���[�̗p�ӂ��ʓ|�ɂȂ� RenderTarget�N���X������RTV�f�B�X�N���v�^�q�[�v�N���X�𐶐����ēo�^
///		OMSetRenderTarget��ClearRenderTargetView�Ȃǎ��ۂɈ����̂�RTV�f�B�X�N���v�^�q�[�v�N���X�ɂȂ�
///		��RenderTarget�Ƃ������O�͕s�K�؂Ȃ̂ł�
///		��Texture�N���X��RTVDescriptorHeap�N���X�����b�v����RenderTarget�N���X�ɂ���
