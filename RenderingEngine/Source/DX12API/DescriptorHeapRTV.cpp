#include <cassert>

#include "DescriptorHeapRTV.h"
#include "RenderTargetBuffer.h"

using namespace NamelessEngine::Utility;

namespace NamelessEngine::DX12API
{
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

	RESULT DescriptorHeapRTV::Create(ID3D12Device& device)
	{
		// �n���h���̃C���N�������g�T�C�Y�擾
		_handleIncrimentSize =
			static_cast<SIZE_T>(device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

		// �f�B�X�N���v�^�q�[�v����
		if (FAILED(CreateDescriptorHeap(device))) { return RESULT::FAILED; }

		return RESULT::SUCCESS;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapRTV::GetNextCPUDescriptorHandle()
	{
		// �͈͊O�Q�Ƃ�h��
		assert(_nextHandleLocation <= _registedRTVNum);

		// ���̃n���h����
		auto handle = _rtvHeap->GetCPUDescriptorHandleForHeapStart();
		handle.ptr += _nextHandleLocation * _handleIncrimentSize;

		// ���̃n���h���ʒu���w��
		_nextHandleLocation++;

		return handle;
	}

	void DescriptorHeapRTV::RegistDescriptor(
		ID3D12Device& device, RenderTargetBuffer& buffer, DXGI_FORMAT format, bool isCubeMap)
	{
		auto handle = _rtvHeap->GetCPUDescriptorHandleForHeapStart();
		handle.ptr += _registedRTVNum * _handleIncrimentSize;

		if (isCubeMap) {
			for (size_t index = 0; index < 6; index++) {
				D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
				rtvDesc.Format = format;
				rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
				rtvDesc.Texture2DArray.FirstArraySlice = index;
				rtvDesc.Texture2DArray.ArraySize = 1;

				// ��H�H�H
				// �o�^�ς݂̃f�B�X�N���v�^�����C���N�������g
				_registedRTVNum++;
			}
		}
		else {
			D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};

			rtvDesc.Format = format;
			rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

			// �����_�[�^�[�Q�b�g�r���[����
			device.CreateRenderTargetView(&buffer.GetBuffer(), &rtvDesc, handle);
			// �o�^�ς݂̃f�B�X�N���v�^�����C���N�������g
			_registedRTVNum++;
		}
	}

}