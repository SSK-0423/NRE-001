#pragma once

#include <array>
#include <DirectXMath.h>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RootSignature.h"
#include "GraphicsPipelineState.h"

#include "Shader.h"

#include "DescriptorHeapCBV_SRV_UAV.h"
#include "ConstantBuffer.h"
#include "Texture.h"


#include "RenderingContext.h"

#include "EngineUtility.h"

struct SphereGeometryData {
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

class SphereGeometry {
public:
	SphereGeometry();

private:
	struct SphereVertex {
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
	};

	std::vector<SphereVertex> _vertices;
	std::vector<unsigned int> _indices;

	void CreateVerticesAndIndicesData();

	VertexBuffer* _vertexBuffer = nullptr;
	MYRESULT CreateVertexBuffer(ID3D12Device& devic);

	IndexBuffer* _indexBuffer = nullptr;
	MYRESULT CreateIndexBuffer(ID3D12Device& device);

	GraphicsPipelineState* _graphicsPipelineState = nullptr;
	MYRESULT CreateGraphicsPipelineState(ID3D12Device& device, SphereGeometryData& data);

	RootSignature* _rootSignature = nullptr;

	DescriptorHeapCBV_SRV_UAV* _descriptorHeap = nullptr;
	MYRESULT CreateDescriptorHeap(ID3D12Device& device);

public:
	MYRESULT Create(ID3D12Device& device, SphereGeometryData& data);

	void Draw(RenderingContext& renderContext);

	void SetConstantBuffer(
		ID3D12Device& device, ConstantBuffer& constantBuffer,
		const int& registerNo = DescriptorHeapCBV_SRV_UAV::_NEXT_REGISTER);

	void SetTexture(
		ID3D12Device& device, Texture& texture,
		const int& registerNo = DescriptorHeapCBV_SRV_UAV::_NEXT_REGISTER);
};