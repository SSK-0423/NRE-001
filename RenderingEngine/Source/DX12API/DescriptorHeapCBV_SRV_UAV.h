#pragma once

#include <d3d12.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")

#include "EngineUtility.h"


namespace NamelessEngine::DX12API
{
	class Texture;
	class ConstantBuffer;
	class UnorderedAccessResource;

	struct ShaderResourceViewDesc;

	class DescriptorHeapCBV_SRV_UAV
	{
	public:
		DescriptorHeapCBV_SRV_UAV() = default;
		~DescriptorHeapCBV_SRV_UAV() = default;
		static constexpr int NEXT_REGISTER = -1;

	private:
		static constexpr UINT _MAX_CBV_DESCRIPTOR_NUM = 64;	// �萔�o�b�t�@�[�̍ő吔
		static constexpr UINT _MAX_SRV_DESCRIPTOR_NUM = 64;	// �V�F�[�_�[���\�[�X�̍ő吔
		static constexpr UINT _MAX_UAV_DESCRIPTOR_NUM = 64;	// �A���I�[�_�[�A�N�Z�X�̍ő吔

		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _descriptorHeap = nullptr;

		SIZE_T _handleIncrimentSize = 0;
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
		/// <summary>
		/// �f�B�X�N���v�^�q�[�v����
		/// </summary>
		/// <param name="device">�f�o�C�X</param>
		/// <returns></returns>
		Utility::RESULT Create(ID3D12Device& device);

		/// <summary>
		/// GPU�̃f�B�X�N���v�^�q�[�v�̐擪�n���h�����擾
		/// </summary>
		/// <returns></returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandleForHeapStart() {
			return _descriptorHeap->GetGPUDescriptorHandleForHeapStart();
		}

		/// <summary>
		/// CPU�̃f�B�X�N���v�^�q�[�v�̐擪�n���h���擾
		/// </summary>
		/// <returns></returns>
		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandleForHeapStart() {
			return _descriptorHeap->GetCPUDescriptorHandleForHeapStart();
		}

		/// <summary>
		/// GPU��CBV�����̃f�B�X�N���v�^�q�[�v�̐擪�n���h�����擾
		/// </summary>
		/// <returns>GPU��CBV�����̃f�B�X�N���v�^�q�[�v�̐擪�n���h��</returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandleForHeapStartCBV() {
			return _descriptorHeap->GetGPUDescriptorHandleForHeapStart();
		}

		/// <summary>
		/// GPU��SRV�����̃f�B�X�N���v�^�q�[�v�̐擪�n���h�����擾
		/// </summary>
		/// <returns>GPU��SRV�����̃f�B�X�N���v�^�q�[�v�̐擪�n���h��</returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandleForHeapStartSRV() {
			auto handle = _descriptorHeap->GetGPUDescriptorHandleForHeapStart();
			handle.ptr += _handleIncrimentSize * _MAX_CBV_DESCRIPTOR_NUM;

			return handle;
		}

		D3D12_GPU_DESCRIPTOR_HANDLE GetSRVHandle(int index) {
			auto handle = _descriptorHeap->GetGPUDescriptorHandleForHeapStart();
			handle.ptr += _handleIncrimentSize * _MAX_CBV_DESCRIPTOR_NUM;
			handle.ptr += _handleIncrimentSize * index;
			return handle;
		}

		/// <summary>
		/// GPU��UAV�����̃f�B�X�N���v�^�q�[�v�̐擪�n���h�����擾
		/// </summary>
		/// <returns>GPU��UAV�����̃f�B�X�N���v�^�q�[�v�̐擪�n���h��</returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandleForHeapStartUAV() {
			auto handle = _descriptorHeap->GetGPUDescriptorHandleForHeapStart();
			handle.ptr += _handleIncrimentSize * (_MAX_CBV_DESCRIPTOR_NUM + _MAX_UAV_DESCRIPTOR_NUM);

			return handle;
		}

		/// <summary>
		/// �f�B�X�N���v�^�q�[�v�n���h���̃C���N�������g�T�C�Y�擾
		/// </summary>
		/// <returns>�C���N�������g�T�C�Y</returns>
		SIZE_T GetHandleIncrimentSize() {
			return _handleIncrimentSize;
		}

		/// <summary>
		/// �R���X�^���g�o�b�t�@�\�o�^
		/// </summary>
		/// <param name="device">�f�o�C�X</param>
		/// <param name="constantBuffer">�R���X�^���g�o�b�t�@�\</param>
		/// <param name="registerNo">
		/// �o�^���郌�W�X�^�ԍ� �f�t�H���g�l:NEXT_REGISTER(-1) ���ݓo�^����Ă��郊�\�[�X���̎��̃��W�X�^���g�p
		/// </param>
		void RegistConstantBuffer(
			ID3D12Device& device, ConstantBuffer& constantBuffer, const int& registerNo = NEXT_REGISTER);

		/// <summary>
		/// �V�F�[�_�[���\�[�X(�e�N�X�`��)�o�^
		/// </summary>
		/// <param name="device">�f�o�C�X</param>
		/// <param name="texture">�e�N�X�`��</param>
		/// <param name="registerNo">
		/// �o�^���郌�W�X�^�ԍ� �f�t�H���g�l:NEXT_REGISTER(-1) ���ݓo�^����Ă��郊�\�[�X���̎��̃��W�X�^���g�p
		/// </param>
		void RegistShaderResource(
			ID3D12Device& device, Texture& texture, ShaderResourceViewDesc& desc, const int& registerNo = NEXT_REGISTER);

		/// <summary>
		/// �A���I�[�_�[�A�N�Z�X���\�[�X�o�^
		/// </summary>
		/// <param name="device">�f�o�C�X</param>
		/// <param name="unorderedAccessResource">�A���I�[�_�[�A�N�Z�X���\�[�X</param>
		/// <param name="registerNo">
		/// �o�^���郌�W�X�^�ԍ� �f�t�H���g�l:NEXT_REGISTER(-1) ���ݓo�^����Ă��郊�\�[�X���̎��̃��W�X�^���g�p
		/// </param>
		void RegistUnorderedAccessResource(
			ID3D12Device& device, UnorderedAccessResource& unorderedAccessResource,
			const int& registerNo = NEXT_REGISTER);

		/// <summary>
		/// �R���X�^���g�o�b�t�@�[���o�^����Ă��邩
		/// </summary>
		/// <returns></returns>
		bool IsRegistedConstantBuffer() { return _registedCBVNum > 0; }

		/// <summary>
		/// �V�F�[�_�[���\�[�X���o�^����Ă��邩
		/// </summary>
		/// <returns></returns>
		bool IsRegistedShaderResource() { return _registedSRVNum > 0; }

		/// <summary>
		/// �A���I�[�_�[�A�N�Z�X���\�[�X���o�^����Ă��邩
		/// </summary>
		/// <returns></returns>
		bool IsRegistedUnorderedAccessResource() { return _registedUAVNum > 0; }

		/// <summary>
		/// �f�B�X�N���v�^�q�[�v�̃A�h���X�擾
		/// </summary>
		/// <returns>�f�B�X�N���v�^�q�[�v�̃A�h���X</returns>
		ID3D12DescriptorHeap** GetDescriptorHeapAddress() {
			return _descriptorHeap.GetAddressOf();
		}
	};
}