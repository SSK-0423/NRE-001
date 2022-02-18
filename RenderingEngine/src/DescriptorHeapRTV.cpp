#include "DescriptorHeapRTV.h"

HRESULT DescriptorHeapRTV::CreateDescriptorHeap(ID3D12Device& device)
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NumDescriptors = MAXDESCRIPTORNUM;
	heapDesc.NodeMask = 0;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	HRESULT result = device.CreateDescriptorHeap(
		&heapDesc, IID_PPV_ARGS(_rtvHeap.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { return result; }

	return result;
}

MYRESULT DescriptorHeapRTV::Create(ID3D12Device& device)
{
	// �n���h���̃C���N�������g�T�C�Y�擾
	_handleIncrimentSize = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// �f�B�X�N���v�^�q�[�v����
	if (FAILED(CreateDescriptorHeap(device))) {
		return MYRESULT::FAILED;
	}

	return MYRESULT::SUCCESS;
}

void DescriptorHeapRTV::RegistDescriptor(ID3D12Device& device, RenderTargetBuffer& buffer)
{
	auto handle = _rtvHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += _registedDescriptorNum * _handleIncrimentSize;

	// �����_�[�^�[�Q�b�g�r���[����
	device.CreateRenderTargetView(&buffer.GetBuffer(), nullptr, handle);

	// �o�^�ς݂̃f�B�X�N���v�^�����C���N�������g
	_registedDescriptorNum++;
}
