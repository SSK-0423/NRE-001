#include "PBRScene.h"
#include <iostream>

using namespace NamelessEngine;

PBRScene::PBRScene() : Scene(new NamelessEngine::Graphics::PBRRenderer())
{
}

PBRScene::~PBRScene()
{
}

NamelessEngine::Utility::RESULT PBRScene::Init()
{
	ID3D12Device& device = Core::Dx12GraphicsEngine::Instance().Device();
	Component::MeshData meshData = Graphics::CubeMesh::CreateMeshData();

	Actor* cubeActor = new Actor();
	cubeActor->AddComponent<Component::Mesh>()->Create(device, meshData);
	cubeActor->AddComponent<Component::Transform>();

	cubeActor->GetComponent<Component::Mesh>()->SetConstantBuffer(device, _camera.GetConstantBuffer());
	cubeActor->GetComponent<Component::Mesh>()->SetConstantBuffer(
		device, cubeActor->GetComponent<Component::Transform>()->GetConstantBuffer());
	_meshActors.push_back(cubeActor);

	return NamelessEngine::Utility::RESULT::SUCCESS;
}

void PBRScene::Update(float deltaTime)
{
	_camera.Update(deltaTime);

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

void PBRScene::Draw()
{
	_renderer->Render(_meshActors, _guiActors);
}

void PBRScene::Final()
{
}
