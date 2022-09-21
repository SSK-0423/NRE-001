#include "SphereMapSample.h"

MYRESULT SphereMapSample::Init(Dx12GraphicsEngine& graphicsEngine, AppWindow& window)
{
	MYRESULT result;
	// シェーダー
	Shader vertexShader;
	Shader pixelShader;
	result = vertexShader.Create(L"src/SphereSampleVS.hlsl", "main", "vs_5_0");
	if (result == MYRESULT::FAILED) { return result; }
	result = pixelShader.Create(L"src/SphereSamplePS.hlsl", "main", "ps_5_0");
	if (result == MYRESULT::FAILED) { return result; }

	SphereGeometryData data;
	data.radius = 0.5f;
	data.sectorNum = 48;
	data.stackNum = 96;
	data.vertexShader = vertexShader;
	data.pixelShader = pixelShader;
	data.rootSignatureData = RootSignatureData();
	data.inputLayout.resize(3);
	data.inputLayout[0] = {
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	data.inputLayout[1] = {
			"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	data.inputLayout[2] = {
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };

	result = sphere.Create(graphicsEngine.Device(), data);
	if (result == MYRESULT::FAILED) {
		MessageBox(NULL, L"スフィアの生成に失敗しました。", L"エラーメッセージ", MB_OK);
		return result;
	}

	// ビューポートセット
	_viewport = CD3DX12_VIEWPORT(0.f, 0.f,
		static_cast<FLOAT>(window.GetWindowSize().cx),
		static_cast<FLOAT>(window.GetWindowSize().cy));

	// シザー矩形セット
	_scissorRect = CD3DX12_RECT(0, 0, window.GetWindowSize().cx, window.GetWindowSize().cy);

	return MYRESULT::SUCCESS;
}

void SphereMapSample::Update(float deltaTime)
{
}

void SphereMapSample::Draw(Dx12GraphicsEngine& graphicsEngine)
{
	RenderingContext& renderContext = graphicsEngine.GetRenderingContext();
	
	renderContext.SetViewport(_viewport);
	renderContext.SetScissorRect(_scissorRect);
	sphere.Draw(renderContext);
}

void SphereMapSample::Final()
{
}
