#include "PhongMaterial.h"
#include "FBXMesh.h"

using namespace NamelessEngine::Utility;
using namespace NamelessEngine::Core;

namespace NamelessEngine::Graphics
{
	PhongMaterial::PhongMaterial()
		: _textureName(L"")
	{
		// PhongMaterialBuff型変数の初期化
		SetAmbient(0.f, 0.f, 0.f, 0.f);
		SetDiffuse(0.f, 0.f, 0.f, 0.f);
		SetSpecular(0.f, 0.f, 0.f, 0.f);
		SetShininess(0.f);
	}

	PhongMaterial::~PhongMaterial()
	{
	}

	PhongMaterial::PhongMaterial(const PhongMaterial& inst)
	{
		this->_material = inst._material;
		this->_textureName = inst._textureName;
		this->_textureFolderPath = inst._textureFolderPath;
		this->_materialCBuffer = inst._materialCBuffer;
		this->_texture = inst._texture;
	}

	RESULT PhongMaterial::CreateMaterialConstantBuffer(ID3D12Device& device)
	{
		RESULT result = _materialCBuffer.Create(device, &_material, sizeof(PhongMaterialBuff));
		if (result == RESULT::FAILED) { return RESULT::FAILED; }
		return RESULT::SUCCESS;
	}

	RESULT PhongMaterial::CreateTexture(Dx12GraphicsEngine& graphicsEngine)
	{
		// テクスチャがない場合
		if (_textureName.empty()) {
			std::vector<ColorRGBA> whiteTexture(_dummyTextureWidth * _dummyTextureHeight);
			for (auto& color : whiteTexture) {
				color.r = 1.f;
				color.g = 1.f;
				color.b = 1.f;
				color.a = 1.f;
			}
			RESULT result = _texture.CreateTextureFromRGBAData(
				graphicsEngine, whiteTexture,
				_dummyTextureWidth, _dummyTextureHeight, DXGI_FORMAT_R32G32B32A32_FLOAT);
		}
		// テクスチャがある場合
		else {
			std::wstring texturePath = _textureFolderPath + L"/" + _textureName;
			RESULT result = _texture.CreateTextureFromWIC(graphicsEngine, texturePath);
			if (result == RESULT::FAILED) { return RESULT::FAILED; }
		}
		return RESULT::SUCCESS;
	}

	RESULT PhongMaterial::Commit()
	{
		Dx12GraphicsEngine& graphicsEngine = Dx12GraphicsEngine::Instance();

		// コンスタントバッファー生成
		RESULT result = CreateMaterialConstantBuffer(graphicsEngine.Device());
		if (result == RESULT::FAILED) { return RESULT::FAILED; }

		// テクスチャ生成
		result = CreateTexture(graphicsEngine);
		if (result == RESULT::FAILED) { return RESULT::FAILED; }

		return RESULT::SUCCESS;
	}

	void PhongMaterial::ApplyMaterial(FBXMesh& mesh)
	{
		Dx12GraphicsEngine& graphicsEngine = Dx12GraphicsEngine::Instance();

		// メッシュにコンスタントバッファーセット
		mesh.SetConstantBuffer(graphicsEngine.Device(), _materialCBuffer);
		// メッシュにテクスチャセット
		mesh.SetTexture(graphicsEngine.Device(), _texture);
	}
}