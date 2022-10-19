#include "Scene.h"

using namespace NamelessEngine::Graphics;

namespace NamelessEngine::Scene
{
	Scene::Scene(IRenderer* renderer)
		: _renderer(renderer), _meshActors(), _guiActors()
	{
	}

	Scene::~Scene()
	{
		Utility::SafetyDelete<IRenderer>(_renderer);
		for (auto actor : _meshActors) {
			Utility::SafetyDelete<Actor>(actor);
		}
		_meshActors.clear();
		_meshActors.shrink_to_fit();
	}
}
