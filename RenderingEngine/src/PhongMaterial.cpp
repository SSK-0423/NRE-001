#include "PhongMaterial.h"
#include "FBXMesh.h"

PhongMaterial::PhongMaterial()
{
	_textureName = L"";
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

MYRESULT PhongMaterial::CreateMaterialConstantBuffer(ID3D12Device& device)
{
	MYRESULT result = _materialCBuffer.Create(device, &_material, sizeof(PhongMaterialBuff));
	if (result == MYRESULT::FAILED) { return MYRESULT::FAILED; }
	return MYRESULT::SUCCESS;
}

MYRESULT PhongMaterial::CreateTexture(Dx12GraphicsEngine& graphicsEngine)
{
	std::wstring texturePath = _textureFolderPath + L"/" + _textureName;
	MYRESULT result = _texture.CreateTextureFromWIC(graphicsEngine, texturePath);
	if (result == MYRESULT::FAILED) { return MYRESULT::FAILED; }
	return MYRESULT::SUCCESS;
}

MYRESULT PhongMaterial::Commit()
{
	Dx12GraphicsEngine& graphicsEngine = Dx12GraphicsEngine::Instance();

	// コンスタントバッファー生成
	MYRESULT result = CreateMaterialConstantBuffer(graphicsEngine.Device());
	if (result == MYRESULT::FAILED) { return MYRESULT::FAILED; }

	// テクスチャ生成
	result = CreateTexture(graphicsEngine);
	if (result == MYRESULT::FAILED) { return MYRESULT::FAILED; }

	return MYRESULT::SUCCESS;
}

void PhongMaterial::ApplyMaterial(FBXMesh& mesh)
{
	Dx12GraphicsEngine& graphicsEngine = Dx12GraphicsEngine::Instance();

	// メッシュにコンスタントバッファーセット
	mesh.SetConstantBuffer(graphicsEngine.Device(), _materialCBuffer);
	// メッシュにテクスチャセット
	mesh.SetTexture(graphicsEngine.Device(), _texture);
}