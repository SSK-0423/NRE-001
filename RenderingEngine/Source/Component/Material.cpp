#include "Material.h"
#include "ConstantBuffer.h"
#include "Dx12GraphicsEngine.h"
#include "RenderingContext.h"
#include "EngineUtility.h"

using namespace NamelessEngine::Core;
using namespace NamelessEngine::DX12API;
using namespace NamelessEngine::Utility;

namespace NamelessEngine::Component
{
	Material::Material()
		: _buffer(new ConstantBuffer())
	{
		_materialData.baseColor = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
		_materialData.metallic = 0.f;
		_materialData.roughness = 0.f;
		_materialData.useReflection = false;

		RESULT result = _buffer->Create(
			Dx12GraphicsEngine::Instance().Device(), (void*)&_materialData, sizeof(MaterialCBuff));
	}
	Material::~Material()
	{
		SafetyDelete<ConstantBuffer>(_buffer);
	}
	void Material::Update(float deltatime)
	{
		_buffer->UpdateData(&_materialData);
	}
	void Material::Draw(DX12API::RenderingContext& renderContext)
	{
	}
	void Material::SetBaseColor(float r, float g, float b, float a)
	{
		_materialData.baseColor = DirectX::XMFLOAT4(r, g, b, a);
	}
	void Material::SetMetallic(float metallic)
	{
		_materialData.metallic = metallic;
	}
	void Material::SetRoughness(float roughness)
	{
		_materialData.roughness = roughness;
	}
	void Material::SetUseReflection(BOOL useReflection)
	{
		_materialData.useReflection = useReflection;
	}
	DX12API::ConstantBuffer& Material::GetConstantBuffer()
	{
		return *_buffer;
	}
}
