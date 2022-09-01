#include "SphereGeometry.h"

SphereGeometry::SphereGeometry()
{
}

void SphereGeometry::CreateVerticesAndIndicesData()
{
}

MYRESULT SphereGeometry::CreateVertexBuffer(ID3D12Device& devic)
{
	return MYRESULT::SUCCESS;
}

MYRESULT SphereGeometry::CreateIndexBuffer(ID3D12Device& device)
{
	return MYRESULT::SUCCESS;
}

MYRESULT SphereGeometry::CreateGraphicsPipelineState(ID3D12Device& device, SphereGeometryData& data)
{
	return MYRESULT::SUCCESS;
}

MYRESULT SphereGeometry::CreateDescriptorHeap(ID3D12Device& device)
{
	return MYRESULT::SUCCESS;
}

MYRESULT SphereGeometry::Create(ID3D12Device& device, SphereGeometryData& data)
{
	CreateVerticesAndIndicesData();

	MYRESULT result = CreateVertexBuffer(device);
	if (result == MYRESULT::FAILED) { return result; }
	
	result = CreateIndexBuffer(device);
	if (result == MYRESULT::FAILED) { return result; }
	
	result = CreateGraphicsPipelineState(device, data);
	if (result == MYRESULT::FAILED) { return result; }
	
	result = CreateDescriptorHeap(device);
	if (result == MYRESULT::FAILED) { return result; }
    
	return MYRESULT::SUCCESS;
}

void SphereGeometry::Draw(RenderingContext& renderContext)
{
}

void SphereGeometry::SetConstantBuffer(ID3D12Device& device, ConstantBuffer& constantBuffer, const int& registerNo)
{
}

void SphereGeometry::SetTexture(ID3D12Device& device, Texture& texture, const int& registerNo)
{
}

size_t SphereGeometryData::GetRenderTargetNum() const
{
	for (size_t idx = 0; idx < colorFormats.size(); idx++)
	{
		if (colorFormats[idx] == DXGI_FORMAT_UNKNOWN)
			return idx;
	}
}
