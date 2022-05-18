#include "DescriptorHeapDSV.h"

HRESULT DescriptorHeapDSV::CreateDescriptorHeap(ID3D12Device& device)
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	heapDesc.NumDescriptors = 1;
	heapDesc.NodeMask = 0;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	HRESULT result = device.CreateDescriptorHeap(
		&heapDesc, IID_PPV_ARGS(_dsvHeap.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { return result; }

	return result;
}

MYRESULT DescriptorHeapDSV::Create(ID3D12Device& device)
{
	// �n���h���̃C���N�������g�T�C�Y�擾
	_handleIncrimentSize =
		static_cast<SIZE_T>(device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV));

	// �f�B�X�N���v�^�q�[�v����
	if (FAILED(CreateDescriptorHeap(device))) { return MYRESULT::FAILED; }

	return MYRESULT::SUCCESS;
}

void DescriptorHeapDSV::RegistDescriptor(ID3D12Device& device, DepthStencilBuffer& buffer)
{
	// �n���h���擾
	auto handle = _dsvHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += _registedDSVNum * _handleIncrimentSize;

	// �f�v�X�X�e���V���r���[�ݒ�
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = buffer.GetDepthStencilBufferData().depthFormat;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

	//�f�v�X�X�e���V���r���[����
	device.CreateDepthStencilView(&buffer.GetBuffer(), &dsvDesc, handle);

	// �o�^�ς݂̃f�B�X�N���v�^�����C���N�������g
	_registedDSVNum++;
}
