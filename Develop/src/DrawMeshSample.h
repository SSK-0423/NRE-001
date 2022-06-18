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

#include "Mesh.h"

#include <DirectXMath.h>

class DrawMeshSample : public Dx12ApplicationImpl
{
	struct MeshConstBuff
	{
		DirectX::XMMATRIX worldViewProj;
	};

public:
	MYRESULT Init(Dx12GraphicsEngine& graphicsEngine, AppWindow& window) override;
	void Update(float deltaTime) override;
	void Draw(Dx12GraphicsEngine& graphicsEngine) override;
	void Final() override;

private:
	Mesh mesh;

	MeshConstBuff _meshCBuffData;
	ConstantBuffer _meshCBuffer;
	DescriptorHeapCBV_SRV_UAV _meshHeap;
	MYRESULT SetConstantBuffer(Dx12GraphicsEngine& graphicsEngine, AppWindow& window);
};