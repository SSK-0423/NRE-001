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

	Actor* sponza = new Actor();
	sponza->AddComponent<Transform>();
	sponza->GetComponent<Transform>()->SetPosition(0.f, -5.f, 0.f);
	sponza->GetComponent<Transform>()->SetDegreeAngle(0.f, -90.f, 0.f);
	sponza->GetComponent<Transform>()->SetScalling(0.1f, 0.1f, 0.1f);
	sponza->AddComponent<Mesh>()->CreateFromGLB(device, "Assets/sponza.glb");
	sponza->GetComponent<Mesh>()->SetConstantBufferOnAllSubMeshes(device, _camera->GetConstantBuffer(), 0);
	sponza->GetComponent<Mesh>()->SetConstantBufferOnAllSubMeshes(
		device, sponza->GetComponent<Transform>()->GetConstantBuffer(), 1);
	_meshActors.push_back(sponza);

	Actor* damagedHelmet = new Actor();
	damagedHelmet->AddComponent<Transform>();
	damagedHelmet->GetComponent<Transform>()->SetPosition(0.f, 0.f, 10.f);
	damagedHelmet->GetComponent<Transform>()->SetScalling(5.f, 5.f, 5.f);
	damagedHelmet->GetComponent<Transform>()->SetDegreeAngle(90.f, 90.f, 0.f);

	damagedHelmet->AddComponent<Mesh>()->CreateFromGLB(device, "Assets/DamagedHelmet.glb");
	damagedHelmet->GetComponent<Mesh>()->SetConstantBufferOnAllSubMeshes(device, _camera->GetConstantBuffer(), 0);
	damagedHelmet->GetComponent<Mesh>()->SetConstantBufferOnAllSubMeshes(
		device, damagedHelmet->GetComponent<Transform>()->GetConstantBuffer(), 1);
	_meshActors.push_back(damagedHelmet);

	Actor* sciFiHelmet = new Actor();
	sciFiHelmet->AddComponent<Transform>();
	sciFiHelmet->GetComponent<Transform>()->SetPosition(0.f, 1.f, -10.f);
	sciFiHelmet->GetComponent<Transform>()->SetScalling(5.f, 5.f, 5.f);
	sciFiHelmet->GetComponent<Transform>()->SetDegreeAngle(0.f, 90.f, 0.f);
	sciFiHelmet->AddComponent<Mesh>()->CreateFromGLB(device, "Assets/SciFiHelmet.glb");
	sciFiHelmet->GetComponent<Mesh>()->SetConstantBufferOnAllSubMeshes(device, _camera->GetConstantBuffer(), 0);
	sciFiHelmet->GetComponent<Mesh>()->SetConstantBufferOnAllSubMeshes(
		device, sciFiHelmet->GetComponent<Transform>()->GetConstantBuffer(), 1);
	_meshActors.push_back(sciFiHelmet);

	Actor* goldCoin = new Actor();
	goldCoin->AddComponent<Transform>();
	goldCoin->GetComponent<Transform>()->SetPosition(0.f, 10.f, -30.f);
	goldCoin->GetComponent<Transform>()->SetScalling(5.f, 5.f, 5.f);
	goldCoin->GetComponent<Transform>()->SetDegreeAngle(0.f, 90.f, 0.f);
	goldCoin->AddComponent<Mesh>()->CreateFromGLB(device, "Assets/gold_coin.glb");
	goldCoin->GetComponent<Mesh>()->SetConstantBufferOnAllSubMeshes(device, _camera->GetConstantBuffer(), 0);
	goldCoin->GetComponent<Mesh>()->SetConstantBufferOnAllSubMeshes(
		device, goldCoin->GetComponent<Transform>()->GetConstantBuffer(), 1);
	_meshActors.push_back(goldCoin);

	return NamelessEngine::Utility::RESULT::SUCCESS;
}

void PBRScene::Update(float deltaTime)
{
	float speed = 0.1f;
	float rotateSpeed = 0.025f;
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
		_camera->GetTransform().Rotation(-rotateSpeed, 0.f, 0.f);
	}
	if (input.GetKeyboradState(DIK_DOWNARROW) == BUTTON_STATE::HOLD) {
		_camera->GetTransform().Rotation(rotateSpeed, 0.f, 0.f);
	}
	if (input.GetKeyboradState(DIK_RIGHTARROW) == BUTTON_STATE::HOLD) {
		_camera->GetTransform().Rotation(0.f, rotateSpeed, 0.f);
	}
	if (input.GetKeyboradState(DIK_LEFTARROW) == BUTTON_STATE::HOLD) {
		_camera->GetTransform().Rotation(0.f, -rotateSpeed, 0.f);
	}

	// ƒRƒCƒ“‚Ì‰ñ“]
	auto coin = _meshActors[3];
	coin->GetComponent<Transform>()->Rotation(0.f, 0.01f, 0.f);

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
