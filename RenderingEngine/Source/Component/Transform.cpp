#include "Transform.h"
#include "Dx12GraphicsEngine.h"

using namespace DirectX;

using namespace NamelessEngine::Core;
using namespace NamelessEngine::DX12API;
using namespace NamelessEngine::Utility;

namespace NamelessEngine::Component
{
	Transform::Transform()
		: position(0.f, 0.f, 0.f), eularAngle(0.f, 0.f, 0.f), scale(1.f, 1.f, 1.f)
	{
		RESULT result = _buffer.Create(
			Dx12GraphicsEngine::Instance().Device(), (void*)&_bufferData, sizeof(TransformCBuff));
	}
	Transform::~Transform()
	{
	}
	void Transform::Update(float deltaTime)
	{
		_bufferData.world = GetTransformMatrix();
		_buffer.UpdateData(&_bufferData);
	}
	void Transform::Draw(DX12API::RenderingContext& renderContext)
	{
	}

	DirectX::XMMATRIX Transform::GetTransformMatrix()
	{
		XMMATRIX translation = XMMatrixTranslation(position.x, position.y, position.z);
		XMMATRIX scaling = XMMatrixScaling(scale.x, scale.y, scale.z);
		XMMATRIX rotation = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&eularAngle));
		XMMATRIX matrix = rotation * translation * scaling;

		return matrix;
	}
	DirectX::XMMATRIX Transform::GetTranslationMatrix()
	{
		return XMMatrixTranslation(position.x, position.y, position.z);
	}
	DirectX::XMMATRIX Transform::GetRotationXYZMatrix()
	{
		return XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&eularAngle));
	}
	DirectX::XMMATRIX Transform::GetScallingMatrix()
	{
		return XMMatrixScaling(scale.x, scale.y, scale.z);
	}
	DX12API::ConstantBuffer& Transform::GetConstantBuffer()
	{
		return _buffer;
	}
}
