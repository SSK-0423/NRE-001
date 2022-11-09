#pragma once
#include "Mesh.h"

namespace NamelessEngine::Graphics {
	template <class T>
	struct ICreateMeshArg{

	};

	class IMesh {
	public:
		virtual Component::MeshData CreateMeshData(ICreateMeshArg<T>) = 0;
		// ○○メッシュクラスでMeshData生成
		// この時に引数を自由にしたい→構造体を定義してそれを継承する形にする
		// 継承した構造体⇒GetDataする
	};
}