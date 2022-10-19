#include "SkyBox.h"
#include "ShaderResourceViewDesc.h"

using namespace NamelessEngine::DX12API;
using namespace NamelessEngine::Utility;

namespace NamelessEngine::Graphics
{
	SkyBox::SkyBox()
	{
	}
	SkyBox::~SkyBox()
	{
	}
	Utility::RESULT SkyBox::Create(Core::Dx12GraphicsEngine& graphicsEngine, const std::wstring& texturePass)
	{
		RESULT result;
		// シェーダー
		Shader vertexShader;
		Shader pixelShader;
		result = vertexShader.Create(L"../RenderingEngine/Shaders/skyBoxVS.hlsl", "main", "vs_5_0");
		if (result == RESULT::FAILED) { return result; }
		result = pixelShader.Create(L"../RenderingEngine/Shaders/skyBoxPS.hlsl", "main", "ps_5_0");
		if (result == RESULT::FAILED) { return result; }

		//CubeMeshData cubeData;
		//cubeData.vertexShader = vertexShader;
		//cubeData.pixelShader = pixelShader;
		//cubeData.rootSignatureData = RootSignatureData(D3D12_FILTER_MIN_MAG_MIP_LINEAR);
		//cubeData.inputLayout.resize(3);
		//cubeData.inputLayout[0] = {
		//	"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
		//	D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
		//cubeData.inputLayout[1] = {
		//		"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
		//		D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
		//cubeData.inputLayout[2] = {
		//		"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
		//		D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };

		//result = _cube.Create(graphicsEngine.Device(), cubeData);
		//if (result == RESULT::FAILED) {
		//	MessageBox(NULL, L"キューブの生成に失敗しました。", L"エラーメッセージ", MB_OK);
		//	return result;
		//}

		//result = _cubeTexture.CreateCubeTextureFromDDS(graphicsEngine, texturePass);
		//if (result == RESULT::FAILED) { return result; }

		//ShaderResourceViewDesc desc(_cubeTexture, true);
		//_cube.SetTexture(graphicsEngine.Device(), _cubeTexture, desc);
	}
	void SkyBox::Draw(DX12API::RenderingContext& renderContext)
	{
		//_cube.Draw(renderContext);
	}
}