#include "PBRScene.h"
#include <iostream>

using namespace NamelessEngine;
using namespace NamelessEngine::Core;
using namespace NamelessEngine::Component;

PBRScene::PBRScene()
	: Scene()
{
}

PBRScene::~PBRScene()
{
}

Utility::RESULT PBRScene::ImplInit()
{
	ID3D12Device& device = Core::Dx12GraphicsEngine::Instance().Device();

	float sphereRadius = 0.25f;
	unsigned int stackNum = 16;
	unsigned int sectorNum = 32;
	MeshData sphereData = Graphics::SphereMesh::CreateMeshData(100, 100, sphereRadius);

	// ラフネス0.2刻み メタリック0.5刻み
	float roughnessStep = 0.2f;
	float metallicStep = 0.2f;
	int width = 1.f / roughnessStep + 1.f;
	int height = 1.f / metallicStep + 1.f;
	_meshActors.resize(static_cast<size_t>(width) * static_cast<size_t>(height));

	// 縦 Metallic 横 Roughness
	for (size_t h = 0; h < height; h++) {
		for (size_t w = 0; w < width; w++) {
			_meshActors[width * h + w] = new Actor();

			// トランスフォーム
			_meshActors[width * h + w]->AddComponent<Transform>();
			_meshActors[width * h + w]->GetComponent<Transform>()->SetPosition(
				2.2f * sphereRadius * static_cast<float>(w), -2.2f * sphereRadius * static_cast<float>(h), 5.f);
			_meshActors[width * h + w]->GetComponent<Transform>()->SetScalling(sphereRadius, sphereRadius, sphereRadius);

			// マテリアル
			_meshActors[width * h + w]->AddComponent<Material>();
			_meshActors[width * h + w]->GetComponent<Material>()->baseColor = DirectX::XMFLOAT4(0.69f, 0.69f, 0.69f, 1.f);
			_meshActors[width * h + w]->GetComponent<Material>()->roughness = roughnessStep * static_cast<float>(w);
			_meshActors[width * h + w]->GetComponent<Material>()->metallic = metallicStep * static_cast<float>(h);
			_meshActors[width * h + w]->GetComponent<Material>()->Build();

			// メッシュ
			_meshActors[width * h + w]->AddComponent<Mesh>()->CreateFromGLB(device, "Assets/sphere.glb");

			// リソース登録
			_meshActors[width * h + w]->GetComponent<Mesh>()->SetConstantBuffer(device, _camera->GetConstantBuffer(), 0);
			_meshActors[width * h + w]->GetComponent<Mesh>()->SetConstantBuffer(
				device, _meshActors[width * h + w]->GetComponent<Transform>()->GetConstantBuffer(), 1);
			_meshActors[width * h + w]->GetComponent<Mesh>()->SetConstantBuffer(
				device, _meshActors[width * h + w]->GetComponent<Material>()->GetConstantBuffer(), 2);

			_meshActors[width * h + w]->GetComponent<Mesh>()->SetTexture(
				device, *_meshActors[width * h + w]->GetComponent<Material>()->baseColorTexture, DX12API::ShaderResourceViewDesc(*_meshActors[width * h + w]->GetComponent<Material>()->baseColorTexture),0);
			_meshActors[width * h + w]->GetComponent<Mesh>()->SetTexture(
				device, *_meshActors[width * h + w]->GetComponent<Material>()->metallicTexture, DX12API::ShaderResourceViewDesc(*_meshActors[width * h + w]->GetComponent<Material>()->metallicTexture),1);
			_meshActors[width * h + w]->GetComponent<Mesh>()->SetTexture(
				device, *_meshActors[width * h + w]->GetComponent<Material>()->roughnessTexture, DX12API::ShaderResourceViewDesc(*_meshActors[width * h + w]->GetComponent<Material>()->roughnessTexture),2);
		}
	}

	Actor* glbActor = new Actor();
	glbActor->AddComponent<Material>();
	glbActor->GetComponent<Material>()->baseColor = DirectX::XMFLOAT4(0.f, 1.f, 0.f, 1.f);
	glbActor->GetComponent<Material>()->roughness = 0.1f;
	glbActor->GetComponent<Material>()->metallic = 0.5f;
	glbActor->GetComponent<Material>()->Build();

	glbActor->AddComponent<Mesh>()->CreateFromGLB(device, "Assets/sphere.glb");

	glbActor->AddComponent<Transform>();
	glbActor->GetComponent<Transform>()->SetPosition(0.f, 0.f, 3.f);
	glbActor->GetComponent<Transform>()->SetScalling(0.5, 0.5, 0.5);
	
	Material* material = glbActor->GetComponent<Material>();
	glbActor->GetComponent<Mesh>()->SetTexture(device, *material->baseColorTexture, DX12API::ShaderResourceViewDesc(*material->baseColorTexture), 0);
	glbActor->GetComponent<Mesh>()->SetTexture(device, *material->metallicTexture, DX12API::ShaderResourceViewDesc(*material->metallicTexture), 1);
	glbActor->GetComponent<Mesh>()->SetTexture(device, *material->roughnessTexture, DX12API::ShaderResourceViewDesc(*material->roughnessTexture), 2);

	glbActor->GetComponent<Mesh>()->SetConstantBuffer(device, _camera->GetConstantBuffer(), 0);
	glbActor->GetComponent<Mesh>()->SetConstantBuffer(
		device, glbActor->GetComponent<Transform>()->GetConstantBuffer(), 1);
	glbActor->GetComponent<Mesh>()->SetConstantBuffer(
		device, glbActor->GetComponent<Material>()->GetConstantBuffer(), 2);

	_meshActors.push_back(glbActor);


	return NamelessEngine::Utility::RESULT::SUCCESS;
}

void PBRScene::Update(float deltaTime)
{
	Input& input = Input::Instance();
	if (input.GetKeyboradState(DIK_W) == BUTTON_STATE::HOLD) {
		_camera->GetTransform().MoveForward(0.01f);
	}
	if (input.GetKeyboradState(DIK_S) == BUTTON_STATE::HOLD) {
		_camera->GetTransform().MoveForward(-0.01f);
	}
	if (input.GetKeyboradState(DIK_A) == BUTTON_STATE::HOLD) {
		_camera->GetTransform().MoveRight(-0.01f);
	}
	if (input.GetKeyboradState(DIK_D) == BUTTON_STATE::HOLD) {
		_camera->GetTransform().MoveRight(0.01f);
	}
	if (input.GetKeyboradState(DIK_SPACE) == BUTTON_STATE::HOLD) {
		_camera->GetTransform().MoveUp(0.01f);
	}
	if (input.GetKeyboradState(DIK_LSHIFT) == BUTTON_STATE::HOLD) {
		_camera->GetTransform().MoveUp(-0.01f);
	}
	if (input.GetKeyboradState(DIK_UPARROW) == BUTTON_STATE::HOLD) {
		_camera->GetTransform().Rotation(-0.01f, 0.f, 0.f);
	}
	if (input.GetKeyboradState(DIK_DOWNARROW) == BUTTON_STATE::HOLD) {
		_camera->GetTransform().Rotation(0.01f, 0.f, 0.f);
	}
	if (input.GetKeyboradState(DIK_RIGHTARROW) == BUTTON_STATE::HOLD) {
		_camera->GetTransform().Rotation(0.f, 0.01f, 0.f);
	}
	if (input.GetKeyboradState(DIK_LEFTARROW) == BUTTON_STATE::HOLD) {
		_camera->GetTransform().Rotation(0.f, -0.01f, 0.f);
	}
	//float sensitive = 0.001f;
	//float moveX = input.GetMouseXMovement() * sensitive;
	//float moveY = input.GetMouseYMovement() * sensitive;

	//_camera->GetTransform().Rotation(moveY, moveX, 0);

	_camera->Update(deltaTime);

	if (!_meshActors.empty()) {
		for (auto actor : _meshActors) {
			actor->Update(deltaTime);
		}
	}

	if (!_guiActors.empty()) {
		for (auto actor : _guiActors) {
			actor->Update(deltaTime);
		}
	}
}

void PBRScene::Final()
{
}
