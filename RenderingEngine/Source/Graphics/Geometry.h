#pragma once
#include "GeometryImpl.h"
#include "RenderingContext.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "DescriptorHeapCBV_SRV_UAV.h"

namespace NamelessEngine::Graphics
{
	class Geometry {
	public:
		Geometry(GeometryImpl* impl);

		void Draw(DX12API::RenderingContext& renderContext);

		void SetConstantBuffer(
			ID3D12Device& device, DX12API::ConstantBuffer& constantBuffer,
			const int& registerNo = DX12API::DescriptorHeapCBV_SRV_UAV::NEXT_REGISTER);

		void SetTexture(
			ID3D12Device& device, DX12API::Texture& texture,
			const int& registerNo = DX12API::DescriptorHeapCBV_SRV_UAV::NEXT_REGISTER);
	private:
		GeometryImpl* geometryImpl = nullptr;

	};
}