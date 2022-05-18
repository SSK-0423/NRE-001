#pragma once

#include "Dx12ApplicationImpl.h"

#include "EngineUtility.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RootSignature.h"
#include "Polygon.h"

#include "DepthStencilBuffer.h"
#include "DescriptorHeapDSV.h"
#include "DescriptorHeapCBV_SRV_UAV.h"
#include "ConstantBuffer.h"

#include "RenderTarget.h"
#include "Sprite.h"

#include <DirectXMath.h>

class DepthBufferSample : public Dx12ApplicationImpl
{
	struct NearConstBuff
	{
		DirectX::XMMATRIX worldViewProj;
		DirectX::XMMATRIX rotation;
	};

public:
	MYRESULT Init(Dx12GraphicsEngine& graphicsEngine, AppWindow& window) override;
	void Update(float deltaTime) override;
	void Draw(Dx12GraphicsEngine& graphicsEngine) override;
	void Final() override;

private:
	VertexBuffer _vertexBuffer;			        // ���_�o�b�t�@�[
	IndexBuffer _indexBuffer;			        // �C���f�b�N�X�o�b�t�@�[
	Shader _vertexShader;				        // ���_�V�F�[�_�[
	Shader _pixelShader;				        // �s�N�Z���V�F�[�_�[
	RootSignatureData _rootSignatureData;		// ���[�g�V�O�l�`��
	MyFrameWork::Polygon _nearPolygon;		    // ��O�|���S��
	MyFrameWork::Polygon _farPolygon;		    // ���|���S��
	CD3DX12_VIEWPORT _viewport;			        // �r���[�|�[�g
	CD3DX12_RECT _scissorRect;			        // �V�U�[��`

	float angle = 0.f;
	NearConstBuff _nearCBuffData;
	DescriptorHeapCBV_SRV_UAV _nearHeap;
	ConstantBuffer _nearCBuffer;

	RenderTarget _firstRender;
	Sprite _firstRenderingSprite;

	MYRESULT CreateNearPolygon(Dx12GraphicsEngine& graphicsEngine);
	MYRESULT CreateFarPolygon(Dx12GraphicsEngine& graphicsEngine);
	MYRESULT SetConstantBuffer(Dx12GraphicsEngine& graphicsEngine, AppWindow& window);
};