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

// �R���|�[�l���g�̃��X�g�����V���O���g���N���X
class ComponentManager{
private:
	ComponentManager();
public:
	static ComponentManager Instance() {
		static ComponentManager instance;
		return instance;
	}
};