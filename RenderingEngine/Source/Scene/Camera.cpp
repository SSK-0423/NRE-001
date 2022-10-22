#include "Camera.h"
#include "EngineUtility.h"
#include "Dx12GraphicsEngine.h"

using namespace NamelessEngine::Core;
using namespace DirectX;

namespace NamelessEngine::Scene
{
	Scene::Camera::Camera() :
		_fov(XM_PIDIV2), _aspect(1.f), _near(0.1f), _far(100.f)
	{
		Utility::RESULT result =
			_buffer.Create(Dx12GraphicsEngine::Instance().Device(), &_bufferData, sizeof(CameraCBuff));
		if (result == Utility::RESULT::FAILED) {
			MessageBox(NULL, L"カメラのコンスタントバッファー生成失敗", L"エラーメッセージ", MB_OK);
		}
	}

	Scene::Camera::~Camera()
	{
	}

	XMMATRIX NamelessEngine::Scene::Camera::ProjectionMatrix()
	{
		return XMMatrixPerspectiveFovLH(_fov, _aspect, _near, _far);
	}

	XMMATRIX NamelessEngine::Scene::Camera::ViewMatrix()
	{
		XMFLOAT3 eyePos = _transform.Position();
		XMFLOAT3 eyeDir = _transform.Forward();
		XMFLOAT3 up = _transform.Up();

		return XMMatrixLookToLH(
			XMLoadFloat3(&eyePos), XMLoadFloat3(&eyeDir), XMLoadFloat3(&up));
	}

	void Camera::Update(float deltaTime)
	{
		_bufferData.eyePosition = _transform.Position();
		_bufferData.projection = ProjectionMatrix();
		_bufferData.view = ViewMatrix();
		_buffer.UpdateData(&_bufferData);
	}

	DX12API::ConstantBuffer& NamelessEngine::Scene::Camera::GetConstantBuffer()
	{
		return _buffer;
	}
	Component::Transform& Camera::GetTransform()
	{
		return _transform;
	}
}