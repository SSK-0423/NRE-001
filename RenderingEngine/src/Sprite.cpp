#include "Sprite.h"

#include "Dx12GraphicsEngine.h"
#include "RenderingContext.h"
#include "Texture.h"
#include "ShaderResourceViewDesc.h"

MYRESULT Sprite::CreatePolygon(ID3D12Device& device, SpriteData& spriteData)
{
	// �l�p�`�|���S��
	std::vector<PolygonVertex> squareVertex(4);
	squareVertex[0] = { {-1.f,-1.f,0.f},{0.f,1.f} };
	squareVertex[1] = { {-1.f,1.f,0.f},{0.f,0.f} };
	squareVertex[2] = { {1.f,-1.f,0.f},{1.f,1.f} };
	squareVertex[3] = { {1.f,1.f,0.f},{1.f,0.f} };

	// ���_�o�b�t�@�[����
	MYRESULT result = _vertexBuffer.Create(device, (void*)&squareVertex[0],
		SizeofVector<PolygonVertex>(squareVertex), sizeof(PolygonVertex));

	// �l�p�`�|���S���̃C���f�b�N�X�f�[�^�p��
	std::vector<UINT> index(6);
	index[0] = 0; index[1] = 1; index[2] = 2;
	index[3] = 2; index[4] = 1; index[5] = 3;

	// �C���f�b�N�X�o�b�t�@�[����
	result = _indexBuffer.Create(device, index);

	// �V�F�[�_�[
	result = _vertexShader.Create(spriteData.vertexShaderData);
	if (result == MYRESULT::FAILED) { return result; }
	result = _pixelShader.Create(spriteData.pixelShaderData);
	if (result == MYRESULT::FAILED) { return result; }

	// �|���S������
	PolygonData polygonData;
	polygonData.vertexBuffer = _vertexBuffer;
	polygonData.indexBuffer = _indexBuffer;
	polygonData.vertexShader = _vertexShader;
	polygonData.pixelShader = _pixelShader;
	polygonData.colorFormats = spriteData.colorFormats;
	polygonData.rootSignatureData = spriteData.rootSignatureData;
	polygonData.inputLayout.push_back
	(
		{
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		}
	);
	polygonData.inputLayout.push_back
	(
		{
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		}
	);

	result = _polygon.Create(device, polygonData);
	if (result == MYRESULT::FAILED) { return result; }

	return result;
}

MYRESULT Sprite::CreateTextureResource(
	Dx12GraphicsEngine& graphicsEngine, ID3D12Device& device, SpriteData& spriteData)
{
	// �e�N�X�`���p�q�[�v����
	MYRESULT result = _textureHeap.Create(device);
	if (result == MYRESULT::FAILED) { return result; }

	// �e�N�X�`�����\�[�X����
	// �t�@�C���p�X����e�N�X�`���𐶐�����
	if (spriteData.texturePaths[0] != nullptr) {
		for (auto path : spriteData.texturePaths) {
			if (path == nullptr) { break; }
			// �ۗ�
			result = MYRESULT::FAILED;
		}
	}

	// �����̃e�N�X�`�����g�p����
	else if (spriteData.textures[0] != nullptr) {
		for (auto tex : spriteData.textures) {
			if (tex == nullptr) { break; }
			ShaderResourceViewDesc desc(*tex);
			_textureHeap.RegistShaderResource(device, *tex, desc);
		}
		_polygon.SetDescriptorHeap(_textureHeap);
	}

	return result;
}

MYRESULT Sprite::Create(Dx12GraphicsEngine& graphicsEngine, SpriteData& spriteData)
{
	ID3D12Device& device = graphicsEngine.Device();

	// �|���S������
	MYRESULT result = CreatePolygon(device, spriteData);
	if (result == MYRESULT::FAILED) { return result; }

	// �e�N�X�`�����\�[�X����
	result = CreateTextureResource(graphicsEngine, device, spriteData);
	if (result == MYRESULT::FAILED) { return result; }

	return result;
}

void Sprite::Draw(RenderingContext& renderContext)
{
	_polygon.Draw(renderContext);
}
