#pragma once
#include "SphereGeometry.h"

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
		SphereGeometry sphere;
		DX12API::Texture texture;
		Utility::MYRESULT CreateTexture(const std::wstring& texturePath);

	public:
		Utility::MYRESULT Create(ID3D12Device& device, SkySphereData& data);

		void Draw(DX12API::RenderingContext& renderContext);
	};
}