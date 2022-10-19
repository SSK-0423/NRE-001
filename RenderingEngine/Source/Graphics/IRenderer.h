#pragma once
#include "Actor.h"
#include <vector>

namespace NamelessEngine::Graphics {
	/// <summary>
	/// ���b�V����GUI�̃����_�����O���s���N���X
	/// </summary>
	class IRenderer {
	public:
		virtual void Render(std::vector<Actor*>& _meshActors, std::vector<Actor*>& _guiActors) = 0;
	};
}