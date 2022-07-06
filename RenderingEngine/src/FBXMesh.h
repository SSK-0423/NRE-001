#pragma once

#include <array>

#include "FBXLoader.h"

#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RootSignature.h"
#include "GraphicsPipelineState.h"

#include "DescriptorHeapCBV_SRV_UAV.h"
#include "ConstantBuffer.h"
#include "Texture.h"

#include "RenderingContext.h"

#include "EngineUtility.h"

/// <summary>
/// ���b�V�������p�f�[�^
/// </summary>
struct FBXMeshCreateData {
	/// <summary>
	/// PBR�̏ꍇ baseColorName,metallicName,roughnessNam,normalName,occlusionName�̐ݒ肪�K�v
	/// </summary>
	MATERIAL_TYPE materialType;

	const char* modelPath;								// 3D���f���ւ̃t�@�C���p�X

	/// <summary>
	/// 3D���f���̃e�N�X�`��������t�H���_�ւ̃p�X
	/// 3D���f�����Ƀt�H���_�𕪂��ĊǗ����邽�߂��̃p�X���K�v
	/// wstring�Ȃ̂̓e�N�X�`�����[�h�֐��Ή��̂���
	/// </summary>
	std::wstring textureFolderPath;

	/// FBXSDK��PBR�}�e���A����ǂݍ��ޕ��@��������Ȃ�(�Ή����ĂȂ����ۂ�)�̂�
	/// PBR�e�N�X�`���̃t�@�C�����𒼐ڏ����Ă��炤
	std::wstring baseColorName;
	std::wstring metallicName;
	std::wstring roughnessName;
	std::wstring normalName;
	std::wstring occlusionName;

	Shader vertexShader;	                            // ���_�V�F�[�_�[
	Shader pixelShader;									// �s�N�Z���V�F�[�_�[
	RootSignatureData rootSignatureData;				// ���[�g�V�O�l�`��
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;	// ���_���C�A�E�g
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

	size_t GetRenderTargetNum() const;
};

/// <summary>
/// ���b�V���N���X
/// </summary>
class FBXMesh {
public:
	FBXMesh();
	~FBXMesh();

private:

	std::vector<FBXMeshData> _meshDataList;		// ���b�V�����̃��X�g

	VertexBuffer* _vertexBuffer = nullptr;
	MYRESULT CreateVertexBuffer(ID3D12Device& device, FBXMeshData& meshData);

	IndexBuffer* _indexBuffer = nullptr;
	MYRESULT CreateIndexBuffer(ID3D12Device& device, FBXMeshData& meshData);

	GraphicsPipelineState* _graphicsPipelineState = nullptr;
	MYRESULT CreateGraphicsPipelineState(ID3D12Device& device, FBXMeshCreateData& meshData);

	RootSignature* _rootSignature = nullptr;

	std::vector<FBXMesh*> _childs;

	// �f�B�X�N���v�^�q�[�v
	DescriptorHeapCBV_SRV_UAV* _descriptorHeap = nullptr;
	MYRESULT CreateDescriptorHeap(ID3D12Device& device);

	/// <summary>
	/// �}�e���A���Z�b�g
	/// </summary>
	/// <param name="material"></param>
	void SetMaterial(IMaterial& material);

	/// <summary>
	///	�e�q���ʂ̕`�揈��
	/// </summary>
	void CommonDraw(RenderingContext& renderContext);

	/// <summary>
	/// �K�w���b�V���Ή��̕`��(�\��)
	/// </summary>
	/// <param name="renderContext"></param>
	/// <param name="isRootMesh"></param>
	void Draw(RenderingContext& renderContext, bool isRootMesh);

	MYRESULT CreateAsChild(Dx12GraphicsEngine& graphicsEngine, FBXMeshData& meshData);

public:
	MYRESULT LoadFBX(Dx12GraphicsEngine& device, FBXMeshCreateData& meshCreateData);

	void Draw(RenderingContext& renderContext);

	void SetConstantBuffer(ID3D12Device& device, ConstantBuffer& constantBuffer);

	void SetTexture(ID3D12Device& device, Texture& texture);

};