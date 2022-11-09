#include "SphereMesh.h"

using namespace NamelessEngine::Component;

namespace NamelessEngine::Graphics
{
	constexpr float PI = DirectX::XM_PI;

	SphereMesh::SphereMesh()
	{
	}

	SphereMesh::~SphereMesh()
	{
	}

	Component::MeshData SphereMesh::CreateMeshData(unsigned int stackNum, unsigned int sectorNum, float radius)
	{
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
		return meshData;
	}
}