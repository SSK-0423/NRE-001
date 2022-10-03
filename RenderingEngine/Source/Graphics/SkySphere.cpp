#include "SkySphere.h"
#include "ShaderResourceViewDesc.h"

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
	SphereGeometryData sphereData;
	sphereData.stackNum = data.stackNum;
	sphereData.sectorNum = data.sectorNum;
	sphereData.radius = data.radius;

	MYRESULT result = sphere.Create(device, sphereData);
	if (result == MYRESULT::FAILED) { return result; }

	result = CreateTexture(data.texturePath);
	if (result == MYRESULT::FAILED) { return result; }

	ShaderResourceViewDesc desc(texture);
	sphere.SetTexture(device, texture, desc);

	return MYRESULT::SUCCESS;
}

void SkySphere::Draw(RenderingContext& renderContext)
{
	sphere.Draw(renderContext);
}
