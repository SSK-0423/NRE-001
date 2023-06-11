#include "ShadowingPass.h"

#include "Dx12GraphicsEngine.h"

#include "RootSignature.h"
#include "ShaderLibrary.h"
#include "RenderTarget.h"
#include "ConstantBuffer.h"
#include "GraphicsPipelineState.h"

#include "Camera.h"

namespace NamelessEngine::Graphics {
	ShadowingPass::ShadowingPass()
	{
	}
	ShadowingPass::~ShadowingPass()
	{
	}
	Utility::RESULT ShadowingPass::CreateRenderTarget(ID3D12Device& device)
	{
		return Utility::RESULT();
	}
	Utility::RESULT ShadowingPass::CreateRootSignature(ID3D12Device& device)
	{
		return Utility::RESULT();
	}
	Utility::RESULT ShadowingPass::CreateGraphicsPipelineState(ID3D12Device& device)
	{
		return Utility::RESULT();
	}
	Utility::RESULT ShadowingPass::CreateDescriptorHeap(ID3D12Device& device)
	{
		return Utility::RESULT();
	}
	Utility::RESULT ShadowingPass::Init()
	{
		return Utility::RESULT();
	}
	void ShadowingPass::Render()
	{
	}
}
