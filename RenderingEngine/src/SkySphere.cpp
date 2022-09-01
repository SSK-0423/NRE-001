#include "SkySphere.h"

SkySphere::SkySphere()
{
}

SkySphere::~SkySphere()
{
}

MYRESULT SkySphere::CreateTexture(const std::wstring& texturePath)
{
	return texture.CreateTextureFromWIC(Dx12GraphicsEngine::Instance(), texturePath);
}

MYRESULT SkySphere::Create(ID3D12Device& device, SkySphereData& data)
{
	MYRESULT result = sphere.Create(device, data.sphereGeometryData);
	if (result == MYRESULT::FAILED) { return result; }
	
	result = CreateTexture(data.texturePath);
	if (result == MYRESULT::FAILED) { return result; }

	sphere.SetTexture(device, texture);

	return MYRESULT::SUCCESS;
}

void SkySphere::Draw(RenderingContext& renderContext)
{
	sphere.Draw(renderContext);
}
