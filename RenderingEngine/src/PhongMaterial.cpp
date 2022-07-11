#include "PhongMaterial.h"
#include "FBXMesh.h"

PhongMaterial::PhongMaterial()
	: _textureName(L"")
{
	// PhongMaterialBuff�^�ϐ��̏�����
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

MYRESULT PhongMaterial::CreateMaterialConstantBuffer(ID3D12Device& device)
{
	MYRESULT result = _materialCBuffer.Create(device, &_material, sizeof(PhongMaterialBuff));
	if (result == MYRESULT::FAILED) { return MYRESULT::FAILED; }
	return MYRESULT::SUCCESS;
}

MYRESULT PhongMaterial::CreateTexture(Dx12GraphicsEngine& graphicsEngine)
{
	// �e�N�X�`�����Ȃ��ꍇ
	if (_textureName.empty()) {
		std::vector<ColorRGBA> whiteTexture(_dummyTextureWidth * _dummyTextureHeight);
		for (auto& color : whiteTexture) {
			color.r = 1.f;
			color.g = 1.f;
			color.b = 1.f;
			color.a = 1.f;
		}
		MYRESULT result = _texture.CreateTextureFromRGBAData(
			graphicsEngine, whiteTexture,
			_dummyTextureWidth, _dummyTextureHeight, DXGI_FORMAT_R32G32B32A32_FLOAT);
	}
	// �e�N�X�`��������ꍇ
	else {
		std::wstring texturePath = _textureFolderPath + L"/" + _textureName;
		MYRESULT result = _texture.CreateTextureFromWIC(graphicsEngine, texturePath);
		if (result == MYRESULT::FAILED) { return MYRESULT::FAILED; }
	}
	return MYRESULT::SUCCESS;
}

MYRESULT PhongMaterial::Commit()
{
	Dx12GraphicsEngine& graphicsEngine = Dx12GraphicsEngine::Instance();

	// �R���X�^���g�o�b�t�@�[����
	MYRESULT result = CreateMaterialConstantBuffer(graphicsEngine.Device());
	if (result == MYRESULT::FAILED) { return MYRESULT::FAILED; }

	// �e�N�X�`������
	result = CreateTexture(graphicsEngine);
	if (result == MYRESULT::FAILED) { return MYRESULT::FAILED; }

	return MYRESULT::SUCCESS;
}

void PhongMaterial::ApplyMaterial(FBXMesh& mesh)
{
	Dx12GraphicsEngine& graphicsEngine = Dx12GraphicsEngine::Instance();

	// ���b�V���ɃR���X�^���g�o�b�t�@�[�Z�b�g
	mesh.SetConstantBuffer(graphicsEngine.Device(), _materialCBuffer);
	// ���b�V���Ƀe�N�X�`���Z�b�g
	mesh.SetTexture(graphicsEngine.Device(), _texture);
}