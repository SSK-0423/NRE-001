#pragma once
#include <d3d12.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")

#include "EngineUtility.h"

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

struct RootSignatureData
{
	D3D12_TEXTURE_ADDRESS_MODE _addressU;	// X�����̃e�N�X�`���}�b�s���O�ݒ�
	D3D12_TEXTURE_ADDRESS_MODE _addressV;   // Y�����̃e�N�X�`���}�b�s���O�ݒ�
	D3D12_TEXTURE_ADDRESS_MODE _addressW;   // Z�����̃e�N�X�`���}�b�s���O�ݒ�
	UINT _srvDescriptorNum;	                // SRV��
	UINT _cbvDescriptorNum;	                // CBV��
	UINT _uavDescriptorNum;	                // UAV��

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="addressU">X�����̃e�N�X�`���}�b�s���O�ݒ�</param>
	/// <param name="addressV">Y�����̃e�N�X�`���}�b�s���O�ݒ�</param>
	/// <param name="addressW">Z�����̃e�N�X�`���}�b�s���O�ݒ�</param>
	/// <param name="srvDescriptorNum">SRV��</param>
	/// <param name="cbvDescriptorNum">CBV��</param>
	/// <param name="uavDescriptorNum">UAV��</param>
	RootSignatureData(
		D3D12_TEXTURE_ADDRESS_MODE addressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE addressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE addressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		UINT srvDescriptorNum = 1, UINT cbvDescriptorNum = 1, UINT uavDescriptorNum = 1);
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
	/// <param name="data">���[�g�V�O�l�`���̐ݒ�</param>
	/// <returns></returns>
	HRESULT CreateRootSignature(ID3D12Device& device, const RootSignatureData& data);
public:

	/// <summary>
	/// ���[�g�V�O�l�`������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="data">���[�g�V�O�l�`���̐ݒ�</param>
	/// <returns></returns>
	MYRESULT Create(ID3D12Device& device, const RootSignatureData& data);
};