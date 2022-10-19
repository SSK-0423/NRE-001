#pragma once
#include "Actor.h"
#include "IRenderer.h"
#include "EngineUtility.h"
#include <vector>

namespace NamelessEngine::Scene {
	class Scene {
	public:
		Scene(Graphics::IRenderer* renderer);
		virtual ~Scene();

	protected:
		std::vector<Actor*> _meshActors;
		std::vector<Actor*> _guiActors;
		
		Graphics::IRenderer* _renderer;

	public:
		/// <summary>
		/// シーンの初期化(アクターの追加)などを行う
		/// </summary>
		/// <returns></returns>
		virtual Utility::RESULT Init() = 0;
		virtual void Update(float deltaTime) = 0;
		virtual void Draw() = 0;
		virtual void Final() = 0;
	};
}