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

		static float _aspect;
		DirectX::XMMATRIX ProjectionMatrix();

		struct CameraCBuff {
			DirectX::XMMATRIX view;
			DirectX::XMMATRIX projection;
			DirectX::XMMATRIX viewProj;
			DirectX::XMFLOAT3 eyePosition;
		};
		CameraCBuff _bufferData;

		DX12API::ConstantBuffer _buffer;
		Component::Transform _transform;

	public:
		float cameraNear;
		float cameraFar;
		float cameraFov;

		Utility::RESULT Init();
		void Update(float deltaTime);
		DX12API::ConstantBuffer& GetConstantBuffer();
		Component::Transform& GetTransform();
		static void Resize(float width, float height);
	};
}