#pragma once
#include <DirectXMath.h>

#include "IComponent.h"
#include "ConstantBuffer.h"

namespace NamelessEngine::Component
{
	class Transform : public IComponent
	{
	public:
		Transform();
		~Transform();
	private:
		struct TransformCBuff {
			DirectX::XMMATRIX world;
		};

		TransformCBuff _bufferData;
		DX12API::ConstantBuffer _buffer;

	public:
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 eularAngle;
		DirectX::XMFLOAT3 scale;

		void Update(float deltaTime);
		void Draw(DX12API::RenderingContext& renderContext);

		DirectX::XMMATRIX GetTransformMatrix();
		DirectX::XMMATRIX GetTranslationMatrix();
		DirectX::XMMATRIX GetRotationXYZMatrix();
		DirectX::XMMATRIX GetScallingMatrix();
		
		DX12API::ConstantBuffer& GetConstantBuffer();
	};
}