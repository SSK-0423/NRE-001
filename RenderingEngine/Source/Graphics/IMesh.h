#pragma once
#include "Mesh.h"

namespace NamelessEngine::Graphics {
	template <class T>
	struct ICreateMeshArg{

	};

	class IMesh {
	public:
		virtual Component::MeshData CreateMeshData(ICreateMeshArg<T>) = 0;
		// �������b�V���N���X��MeshData����
		// ���̎��Ɉ��������R�ɂ��������\���̂��`���Ă�����p������`�ɂ���
		// �p�������\���́�GetData����
	};
}