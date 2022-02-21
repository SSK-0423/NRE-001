#include <d3dx12.h>
#include <vector>

#include "RootSignature.h"

RootSignatureData::RootSignatureData(
	D3D12_TEXTURE_ADDRESS_MODE addressU, D3D12_TEXTURE_ADDRESS_MODE addressV,
	D3D12_TEXTURE_ADDRESS_MODE addressW,
	UINT srvDescriptorNum, UINT cbvDescriptorNum, UINT uavDescriptorNum)
	: _addressU(addressU), _addressV(addressV), _addressW(addressW),
	_srvDescriptorNum(srvDescriptorNum), _cbvDescriptorNum(cbvDescriptorNum),
	_uavDescriptorNum(uavDescriptorNum)
{}

HRESULT RootSignature::CreateRootSignature(ID3D12Device& device, const RootSignatureData& data)
{
	CD3DX12_DESCRIPTOR_RANGE descTblRange[DESCRIPTORTYPE_NUM] = {};
	descTblRange[DESCRIPTORTYPE_SRV].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, data._srvDescriptorNum, 0);
	descTblRange[DESCRIPTORTYPE_CBV].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, data._cbvDescriptorNum, 0);
	descTblRange[DESCRIPTORTYPE_UAV].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, data._uavDescriptorNum, 0);

	CD3DX12_ROOT_PARAMETER	rootPrams[DESCRIPTORTYPE_NUM] = {};
	rootPrams[DESCRIPTORTYPE_SRV].InitAsDescriptorTable(1, &descTblRange[DESCRIPTORTYPE_SRV]);
	rootPrams[DESCRIPTORTYPE_CBV].InitAsDescriptorTable(1, &descTblRange[DESCRIPTORTYPE_CBV]);
	rootPrams[DESCRIPTORTYPE_UAV].InitAsDescriptorTable(1, &descTblRange[DESCRIPTORTYPE_UAV]);

	// TODO:サンプラーは様々な設定に対応できるようにする
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc[1] = {};
	samplerDesc[0].Init(0, D3D12_FILTER_ANISOTROPIC, data._addressU, data._addressV, data._addressW);

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Init(
		_countof(rootPrams), rootPrams, _countof(samplerDesc), samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	Microsoft::WRL::ComPtr<ID3DBlob> rootSignatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

	// ルートシグネチャのバイナリコード生成
	HRESULT result = D3D12SerializeRootSignature(
		&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSignatureBlob, &errorBlob);
	if (FAILED(result)) { return result; }

	// ルートシグネチャ生成
	result = device.CreateRootSignature(
		0, rootSignatureBlob->GetBufferPointer(), rootSignatureBlob->GetBufferSize(),
		IID_PPV_ARGS(_rootSignature.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { return result; }

	return result;
}

MYRESULT RootSignature::Create(ID3D12Device& device, const RootSignatureData& data)
{
	if (FAILED(CreateRootSignature(device, data))) { return MYRESULT::FAILED; }

	return MYRESULT::SUCCESS;
}

