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

#include "FBXMesh.h"

#include <DirectXMath.h>

class PBRSample : public Dx12ApplicationImpl
{
	struct MeshConstBuff
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX worldViewProj;
		DirectX::XMFLOAT3 eye;
	};

public:
	MYRESULT Init(Dx12GraphicsEngine& graphicsEngine, AppWindow& window) override;
	void Update(float deltaTime) override;
	void Draw(Dx12GraphicsEngine& graphicsEngine) override;
	void Final() override;

private:
	FBXMesh mesh;

	MeshConstBuff _meshCBuffData;
	ConstantBuffer _meshCBuffer;
	DescriptorHeapCBV_SRV_UAV _meshHeap;

	CD3DX12_VIEWPORT _viewport;			        // ビューポート
	CD3DX12_RECT _scissorRect;			        // シザー矩形

	float _angle;

	DirectX::XMMATRIX _view;
	DirectX::XMMATRIX _proj;

	MYRESULT SetConstantBuffer(Dx12GraphicsEngine& graphicsEngine, AppWindow& window);
};