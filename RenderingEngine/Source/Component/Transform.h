#pragma once
#include <DirectXMath.h>

#include "IComponent.h"

namespace NamelessEngine::Component
{
	class Transform : public IComponent
	{
	public:
		Transform();
		~Transform();

		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 rotation;
		DirectX::XMFLOAT3 scale;

		void Update(float deltaTime);
		void Draw();

		DirectX::XMMATRIX GetTransformMatrix();
	};
}