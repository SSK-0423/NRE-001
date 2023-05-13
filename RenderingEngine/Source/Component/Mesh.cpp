#include "Mesh.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "ShaderResourceViewDesc.h"
#include "GLBLoader.h"

using namespace NamelessEngine::Utility;

namespace NamelessEngine::Component
{
	SubMesh::SubMesh()
	{
	}

	SubMesh::~SubMesh()
	{
	}

	Utility::RESULT SubMesh::CreateVertexBuffer(ID3D12Device& device, MeshData& data)
	{
		if (_vertexBuffer != nullptr) { delete _vertexBuffer; }
		_vertexBuffer = new DX12API::VertexBuffer();

		return _vertexBuffer->Create(
			device, &data.vertices[0], SizeofVector(data.vertices), sizeof(MeshVertex));
	}

	Utility::RESULT SubMesh::CreateIndexBuffer(ID3D12Device& device, MeshData& data)
	{
		if (_indexBuffer != nullptr) { delete _indexBuffer; }
		_indexBuffer = new DX12API::IndexBuffer();

		return _indexBuffer->Create(device, data.indices);
	}

	Utility::RESULT SubMesh::CreateDescriptorHeap(ID3D12Device& device)
	{
		if (_descriptorHeap != nullptr) { delete _descriptorHeap; }

		_descriptorHeap = new DX12API::DescriptorHeapCBV_SRV_UAV();

		return _descriptorHeap->Create(device);
	}

	Utility::RESULT SubMesh::Create(ID3D12Device& device, MeshData& data)
	{
		RESULT result = CreateVertexBuffer(device, data);
		if (result == RESULT::FAILED) { return result; }

		result = CreateIndexBuffer(device, data);
		if (result == RESULT::FAILED) { return result; }

		result = CreateDescriptorHeap(device);
		if (result == RESULT::FAILED) { return result; }

		_materialIndex = data.materialIndex;

		// マテリアルセット


		return RESULT::SUCCESS;
	}

	void SubMesh::SetConstantBuffer(ID3D12Device& device, DX12API::ConstantBuffer& constantBuffer, const int& registerNo)
	{
		_descriptorHeap->RegistConstantBuffer(device, constantBuffer, registerNo);
	}

	void SubMesh::SetTexture(ID3D12Device& device, DX12API::Texture& texture, DX12API::ShaderResourceViewDesc desc, const int& registerNo)
	{
		_descriptorHeap->RegistShaderResource(device, texture, desc, registerNo);
	}


	Mesh::Mesh()
	{
	}
	Mesh::~Mesh()
	{
	}

	RESULT Mesh::Create(ID3D12Device& device, MeshData& data)
	{
		return Utility::RESULT::SUCCESS;
	}

	Utility::RESULT Mesh::CreateSphere(ID3D12Device& device, unsigned int stackNum, unsigned int sectorNum, float radius)
	{
		const float PI = DirectX::XM_PI;

		MeshData meshData;

		float lengthInv = 1.f / radius;

		float sectorStep = 2.f * PI / static_cast<float>(sectorNum);
		float stackStep = PI / static_cast<float>(stackNum);

		float sectorAngle = 0.f;
		float stackAngle = 0.f;

		meshData.vertices.resize((static_cast<size_t>(stackNum) + 1) * (static_cast<size_t>(sectorNum) + 1));

		// k1--k1+1
		// |  / |
		// | /  |
		// k2--k2+1

		// 頂点データ生成
		// スタック数：縦をいくつの面で構成するか
		// 縦の頂点数：上端と下端があるのでスタック数+1になる セクターも一緒
		for (size_t stack = 0; stack <= stackNum; stack++) {
			stackAngle = PI / 2 - PI * stackStep * stack;
			float xz = radius * cosf(stackAngle);
			float y = radius * sinf(stackAngle);

			for (size_t sector = 0; sector <= sectorNum; sector++) {
				sectorAngle = sector * sectorStep;
				float x = xz * cosf(sectorAngle);
				float z = xz * sinf(sectorAngle);

				MeshVertex vertex;
				vertex.position = DirectX::XMFLOAT3(x, y, z);

				// 法線計算
				// 頂点位置は球の中心からの位置ベクトルなので法線方向と同じと考えることができる
				// また、ベクトルの長さは球の半径になるので半径で割ることで正規化できる
				float nx = x * lengthInv;
				float ny = y * lengthInv;
				float nz = z * lengthInv;
				vertex.normal = DirectX::XMFLOAT3(nx, ny, nz);

				// UV計算
				// 球を切り開いて長方形にすることをイメージするとわかりやすい
				float u = static_cast<float>(sector) / sectorNum;
				float v = static_cast<float>(stack) / stackNum;
				vertex.uv = DirectX::XMFLOAT2(u, v);

				meshData.vertices[stack * (static_cast<size_t>(sectorNum) + 1) + sector] = vertex;
			}
		}
		unsigned int k1 = 0;
		unsigned int k2 = 0;

		// 頂点インデックスデータ生成
		for (size_t stack = 0; stack < stackNum; stack++) {
			k1 = stack * (static_cast<size_t>(sectorNum) + 1);	// 現在のスタック
			k2 = k1 + sectorNum + 1;							// 次のスタック

			for (size_t sector = 0; sector < sectorNum; sector++, k1++, k2++) {
				// セクターごとに2つの三角形が必要
				// k1 => k2 => k1+1 下端も
				if (stack != 0) {
					meshData.indices.push_back(k1);
					meshData.indices.push_back(k2);
					meshData.indices.push_back(k1 + 1);
				}

				// k1+1 => k2 => k2+1 上端も
				if (stack != (stackNum - 1)) {
					meshData.indices.push_back(k1 + 1);
					meshData.indices.push_back(k2);
					meshData.indices.push_back(k2 + 1);
				}
			}
		}

		SubMesh submesh;
		submesh.Create(device, meshData);
		_submeshes.push_back(submesh);

		return Utility::RESULT::SUCCESS;
	}

	Utility::RESULT Mesh::CreateCube(ID3D12Device& device)
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

		SubMesh submesh;
		submesh.Create(device, meshData);
		_submeshes.push_back(submesh);

		return Utility::RESULT::SUCCESS;
	}

	Utility::RESULT Mesh::CreateFromGLB(ID3D12Device& device, const std::string& path)
	{
		if (Graphics::GLBLoader::LoadModel(device, path, _submeshes, _materials) == Utility::RESULT::FAILED)
			return Utility::RESULT::FAILED;

		// メッシュとマテリアルを結びつける
		for (auto& submesh : _submeshes) {
			if (submesh._materialIndex >= 0) {
				Material& material = _materials[submesh._materialIndex];
				if (material.baseColorTexture != nullptr)
					submesh.SetTexture(device, *material.baseColorTexture, DX12API::ShaderResourceViewDesc(*material.baseColorTexture), 0);

				if (material.metalRoughTexture != nullptr)
					submesh.SetTexture(device, *material.metalRoughTexture, DX12API::ShaderResourceViewDesc(*material.metalRoughTexture), 1);

				if (material.normalTexture != nullptr)
					submesh.SetTexture(device, *material.normalTexture, DX12API::ShaderResourceViewDesc(*material.normalTexture), 2);

				if (material.occlusionTexture != nullptr)
					submesh.SetTexture(device, *material.occlusionTexture, DX12API::ShaderResourceViewDesc(*material.occlusionTexture), 3);

				if (material.emissiveTexture != nullptr)
					submesh.SetTexture(device, *material.emissiveTexture, DX12API::ShaderResourceViewDesc(*material.emissiveTexture), 4);
			}
		}

		return Utility::RESULT::SUCCESS;
	}

	void Mesh::Update(float deltatime)
	{
	}

	void Mesh::Draw(DX12API::RenderingContext& renderContext)
	{
		for (auto& submesh : _submeshes) {
			renderContext.SetDescriptorHeap(*submesh._descriptorHeap);
			renderContext.SetVertexBuffer(0, *submesh._vertexBuffer);
			renderContext.SetIndexBuffer(*submesh._indexBuffer);
			renderContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			renderContext.DrawIndexedInstanced(submesh._indexBuffer->GetIndexNum(), 1);
		}
	}
	void Mesh::SetConstantBufferOnAllSubMeshes(ID3D12Device& device, DX12API::ConstantBuffer& constantBuffer, const int& registerNo)
	{
		for (auto& submesh : _submeshes) {
			submesh._descriptorHeap->RegistConstantBuffer(device, constantBuffer, registerNo);
		}
	}
	void Mesh::SetTextureOnAllSubMeshes(ID3D12Device& device, DX12API::Texture& texture, DX12API::ShaderResourceViewDesc desc, const int& registerNo)
	{
		for (auto& submesh : _submeshes) {
			submesh._descriptorHeap->RegistShaderResource(device, texture, desc, registerNo);
		}
	}
}
