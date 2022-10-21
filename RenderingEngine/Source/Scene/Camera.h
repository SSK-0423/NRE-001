#pragma once
#include <DirectXMath.h>
#include "ConstantBuffer.h"
#include "Transform.h"

namespace NamelessEngine::Scene {
	class Camera {
	public:
		Camera();
		~Camera();
	private:
		DirectX::XMFLOAT3 _forward;
		DirectX::XMFLOAT3 _right;
		DirectX::XMFLOAT3 _up;

		DirectX::XMFLOAT3 _eyePos;
		DirectX::XMFLOAT3 _upDir;
		DirectX::XMFLOAT3 _eyeDir;
		DirectX::XMMATRIX ViewMatrix();

		float _fov;
		float _aspect;
		float _near;
		float _far;
		DirectX::XMMATRIX ProjectionMatrix();

		struct CameraCBuff {
			DirectX::XMMATRIX view;
			DirectX::XMMATRIX projection;
			DirectX::XMFLOAT3 eyePosition;
		};
		CameraCBuff _bufferData;

		DX12API::ConstantBuffer _buffer;
		Component::Transform _transform;

	public:
		void Update(float deltaTime);
		DX12API::ConstantBuffer& GetConstantBuffer();

		void MoveForward(float distance);
		void RotateX(float eularAngle);
		void RotateY(float eularAngle);
		void RotateZ(float eularAngle);
		void SetRotateX(float eularAngle);
		void SetRotateY(float eularAngle);
		void SetRotateZ(float eularAngle);

	};
}