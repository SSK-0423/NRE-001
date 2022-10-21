#include "Camera.h"
#include "EngineUtility.h"
#include "Dx12GraphicsEngine.h"

using namespace NamelessEngine::Core;
using namespace DirectX;

namespace NamelessEngine::Scene
{
	Scene::Camera::Camera()
		: _eyePos(0.f, 0.f, 0.f), _upDir(0.f, 1.f, 0.f), _eyeDir(0.f, 0.f, 1.f),
		_fov(XM_PIDIV2), _aspect(1.f), _near(0.1f), _far(100.f),
		_forward(0.f, 0.f, 1.f), _up(0.f, 1.f, 0.f), _right(1.f, 0.f, 0.f)
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
		return XMMatrixLookToLH(
			XMLoadFloat3(&_eyePos), XMLoadFloat3(&_eyeDir), XMLoadFloat3(&_upDir));
	}

	void Camera::Update(float deltaTime)
	{
		XMStoreFloat3(
			&_eyeDir, XMVector3Transform(XMLoadFloat3(&_forward), _transform.GetRotationXYZMatrix()));

		XMFLOAT3 initialPos(0, 0, 0);
		XMStoreFloat3(
			&_eyePos, XMVector3Transform(XMLoadFloat3(&initialPos), _transform.GetTranslationMatrix()));

		_bufferData.eyePosition = _eyePos;
		_bufferData.projection = ProjectionMatrix();
		_bufferData.view = ViewMatrix();
		_buffer.UpdateData(&_bufferData);
	}

	DX12API::ConstantBuffer& NamelessEngine::Scene::Camera::GetConstantBuffer()
	{
		return _buffer;
	}

	void Camera::MoveForward(float distance)
	{
		_transform.position;
		XMFLOAT3 move = XMFLOAT3(_eyeDir.x * distance, _eyeDir.y * distance, _eyeDir.z * distance);
		XMVECTOR translation = XMVectorAdd(XMLoadFloat3(&_transform.position), XMLoadFloat3(&move));
		XMStoreFloat3(&_transform.position, translation);
	}

	void Camera::RotateX(float eularAngle)
	{
		_transform.eularAngle.x += eularAngle;
	}

	void Camera::RotateY(float eularAngle)
	{
		_transform.eularAngle.y += eularAngle;
	}
	void Camera::RotateZ(float eularAngle)
	{
		_transform.eularAngle.z += eularAngle;
	}
	void Camera::SetRotateX(float eularAngle)
	{
		_transform.eularAngle.x = eularAngle;
	}
	void Camera::SetRotateY(float eularAngle)
	{
		_transform.eularAngle.y = eularAngle;
	}
	void Camera::SetRotateZ(float eularAngle)
	{
		_transform.eularAngle.z = eularAngle;
	}
}