#include "Material.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "Dx12GraphicsEngine.h"
#include "RenderingContext.h"
#include "EngineUtility.h"

#include <algorithm>

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

		// TODO: 作成したテクスチャを利用可能にする
		std::vector<unsigned char> color = {
			static_cast<unsigned char>(std::clamp(baseColor.x,0.f,1.f) * 255.f / 255.f),
			static_cast<unsigned char>(std::clamp(baseColor.y,0.f,1.f) * 255.f / 255.f),
			static_cast<unsigned char>(std::clamp(baseColor.z,0.f,1.f) * 255.f / 255.f),
			static_cast<unsigned char>(std::clamp(baseColor.w,0.f,1.f) * 255.f / 255.f) };

		baseColorTexture->CreateTextureFromConstantData(
			Core::Dx12GraphicsEngine::Instance(), color.data(), sizeof(unsigned char) * 4,
			1, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
		baseColorTexture->CreateTextureFromConstantData(Dx12GraphicsEngine::Instance(), (uint8_t*)&color, sizeof(DirectX::XMFLOAT4), 1, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT);

		if (metalRoughTexture == nullptr)
			metalRoughTexture = new Texture();

		std::vector<unsigned char> metalRough = {
			0,
			static_cast<unsigned char>(std::clamp(roughness,0.f,1.f) * 255.f / 255.f),
			static_cast<unsigned char>(std::clamp(metallic,0.f,1.f) * 255.f / 255.f),
			255 };
		metalRoughTexture->CreateTextureFromConstantData(
			Core::Dx12GraphicsEngine::Instance(), metalRough.data(), sizeof(unsigned char) * 4,
			1, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM);
	}
}
