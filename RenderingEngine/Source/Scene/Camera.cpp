#include "Camera.h"
#include "EngineUtility.h"
#include "Dx12GraphicsEngine.h"

using namespace NamelessEngine::Core;
using namespace DirectX;

float NamelessEngine::Scene::Camera::_aspect = 1.f;

namespace NamelessEngine::Scene
{
	Scene::Camera::Camera() :
		_fov(XM_PIDIV2), _near(0.1f), _far(1000.f)
	{
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

	Utility::RESULT Camera::Init()
	{
		return _buffer.Create(
			Dx12GraphicsEngine::Instance().Device(), &_bufferData, sizeof(CameraCBuff));

	}

	void Camera::Update(float deltaTime)
	{
		_transform.Update(deltaTime);
		_bufferData.eyePosition = _transform.Position();
		_bufferData.projection = ProjectionMatrix();
		_bufferData.view = ViewMatrix();
		_bufferData.viewProj = _bufferData.view * _bufferData.projection;
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
	void Camera::Resize(float width, float height)
	{
		_aspect = width / height;
	}
}