#pragma once

#include "Dx12ApplicationImpl.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RootSignature.h"
#include "Polygon.h"

#include "DescriptorHeapCBV_SRV_UAV.h"
#include "Texture.h"

class TextureMappingSample : public Dx12ApplicationImpl {
public:
	MYRESULT Init(Dx12GraphicsEngine& graphicsEngine, AppWindow& window) override;
	void Update(float deltaTime) override;
	void Draw(Dx12GraphicsEngine& graphicsEngine) override;
	void Final() override;

// �|���S���֘A
private:
	VertexBuffer _vertexBuffer;			// ���_�o�b�t�@�[
	IndexBuffer _indexBuffer;			// �C���f�b�N�X�o�b�t�@�[
	Shader _vertexShader;				// ���_�V�F�[�_�[
	Shader _pixelShader;				// �s�N�Z���V�F�[�_�[
	MyFrameWork::Polygon _square;		// �l�p�`�|���S��

	/// <summary>
	/// �|���S��������
	/// </summary>
	/// <returns></returns>
	MYRESULT InitPolygon(Dx12GraphicsEngine& graphicsEngine, AppWindow& window);	            // �|���S��������

// �e�N�X�`���֘A
private:
	DescriptorHeapCBV_SRV_UAV _textureHeap;		// �e�N�X�`���p�q�[�v
	Texture _texture;							// �e�N�X�`��

	/// <summary>
	/// �e�N�X�`��������
	/// </summary>
	/// <returns></returns>
	MYRESULT InitTexture(Dx12GraphicsEngine& graphicsEngine);

	CD3DX12_VIEWPORT _viewport;			// �r���[�|�[�g
	CD3DX12_RECT _scissorRect;			// �V�U�[��`
};