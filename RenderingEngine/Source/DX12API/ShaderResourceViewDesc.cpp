#include "ShaderResourceViewDesc.h"
#include "Texture.h"

namespace NamelessEngine::DX12API
{
	ShaderResourceViewDesc::ShaderResourceViewDesc(Texture& texture, bool isCubeTex)
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};

		if (isCubeTex) {
			desc.Format = texture.GetTexMetadata().format;
			desc.TextureCube.MipLevels = UINT(texture.GetTexMetadata().mipLevels);
			desc.TextureCube.MostDetailedMip = 0;
			desc.TextureCube.ResourceMinLODClamp = 0;
			desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		}
		else {
			desc.Format = texture.GetImage().format;
			desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipLevels = texture.GetTexMetadata().mipLevels;
		}

		desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

		this->desc = desc;

	}

	ShaderResourceViewDesc::~ShaderResourceViewDesc()
	{
	}

}