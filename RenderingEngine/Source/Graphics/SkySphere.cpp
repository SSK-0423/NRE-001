#include "SkySphere.h"
#include "ShaderResourceViewDesc.h"
#include "Dx12GraphicsEngine.h"

using namespace NamelessEngine::Utility;
using namespace NamelessEngine::DX12API;
using namespace NamelessEngine::Core;

namespace NamelessEngine::Graphics
{
	SkySphere::SkySphere()
	{
	}

	SkySphere::~SkySphere()
	{
	}

	RESULT SkySphere::CreateTexture(const std::wstring& texturePath)
	{
		//return texture.CreateTextureFromWIC(Dx12GraphicsEngine::Instance(), texturePath);
		return RESULT::SUCCESS;
	}

	RESULT SkySphere::Create(ID3D12Device& device, SkySphereData& data)
	{
		//SphereMeshData sphereData;
		//sphereData.stackNum = data.stackNum;
		//sphereData.sectorNum = data.sectorNum;
		//sphereData.radius = data.radius;

		//RESULT result = sphere.Create(device, sphereData);
		//if (result == RESULT::FAILED) { return result; }

		//result = CreateTexture(data.texturePath);
		//if (result == RESULT::FAILED) { return result; }

		//ShaderResourceViewDesc desc(texture);
		//sphere.SetTexture(device, texture, desc);

		return RESULT::SUCCESS;
	}

	void SkySphere::Draw(RenderingContext& renderContext)
	{
		//sphere.Draw(renderContext);
	}

}