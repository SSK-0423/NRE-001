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
			DirectX::XMFLOAT3(0.5,-0.5, -0.5),	//0
			DirectX::XMFLOAT3(0.5, 0.5, -0.5),
			DirectX::XMFLOAT3(0.5,-0.5,  0.5),
			DirectX::XMFLOAT3(0.5, 0.5,  0.5),
			// negativeX
			DirectX::XMFLOAT3(-0.5,-0.5,-0.5),	//4
			DirectX::XMFLOAT3(-0.5, 0.5,-0.5),
			DirectX::XMFLOAT3(-0.5,-0.5, 0.5),
			DirectX::XMFLOAT3(-0.5, 0.5, 0.5),

			// positiveY
			DirectX::XMFLOAT3(-0.5, 0.5,-0.5),	//8
			DirectX::XMFLOAT3(0.5, 0.5,-0.5),
			DirectX::XMFLOAT3(-0.5, 0.5, 0.5),
			DirectX::XMFLOAT3(0.5, 0.5, 0.5),
			// negativeY
			DirectX::XMFLOAT3(-0.5,-0.5,-0.5),	//12
			DirectX::XMFLOAT3(0.5,-0.5,-0.5),
			DirectX::XMFLOAT3(-0.5,-0.5, 0.5),
			DirectX::XMFLOAT3(0.5,-0.5, 0.5),

			// positiveZ
			DirectX::XMFLOAT3(-0.5,-0.5, 0.5),	//16
			DirectX::XMFLOAT3(0.5,-0.5, 0.5),
			DirectX::XMFLOAT3(-0.5, 0.5, 0.5),
			DirectX::XMFLOAT3(0.5, 0.5, 0.5),
			// negativeZ
			DirectX::XMFLOAT3(-0.5,-0.5,-0.5),	//20
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
			1,3,2,
			// negativeX
			5,4,6,
			5,6,7,
			// positiveY
			8,10,11,
			8,11,9,
			// negativeY
			12,13,15,
			15,14,12,
			// positiveZ
			16,17,19,
			19,18,16,
			// negativeZ
			20,22,23,
			20,23,21
		};
		meshData.indices = indices;

		return meshData;
	}
}
