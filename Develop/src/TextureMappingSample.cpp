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

	return result;
}

void TextureMappingSample::Update()
{
}

void TextureMappingSample::Draw(Dx12GraphicsEngine& graphicsEngine)
{
	// �����_�����O�R���e�L�X�g�擾
	RenderingContext& renderContext = graphicsEngine.GetRenderingContext();

	renderContext.SetGraphicsRootSignature(_rootSignature);
	renderContext.SetViewport(_viewport);
	renderContext.SetScissorRect(_scissorRect);

	// �e�N�X�`���p�̐ݒ�
	renderContext.SetDescriptorHeap(_textureHeap.GetDescriptorHeapAddress());
	renderContext.SetGraphicsRootDescriptorTable(0, _textureHeap.GetGPUDescriptorHandleForHeapStartSRV());

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
	squareVertex[0] = { {-1.f,-1.f,0.f},{0.f,1.f} };
	squareVertex[1] = { {-1.f,1.f,0.f},{0.f,0.f} };
	squareVertex[2] = { {1.f,-1.f,0.f},{1.f,1.f} };
	squareVertex[3] = { {1.f,1.f,0.f},{1.f,0.f} };

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
	RootSignatureData data;
	result = _rootSignature.Create(graphicsEngine.Device(), data);

	// �V�F�[�_�[
	result = _vertexShader.Create(L"src/TextureMapSampleVertex.hlsl", "TexMapVS", "vs_5_0");
	result = _pixelShader.Create(L"src/TextureMapSamplePixel.hlsl", "TexMapPS", "ps_5_0");

	// �|���S������
	PolygonData polygonData;
	polygonData.vertexBuffer = _vertexBuffer;
	polygonData.indexBuffer = _indexBuffer;
	polygonData.vertexShader = _vertexShader;
	polygonData.pixelShader = _pixelShader;
	polygonData.rootSignature = _rootSignature;
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

	// �r���[�|�[�g�Z�b�g
	_viewport.TopLeftX = 0.f;
	_viewport.TopLeftY = 0.f;
	_viewport.Width = static_cast<FLOAT>(window.GetWindowSize().cx);
	_viewport.Height = static_cast<FLOAT>(window.GetWindowSize().cy);

	// �V�U�[��`�Z�b�g
	_scissorRect.left = 0;
	_scissorRect.top = 0;
	_scissorRect.right = window.GetWindowSize().cx;
	_scissorRect.bottom = window.GetWindowSize().cy;

	return result;
}

MYRESULT TextureMappingSample::InitTexture(Dx12GraphicsEngine& graphicsEngine)
{
	ID3D12Device& device = graphicsEngine.Device();

	// �e�N�X�`���t�@�C���p�X
	std::wstring path(L"Ramen.JPG");

	// �e�N�X�`������
	MYRESULT result = _texture.CreateTextureFromWIC(graphicsEngine, path);
	if (result == MYRESULT::FAILED) { return result; }
	
	// �e�N�X�`���p�q�[�v����
	result = _textureHeap.Create(device);
	if (result == MYRESULT::FAILED) { return result; }

	// �e�N�X�`�����q�[�v�ɓo�^
	_textureHeap.RegistShaderResource(device, _texture);

	return result;
}