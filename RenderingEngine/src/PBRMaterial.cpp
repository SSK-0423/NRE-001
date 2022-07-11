#include <cassert>
#include "PBRMaterial.h"

PBRMaterial::PBRMaterial()
{
}

PBRMaterial::~PBRMaterial()
{
}

MYRESULT PBRMaterial::CreateDammyTexture(Dx12GraphicsEngine& graphicsEngine)
{
	return MYRESULT::SUCCESS;
}

MYRESULT PBRMaterial::LoadPBRTextures(Dx12GraphicsEngine& graphicsEngine)
{
	return MYRESULT::SUCCESS;
}

MYRESULT PBRMaterial::Commit()
{
	Dx12GraphicsEngine& graphicsEngine = Dx12GraphicsEngine::Instance();

	// PBRテクスチャ読み込み
	MYRESULT result = LoadPBRTextures(graphicsEngine);
	if (result == MYRESULT::FAILED) { return MYRESULT::FAILED; }

	// ダミーテクスチャ生成
	result = CreateDammyTexture(graphicsEngine);
	if (result == MYRESULT::FAILED) { return MYRESULT::FAILED; }

	return MYRESULT::SUCCESS;
}

void PBRMaterial::ApplyMaterial(FBXMesh& mesh)
{
}

void PBRMaterial::SetTextureFolderPath(std::wstring folderPath)
{
	_textureFolderPath = folderPath;
}

void PBRMaterial::SetTextureName(TEXTURETYPE type, std::wstring path)
{
	if (type == TEXTURETYPE::TYPE_NUM) {
		MessageBox(NULL, L"無効なTEXTURETYPEです。", L"エラーメッセージ", MB_OK);
		return;
	}

	_pbrTexturePaths[type] = path;
}