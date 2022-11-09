#include "Scene.h"
#include "Actor.h"
#include "Camera.h"

using namespace NamelessEngine::Utility;

namespace NamelessEngine::Scene
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
		for (auto actor : _meshActors) {
			Utility::SafetyDelete<Actor>(actor);
		}
		_meshActors.clear();
		_meshActors.shrink_to_fit();

		for (auto actor : _guiActors) {
			Utility::SafetyDelete<Actor>(actor);
		}
		_guiActors.clear();
		_guiActors.shrink_to_fit();
	}
	Utility::RESULT Scene::Init()
	{
		_camera = std::make_unique<Camera>();
		RESULT result = _camera->Init();
		if (result == RESULT::FAILED) { return result; }

		return ImplInit();
	}
	std::vector<Actor*>& Scene::GetMeshActors()
	{
		return _meshActors;
	}
	std::vector<Actor*>& Scene::GetGUIActors()
	{
		return _guiActors;
	}
	Camera& Scene::GetCamera()
	{
		return *_camera;
	}
}
