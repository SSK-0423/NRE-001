#pragma once
#include "EngineUtility.h"
#include <vector>
#include <memory>

namespace NamelessEngine
{
	class Actor;
}

namespace NamelessEngine::Scene {
	class Camera;

	class Scene {
	public:
		Scene();
		virtual ~Scene();

	protected:
		std::vector<Actor*> _meshActors;
		std::vector<Actor*> _guiActors;
		
		std::unique_ptr<Camera> _camera;
	public:
		Utility::RESULT Init();
		virtual Utility::RESULT ImplInit() = 0;
		virtual void Update(float deltaTime) = 0;
		virtual void Final() = 0;
		std::vector<Actor*>& GetMeshActors();
		std::vector<Actor*>& GetGUIActors();
		Camera& GetCamera();
	};
}