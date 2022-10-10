#pragma once

class IComponent {

};

class Transform : IComponent {

};

class Draw : IComponent {

};

class Entity {
	unsigned int _id;

};

// コンポーネントのリストを持つシングルトンクラス
class ComponentManager{
private:
	ComponentManager();
public:
	static ComponentManager Instance() {
		static ComponentManager instance;
		return instance;
	}
};