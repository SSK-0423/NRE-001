#pragma once

#include <DirectXMath.h>

#include "Mesh.h"

namespace NamelessEngine::Graphics
{
	class SphereMesh {
	public:
		SphereMesh();
		~SphereMesh();

		static Component::MeshData CreateMeshData(
			unsigned int stackNum, unsigned int sectorNum, float radius);
	};
}