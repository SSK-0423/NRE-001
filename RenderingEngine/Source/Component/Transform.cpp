#include "Transform.h"
#include "Dx12GraphicsEngine.h"

using namespace DirectX;

using namespace NamelessEngine::Core;
using namespace NamelessEngine::DX12API;

namespace NamelessEngine::Component
{
	Transform::Transform()
		: position(0.f, 0.f, 0.f), rotation(0.f, 0.f, 0.f), scale(0.f, 0.f, 0.f)
	{
	}
	Transform::~Transform()
	{
	}
	void Transform::Update(float deltaTime)
	{
	}
	void Transform::Draw()
	{
	}

	DirectX::XMMATRIX Transform::GetTransformMatrix()
	{
		XMMATRIX translation = XMMatrixTranslation(position.x, position.y, position.z);
		XMMATRIX scaling = XMMatrixScaling(scale.x, scale.y, scale.z);
		XMMATRIX matrix = translation * scaling;

		return matrix;
	}
}
