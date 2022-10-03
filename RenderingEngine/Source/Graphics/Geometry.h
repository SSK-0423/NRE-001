#pragma once
#include "GeometryImpl.h"
#include "RenderingContext.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "DescriptorHeapCBV_SRV_UAV.h"

class Geometry {
public:
	Geometry(GeometryImpl* impl);
	
	void Draw(RenderingContext& renderContext);
	
	void SetConstantBuffer(
		ID3D12Device& device, ConstantBuffer& constantBuffer,
		const int& registerNo = DescriptorHeapCBV_SRV_UAV::_NEXT_REGISTER);
	
	void SetTexture(
		ID3D12Device& device, Texture& texture,
		const int& registerNo = DescriptorHeapCBV_SRV_UAV::_NEXT_REGISTER);
private:
	GeometryImpl* geometryImpl = nullptr;

};