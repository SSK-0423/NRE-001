#include "PBRScene.h"
#include <iostream>

using namespace NamelessEngine;

PBRScene::PBRScene(unsigned int window_width, unsigned int window_height)
	: Scene(new NamelessEngine::Graphics::PBRRenderer(window_width, window_height))
{
}

PBRScene::~PBRScene()
{
}

NamelessEngine::Utility::RESULT PBRScene::Init()
{
	ID3D12Device& device = Core::Dx12GraphicsEngine::Instance().Device();
	Component::MeshData meshData = Graphics::CubeMesh::CreateMeshData();
	Component::MeshData sphereData = Graphics::SphereMesh::CreateMeshData(100, 100, 0.5);

	Actor* cubeActor = new Actor();
	cubeActor->AddComponent<Component::Transform>();
	cubeActor->AddComponent<Component::Mesh>()->Create(device, meshData);

	cubeActor->GetComponent<Component::Mesh>()->SetConstantBuffer(device, _camera.GetConstantBuffer(), 0);
	cubeActor->GetComponent<Component::Mesh>()->SetConstantBuffer(
		device, cubeActor->GetComponent<Component::Transform>()->GetConstantBuffer(), 1);
	cubeActor->GetComponent<Component::Transform>()->SetPosition(0.f, 0.f, 3.f);
	_meshActors.push_back(cubeActor);

	return NamelessEngine::Utility::RESULT::SUCCESS;
}

void PBRScene::Update(float deltaTime)
{
	//_camera.GetTransform().MoveForward(0.001f);
	//_camera.GetTransform().MoveRight(0.001f);
	//_camera.GetTransform().MoveUp(0.001f);

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
