#include "OffScreenRender.h"
#include "RenderingContext.h"
#include "ShaderResourceViewDesc.h"

MYRESULT OffScreenRender::CreateOffScreenPolygon(
	ID3D12Device& device, const OffScreenRenderData& offScreenRenderData)
{
	// ���_�o�b�t�@�[
	std::vector<PolygonVertex> squareVertex(4);
	squareVertex[0] = { {-1.f,-1.f,0.f}	,{0.f,1.f} };
	squareVertex[1] = { {-1.f,1.f,0.f}	,{0.f,0.f} };
	squareVertex[2] = { {1.f,-1.f,0.f}	,{1.f,1.f} };
	squareVertex[3] = { {1.f,1.f,0.f}	,{1.f,0.f} };

	MYRESULT result = _offscreenPolygonVB.Create(device, (void*)&squareVertex[0],
		SizeofVector<PolygonVertex>(squareVertex), sizeof(PolygonVertex));
	if (result == MYRESULT::FAILED) { return result; }

	// �C���f�b�N�X�o�b�t�@�[
	// TODO: index(6)
	std::vector<UINT> index;
	index.push_back(0); index.push_back(1); index.push_back(2);
	index.push_back(2); index.push_back(1); index.push_back(3);

	result = _offscreenPolygonIB.Create(device, index);
	if (result == MYRESULT::FAILED) { return result; }

	// �V�F�[�_�[
	result = _offscreenVS.Create(offScreenRenderData.vertexShaderData);
	if (result == MYRESULT::FAILED) { return result; }
	result = _offscreenPS.Create(offScreenRenderData.pixelShaderData);
	if (result == MYRESULT::FAILED) { return result; }

	// �I�t�X�N���[���|���S������
	PolygonData polygonData;
	polygonData.vertexBuffer = _offscreenPolygonVB;
	polygonData.indexBuffer = _offscreenPolygonIB;
	polygonData.vertexShader = _offscreenVS;
	polygonData.pixelShader = _offscreenPS;
	//polygonData.rootSignature = offScreenRenderData.rootSignature;
	polygonData.inputLayout.push_back(
		{
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		}
	);
	polygonData.inputLayout.push_back(
		{
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		}
	);

	result = _offscreenPolygon.Create(device, polygonData);
	if (result == MYRESULT::FAILED) { return result; }

	return result;
}

MYRESULT OffScreenRender::Create(ID3D12Device& device, const OffScreenRenderData& offScreenRenderData)
{
	_offScreenRenderData = offScreenRenderData;

	// �I�t�X�N���[�������_�[�^�[�Q�b�g����
	MYRESULT result = _offscreenRender.Create(device, offScreenRenderData.renderTargetData);
	if (result == MYRESULT::FAILED) { return result; }

	// �I�t�X�N���[�������_�[�^�[�Q�b�g�q�[�v����
	result = _offscreenRTVHeap.Create(device);
	if (result == MYRESULT::FAILED) { return result; }

	// �I�t�X�N���[�������_�[�^�[�Q�b�g�r���[����
	_offscreenRTVHeap.RegistDescriptor(device, _offscreenRender);

	// �I�t�X�N���[���e�N�X�`���o�b�t�@�[����
	_offscreenTexture.CreateTextureFromRenderTarget(_offscreenRender);
	if (result == MYRESULT::FAILED) { return result; }

	// �I�t�X�N���[���e�N�X�`���q�[�v����
	result = _offscreenHeap.Create(device);
	if (result == MYRESULT::FAILED) { return result; }

	// �e�N�X�`���Ƃ��ēo�^
	ShaderResourceViewDesc desc(_offscreenTexture);
	_offscreenHeap.RegistShaderResource(device, _offscreenTexture, desc);

	// �I�t�X�N���[���|���S������
	result = CreateOffScreenPolygon(device, offScreenRenderData);
	if (result == MYRESULT::FAILED) { return result; }

	return MYRESULT::SUCCESS;
}

void OffScreenRender::BeginRendering(RenderingContext& renderContext)
{
	// �o���A����
	renderContext.TransitionResourceState(
		_offscreenRender.GetBuffer(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	// �I�t�X�N���[���̃����_�[�^�[�Q�b�g�ݒ�
	auto offRtvHandle = _offscreenRTVHeap.GetCPUDescriptorHandleForHeapStart();
	renderContext.SetRenderTarget(&offRtvHandle, nullptr);

	// ��ʂ��w��F�ŃN���A
	renderContext.ClearRenderTarget(offRtvHandle, _offScreenRenderData.renderTargetData.clearColor, 0, nullptr);

	// ���[�g�V�O�l�`���Z�b�g
	renderContext.SetGraphicsRootSignature(_offScreenRenderData.rootSignature);

	// �r���[�|�[�g�ƃV�U�[��`�Z�b�g
	renderContext.SetViewport(_offScreenRenderData.viewport);
	renderContext.SetScissorRect(_offScreenRenderData.scissorRect);
}

void OffScreenRender::EndRendering(RenderingContext& renderContext)
{
	// ���̃p�X�Ń����_�����O���ʂ��e�N�X�`���Ƃ��ė��p�ł���悤�ɂ���
	renderContext.TransitionResourceState(
		_offscreenRender.GetBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

void OffScreenRender::Draw(RenderingContext& renderContext)
{
	_offscreenPolygon.Draw(renderContext);
}
