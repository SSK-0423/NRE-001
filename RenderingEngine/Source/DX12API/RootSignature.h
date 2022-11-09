#pragma once
#include <d3d12.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")

#include "EngineUtility.h"

namespace NamelessEngine::DX12API
{
	/// <summary>
	/// �f�B�X�N���v�^�̎��
	/// </summary>
	enum DESCRIPTORTYPE
	{
		DESCRIPTORTYPE_SRV,
		DESCRIPTORTYPE_CBV,
		DESCRIPTORTYPE_UAV,
		DESCRIPTORTYPE_NUM
	};

	struct SamplerData
	{
		D3D12_FILTER samplerFilter;			// �T���v���[�t�B���^�[
		D3D12_TEXTURE_ADDRESS_MODE addressU;	// X�����̃e�N�X�`���}�b�s���O�ݒ�
		D3D12_TEXTURE_ADDRESS_MODE addressV;   // Y�����̃e�N�X�`���}�b�s���O�ݒ�
		D3D12_TEXTURE_ADDRESS_MODE addressW;   // Z�����̃e�N�X�`���}�b�s���O�ݒ�

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="samplerFilter">�T���v���|�t�B���^�[�ݒ�</param>
		/// <param name="addressU">X�����̃e�N�X�`���}�b�s���O�ݒ�</param>
		/// <param name="addressV">Y�����̃e�N�X�`���}�b�s���O�ݒ�</param>
		/// <param name="addressW">Z�����̃e�N�X�`���}�b�s���O�ݒ�</param>
		SamplerData(
			D3D12_FILTER samplerFilter = D3D12_FILTER_ANISOTROPIC,
			D3D12_TEXTURE_ADDRESS_MODE addressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE addressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE addressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP);
	};

	struct DescriptorRangeData
	{
		UINT srvDescriptorNum;	                // SRV��
		UINT cbvDescriptorNum;	                // CBV��
		UINT uavDescriptorNum;	                // UAV��
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="srvDescriptorNum">SRV��</param>
		/// <param name="cbvDescriptorNum">CBV��</param>
		/// <param name="uavDescriptorNum">UAV��</param>
		DescriptorRangeData(UINT srvDescriptorNum = 1, UINT cbvDescriptorNum = 1, UINT uavDescriptorNum = 1);
	};

	/// <summary>
	/// ���[�g�V�O�l�`���N���X�����p�\����
	/// </summary>
	struct RootSignatureData
	{
		SamplerData _samplerData;				// �T���v���[�̐ݒ�
		DescriptorRangeData _descRangeData;		// �f�B�X�N���v�^�����W�̐ݒ�

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="samplerFilter">�T���v���|�t�B���^�[�ݒ�</param>
		/// <param name="addressU">X�����̃e�N�X�`���}�b�s���O�ݒ�</param>
		/// <param name="addressV">Y�����̃e�N�X�`���}�b�s���O�ݒ�</param>
		/// <param name="addressW">Z�����̃e�N�X�`���}�b�s���O�ݒ�</param>
		/// <param name="srvDescriptorNum">SRV��</param>
		/// <param name="cbvDescriptorNum">CBV��</param>
		/// <param name="uavDescriptorNum">UAV��</param>
		RootSignatureData(
			D3D12_FILTER samplerFilter = D3D12_FILTER_ANISOTROPIC,
			D3D12_TEXTURE_ADDRESS_MODE addressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE addressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE addressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			UINT srvDescriptorNum = 1, UINT cbvDescriptorNum = 1, UINT uavDescriptorNum = 1);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="samplerData">�T���v���[�f�[�^</param>
		/// <param name="descRangeData">�f�B�X�N���v�^�����W�f�[�^</param>
		RootSignatureData(SamplerData samplerData, DescriptorRangeData descRangeData);
	};

	class RootSignature {
	public:
		RootSignature() = default;
		~RootSignature() = default;

	private:
		Microsoft::WRL::ComPtr<ID3D12RootSignature> _rootSignature = nullptr;	// ���[�g�V�O�l�`��

		/// <summary>
		/// ���[�g�V�O�l�`������
		/// </summary>
		/// <param name="device">�f�o�C�X</param>
		/// <param name="descRangeData">�f�B�X�N���v�^�����W�̐ݒ�</param>
		/// <param name="samplerDescs">�T���v���[</param>
		/// <param name="samplerNum">�T���v���[��</param>
		/// <returns></returns>
		HRESULT CreateRootSignature(
			ID3D12Device& device, const DescriptorRangeData& descRangeData,
			const D3D12_STATIC_SAMPLER_DESC& samplerDescs, UINT samplerNum);

	public:

		/// <summary>
		/// ���[�g�V�O�l�`������
		/// </summary>
		/// <param name="device">�f�o�C�X</param>
		/// <param name="data">���[�g�V�O�l�`���̐ݒ�</param>
		/// <returns></returns>
		Utility::RESULT Create(ID3D12Device& device, const RootSignatureData& data);

		/// <summary>
		/// ���[�g�V�O�l�`������
		/// </summary>
		/// <param name="device"></param>
		/// <param name="data"></param>
		/// <param name="samplerDescs"></param>
		/// <param name="samplerNum"></param>
		/// <returns></returns>
		Utility::RESULT Create(
			ID3D12Device& device, const DescriptorRangeData& descRangeData,
			const D3D12_STATIC_SAMPLER_DESC& samplerDescs, UINT samplerNum);

		/// <summary>
		/// ���[�g�V�O�l�`���擾
		/// </summary>
		/// <returns>���[�g�V�O�l�`��</returns>
		ID3D12RootSignature& GetRootSignature() const {
			return *_rootSignature.Get();
		}

	};
}