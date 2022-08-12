#include "PBRSample.h"
#include "Shader.h"

using namespace DirectX;

MYRESULT PBRSample::Init(Dx12GraphicsEngine& graphicsEngine, AppWindow& window)
{
	MYRESULT result;
	// シェーダー
	Shader vertexShader;
	Shader pixelShader;
	result = vertexShader.Create(L"src/PBRSampleVS.hlsl", "main", "vs_5_0");
	if (result == MYRESULT::FAILED) { return result; }
	result = pixelShader.Create(L"src/PBRSamplePS.hlsl", "main", "ps_5_0");
	if (result == MYRESULT::FAILED) { return result; }

	// FBXMeshData用意
	FBXMeshCreateData meshData;
	meshData.materialType = MATERIAL_TYPE::PBR;

	meshData.modelPath = "res/Renault12TL/Renault12TL.fbx";
	meshData.textureFolderPath= L"res/Renault12TL/Textures";
	meshData.baseColorName = L"Renault12TL_BaseColor.png";
	meshData.metallicName = L"Renault12TL_Metallic.png";
	meshData.roughnessName = L"Renault12TL_Roughness.png";
	meshData.normalName = L"Renault12TL_Normal.png";
	meshData.occlusionName = L"Renault12TL_AO.png";

	meshData.vertexShader = vertexShader;
	meshData.pixelShader = pixelShader;
	meshData.rootSignatureData = RootSignatureData();
	meshData.rootSignatureData._descRangeData.cbvDescriptorNum = 1;
	meshData.rootSignatureData._descRangeData.srvDescriptorNum = 5;
	meshData.inputLayout.resize(3);
	meshData.inputLayout[0] = {
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	meshData.inputLayout[1] = {
			"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	meshData.inputLayout[2] = {
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };

	// メッシュ読み込み
	result = mesh.LoadFBX(graphicsEngine, meshData);
	if (result == MYRESULT::FAILED) { return result; }
	// メッシュにコンスタントバッファーセット
	result = SetConstantBuffer(graphicsEngine, window);
	if (result == MYRESULT::FAILED) { return result; }

	// ビューポートセット
	_viewport = CD3DX12_VIEWPORT(0.f, 0.f,
		static_cast<FLOAT>(window.GetWindowSize().cx),
		static_cast<FLOAT>(window.GetWindowSize().cy));

	// シザー矩形セット
	_scissorRect = CD3DX12_RECT(0, 0, window.GetWindowSize().cx, window.GetWindowSize().cy);

	return MYRESULT::SUCCESS;
}

void PBRSample::Update(float deltaTime)
{
	_angle -= 0.001f;
	_meshCBuffData.world =
		XMMatrixScaling(0.3, 0.3, 0.3) *
		XMMatrixRotationY(_angle);
	//XMMatrixTranslation(0.f, -1.f, 0.f);
	_meshCBuffData.worldViewProj =
		XMMatrixIdentity() * _meshCBuffData.world * _view * _proj;
	_meshCBuffer.UpdateData(&_meshCBuffData);
}

void PBRSample::Draw(Dx12GraphicsEngine& graphicsEngine)
{
	// レンダリングコンテキスト取得
	RenderingContext& renderContext = graphicsEngine.GetRenderingContext();

	renderContext.SetViewport(_viewport);
	renderContext.SetScissorRect(_scissorRect);
	mesh.Draw(renderContext);
}

void PBRSample::Final()
{
}

MYRESULT PBRSample::SetConstantBuffer(Dx12GraphicsEngine& graphicsEngine, AppWindow& window)
{
	// ワールド行列
	_angle = 0.f;
	_meshCBuffData.world = XMMatrixRotationY(_angle);

	// カメラ行列
	XMVECTOR eye = XMVectorSet(0, 100.f, -150.f, 0);
	XMVECTOR target = XMVectorSet(0, 0.f, 0, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	_view = XMMatrixLookAtLH(eye, target, up);
	XMStoreFloat3(&_meshCBuffData.eye, eye);

	// プロジェクション行列
	_proj = XMMatrixPerspectiveFovLH(
		XM_PIDIV2,
		static_cast<float>(window.GetWindowSize().cx) / static_cast<float>(window.GetWindowSize().cy),
		0.1f,
		1000.f);

	// ワールドビュープロジェクション行列
	_meshCBuffData.worldViewProj = XMMatrixIdentity() * _meshCBuffData.world * _view * _proj;

	// コンスタントバッファー生成
	MYRESULT result = _meshCBuffer.Create(graphicsEngine.Device(), &_meshCBuffData, sizeof(MeshConstBuff));
	if (result == MYRESULT::FAILED) { return MYRESULT::FAILED; }

	// メッシュ用のディスクリプタヒープ生成
	result = _meshHeap.Create(graphicsEngine.Device());
	if (result == MYRESULT::FAILED) { return MYRESULT::FAILED; }

	mesh.SetConstantBuffer(graphicsEngine.Device(), _meshCBuffer);

	return result;
}
