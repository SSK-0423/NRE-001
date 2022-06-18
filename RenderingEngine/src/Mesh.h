#pragma once

#include <array>

#include "FBXLoader.h"

#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RootSignature.h"
#include "GraphicsPipelineState.h"
#include "DescriptorHeapCBV_SRV_UAV.h"
#include "RenderingContext.h"

#include "EngineUtility.h"
/// <summary>
/// ���b�V�������p�f�[�^
/// </summary>
struct MeshData {
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
class Mesh {
public:
	Mesh();
	~Mesh();

private:
	IMeshLoader* _loader = nullptr;
	FBXLoader* _fbxLoader = nullptr;

	VertexBuffer _vertexBuffer;
	MYRESULT CreateVertexBuffers(ID3D12Device& device);

	IndexBuffer _indexBuffer;
	MYRESULT CreateIndexBuffers(ID3D12Device& device);

	GraphicsPipelineState _graphicsPipelineState;
	MYRESULT CreateGraphicsPipelineState(ID3D12Device& device, MeshData& meshData);

	RootSignature _rootSignature;

	DescriptorHeapCBV_SRV_UAV* _descriptorHeap = nullptr;

public:
	/// <summary>
	/// ���b�V���ǂݍ���
	/// </summary>
	/// <param name="modelPath">�t�@�C���p�X</param>
	/// <returns></returns>
	MYRESULT LoadMesh(const char* modelPath, ID3D12Device& device, MeshData& meshData);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="renderContext"></param>
	void Draw(RenderingContext& renderContext);

	void SetDescriptorHeap(DescriptorHeapCBV_SRV_UAV& descriptorHeap);

};