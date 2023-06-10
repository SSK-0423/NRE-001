#pragma once

#include "Mesh.h"
#include "Material.h"

namespace tinygltf {
	class Model;
}

namespace NamelessEngine::Graphics
{
	class GLBLoader {
	private:
		GLBLoader();
		~GLBLoader() = default;

		static Utility::RESULT LoadMesh(ID3D12Device& device, tinygltf::Model& model, std::vector<Component::SubMesh>& submeshes);
		static Utility::RESULT LoadMaterial(ID3D12Device& device, tinygltf::Model& model, std::vector<Component::Material>& materials);
		static DirectX::XMFLOAT3 CalcTangent(
			const DirectX::XMFLOAT3& p0, const DirectX::XMFLOAT3& p1, const DirectX::XMFLOAT3& p2,
			const DirectX::XMFLOAT2& uv0, const DirectX::XMFLOAT2& uv1, const DirectX::XMFLOAT2& uv2);
	public:
		static Utility::RESULT LoadModel(
			ID3D12Device& device, const std::string& path,
			std::vector<Component::SubMesh>& submeshes, std::vector<Component::Material>& materials);
	};
}