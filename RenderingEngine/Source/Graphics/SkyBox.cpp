#include "SkyBox.h"

#include "ShaderResourceViewDesc.h"
#include "Dx12GraphicsEngine.h"
#include "RenderingContext.h"
#include "Texture.h"

#include "Mesh.h"

#include "CubeMesh.h"

using namespace NamelessEngine::DX12API;
using namespace NamelessEngine::Component;
using namespace NamelessEngine::Utility;

namespace NamelessEngine::Graphics
{
	SkyBox::SkyBox()
		: _cubeTexture(new Texture), _cubeMesh(new Mesh())
	{
	}
	SkyBox::~SkyBox()
	{
	}
	Utility::RESULT SkyBox::Create(Core::Dx12GraphicsEngine& graphicsEngine, const std::wstring& texturePass)
	{
		RESULT result = _cubeTexture->CreateCubeTextureFromDDS(graphicsEngine, texturePass);
		if (result == RESULT::FAILED) { return result; }

		MeshData data = CubeMesh::CreateMeshData();
		result = _cubeMesh->Create(graphicsEngine.Device(), data);
		if (result == RESULT::FAILED) { return result; }

		ShaderResourceViewDesc desc(*_cubeTexture, true);
		_cubeMesh->SetTexture(graphicsEngine.Device(), *_cubeTexture, desc);

		return result;
	}
	void SkyBox::Draw(DX12API::RenderingContext& renderContext)
	{
		_cubeMesh->Draw(renderContext);
	}
}