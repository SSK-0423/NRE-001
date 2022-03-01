#pragma once

#include <d3d12.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")

#include "EngineUtility.h"

class Texture;
class ConstantBuffer;

class DescriptorHeapCBV_SRV_UAV
{
public:
	DescriptorHeapCBV_SRV_UAV() = default;
	~DescriptorHeapCBV_SRV_UAV() = default;

private:
	static constexpr UINT MAX_CBV_DESCRIPTOR_NUM = 8;	// �萔�o�b�t�@�[�̍ő吔
	static constexpr UINT MAX_SRV_DESCRIPTOR_NUM = 8;	// �V�F�[�_�[���\�[�X�̍ő吔
	static constexpr UINT MAX_UAV_DESCRIPTOR_NUM = 8;	// �A���I�[�_�[�A�N�Z�X�̍ő吔

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _descriptorHeap = nullptr;

	UINT _handleIncrimentSize = 0;
	UINT _registedCBVNum = 0;
	UINT _registedSRVNum = 0;
	UINT _registedUAVNum = 0;

	/// <summary>
	/// �f�B�X�N���v�^�q�[�v����
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <returns></returns>
	HRESULT CreateDescriptorHeap(ID3D12Device& device);

public:
	MYRESULT Create(ID3D12Device& device);

	/// <summary>
	/// CPU�̃f�B�X�N���v�^�q�[�v�̐擪�n���h�����擾
	/// </summary>
	/// <returns>CPU�̃f�B�X�N���v�^�q�[�v�̐擪�n���h��</returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandleForHeapStart() {
		return _descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	}

	/// <summary>
	/// �f�B�X�N���v�^�q�[�v�n���h���̃C���N�������g�T�C�Y�擾
	/// </summary>
	/// <returns>�C���N�������g�T�C�Y</returns>
	UINT GetHandleIncrimentSize() {
		return _handleIncrimentSize;
	}

	/// <summary>
	/// �V�F�[�_�[���\�[�X(�e�N�X�`��)�o�^
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="texture">�e�N�X�`��</param>
	void RegistShaderResource(ID3D12Device& device, Texture& texture);

	/// <summary>
	/// �R���X�^���g�o�b�t�@�\�o�^
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="constantBuffer">�R���X�^���g�o�b�t�@�\</param>
	void RegistConstantBuffer(ID3D12Device& device, ConstantBuffer& constantBuffer);
};