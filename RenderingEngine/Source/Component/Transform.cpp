#include "Transform.h"
#include "Dx12GraphicsEngine.h"

using namespace DirectX;

using namespace NamelessEngine::Core;
using namespace NamelessEngine::DX12API;
using namespace NamelessEngine::Utility;

const DirectX::XMFLOAT3 NamelessEngine::Component::Transform::FORWARD(0.f, 0.f, 1.f);
const DirectX::XMFLOAT3 NamelessEngine::Component::Transform::RIGHT(1.f, 0.f, 0.f);
const DirectX::XMFLOAT3 NamelessEngine::Component::Transform::UP(0.f, 1.f, 0.f);

namespace NamelessEngine::Component
{

	Transform::Transform()
		: _position(0.f, 0.f, 0.f), _eularAngle(0.f, 0.f, 0.f), _scale(1.f, 1.f, 1.f),
		_forward(FORWARD), _right(RIGHT), _up(UP)
	{
		RESULT result = _buffer.Create(
			Dx12GraphicsEngine::Instance().Device(), (void*)&_bufferData, sizeof(TransformCBuff));
	}
	Transform::~Transform()
	{
	}
	void Transform::Update(float deltaTime)
	{
		// フォワード/ライト/アップベクトル計算
		XMStoreFloat3(&_forward, XMVector3Transform(XMLoadFloat3(&FORWARD), GetRotationXYZMatrix()));
		XMStoreFloat3(&_forward, XMVector3Transform(XMLoadFloat3(&RIGHT), GetRotationXYZMatrix()));
		XMStoreFloat3(&_forward, XMVector3Transform(XMLoadFloat3(&UP), GetRotationXYZMatrix()));

		_bufferData.world = GetTransformMatrix();
		_buffer.UpdateData(&_bufferData);
	}
	void Transform::Draw(DX12API::RenderingContext& renderContext)
	{
	}

	DirectX::XMMATRIX Transform::GetTransformMatrix()
	{
		XMMATRIX translation = XMMatrixTranslation(_position.x, _position.y, _position.z);
		XMMATRIX scaling = XMMatrixScaling(_scale.x, _scale.y, _scale.z);
		XMMATRIX rotation = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&_eularAngle));
		XMMATRIX matrix = rotation * translation * scaling;

		return matrix;
	}
	DirectX::XMMATRIX Transform::GetTranslationMatrix()
	{
		return XMMatrixTranslation(_position.x, _position.y, _position.z);
	}
	DirectX::XMMATRIX Transform::GetRotationXYZMatrix()
	{
		return XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&_eularAngle));
	}
	DirectX::XMMATRIX Transform::GetScallingMatrix()
	{
		return XMMatrixScaling(_scale.x, _scale.y, _scale.z);
	}
	void Transform::SetPosition(float x, float y, float z)
	{
		_position = XMFLOAT3(x, y, z);
	}
	void Transform::SetEularAngle(float x, float y, float z)
	{
		_eularAngle = XMFLOAT3(x, y, z);
	}
	void Transform::SetScalling(float x, float y, float z)
	{
		_scale = XMFLOAT3(x, y, z);
	}
	void Transform::Translation(float x, float y, float z)
	{
		XMFLOAT3 move(x, y, z);
		XMStoreFloat3(&_position, XMVectorAdd(XMLoadFloat3(&_position), XMLoadFloat3(&move)));
	}
	void Transform::Rotation(float x, float y, float z)
	{
		XMFLOAT3 move(x, y, z);
		XMStoreFloat3(&_eularAngle, XMVectorAdd(XMLoadFloat3(&_eularAngle), XMLoadFloat3(&move)));
	}
	void Transform::Scalling(float x, float y, float z)
	{

		XMFLOAT3 move(x, y, z);
		XMStoreFloat3(&_scale, XMVectorAdd(XMLoadFloat3(&_scale), XMLoadFloat3(&move)));
	}
	void Transform::MoveForward(float distance)
	{
		XMFLOAT3 move = XMFLOAT3(_forward.x * distance, _forward.y * distance, _forward.z * distance);

		XMStoreFloat3(&_position, XMVectorAdd(XMLoadFloat3(&move), XMLoadFloat3(&_position)));
	}
	void Transform::MoveRight(float distance)
	{
		XMFLOAT3 move = XMFLOAT3(_right.x * distance, _right.y * distance, _right.z * distance);

		XMStoreFloat3(&_position, XMVectorAdd(XMLoadFloat3(&move), XMLoadFloat3(&_position)));
	}
	void Transform::MoveUp(float distance)
	{
		XMFLOAT3 move = XMFLOAT3(_up.x * distance, _up.y * distance, _up.z * distance);

		XMStoreFloat3(&_position, XMVectorAdd(XMLoadFloat3(&move), XMLoadFloat3(&_position)));
	}
	DirectX::XMFLOAT3 Transform::Position()
	{
		return _position;
	}
	DirectX::XMFLOAT3 Transform::EularAngle()
	{
		return _eularAngle;
	}
	DirectX::XMFLOAT3 Transform::Scale()
	{
		return _scale;
	}
	DirectX::XMFLOAT3 Transform::Forward()
	{
		return _forward;
	}
	DirectX::XMFLOAT3 Transform::Right()
	{
		return _right;
	}
	DirectX::XMFLOAT3 Transform::Up()
	{
		return _up;
	}
	DX12API::ConstantBuffer& Transform::GetConstantBuffer()
	{
		return _buffer;
	}
}
