#pragma once

#include "Mesh.h"
#include "Singleton.h"

namespace NamelessEngine::Graphics
{
	class GLBLoader {
	private:
		GLBLoader();
		~GLBLoader() = default;

	public:
		static Utility::RESULT LoadModel(ID3D12Device& device, const std::string& path, std::vector<Component::SubMesh>& submeshes);
	};
}