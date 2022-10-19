#include "Transform.h"
#include "Dx12GraphicsEngine.h"

using namespace DirectX;

using namespace NamelessEngine::Core;
using namespace NamelessEngine::DX12API;
using namespace NamelessEngine::Utility;

namespace NamelessEngine::Component
{
	Transform::Transform()
		: position(0.f, 0.f, 0.f), rotation(0.f, 0.f, 0.f), scale(0.f, 0.f, 0.f)
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
		XMMATRIX rotation = XMMatrixIdentity();
		XMMATRIX matrix = rotation * translation * scaling;

		return matrix;
	}
	DX12API::ConstantBuffer& Transform::GetConstantBuffer()
	{
		return _buffer;
	}
}
