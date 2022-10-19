#include "Camera.h"
#include "EngineUtility.h"
#include "Dx12GraphicsEngine.h"

using namespace NamelessEngine::Core;

namespace NamelessEngine::Scene
{
	Scene::Camera::Camera()
		: _eyePos(0.f, 0.f, 0.f), _upDir(0.f, 1.f, 0.f), _eyeDir(0.f, 0.f, 1.f),
		_fov(DirectX::XM_PIDIV2), _aspect(1.f), _near(0.1f), _far(100.f)
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

	DirectX::XMMATRIX NamelessEngine::Scene::Camera::ProjectionMatrix()
	{
		return DirectX::XMMatrixPerspectiveFovLH(_fov, _aspect, _near, _far);
	}

	DirectX::XMMATRIX NamelessEngine::Scene::Camera::ViewMatrix()
	{
		DirectX::XMVECTOR eye = DirectX::XMVectorSet(_eyePos.x, _eyePos.y, _eyePos.z, 0.f);
		DirectX::XMVECTOR up = DirectX::XMVectorSet(_upDir.x, _upDir.y, _upDir.z, 0.f);
		DirectX::XMVECTOR dir = DirectX::XMVectorSet(_eyeDir.x, _eyeDir.y, _eyeDir.z, 0.f);
		return DirectX::XMMatrixLookToLH(eye, up, dir);
	}

	void Camera::Update(float deltaTime)
	{
		_bufferData.eyePosition = _eyePos;
		_bufferData.projection = ProjectionMatrix();
		_bufferData.view = ViewMatrix();
		_buffer.UpdateData(&_bufferData);
	}

	DX12API::ConstantBuffer& NamelessEngine::Scene::Camera::GetConstantBuffer()
	{
		return _buffer;
	}

}