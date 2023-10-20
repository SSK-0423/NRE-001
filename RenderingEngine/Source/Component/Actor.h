#pragma once

#include "IComponent.h"
#include "RenderingContext.h"
#include <vector>

namespace NamelessEngine {
	class Actor {
	public:
		Actor(const std::string& name);
		~Actor();
	private:
		std::vector<Component::IComponent*> _components;
		std::string _actorName;

	public:
		void Update(float deltaTime);
		void Draw(DX12API::RenderingContext& renderContext);
		std::string GetActorName() { return _actorName; }

		template<class T>
		T* GetComponent()
		{
			for (auto component : _components) {
				if (typeid(*component) == typeid(T)) {
					return dynamic_cast<T*>(component);
				}
			}

			return nullptr;
		}
		template<class T, class ...K>
		T* AddComponent(K ...k)
		{
			// 同じコンポーネントがある場合は追加しない
			for (auto component : _components) {
				if (typeid(*component) == typeid(T))
					return nullptr;
			}
			T* component = new T(k...);
			_components.push_back(component);

			return component;
		}
	};
}