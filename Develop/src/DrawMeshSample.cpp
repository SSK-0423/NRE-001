#include "DrawMeshSample.h"
#include "Shader.h"

using namespace DirectX;

MYRESULT DrawMeshSample::Init(Dx12GraphicsEngine& graphicsEngine, AppWindow& window)
{
	MYRESULT result;
	// �V�F�[�_�[
	Shader vertexShader;
	Shader pixelShader;
	result = vertexShader.Create(L"src/MeshVS.hlsl", "main", "vs_5_0");
	if (result == MYRESULT::FAILED) { return result; }
	result = pixelShader.Create(L"src/MeshPS.hlsl", "main", "ps_5_0");
	if (result == MYRESULT::FAILED) { return result; }

	// FBXMeshData�p��
	FBXMeshCreateData meshData;
	//meshData.modelPath = "res/Renault12TL/Renault12TL.fbx";
	meshData.modelPath = "res/city/city.fbx";
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

	// ���b�V���ǂݍ���
	result = mesh.LoadFBX(graphicsEngine.Device(), meshData);
	if (result == MYRESULT::FAILED) { return result; }
	// ���b�V���ɃR���X�^���g�o�b�t�@�[�Z�b�g
	result = SetConstantBuffer(graphicsEngine, window);
	if (result == MYRESULT::FAILED) { return result; }

	// �r���[�|�[�g�Z�b�g
	_viewport = CD3DX12_VIEWPORT(0.f, 0.f,
		static_cast<FLOAT>(window.GetWindowSize().cx),
		static_cast<FLOAT>(window.GetWindowSize().cy));

	// �V�U�[��`�Z�b�g
	_scissorRect = CD3DX12_RECT(0, 0, window.GetWindowSize().cx, window.GetWindowSize().cy);

	return MYRESULT::SUCCESS;
}

void DrawMeshSample::Update(float deltaTime)
{
	_angle -= 0.01f;
	_meshCBuffData.world =
		XMMatrixScaling(0.1, 0.1, 0.1) *
		XMMatrixRotationY(_angle) *
		XMMatrixTranslation(0.f, -1.f, 0.f);
	_meshCBuffData.worldViewProj =
		XMMatrixIdentity() * _meshCBuffData.world * _view * _proj;
	_meshCBuffer.UpdateData(&_meshCBuffData);
}

void DrawMeshSample::Draw(Dx12GraphicsEngine& graphicsEngine)
{
	// �����_�����O�R���e�L�X�g�擾
	RenderingContext& renderContext = graphicsEngine.GetRenderingContext();

	renderContext.SetViewport(_viewport);
	renderContext.SetScissorRect(_scissorRect);
	mesh.Draw(renderContext);
}

void DrawMeshSample::Final()
{
}

MYRESULT DrawMeshSample::SetConstantBuffer(Dx12GraphicsEngine& graphicsEngine, AppWindow& window)
{
	// ���[���h�s��
	_angle = 0.f;
	_meshCBuffData.world = XMMatrixRotationY(_angle);

	// �J�����s��
	XMVECTOR eye = XMVectorSet(0, 100.f, -70.f, 0);
	XMVECTOR target = XMVectorSet(0, 0.f, 0, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	_view = XMMatrixLookAtLH(eye, target, up);
	XMStoreFloat3(&_meshCBuffData.eye, eye);

	// �v���W�F�N�V�����s��
	_proj = XMMatrixPerspectiveFovLH(
		XM_PIDIV2,
		static_cast<float>(window.GetWindowSize().cx) / static_cast<float>(window.GetWindowSize().cy),
		0.1f,
		1000.f);

	// ���[���h�r���[�v���W�F�N�V�����s��
	_meshCBuffData.worldViewProj = XMMatrixIdentity() * _meshCBuffData.world * _view * _proj;

	// �R���X�^���g�o�b�t�@�[����
	MYRESULT result = _meshCBuffer.Create(graphicsEngine.Device(), &_meshCBuffData, sizeof(MeshConstBuff));
	if (result == MYRESULT::FAILED) { return MYRESULT::FAILED; }

	// ���b�V���p�̃f�B�X�N���v�^�q�[�v����
	result = _meshHeap.Create(graphicsEngine.Device());
	if (result == MYRESULT::FAILED) { return MYRESULT::FAILED; }

	_meshHeap.RegistConstantBuffer(graphicsEngine.Device(), _meshCBuffer);

	mesh.SetDescriptorHeap(_meshHeap);

	return result;
}
