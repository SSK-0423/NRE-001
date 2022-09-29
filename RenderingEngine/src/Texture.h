#pragma once

#include <d3d12.h>
#include <DirectXTex.h>
#include <wrl.h>
#include <string>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"DirectXTex.lib")
#pragma comment(lib,"dxguid.lib")

#include "Dx12GraphicsEngine.h"
#include "EngineUtility.h"
#include "RenderTargetBuffer.h"
#include "DepthStencilBuffer.h"

class Texture {
public:
	Texture() = default;
	~Texture() = default;

	Texture& operator=(const Texture& inst);
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> _uploadBuffer = nullptr;		// ���ԃo�b�t�@�[(�A�b�v���[�h��)
	Microsoft::WRL::ComPtr<ID3D12Resource> _textureBuffer = nullptr;	// �e�N�X�`���o�b�t�@�[(�A�b�v���[�h��)

	std::vector<D3D12_SUBRESOURCE_DATA> _subresources; // �L���[�u�e�N�X�`���p�T�u���\�[�X

	// ScratchImage::GetImage�̖߂�l��const�����Ă��邽��
	const DirectX::Image* _image = nullptr;	// �e�N�X�`���̐��f�[�^
	DirectX::TexMetadata _metaData;	        // �e�N�X�`���̃��^���
	DirectX::ScratchImage _scratchImage;    // 

	/// <summary>
	/// WIC�Ή��̃t�@�C��(bmp,png,jpeg,tiff�Ȃ�)��ǂݍ���
	/// </summary>
	/// <param name="texPath">�e�N�X�`���ւ̃p�X</param>
	/// <returns></returns>
	HRESULT LoadTextureFromWICFile(const std::wstring& texturePath);

	/// <summary>
	/// DDS�t�@�C���ǂݍ���
	/// </summary>
	/// <param name="texturePath">�e�N�X�`���ւ̃p�X</param>
	/// <returns></returns>
	HRESULT LoadTextureFromDDSFile(const std::wstring& texturePath);

	/// <summary>
	/// ���ԃo�b�t�@�[�ƃe�N�X�`���o�b�t�@�[����
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <returns></returns>
	HRESULT CreateUploadAndTextureBuffer(ID3D12Device& device);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="device"></param>
	/// <returns></returns>
	HRESULT CreateUploadAndCubeTextureBuffer(ID3D12Device& device);

	/// <summary>
	/// �}�b�v����
	/// </summary>
	/// <returns></returns>
	HRESULT MapTexture();

	/// <summary>
	/// �A�b�v���[�h�o�b�t�@�[�̓��e���e�N�X�`���o�b�t�@�[�փR�s�[
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="graphicsEngine">�O���t�B�N�X�G���W��</param>
	/// <returns></returns>
	HRESULT CopyTexture(ID3D12Device& device, Dx12GraphicsEngine& graphicsEngine);

	HRESULT CopyCubeTexture(Dx12GraphicsEngine& graphicsEngine);

	void SetTextureData(
		std::vector<ColorRGBA>& data, const size_t& width, const size_t& height, const DXGI_FORMAT& format);

public:
	/// <summary>
	/// WIC�Ή��̃t�@�C��((bmp,png,jpeg,tiff�Ȃ�)����e�N�X�`���𐶐�
	/// </summary>
	/// <param name="graphicsEngine">�O���t�B�N�X�G���W��</param>
	/// <param name="texturePath">�e�N�X�`���ւ̃p�X</param>
	/// <returns>�����FMYRESULT::SUCCESS ���s�FMYRESULT::FAILED</returns>
	MYRESULT CreateTextureFromWIC(Dx12GraphicsEngine& graphicsEngine, const std::wstring& texturePath);

	/// <summary>
	/// DDS�t�@�C������e�N�X�`���𐶐�
	/// </summary>
	/// <param name="graphicsEngine">�O���t�B�N�X�G���W��</param>
	/// <param name="texturePath">�e�N�X�`���ւ̃p�X</param>
	/// <returns>�����FMYRESULT::SUCCESS ���s�FMYRESULT::FAILED</returns>
	MYRESULT CreateTextureFromDDS(Dx12GraphicsEngine& graphicsEngine, const std::wstring& texturePath);

	/// <summary>
	/// �p�ӂ���RGBA�f�[�^����e�N�X�`������
	/// </summary>
	/// <param name="graphicsEngine"></param>
	/// <param name="data"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <returns></returns>
	MYRESULT CreateTextureFromRGBAData(
		Dx12GraphicsEngine& graphicsEngine, std::vector<ColorRGBA>& data,
		const size_t& width, const size_t& height, const DXGI_FORMAT& format);

	/// <summary>
	/// �����_�[�^�[�Q�b�g����e�N�X�`������
	/// </summary>
	/// <param name="renderTargetBuffer">�����_�[�^�[�Q�b�g�o�b�t�@�[</param>
	void CreateTextureFromRenderTarget(RenderTargetBuffer& renderTargetBuffer);

	/// <summary>
	/// �f�v�X�X�e���V���o�b�t�@�[����e�N�X�`������
	/// </summary>
	/// <param name="depthStencilBuffer">�f�v�X�X�e���V���o�b�t�@�[</param>
	void CreateTextureFromDepthStencil(DepthStencilBuffer& depthStencilBuffer);

	MYRESULT CreateCubeTextureFromDDS(Dx12GraphicsEngine& graphicsEngine, const std::wstring& texturePath);

	/// <summary>
	/// �e�N�X�`���o�b�t�@�[�擾
	/// </summary>
	/// <returns>�e�N�X�`���o�b�t�@�[</returns>
	ID3D12Resource& GetBuffer() const {
		return *_textureBuffer.Get();
	}

	/// <summary>
	/// �e�N�X�`���̃��^�f�[�^�擾
	/// </summary>
	/// <returns>�e�N�X�`���̃��^�f�[�^</returns>
	DirectX::TexMetadata& GetTexMetadata() {
		return _metaData;
	}

	/// <summary>
	/// �e�N�X�`���̐��f�[�^�擾
	/// </summary>
	/// <returns>�e�N�X�`���̐��f�[�^</returns>
	const DirectX::Image& GetImage() {
		return *_image;
	}
};