#pragma once
#include "Dx12ApplicationImpl.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RootSignature.h"
#include "Polygon.h"

#include "RenderTarget.h"
#include "Sprite.h"

class MultiPassRendering : public Dx12ApplicationImpl {
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


private:
	RenderTarget _renderTarget;			// �����_�[�^�[�Q�b�g
	CD3DX12_VIEWPORT _viewport;			// �r���[�|�[�g
	CD3DX12_RECT _scissorRect;			// �V�U�[��`

	Sprite _sprite;						// �X�v���C�g
	Sprite _myself;						// 1�p�X�ڂɕ`�悷��X�v���C�g(�����_�[�^�[�Q�b�g2��)

	Texture _texture;					// �P�p�X�ڂɕ`�悷��e�N�X�`��
};