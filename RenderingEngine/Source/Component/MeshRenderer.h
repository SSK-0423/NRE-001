#pragma once

#include "IComponent.h"
#include "Mesh.h"
#include "Transform.h"

namespace NamelessEngine::Component {
	class MeshRenderer : IComponent {
	public:
		MeshRenderer();
		~MeshRenderer();

		Graphics::Mesh* mesh = nullptr;


	private:
		void Update(float deltaTime);
		void Draw();
	};
}