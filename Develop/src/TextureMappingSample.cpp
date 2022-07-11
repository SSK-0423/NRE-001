#include "TextureMappingSample.h"

#include <iostream>

MYRESULT TextureMappingSample::Init(Dx12GraphicsEngine& graphicsEngine, AppWindow& window)
{
	// �|���S��������
	MYRESULT result = InitPolygon(graphicsEngine, window);
	if (result == MYRESULT::FAILED) { return result; }

	// �e�N�X�`��������
	result = InitTexture(graphicsEngine);
	if (result == MYRESULT::FAILED) { return result; }

	// �r���[�|�[�g�Z�b�g
	_viewport = CD3DX12_VIEWPORT(0.f, 0.f,
		static_cast<FLOAT>(window.GetWindowSize().cx),
		static_cast<FLOAT>(window.GetWindowSize().cy));

	// �V�U�[��`�Z�b�g
	_scissorRect = CD3DX12_RECT(0, 0, window.GetWindowSize().cx, window.GetWindowSize().cy);

	return result;
}

void TextureMappingSample::Update(float deltaTime)
{
}

void TextureMappingSample::Draw(Dx12GraphicsEngine& graphicsEngine)
{
	// �����_�����O�R���e�L�X�g�擾
	RenderingContext& renderContext = graphicsEngine.GetRenderingContext();

	renderContext.SetViewport(_viewport);
	renderContext.SetScissorRect(_scissorRect);

	// �|���S���`��
	_square.Draw(renderContext);
}

void TextureMappingSample::Final()
{
}

MYRESULT TextureMappingSample::InitPolygon(Dx12GraphicsEngine& graphicsEngine, AppWindow& window)
{
	// �l�p�`�|���S��
	std::vector<PolygonVertex> squareVertex(4);
	squareVertex[0] = { {-1.f,-1.f,0.f},{0.f,2.f} };
	squareVertex[1] = { {-1.f,1.f,0.f},{0.f,0.f} };
	squareVertex[2] = { {1.f,-1.f,0.f},{2.f,2.f} };
	squareVertex[3] = { {1.f,1.f,0.f},{2.f,0.f} };

	// ���_�o�b�t�@�[����
	MYRESULT result = _vertexBuffer.Create(graphicsEngine.Device(), (void*)&squareVertex[0],
		SizeofVector<PolygonVertex>(squareVertex), sizeof(PolygonVertex));

	// �l�p�`�|���S���̃C���f�b�N�X�f�[�^�p��
	std::vector<UINT> index(6);
	index[0] = 0; index[1] = 1; index[2] = 2;
	index[3] = 2; index[4] = 1; index[5] = 3;

	// �C���f�b�N�X�o�b�t�@�[����
	result = _indexBuffer.Create(graphicsEngine.Device(), index);

	// ���[�g�V�O�l�`������
	RootSignatureData rootSignatureData;
	rootSignatureData._samplerData.addressU = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
	rootSignatureData._samplerData.addressV = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;

	// �V�F�[�_�[
	result = _vertexShader.Create(L"src/TextureMapSampleVertex.hlsl", "main", "vs_5_0");
	result = _pixelShader.Create(L"src/TextureMapSamplePixel.hlsl", "main", "ps_5_0");

	// �|���S������
	PolygonData polygonData;
	polygonData.vertexBuffer = _vertexBuffer;
	polygonData.indexBuffer = _indexBuffer;
	polygonData.vertexShader = _vertexShader;
	polygonData.pixelShader = _pixelShader;
	polygonData.rootSignatureData = rootSignatureData;
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

	result = _square.Create(graphicsEngine.Device(), polygonData);

	return result;
}

MYRESULT TextureMappingSample::InitTexture(Dx12GraphicsEngine& graphicsEngine)
{
	ID3D12Device& device = graphicsEngine.Device();

	std::vector<ColorRGBA> _texData(512 * 512);
	for (auto& color : _texData) {
		color = ColorRGBA(0.f, 1.f, 1.f, 1.f);
	}

	// �e�N�X�`���t�@�C���p�X
	std::wstring path(L"Ramen.JPG");

	// �e�N�X�`������
	//MYRESULT result = _texture.CreateTextureFromWIC(graphicsEngine, path);
	MYRESULT result = _texture.CreateTextureFromRGBAData(graphicsEngine, _texData, 512, 512, DXGI_FORMAT_R32G32B32A32_FLOAT);
	if (result == MYRESULT::FAILED) { return result; }

	// �e�N�X�`���p�q�[�v����
	result = _textureHeap.Create(device);
	if (result == MYRESULT::FAILED) { return result; }

	// �e�N�X�`�����q�[�v�ɓo�^
	_textureHeap.RegistShaderResource(device, _texture);

	// �|���S���̃f�B�X�N���v�^�q�[�v�Z�b�g
	_square.SetDescriptorHeap(_textureHeap);

	return result;
}