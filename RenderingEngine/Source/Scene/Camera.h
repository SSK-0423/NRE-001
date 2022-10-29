#pragma once
#include <DirectXMath.h>
#include "ConstantBuffer.h"
#include "Transform.h"
#include "EngineUtility.h"

namespace NamelessEngine::Scene {
	class Camera {
	public:
		Camera();
		~Camera();
	private:
		DirectX::XMMATRIX ViewMatrix();

		float _fov;
		static float _aspect;
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
		Utility::RESULT Init();
		void Update(float deltaTime);
		DX12API::ConstantBuffer& GetConstantBuffer();
		Component::Transform& GetTransform();
		static void Resize(float width, float height);
	};
}