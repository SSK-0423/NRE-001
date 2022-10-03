#include <cassert>
#include "PBRMaterial.h"
#include "FBXMesh.h"

PBRMaterial::PBRMaterial()
{
}

PBRMaterial::~PBRMaterial()
{
}

PBRMaterial::PBRMaterial(const PBRMaterial& inst)
{
}

MYRESULT PBRMaterial::CreateDammyTexture(Dx12GraphicsEngine& graphicsEngine)
{
	// ���F�f�[�^�p��
	std::vector<ColorRGBA> colorData(_dummyTextureWidth * _dummyTextureHeight);
	for (auto& color : colorData) {
		color.r = 1.f;
		color.g = 1.f;
		color.b = 1.f;
	}

	MYRESULT result = _dummyTexture.CreateTextureFromRGBAData(
		graphicsEngine, colorData, _dummyTextureWidth, _dummyTextureHeight, DXGI_FORMAT_R32G32B32A32_FLOAT);
	if (result == MYRESULT::FAILED) { return MYRESULT::FAILED; }

	return MYRESULT::SUCCESS;
}

MYRESULT PBRMaterial::LoadPBRTextures(Dx12GraphicsEngine& graphicsEngine)
{
	// �e�N�X�`���ǂݍ���
	for (auto& texName : _pbrTextureNames) {
		// �e�N�X�`�����w�肳��Ă���ꍇ
		if (texName.second != L"") {
			_pbrTextures[texName.first] = new Texture();
			std::wstring texturePath = _textureFolderPath + L"/" + texName.second;
			// DDS�t�@�C��
			if (GetExtension(texName.second) == L"dds") {
				MYRESULT result = _pbrTextures[texName.first]->CreateTextureFromDDS(graphicsEngine, texturePath);
				if (result == MYRESULT::FAILED) { return MYRESULT::SUCCESS; }
			}
			// ����ȊO
			else {
				MYRESULT result = _pbrTextures[texName.first]->CreateTextureFromWIC(graphicsEngine, texturePath);
				if (result == MYRESULT::FAILED) { return MYRESULT::SUCCESS; }
			}
		}
		// �w�肳��Ă��Ȃ��ꍇ
		else {
			_pbrTextures[texName.first] = &_dummyTexture;
		}
	}

	return MYRESULT::SUCCESS;
}

MYRESULT PBRMaterial::Commit()
{
	Dx12GraphicsEngine& graphicsEngine = Dx12GraphicsEngine::Instance();

	// �_�~�[�e�N�X�`������
	MYRESULT result = CreateDammyTexture(graphicsEngine);
	if (result == MYRESULT::FAILED) { return MYRESULT::FAILED; }

	// PBR�e�N�X�`���ǂݍ���
	result = LoadPBRTextures(graphicsEngine);
	if (result == MYRESULT::FAILED) { return MYRESULT::FAILED; }

	return MYRESULT::SUCCESS;
}

void PBRMaterial::ApplyMaterial(FBXMesh& mesh)
{
	Dx12GraphicsEngine& graphicsEngine = Dx12GraphicsEngine::Instance();

	// ���b�V���Ƀe�N�X�`���Z�b�g
	mesh.SetTexture(
		graphicsEngine.Device(), *_pbrTextures[PBRTEXTURETYPE::BASE_COLOR],
		static_cast<int>(PBRTEXTURETYPE::BASE_COLOR));
	mesh.SetTexture(
		graphicsEngine.Device(), *_pbrTextures[PBRTEXTURETYPE::METALLIC],
		static_cast<int>(PBRTEXTURETYPE::METALLIC));
	mesh.SetTexture(
		graphicsEngine.Device(), *_pbrTextures[PBRTEXTURETYPE::ROUGHNESS],
		static_cast<int>(PBRTEXTURETYPE::ROUGHNESS));
	mesh.SetTexture(
		graphicsEngine.Device(), *_pbrTextures[PBRTEXTURETYPE::NORMAL],
		static_cast<int>(PBRTEXTURETYPE::NORMAL));
	mesh.SetTexture(
		graphicsEngine.Device(), *_pbrTextures[PBRTEXTURETYPE::OCCULUSION],
		static_cast<int>(PBRTEXTURETYPE::OCCULUSION));
}

void PBRMaterial::SetTextureFolderPath(std::wstring folderPath)
{
	_textureFolderPath = folderPath;
}

void PBRMaterial::SetTextureName(PBRTEXTURETYPE type, std::wstring textureName)
{
	if (type == PBRTEXTURETYPE::TYPE_NUM) {
		MessageBox(NULL, L"������TEXTURETYPE�ł��B", L"�G���[���b�Z�[�W", MB_OK);
		return;
	}
	_pbrTextureNames[type] = textureName;
}