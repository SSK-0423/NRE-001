#pragma once

#include <array>

#include "EngineUtility.h"
#include "Polygon.h"
#include "DescriptorHeapCBV_SRV_UAV.h"

class RenderingContext;
class Dx12GraphicsEngine;
class Texture;

// �X�v���C�g�ɐݒ�ł���ő�e�N�X�`����
const int MAX_TEXTURE = 16;

/// <summary>
/// �X�v���C�g�����p�f�[�^
/// </summary>
struct SpriteData {
	std::array<Texture*, MAX_TEXTURE> textures = { nullptr };			        // �e�N�X�`��
	std::array<const std::wstring*, MAX_TEXTURE> texturePaths = { nullptr };	// �e�N�X�`���p�X(���݂�WIC�z��)
	ShaderData vertexShaderData;				                                // ���_�V�F�[�_�[�����f�[�^
	ShaderData pixelShaderData;					                                // �s�N�Z���V�F�[�_�[�����f�[�^
	RootSignatureData rootSignatureData;			                            // ���[�g�V�O�l�`�������f�[�^
	unsigned int width = 0;	                                                    // �X�v���C�g��(�s�N�Z���P��)
	unsigned int height = 0;	                                                // �X�v���C�g��(�s�N�Z���P��)

	std::array<DXGI_FORMAT, D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT> colorFormats = {
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
	};	// �����_�[�^�[�Q�b�g�̃J���[�t�H�[�}�b�g
};

/// <summary>
/// �X�v���C�g�N���X
/// </summary>
class Sprite {
public:
	Sprite() = default;
	~Sprite() = default;

private:
	// �Œ�
	VertexBuffer _vertexBuffer;			    // ���_�o�b�t�@�[
	IndexBuffer _indexBuffer;			    // �C���f�b�N�X�o�b�t�@�[

	Shader _vertexShader;				    // ���_�V�F�[�_�[
	Shader _pixelShader;				    // �s�N�Z���V�F�[�_�[

	MyFrameWork::Polygon _polygon;		    // �|���S��

	DescriptorHeapCBV_SRV_UAV _textureHeap;	// �e�N�X�`���p�q�[�v

	/// <summary>
	/// �|���S��������
	/// </summary>
	/// <param name="device"></param>
	/// <returns></returns>
	MYRESULT CreatePolygon(ID3D12Device& device, SpriteData& spriteData);

	/// <summary>
	/// �e�N�X�`�����\�[�X����
	/// </summary>
	/// <param name="graphicsEngine">�O���t�B�b�N�X�G���W��</param>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="spriteData">�X�v���C�g�f�[�^</param>
	/// <returns></returns>
	MYRESULT CreateTextureResource(
		Dx12GraphicsEngine& graphicsEngine, ID3D12Device& device, SpriteData& spriteData);

public:
	/// <summary>
	/// �X�v���C�g����
	/// </summary>
	/// <param name="device"></param>
	/// <param name="spriteData"></param>
	/// <returns></returns>
	MYRESULT Create(Dx12GraphicsEngine& graphcisEngine, SpriteData& spriteData);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="renderContext"></param>
	void Draw(RenderingContext& renderContext);
};