#include "PBRScene.h"
#include "EngineUtility.h"
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
	for (auto actor : _meshActors) {
		Utility::SafetyDelete(actor);
	}
	_meshActors.clear();
	_meshActors.shrink_to_fit();
}

Utility::RESULT PBRScene::ImplInit()
{
	ID3D12Device& device = Core::Dx12GraphicsEngine::Instance().Device();

	//Actor* metalRoughSpheres = new Actor();
	//metalRoughSpheres->AddComponent<Transform>();
	//metalRoughSpheres->GetComponent<Transform>()->SetPosition(0.f, 0.f, 0.f);
	//metalRoughSpheres->GetComponent<Transform>()->SetDegreeAngle(0.f, 180.f, 0.f);
	//metalRoughSpheres->GetComponent<Transform>()->SetScalling(1.f, 1.f, 1.f);

	//metalRoughSpheres->AddComponent<Mesh>()->CreateFromGLB(device, "Assets/MetalRoughSpheres.glb");

	//metalRoughSpheres->GetComponent<Mesh>()->SetConstantBufferOnAllSubMeshes(device, _camera->GetConstantBuffer(), 0);
	//metalRoughSpheres->GetComponent<Mesh>()->SetConstantBufferOnAllSubMeshes(
	//	device, metalRoughSpheres->GetComponent<Transform>()->GetConstantBuffer(), 1);

	//_meshActors.push_back(metalRoughSpheres);

	//Actor* sponza = new Actor();
	//sponza->AddComponent<Transform>();
	//sponza->GetComponent<Transform>()->SetPosition(0.f, 0.f, 0.f);
	//sponza->GetComponent<Transform>()->SetScalling(0.1f, 0.1f, 0.1f);

	//sponza->AddComponent<Mesh>()->CreateFromGLB(device, "Assets/sponza.glb");
	//sponza->GetComponent<Mesh>()->SetConstantBufferOnAllSubMeshes(device, _camera->GetConstantBuffer(), 0);
	//sponza->GetComponent<Mesh>()->SetConstantBufferOnAllSubMeshes(
	//	device, sponza->GetComponent<Transform>()->GetConstantBuffer(), 1);
	//_meshActors.push_back(sponza);


	//Actor* bunny = new Actor();
	//bunny->AddComponent<Transform>();
	//bunny->GetComponent<Transform>()->SetPosition(0.f, 0.f, 0.f);
	//bunny->GetComponent<Transform>()->SetScalling(1.f, 1.f, 1.f);
	//bunny->GetComponent<Transform>()->SetDegreeAngle(0.f, 0.f, 0.f);
	//
	//bunny->AddComponent<Mesh>()->CreateFromGLB(device, "Assets/stanford_bunny_pbr.glb");
	//bunny->GetComponent<Mesh>()->SetConstantBufferOnAllSubMeshes(device, _camera->GetConstantBuffer(), 0);
	//bunny->GetComponent<Mesh>()->SetConstantBufferOnAllSubMeshes(device, bunny->GetComponent<Transform>()->GetConstantBuffer(), 1);

	//_meshActors.push_back(bunny);

	Actor* damagedHelmet = new Actor();
	damagedHelmet->AddComponent<Transform>();
	damagedHelmet->GetComponent<Transform>()->SetPosition(0.f, 5.f, 0.f);
	damagedHelmet->GetComponent<Transform>()->SetScalling(5.f, 5.f, 5.f);
	damagedHelmet->GetComponent<Transform>()->SetDegreeAngle(90.f, -90.f, 0.f);

	damagedHelmet->AddComponent<Mesh>()->CreateFromGLB(device, "Assets/DamagedHelmet.glb");
	damagedHelmet->GetComponent<Mesh>()->SetConstantBufferOnAllSubMeshes(device, _camera->GetConstantBuffer(), 0);
	damagedHelmet->GetComponent<Mesh>()->SetConstantBufferOnAllSubMeshes(
		device, damagedHelmet->GetComponent<Transform>()->GetConstantBuffer(), 1);

	_meshActors.push_back(damagedHelmet);

	Actor* plane = new Actor();
	plane->AddComponent<Transform>();
	plane->GetComponent<Transform>()->SetPosition(0, 0, 0);
	plane->GetComponent<Transform>()->SetScalling(20.f, 1.f, 20.f);
	plane->AddComponent<Mesh>()->CreateFromGLB(device, "Assets/plane.glb");
	plane->GetComponent<Mesh>()->SetConstantBufferOnAllSubMeshes(device, _camera->GetConstantBuffer(), 0);
	plane->GetComponent<Mesh>()->SetConstantBufferOnAllSubMeshes(device, plane->GetComponent<Transform>()->GetConstantBuffer(), 1);

	_meshActors.push_back(plane);

	// シャドウテストシーン

	return NamelessEngine::Utility::RESULT::SUCCESS;
}

void PBRScene::Update(float deltaTime)
{
	float speed = 0.1f;
	Input& input = Input::Instance();
	if (input.GetKeyboradState(DIK_W) == BUTTON_STATE::HOLD) {
		_camera->GetTransform().MoveForward(speed);
	}
	if (input.GetKeyboradState(DIK_S) == BUTTON_STATE::HOLD) {
		_camera->GetTransform().MoveForward(-speed);
	}
	if (input.GetKeyboradState(DIK_A) == BUTTON_STATE::HOLD) {
		_camera->GetTransform().MoveRight(-speed);
	}
	if (input.GetKeyboradState(DIK_D) == BUTTON_STATE::HOLD) {
		_camera->GetTransform().MoveRight(speed);
	}
	if (input.GetKeyboradState(DIK_SPACE) == BUTTON_STATE::HOLD) {
		_camera->GetTransform().MoveUp(speed);
	}
	if (input.GetKeyboradState(DIK_LSHIFT) == BUTTON_STATE::HOLD) {
		_camera->GetTransform().MoveUp(-speed);
	}
	if (input.GetKeyboradState(DIK_UPARROW) == BUTTON_STATE::HOLD) {
		_camera->GetTransform().Rotation(-speed, 0.f, 0.f);
	}
	if (input.GetKeyboradState(DIK_DOWNARROW) == BUTTON_STATE::HOLD) {
		_camera->GetTransform().Rotation(speed, 0.f, 0.f);
	}
	if (input.GetKeyboradState(DIK_RIGHTARROW) == BUTTON_STATE::HOLD) {
		_camera->GetTransform().Rotation(0.f, speed / 2.f, 0.f);
	}
	if (input.GetKeyboradState(DIK_LEFTARROW) == BUTTON_STATE::HOLD) {
		_camera->GetTransform().Rotation(0.f, -speed / 2.f, 0.f);
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
