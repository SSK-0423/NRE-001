#include "PBRScene.h"
#include "Input.h"
#include "Camera.h"
#include "GLTFLoader.h"
#include "tiny_gltf.h"
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

	float sphereRadius = 0.5f;
	unsigned int stackNum = 16;
	unsigned int sectorNum = 32;
	MeshData sphereData = Graphics::SphereMesh::CreateMeshData(100, 100, sphereRadius);

	// ラフネス0.2刻み メタリック0.5刻み
	float roughnessStep = 0.2f;
	float metallicStep = 0.5f;
	int width = 1.f / roughnessStep + 1.f;
	int height = 1.f / metallicStep + 1.f;
	_meshActors.resize(static_cast<size_t>(width) * static_cast<size_t>(height));

	for (size_t h = 0; h < height; h++) {
		for (size_t w = 0; w < width; w++) {
			_meshActors[width * h + w] = new Actor();
			// コンポーネント追加
			_meshActors[width * h + w]->AddComponent<Transform>();
			_meshActors[width * h + w]->AddComponent<Mesh>()->Create(device, sphereData);
			_meshActors[width * h + w]->AddComponent<Material>();
			// リソースセット
			_meshActors[width * h + w]->GetComponent<Mesh>()->SetConstantBuffer(device, _camera->GetConstantBuffer(), 0);
			_meshActors[width * h + w]->GetComponent<Mesh>()->SetConstantBuffer(
				device, _meshActors[width * h + w]->GetComponent<Transform>()->GetConstantBuffer(), 1);
			_meshActors[width * h + w]->GetComponent<Mesh>()->SetConstantBuffer(
				device, _meshActors[width * h + w]->GetComponent<Material>()->GetConstantBuffer(), 2);
			// マテリアルセット
			_meshActors[width * h + w]->GetComponent<Material>()->SetBaseColor(0.69f, 0.69f, 0.69f);
			_meshActors[width * h + w]->GetComponent<Material>()->SetRoughness(
				roughnessStep * static_cast<float>(w));
			_meshActors[width * h + w]->GetComponent<Material>()->SetMetallic(
				metallicStep * static_cast<float>(h));
			_meshActors[width * h + w]->GetComponent<Transform>()->SetPosition(
				2.2f * sphereRadius * static_cast<float>(w), -2.2f * sphereRadius * static_cast<float>(h), 5.f);
		}
	}

	//Actor* actor = new Actor();
	//actor->AddComponent<Transform>();
	//actor->AddComponent<Mesh>()->Create(device, sphereData);
	//actor->AddComponent<Material>();

	//actor->GetComponent<Mesh>()->SetConstantBuffer(device, _camera->GetConstantBuffer(), 0);
	//actor->GetComponent<Mesh>()->SetConstantBuffer(
	//	device, actor->GetComponent<Transform>()->GetConstantBuffer(), 1);
	//actor->GetComponent<Mesh>()->SetConstantBuffer(
	//	device, actor->GetComponent<Material>()->GetConstantBuffer(), 2);

	//actor->GetComponent<Material>()->SetBaseColor(0.f, 1.f, 0.f);
	//actor->GetComponent<Material>()->SetRoughness(0.1f);
	//actor->GetComponent<Material>()->SetMetallic(0.5f);
	//actor->GetComponent<Transform>()->SetPosition(0.f, 0.f, 3.f);

	//_meshActors.push_back(actor);

	Graphics::GLTFLoader::LoadModel("res/cube.glb");

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
