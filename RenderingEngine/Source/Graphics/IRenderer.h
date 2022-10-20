#pragma once
#include "Actor.h"
#include <vector>

namespace NamelessEngine::Graphics {
	/// <summary>
	/// メッシュやGUIのレンダリングを行うクラス
	/// </summary>
	class IRenderer {
	public:
		IRenderer() {}
		virtual ~IRenderer() {}
		virtual void Render(std::vector<Actor*>& _meshActors, std::vector<Actor*>& _guiActors) = 0;
	};
}