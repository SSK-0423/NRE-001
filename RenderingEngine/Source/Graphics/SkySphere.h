#pragma once
#include "SphereMesh.h"

namespace NamelessEngine::Graphics
{
	struct SkySphereData {
		unsigned int stackNum;
		unsigned int sectorNum;
		float radius;
		const std::wstring& texturePath;
	};

	class SkySphere {
	public:
		SkySphere();
		~SkySphere();

	private:
		SphereMesh sphere;
		//DX12API::Texture texture;
		Utility::RESULT CreateTexture(const std::wstring& texturePath);

	public:
		Utility::RESULT Create(ID3D12Device& device, SkySphereData& data);

		void Draw(DX12API::RenderingContext& renderContext);
	};
}