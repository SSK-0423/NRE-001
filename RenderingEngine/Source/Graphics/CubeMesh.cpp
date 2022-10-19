#include "CubeMesh.h"

using namespace NamelessEngine::Component;

namespace NamelessEngine::Graphics
{
	CubeMesh::CubeMesh()
	{
	}

	CubeMesh::~CubeMesh()
	{
	}

	Component::MeshData CubeMesh::CreateMeshData()
	{
		MeshData meshData;

		std::vector<DirectX::XMFLOAT3> vertices = {
			// positiveX
			DirectX::XMFLOAT3(0.5,-0.5, -0.5),
			DirectX::XMFLOAT3(0.5, 0.5, -0.5),
			DirectX::XMFLOAT3(0.5,-0.5,  0.5),
			DirectX::XMFLOAT3(0.5, 0.5,  0.5),
			// negativeX
			DirectX::XMFLOAT3(-0.5,-0.5,-0.5),
			DirectX::XMFLOAT3(-0.5, 0.5,-0.5),
			DirectX::XMFLOAT3(-0.5,-0.5, 0.5),
			DirectX::XMFLOAT3(-0.5, 0.5, 0.5),

			// positiveY
			DirectX::XMFLOAT3(-0.5, 0.5,-0.5),
			DirectX::XMFLOAT3(0.5, 0.5,-0.5),
			DirectX::XMFLOAT3(-0.5, 0.5, 0.5),
			DirectX::XMFLOAT3(0.5, 0.5, 0.5),
			// negativeY
			DirectX::XMFLOAT3(-0.5,-0.5,-0.5),
			DirectX::XMFLOAT3(0.5,-0.5,-0.5),
			DirectX::XMFLOAT3(-0.5,-0.5, 0.5),
			DirectX::XMFLOAT3(0.5,-0.5, 0.5),

			// positiveZ
			DirectX::XMFLOAT3(-0.5,-0.5, 0.5),
			DirectX::XMFLOAT3(0.5,-0.5, 0.5),
			DirectX::XMFLOAT3(-0.5, 0.5, 0.5),
			DirectX::XMFLOAT3(0.5, 0.5, 0.5),
			// negativeZ
			DirectX::XMFLOAT3(-0.5,-0.5,-0.5),
			DirectX::XMFLOAT3(0.5,-0.5,-0.5),
			DirectX::XMFLOAT3(-0.5, 0.5,-0.5),
			DirectX::XMFLOAT3(0.5, 0.5,-0.5),
		};

		std::vector<DirectX::XMFLOAT3> normals = {
			// positiveX
			DirectX::XMFLOAT3(1,0,0),
			DirectX::XMFLOAT3(1,0,0),
			DirectX::XMFLOAT3(1,0,0),
			DirectX::XMFLOAT3(1,0,0),
			// negativeX
			DirectX::XMFLOAT3(-1,0,0),
			DirectX::XMFLOAT3(-1,0,0),
			DirectX::XMFLOAT3(-1,0,0),
			DirectX::XMFLOAT3(-1,0,0),

			// positiveY
			DirectX::XMFLOAT3(0,1,0),
			DirectX::XMFLOAT3(0,1,0),
			DirectX::XMFLOAT3(0,1,0),
			DirectX::XMFLOAT3(0,1,0),
			// negativeY
			DirectX::XMFLOAT3(0,-1,0),
			DirectX::XMFLOAT3(0,-1,0),
			DirectX::XMFLOAT3(0,-1,0),
			DirectX::XMFLOAT3(0,-1,0),

			// positiveZ
			DirectX::XMFLOAT3(0,0,1),
			DirectX::XMFLOAT3(0,0,1),
			DirectX::XMFLOAT3(0,0,1),
			DirectX::XMFLOAT3(0,0,1),
			// negativeZ
			DirectX::XMFLOAT3(0,0,-1),
			DirectX::XMFLOAT3(0,0,-1),
			DirectX::XMFLOAT3(0,0,-1),
			DirectX::XMFLOAT3(0,0,-1),
		};

		std::vector<DirectX::XMFLOAT2> uv = {
			// positiveX
			DirectX::XMFLOAT2(0, 0),     //左下
			DirectX::XMFLOAT2(0, 1),     //左上
			DirectX::XMFLOAT2(1, 0),     //右下
			DirectX::XMFLOAT2(1, 1),     //右上
			// negativeX
			DirectX::XMFLOAT2(0, 0),     //左下
			DirectX::XMFLOAT2(0, 1),     //左上
			DirectX::XMFLOAT2(1, 0),     //右下
			DirectX::XMFLOAT2(1, 1),     //右上
			// positiveY
			DirectX::XMFLOAT2(0, 0),     //左下
			DirectX::XMFLOAT2(1, 0),     //右下
			DirectX::XMFLOAT2(0, 1),     //左上
			DirectX::XMFLOAT2(1, 1),     //右上
			// negativeY
			DirectX::XMFLOAT2(0, 0),     //左下
			DirectX::XMFLOAT2(1, 0),     //右下
			DirectX::XMFLOAT2(0, 1),     //左上
			DirectX::XMFLOAT2(1, 1),     //右上
			// positiveZ
			DirectX::XMFLOAT2(0, 0),     //左下
			DirectX::XMFLOAT2(1, 0),     //右下
			DirectX::XMFLOAT2(0, 1),     //左上
			DirectX::XMFLOAT2(1, 1),     //右上
			// negativeZ
			DirectX::XMFLOAT2(0, 0),     //左下
			DirectX::XMFLOAT2(1, 0),     //右下
			DirectX::XMFLOAT2(0, 1),     //左上
			DirectX::XMFLOAT2(1, 1)      //右上
		};

		meshData.vertices.resize(vertices.size());
		for (size_t idx = 0; idx < vertices.size(); idx++) {
			meshData.vertices[idx].position = vertices[idx];
			meshData.vertices[idx].normal = normals[idx];
			meshData.vertices[idx].uv = uv[idx];
		}

		std::vector<unsigned int> indices = {
			// positiveX
			0,1,2,
			0,2,3,
			// negativeX
			4,5,7,
			4,7,6,
			// positiveY
			8,10,11,
			8,11,9,
			// negativeY
			12,14,15,
			12,15,13,
			// positiveZ
			16,18,19,
			16,19,17,
			// negativeZ
			20,22,23,
			20,23,21
		};
		meshData.indices = indices;

		return meshData;
	}
}
