#pragma once

#include "IComponent.h"
#include <vector>

namespace NamelessEngine {
	class Actor {
	public:
		Actor();
		~Actor();
	private:
		std::vector<Component::IComponent*> _components;
	
	public:
		void Update(float deltaTime);
		void Draw();

		template <class T>
		T* GetComponent();
		template <class T, class ...K>
		void AddComponent(K... k);
	};
}