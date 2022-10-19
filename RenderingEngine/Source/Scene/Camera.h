#pragma once
#include <DirectXMath.h>
#include "ConstantBuffer.h"

namespace NamelessEngine::Scene {
	class Camera {
	public:
		Camera();
		~Camera();
	private:
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

	public:
		void Update(float deltaTime);
		DX12API::ConstantBuffer& GetConstantBuffer();
	};
}