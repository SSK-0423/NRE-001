#include "CubeMapSample.h"
#include "ShaderResourceViewDesc.h"

using namespace DirectX;

MYRESULT CubeMapSample::Init(Dx12GraphicsEngine& graphicsEngine, AppWindow& window)
{
	MYRESULT result;
	// シェーダー
	Shader vertexShader;
	Shader pixelShader;
	result = vertexShader.Create(L"src/CubeMapSampleVS.hlsl", "main", "vs_5_0");
	if (result == MYRESULT::FAILED) { return result; }
	result = pixelShader.Create(L"src/CubeMapSamplePS.hlsl", "main", "ps_5_0");
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

	result = _sphere.Create(graphicsEngine.Device(), data);
	if (result == MYRESULT::FAILED) {
		MessageBox(NULL, L"スフィアの生成に失敗しました。", L"エラーメッセージ", MB_OK);
		return result;
	}

	// キューブ生成
	result = vertexShader.Create(L"../RenderingEngine/src/SkyBoxVS.hlsl", "main", "vs_5_0");
	if (result == MYRESULT::FAILED) { return result; }
	result = pixelShader.Create(L"../RenderingEngine/src/SkyBoxPS.hlsl", "main", "ps_5_0");
	if (result == MYRESULT::FAILED) { return result; }

	CubeGeometryData cubeData;
	cubeData.vertexShader = vertexShader;
	cubeData.pixelShader = pixelShader;
	cubeData.rootSignatureData = RootSignatureData(D3D12_FILTER_MIN_MAG_MIP_LINEAR);
	cubeData.inputLayout.resize(3);
	cubeData.inputLayout[0] = {
		"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	cubeData.inputLayout[1] = {
			"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	cubeData.inputLayout[2] = {
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };

	result = _cube.Create(graphicsEngine.Device(), cubeData);
	if (result == MYRESULT::FAILED) {
		MessageBox(NULL, L"キューブの生成に失敗しました。", L"エラーメッセージ", MB_OK);
		return result;
	}

	// ビューポートセット
	_viewport = CD3DX12_VIEWPORT(0.f, 0.f,
		static_cast<FLOAT>(window.GetWindowSize().cx),
		static_cast<FLOAT>(window.GetWindowSize().cy));

	// シザー矩形セット
	_scissorRect = CD3DX12_RECT(0, 0, window.GetWindowSize().cx, window.GetWindowSize().cy);

	result = _cubeTexture.CreateCubeTextureFromDDS(graphicsEngine, L"res/SanFrancisco3/SanFrancisco3_cube.dds");
	//result = _cubeTexture.CreateCubeTextureFromDDS(graphicsEngine, L"res/yokohama_cube.dds");
	if (result == MYRESULT::FAILED) { return result; }

	ShaderResourceViewDesc desc(_cubeTexture, true);
	_sphere.SetTexture(graphicsEngine.Device(), _cubeTexture, desc);
	_cube.SetTexture(graphicsEngine.Device(), _cubeTexture, desc);

	result = SetConstantBuffer(graphicsEngine, window);
	if (result == MYRESULT::FAILED) { return result; }

	return MYRESULT::SUCCESS;
}

void CubeMapSample::Update(float deltaTime)
{
	_angle -= 0.01f;
	//_cbuffData.world = XMMatrixRotationY(_angle);
	//_cbuffData.worldViewProj = XMMatrixIdentity() * _cbuffData.world * _view * _proj;
	//_constantBuffer.UpdateData(&_cbuffData);

	//_cubeCbuffData.world = XMMatrixRotationY(_angle) * XMMatrixScaling(1000,1000,1000);
	//_cubeCbuffData.worldViewProj = XMMatrixIdentity() * _cubeCbuffData.world * _view * _proj;
	//_cubeConstantBuffer.UpdateData(&_cubeCbuffData);
}

void CubeMapSample::Draw(Dx12GraphicsEngine& graphicsEngine)
{
	RenderingContext& renderContext = graphicsEngine.GetRenderingContext();

	renderContext.SetViewport(_viewport);
	renderContext.SetScissorRect(_scissorRect);
	_sphere.Draw(renderContext);
	_cube.Draw(renderContext);
}

void CubeMapSample::Final()
{
}

MYRESULT CubeMapSample::SetConstantBuffer(Dx12GraphicsEngine& graphicsEngine, AppWindow& window)
{
	// ワールド行列
	_angle = 0.f;
	_cbuffData.world = XMMatrixRotationY(_angle);

	// カメラ行列
	XMVECTOR eye = XMVectorSet(0, 1.f, -2.f, 0);
	XMVECTOR target = XMVectorSet(0, 0.f, 0, 1);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	_view = XMMatrixLookAtLH(eye, target, up);
	XMStoreFloat3(&_cbuffData.eye, eye);

	// プロジェクション行列
	_proj = XMMatrixPerspectiveFovLH(
		XM_PIDIV2,
		static_cast<float>(window.GetWindowSize().cx) / static_cast<float>(window.GetWindowSize().cy),
		0.1f,
		1000.f);

	// ワールドビュープロジェクション行列
	_cbuffData.worldViewProj = XMMatrixIdentity() * _cbuffData.world * _view * _proj;

	// コンスタントバッファー生成
	MYRESULT result = _constantBuffer.Create(graphicsEngine.Device(), &_cbuffData, sizeof(ConstBuff));
	if (result == MYRESULT::FAILED) { return MYRESULT::FAILED; }

	_cubeCbuffData = _cbuffData;
	_cubeCbuffData.world = XMMatrixScaling(1000, 1000, 1000);
	_cubeCbuffData.worldViewProj = XMMatrixIdentity() * _cubeCbuffData.world * _view * _proj;

	result = _cubeConstantBuffer.Create(graphicsEngine.Device(), &_cubeCbuffData, sizeof(ConstBuff));
	if (result == MYRESULT::FAILED) { return MYRESULT::FAILED; }

	_cube.SetConstantBuffer(graphicsEngine.Device(), _cubeConstantBuffer);
	_sphere.SetConstantBuffer(graphicsEngine.Device(), _constantBuffer);

	return result;
}
