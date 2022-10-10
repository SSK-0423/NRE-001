#pragma once

#include "IComponent.h"

namespace NamelessEngine::Component {
	class MeshRenderer : IComponent {
	public:
		MeshRenderer();
		~MeshRenderer();

	private:
		void Update(float deltaTime);
		void Draw();
	};
}