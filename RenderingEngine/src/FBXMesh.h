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
	const char* modelPath;								// 3D���f���ւ̃t�@�C���p�X
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
	
	/// <summary>
	/// �J���[�t�H�[�}�b�g�z��𒲂ׂă����_�[�^�[�Q�b�g����Ԃ�
	/// </summary>
	/// <returns>�����_�[�^�[�Q�b�g��</returns>
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

	std::vector<FBXMeshData> _meshDataList;

	VertexBuffer _vertexBuffer;
	MYRESULT CreateVertexBuffer(ID3D12Device& device, FBXMeshData& meshData);

	IndexBuffer _indexBuffer;
	MYRESULT CreateIndexBuffer(ID3D12Device& device, FBXMeshData& meshData);

	GraphicsPipelineState* _graphicsPipelineState = nullptr;
	MYRESULT CreateGraphicsPipelineState(ID3D12Device& device, FBXMeshCreateData& meshData);

	RootSignature* _rootSignature = nullptr;

	std::vector<FBXMesh*> _childs;

	// �}�e���A���p�R���X�^���g�o�b�t�@�[�\����
	struct MaterialBuff {
		DirectX::XMFLOAT4 ambient;
		DirectX::XMFLOAT4 diffuse;
		DirectX::XMFLOAT4 specular;
		float shiness;
	};
	// �}�e���A��
	MaterialBuff _material;
	void SetMaterial(const FBXMaterial& material);

	// �}�e���A���p�̃R���X�^���g�o�b�t�@�[
	ConstantBuffer _materialConstantBuffer;
	MYRESULT CreateMaterialConsnantBuffer(ID3D12Device& device);
	
	// �f�B�X�N���v�^�q�[�v
	DescriptorHeapCBV_SRV_UAV _descriptorHeap;
	MYRESULT CreateDescriptorHeap(ID3D12Device& device);

	/// <summary>
	/// �q���b�V���𐶐�����ۂ̍\����
	/// </summary>
	struct ChildMeshCreateData {
		FBXMeshData meshData;
	};

	// �e�q���ʂ̕`�揈��
	void CommonDraw(RenderingContext& renderContext);
	// �K�w���b�V���Ή��̕`��
	void Draw(RenderingContext& renderContext, bool isRootMesh);

	/// <summary>
	/// �q���b�V���Ƃ��Đ���
	/// </summary>
	/// <param name="device"></param>
	/// <param name="meshData"></param>
	/// <returns></returns>
	MYRESULT CreateAsChild(ID3D12Device& device, ChildMeshCreateData& meshData);

public:
	/// <summary>
	/// ���b�V���ǂݍ���
	/// </summary>
	/// <param name="modelPath">�t�@�C���p�X</param>
	/// <returns></returns>
	MYRESULT LoadFBX(ID3D12Device& device, FBXMeshCreateData& meshData);

	void Draw(RenderingContext& renderContext);

	void SetConstantBuffer(ID3D12Device& device, ConstantBuffer& constantBuffer);
	
	void SetTexture(ID3D12Device& device, Texture& texture);

};