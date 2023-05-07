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
		: /*baseColorTexture(new Texture()), metallicTexture(new Texture()), roughnessTexture(new Texture()),
		normalTexture(new Texture()), occulusionTexture(new Texture())*/
		_buffer(new ConstantBuffer()), baseColor(1.f, 1.f, 1.f, 1.f), metallic(0.f), roughness(0.f)
	{
		materialData.baseColor = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
		materialData.metallic = 0.f;
		materialData.roughness = 0.f;

		RESULT result = _buffer->Create(
			Dx12GraphicsEngine::Instance().Device(), (void*)&materialData, sizeof(MaterialCBuff));
	}
	Material::~Material()
	{
		SafetyDelete<ConstantBuffer>(_buffer);
	}
	void Material::Update(float deltatime)
	{
		_buffer->UpdateData(&materialData);
	}
	void Material::Draw(DX12API::RenderingContext& renderContext)
	{
	}

	void Material::Build()
	{
		if (baseColorTexture == nullptr)
			baseColorTexture = new Texture();
		baseColorTexture->CreateTextureFromConstantData(Dx12GraphicsEngine::Instance(), (uint8_t*)&baseColor, sizeof(DirectX::XMFLOAT4), 1, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT);

		if (metallicTexture == nullptr)
			metallicTexture = new Texture();
		metallicTexture->CreateTextureFromConstantData(Dx12GraphicsEngine::Instance(), (uint8_t*)&metallic, sizeof(float), 1, 1, 1, DXGI_FORMAT_R32_FLOAT);

		if (roughnessTexture == nullptr)
			roughnessTexture = new Texture();
		roughnessTexture->CreateTextureFromConstantData(Dx12GraphicsEngine::Instance(), (uint8_t*)&roughness, sizeof(float), 1, 1, 1, DXGI_FORMAT_R32_FLOAT);
	}

	DX12API::ConstantBuffer& Material::GetConstantBuffer()
	{
		return *_buffer;
	}
}
