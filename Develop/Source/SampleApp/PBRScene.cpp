#include "PBRScene.h"
#include "Input.h"
#include "Camera.h"
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
	_camera->GetTransform().SetPosition(0, 1.f, 2.f);

	ID3D12Device& device = Core::Dx12GraphicsEngine::Instance().Device();
	MeshData meshData = Graphics::CubeMesh::CreateMeshData();
	MeshData sphereData = Graphics::SphereMesh::CreateMeshData(100, 100, 0.5);

	Actor* cubeActor = new Actor();
	cubeActor->AddComponent<Transform>();
	cubeActor->AddComponent<Mesh>()->Create(device, sphereData);
	cubeActor->AddComponent<Material>();

	cubeActor->GetComponent<Mesh>()->SetConstantBuffer(device, _camera->GetConstantBuffer(), 0);
	cubeActor->GetComponent<Mesh>()->SetConstantBuffer(
		device, cubeActor->GetComponent<Transform>()->GetConstantBuffer(), 1);
	cubeActor->GetComponent<Mesh>()->SetConstantBuffer(
		device, cubeActor->GetComponent<Material>()->GetConstantBuffer(), 2);

	cubeActor->GetComponent<Material>()->SetBaseColor(0.f, 1.f, 0.f);
	cubeActor->GetComponent<Material>()->SetRoughness(0.1f);
	cubeActor->GetComponent<Material>()->SetMetallic(0.5f);
	cubeActor->GetComponent<Material>()->SetUseReflection(true);
	cubeActor->GetComponent<Transform>()->SetPosition(0.f, 0.f, 3.f);

	_meshActors.push_back(cubeActor);

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

	float sensitive = 0.001f;
	float moveX = input.GetMouseXMovement() * sensitive;
	float moveY = input.GetMouseYMovement() * sensitive;

	_camera->GetTransform().Rotation(moveY, moveX, 0);

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
