#pragma once
#include "Actor.h"
#include <vector>

namespace NamelessEngine::Scene {
	class Scene {
	public:
		Scene();
		~Scene();

	private:
		std::vector<Actor*> actors;

	public:
		void Update(float deltaTime);
		void Draw();
	};
}