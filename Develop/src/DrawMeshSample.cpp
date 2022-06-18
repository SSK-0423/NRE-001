#include "DrawMeshSample.h"
#include "Shader.h"

using namespace DirectX;

MYRESULT DrawMeshSample::Init(Dx12GraphicsEngine& graphicsEngine, AppWindow& window)
{
	MYRESULT result;
	// シェーダー
	Shader vertexShader;
	Shader pixelShader;
	result = vertexShader.Create(L"src/MeshVS.hlsl", "main", "vs_5_0");
	if (result == MYRESULT::FAILED) { return result; }
	result = pixelShader.Create(L"src/MeshPS.hlsl", "main", "ps_5_0");
	if (result == MYRESULT::FAILED) { return result; }

	// MeshData用意
	MeshData meshData;
	meshData.vertexShader = vertexShader;
	meshData.pixelShader = pixelShader;
	meshData.rootSignatureData = RootSignatureData();
	meshData.inputLayout.resize(2);
	meshData.inputLayout[0] = {
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	meshData.inputLayout[1] = {
			"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };

	// メッシュ読み込み
	result = mesh.LoadMesh(
		"res/bunny/bunny.fbx", graphicsEngine.Device(), meshData);
	if (result == MYRESULT::FAILED) { return result; }

	result = SetConstantBuffer(graphicsEngine, window);
	if (result == MYRESULT::FAILED) { return result; }


	return MYRESULT::SUCCESS;
}

void DrawMeshSample::Update(float deltaTime)
{
}

void DrawMeshSample::Draw(Dx12GraphicsEngine& graphicsEngine)
{
	// レンダリングコンテキスト取得
	RenderingContext& renderContext = graphicsEngine.GetRenderingContext();

	mesh.Draw(renderContext);
}

void DrawMeshSample::Final()
{
}

MYRESULT DrawMeshSample::SetConstantBuffer(Dx12GraphicsEngine& graphicsEngine, AppWindow& window)
{
	// 変換行列用意
	XMMATRIX worldViewProj = XMMatrixIdentity();

	XMVECTOR eye = XMVectorSet(0, 0, -3.f, 0);
	XMVECTOR target = XMVectorSet(0, 0, 0, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);

	// カメラ行列
	worldViewProj *= XMMatrixLookAtLH(eye, target, up);


	worldViewProj *= XMMatrixPerspectiveFovLH(
		XM_PIDIV2,
		static_cast<float>(window.GetWindowSize().cx) / static_cast<float>(window.GetWindowSize().cy),
		0.1f,
		10.f);

	_meshCBuffData.worldViewProj = worldViewProj;

	MYRESULT result = _meshCBuffer.Create(graphicsEngine.Device(), &_meshCBuffData, sizeof(MeshConstBuff));
	if (result == MYRESULT::FAILED) { return MYRESULT::FAILED; }

	result = _meshHeap.Create(graphicsEngine.Device());
	if (result == MYRESULT::FAILED) { return MYRESULT::FAILED; }

	_meshHeap.RegistConstantBuffer(graphicsEngine.Device(), _meshCBuffer);

	mesh.SetDescriptorHeap(_meshHeap);

	return result;
}
