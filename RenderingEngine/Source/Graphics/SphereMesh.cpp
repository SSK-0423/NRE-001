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

		// ���_�f�[�^����
		// �X�^�b�N���F�c�������̖ʂō\�����邩
		// �c�̒��_���F��[�Ɖ��[������̂ŃX�^�b�N��+1�ɂȂ� �Z�N�^�[���ꏏ
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

				// �@���v�Z
				// ���_�ʒu�͋��̒��S����̈ʒu�x�N�g���Ȃ̂Ŗ@�������Ɠ����ƍl���邱�Ƃ��ł���
				// �܂��A�x�N�g���̒����͋��̔��a�ɂȂ�̂Ŕ��a�Ŋ��邱�ƂŐ��K���ł���
				float nx = x * lengthInv;
				float ny = y * lengthInv;
				float nz = z * lengthInv;
				vertex.normal = DirectX::XMFLOAT3(nx, ny, nz);

				// UV�v�Z
				// ����؂�J���Ē����`�ɂ��邱�Ƃ��C���[�W����Ƃ킩��₷��
				float u = static_cast<float>(sector) / sectorNum;
				float v = static_cast<float>(stack) / stackNum;
				vertex.uv = DirectX::XMFLOAT2(u, v);

				meshData.vertices[stack * (static_cast<size_t>(sectorNum) + 1) + sector] = vertex;
			}
		}
		unsigned int k1 = 0;
		unsigned int k2 = 0;

		// ���_�C���f�b�N�X�f�[�^����
		for (size_t stack = 0; stack < stackNum; stack++) {
			k1 = stack * (static_cast<size_t>(sectorNum) + 1);	// ���݂̃X�^�b�N
			k2 = k1 + sectorNum + 1;							// ���̃X�^�b�N

			for (size_t sector = 0; sector < sectorNum; sector++, k1++, k2++) {
				// �Z�N�^�[���Ƃ�2�̎O�p�`���K�v
				// k1 => k2 => k1+1 ���[��
				if (stack != 0) {
					meshData.indices.push_back(k1);
					meshData.indices.push_back(k2);
					meshData.indices.push_back(k1 + 1);
				}

				// k1+1 => k2 => k2+1 ��[��
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