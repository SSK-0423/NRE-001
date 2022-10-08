#include "Geometry.h"

using namespace NamelessEngine::DX12API;

namespace NamelessEngine::Graphics
{
	Geometry::Geometry(GeometryImpl* impl)
	{
	}

	void Geometry::Draw(DX12API::RenderingContext& renderContext)
	{
	}

	void Geometry::SetConstantBuffer(ID3D12Device& device, ConstantBuffer& constantBuffer, const int& registerNo)
	{
	}

	void Geometry::SetTexture(ID3D12Device& device, Texture& texture, const int& registerNo)
	{
	}
}
