#pragma once

#include <DirectXMath.h>
#include <d3d12.h>

#include "Mesh.h"

namespace NamelessEngine::Graphics
{
	class CubeMesh {
	public:
		CubeMesh();
		~CubeMesh();
		
		static Component::MeshData CreateMeshData();
	};
}