#include "Scene.h"

namespace NamelessEngine::Scene
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::Update(float deltaTime)
	{
		for (auto& actor : actors) {
			actor->Update(deltaTime);
		}
	}

	void Scene::Draw()
	{
		for (auto& actor : actors) {
			actor->Draw();
		}
	}
}
