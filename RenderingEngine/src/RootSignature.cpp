#include <d3dx12.h>
#include <vector>

#include "RootSignature.h"

SamplerData::SamplerData(
	D3D12_FILTER samplerFilter,
	D3D12_TEXTURE_ADDRESS_MODE addressU,
	D3D12_TEXTURE_ADDRESS_MODE addressV,
	D3D12_TEXTURE_ADDRESS_MODE addressW)
	: samplerFilter(samplerFilter), addressU(addressU), addressV(addressV), addressW(addressW)
{}

DescriptorRangeData::DescriptorRangeData(UINT srvDescriptorNum, UINT cbvDescriptorNum, UINT uavDescriptorNum)
	: _srvDescriptorNum(srvDescriptorNum), _cbvDescriptorNum(cbvDescriptorNum),
	_uavDescriptorNum(uavDescriptorNum)
{}

RootSignatureData::RootSignatureData(
	D3D12_FILTER samplerFilter,
	D3D12_TEXTURE_ADDRESS_MODE addressU, D3D12_TEXTURE_ADDRESS_MODE addressV,
	D3D12_TEXTURE_ADDRESS_MODE addressW,
	UINT srvDescriptorNum, UINT cbvDescriptorNum, UINT uavDescriptorNum)
{
	_samplerData.samplerFilter = samplerFilter;
	_samplerData.addressU = addressU;
	_samplerData.addressV = addressV;
	_samplerData.addressW = addressW;

	_descRangeData._srvDescriptorNum = srvDescriptorNum;
	_descRangeData._cbvDescriptorNum = cbvDescriptorNum;
	_descRangeData._uavDescriptorNum = uavDescriptorNum;
}

RootSignatureData::RootSignatureData(SamplerData samplerData, DescriptorRangeData descRangeData)
	: _samplerData(samplerData), _descRangeData(descRangeData)
{}

HRESULT RootSignature::CreateRootSignature(
	ID3D12Device& device, const DescriptorRangeData& descRangeData,
	const D3D12_STATIC_SAMPLER_DESC& samplerDescs, UINT samplerNum)
{
	// ディスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descTblRange[DESCRIPTORTYPE_NUM] = {};
	descTblRange[DESCRIPTORTYPE_SRV].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, descRangeData._srvDescriptorNum, 0);
	descTblRange[DESCRIPTORTYPE_CBV].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, descRangeData._cbvDescriptorNum, 0);
	descTblRange[DESCRIPTORTYPE_UAV].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, descRangeData._uavDescriptorNum, 0);

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER	rootPrams[DESCRIPTORTYPE_NUM] = {};
	rootPrams[DESCRIPTORTYPE_SRV].InitAsDescriptorTable(1, &descTblRange[DESCRIPTORTYPE_SRV]);
	rootPrams[DESCRIPTORTYPE_CBV].InitAsDescriptorTable(
		1, &descTblRange[DESCRIPTORTYPE_CBV],D3D12_SHADER_VISIBILITY_VERTEX);
	rootPrams[DESCRIPTORTYPE_UAV].InitAsDescriptorTable(1, &descTblRange[DESCRIPTORTYPE_UAV]);

	// ルートシグネチャ
	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Init(
		_countof(rootPrams), rootPrams, samplerNum, &samplerDescs,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	Microsoft::WRL::ComPtr<ID3DBlob> rootSignatureBlob = nullptr;	// ルートシグネチャのバイナリ
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;			// 

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
	// STATIC_SAMPLER_DESC作成
	SamplerData samplerData = data._samplerData;

	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Init(
		0, samplerData.samplerFilter, samplerData.addressU, samplerData.addressV, samplerData.addressW);

	// ルートシグネチャ生成
	return Create(device, data._descRangeData, samplerDesc, 1);
}

MYRESULT RootSignature::Create(
	ID3D12Device& device, const DescriptorRangeData& descRangeData,
	const D3D12_STATIC_SAMPLER_DESC& samplerDescs, UINT samplerNum)
{
	// ルートシグネチャ生成
	if (FAILED(CreateRootSignature(device, descRangeData, samplerDescs, samplerNum))) {
		return MYRESULT::FAILED;
	}

	return MYRESULT::SUCCESS;
}