#include "Actor.h"


namespace NamelessEngine
{
	Actor::Actor()
	{
	}
	Actor::~Actor()
	{
	}

	void Actor::Update(float deltaTime)
	{
		for (auto component : _components) {
			component->Update(deltaTime);
		}
	}

	void Actor::Draw()
	{
		for (auto component : _components) {
			component->Draw();
		}
	}
	
	template<class T>
	T* Actor::GetComponent()
	{
		for (auto component: _components) {
			if (typeid(component) == typeid(T)) {
				return component;
			}
		}

		return nullptr;
	}
	template<class T, class ...K>
	void Actor::AddComponent(K ...k)
	{
		for (auto component : _components) {
			if (typeid(component) == typeid(T))
				return;
		}

		_components.push_back(new T(k...));
	}
}