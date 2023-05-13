#include "Material.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "Dx12GraphicsEngine.h"
#include "RenderingContext.h"
#include "EngineUtility.h"

using namespace NamelessEngine::Core;
using namespace NamelessEngine::DX12API;
using namespace NamelessEngine::Utility;

namespace NamelessEngine::Component
{
	Material::Material()
		: baseColor(1.f, 1.f, 1.f, 1.f), metallic(0.f), roughness(0.f)
	{
		materialData.baseColor = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
		materialData.metallic = 0.f;
		materialData.roughness = 0.f;

	}
	Material::~Material()
	{
		SafetyDelete<Texture>(baseColorTexture);
		SafetyDelete<Texture>(metalRoughTexture);
		SafetyDelete<Texture>(normalTexture);
		SafetyDelete<Texture>(occlusionTexture);
	}
	void Material::Update(float deltatime)
	{
	}
	void Material::Draw(DX12API::RenderingContext& renderContext)
	{
	}

	void Material::Build()
	{
		if (baseColorTexture == nullptr)
			baseColorTexture = new Texture();
		baseColorTexture->CreateTextureFromConstantData(Dx12GraphicsEngine::Instance(), (uint8_t*)&baseColor, sizeof(DirectX::XMFLOAT4), 1, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT);

		if (metalRoughTexture == nullptr)
			metalRoughTexture = new Texture();

		DirectX::XMFLOAT2 metalRough(metallic, roughness);
		metalRoughTexture->CreateTextureFromConstantData(Dx12GraphicsEngine::Instance(), (uint8_t*)&metalRough, sizeof(DirectX::XMFLOAT2), 1, 1, 1, DXGI_FORMAT_R32G32_FLOAT);
	}
}
