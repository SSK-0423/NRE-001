#pragma once
#include <vector>
#include "EngineUtility.h"

namespace NamelessEngine::Scene {
	class Scene;
}

namespace NamelessEngine::Graphics {
	/// <summary>
	/// ���b�V����GUI�̃����_�����O���s���N���X
	/// </summary>
	class IRenderer {
	public:
		IRenderer() {}
		virtual ~IRenderer() {}
		virtual Utility::RESULT Init() = 0;
		virtual void Update(float deltatime) = 0;
		virtual void Render(Scene::Scene& scene) = 0;
	};
}