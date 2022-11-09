#pragma once
#include <DirectXMath.h>

#include "IComponent.h"
#include "ConstantBuffer.h"

namespace NamelessEngine::Component
{
	class Transform : public IComponent
	{
		static const DirectX::XMFLOAT3 FORWARD;
		static const DirectX::XMFLOAT3 RIGHT;
		static const DirectX::XMFLOAT3 UP;

	public:
		Transform();
		~Transform();
	private:
		struct TransformCBuff {
			DirectX::XMMATRIX world;
		};

		TransformCBuff _bufferData;
		DX12API::ConstantBuffer _buffer;

		DirectX::XMFLOAT3 _forward;
		DirectX::XMFLOAT3 _right;
		DirectX::XMFLOAT3 _up;

		DirectX::XMFLOAT3 _position;
		DirectX::XMFLOAT3 _eularAngle;
		DirectX::XMFLOAT3 _scale;

	public:
		void Update(float deltaTime);
		void Draw(DX12API::RenderingContext& renderContext);

		DirectX::XMMATRIX GetTransformMatrix();
		DirectX::XMMATRIX GetTranslationMatrix();
		DirectX::XMMATRIX GetRotationXYZMatrix();
		DirectX::XMMATRIX GetScallingMatrix();

		void SetPosition(float x, float y, float z);
		void SetEularAngle(float x, float y, float z);
		void SetScalling(float x, float y, float z);

		void Translation(float x, float y, float z);
		void Rotation(float x, float y, float z);
		void Scalling(float x, float y, float z);

		void MoveForward(float distance);
		void MoveRight(float distance);
		void MoveUp(float distance);

		DirectX::XMFLOAT3 Position();
		DirectX::XMFLOAT3 EularAngle();
		DirectX::XMFLOAT3 Scale();

		DirectX::XMFLOAT3 Forward();
		DirectX::XMFLOAT3 Right();
		DirectX::XMFLOAT3 Up();

		DX12API::ConstantBuffer& GetConstantBuffer();
	};
}