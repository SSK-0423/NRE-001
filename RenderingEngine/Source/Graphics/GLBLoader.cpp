#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "tiny_gltf.h"

#include "GLBLoader.h"
#include "Texture.h"

#include "../Utility/EngineUtility.h"

using namespace DirectX;

namespace NamelessEngine::Graphics
{

	Utility::RESULT GLBLoader::LoadModel(
		ID3D12Device& device, const std::string& path,
		std::vector<Component::SubMesh>& submeshes, std::vector<Component::Material>& materials)
	{
		tinygltf::TinyGLTF loader;
		tinygltf::Model model;
		std::string err;
		std::string warn;

		std::string extension = Utility::GetExtension(path);
		if (extension != "glb") {
			MessageBox(NULL, L"��Ή��`���̃t�H�[�}�b�g�ł��B(�Ή��`���F.glb)", L"�G���[���b�Z�[�W", MB_OK);
			return Utility::RESULT::FAILED;
		}

		bool res = loader.LoadBinaryFromFile(&model, &err, &warn, path);

		if (!warn.empty()) {
			wchar_t message[128];
			Utility::charToWchar(warn.c_str(), message, warn.length());
			MessageBox(NULL, message, L"�x�����b�Z�[�W", MB_OK);
		}

		if (!err.empty()) {
			wchar_t message[128];
			Utility::charToWchar(err.c_str(), message, err.length());
			MessageBox(NULL, message, L"�G���[���b�Z�[�W", MB_OK);
		}

		if (!res) {
			MessageBox(NULL, L"���f���ǂݍ��݂Ɏ��s���܂����B", L"�G���[���b�Z�[�W", MB_OK);
			return Utility::RESULT::FAILED;
		}

		// ���b�V���ǂݍ���
		if (LoadMesh(device, model, submeshes) == Utility::RESULT::FAILED) {
			MessageBox(NULL, L"���b�V���ǂݍ��݂Ɏ��s���܂����B", L"�G���[���b�Z�[�W", MB_OK);
			return Utility::RESULT::FAILED;
		}

		// �}�e���A���ǂݍ���
		if (LoadMaterial(device, model, materials) == Utility::RESULT::FAILED) {
			MessageBox(NULL, L"�}�e���A���ǂݍ��݂Ɏ��s���܂����B", L"�G���[���b�Z�[�W", MB_OK);
			return Utility::RESULT::FAILED;
		}

		return Utility::RESULT::SUCCESS;
	}

	Utility::RESULT GLBLoader::LoadMesh(ID3D12Device& device, tinygltf::Model& model, std::vector<Component::SubMesh>& submeshes)
	{
		// ���b�V���ǂݍ���
		for (auto& mesh : model.meshes) {
			for (auto& primitive : mesh.primitives) {
				Component::MeshData submeshData;
				// �ʒu�f�[�^
				const tinygltf::Accessor& posAccessor = model.accessors[primitive.attributes["POSITION"]];
				const tinygltf::BufferView& posBufferView = model.bufferViews[posAccessor.bufferView];
				const tinygltf::Buffer& posBuffer = model.buffers[posBufferView.buffer];
				const float* positions = reinterpret_cast<const float*>(&posBuffer.data[posBufferView.byteOffset + posAccessor.byteOffset]);

				submeshData.vertices.resize(posAccessor.count);

				for (size_t i = 0; i < posAccessor.count; i++) {
					// gltf�͉E����W�n(OpenGL)�Œ�`����Ă���̂ŁAX�𔽓]���č�����W�n(DirectX)�ɕϊ�����
					XMFLOAT3 pos = XMFLOAT3(-positions[i * 3 + 0], positions[i * 3 + 1], positions[i * 3 + 2]);
					submeshData.vertices[i].position = pos;
				}

				// �@���f�[�^
				const tinygltf::Accessor& normalAccessor = model.accessors[primitive.attributes["NORMAL"]];
				const tinygltf::BufferView& normalBufferView = model.bufferViews[normalAccessor.bufferView];
				const tinygltf::Buffer& normalBuffer = model.buffers[normalBufferView.buffer];
				const float* normals = reinterpret_cast<const float*>(&normalBuffer.data[normalBufferView.byteOffset + normalAccessor.byteOffset]);

				for (size_t i = 0; i < normalAccessor.count; i++) {
					XMFLOAT3 normal = XMFLOAT3(-normals[i * 3 + 0], normals[i * 3 + 1], normals[i * 3 + 2]);
					submeshData.vertices[i].normal = normal;
				}

				// UV�f�[�^
				const tinygltf::Accessor& uvAccessor = model.accessors[primitive.attributes["TEXCOORD_0"]];
				const tinygltf::BufferView& uvBufferView = model.bufferViews[uvAccessor.bufferView];
				const tinygltf::Buffer& uvBuffer = model.buffers[uvBufferView.buffer];
				const float* uvs = reinterpret_cast<const float*>(&uvBuffer.data[uvBufferView.byteOffset + uvAccessor.byteOffset]);

				for (size_t i = 0; i < uvAccessor.count; i++) {
					XMFLOAT2 uv = XMFLOAT2(uvs[i * 2 + 0], uvs[i * 2 + 1]);
					submeshData.vertices[i].uv = uv;
				}

				// �C���f�b�N�X�f�[�^
				const tinygltf::Accessor& indexAccessor = model.accessors[primitive.indices];
				const tinygltf::BufferView& indexBufferView = model.bufferViews[indexAccessor.bufferView];
				const tinygltf::Buffer& indexBuffer = model.buffers[indexBufferView.buffer];

				if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
					const unsigned short* indi = reinterpret_cast<const unsigned short*>(&indexBuffer.data[indexBufferView.byteOffset + indexAccessor.byteOffset]);
					// glb��OpenGL�n�Ȃ̂Ŕ����v���肪�\�ʂƂȂ�
					// DirectX�͎��v��肪�\�ʂȂ̂ŁA���_�C���f�b�N�X���t���Ŏ擾����K�v������
					submeshData.indices.resize(indexAccessor.count);
					for (size_t i = 0; i < indexAccessor.count; i += 3) {
						submeshData.indices[i] = indi[i + 2];
						submeshData.indices[i + 1] = indi[i + 1];
						submeshData.indices[i + 2] = indi[i];
					}
				}
				else if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
					const unsigned int* indi = reinterpret_cast<const unsigned int*>(&indexBuffer.data[indexBufferView.byteOffset + indexAccessor.byteOffset]);
					// glb��OpenGL�n�Ȃ̂Ŕ����v���肪�\�ʂƂȂ�
					// DirectX�͎��v��肪�\�ʂȂ̂ŁA���_�C���f�b�N�X���t���Ŏ擾����K�v������
					submeshData.indices.resize(indexAccessor.count);
					for (size_t i = 0; i < indexAccessor.count; i += 3) {
						submeshData.indices[i] = indi[i + 2];
						submeshData.indices[i + 1] = indi[i + 1];
						submeshData.indices[i + 2] = indi[i];
					}
				}

				// �ڐ��f�[�^(���݂��Ȃ��ꍇ������)
				// ���݂��Ȃ��ꍇ�͑���Accessor��count�ϐ��̒l���قȂ�
				// TODO: �ꕔ�̐ڐ��x�N�g�����������v�Z����Ȃ��o�O�̏C��
				const tinygltf::Accessor& tangentAccessor = model.accessors[primitive.attributes["TANGENT"]];
				if (tangentAccessor.count != posAccessor.count) {
					for (size_t i = 0; i < posAccessor.count; i++) {
						// �|���S�����_��UV���o
						XMFLOAT3 p0;
						XMFLOAT3 p1;
						XMFLOAT3 p2;
						XMFLOAT2 uv0;
						XMFLOAT2 uv1;
						XMFLOAT2 uv2;

						XMFLOAT3 tangent;
						XMFLOAT3 tangentSum = XMFLOAT3(0.f, 0.f, 0.f);
						int tangentCount = 0;

						// ���_�ڐ��x�N�g���Ȃ̂ŋ��L�ʂ�S�ă��X�g�A�b�v���ĕ��ώ��K�v���肻��
						for (size_t j = 0; j < indexAccessor.count; j++) {

							// �ڐ��x�N�g�������߂������_���܂ރ|���S����������
							if (XMVector3Equal(XMLoadFloat3(&submeshData.vertices[i].position), XMLoadFloat3(&submeshData.vertices[submeshData.indices[j]].position))) {
								size_t index0;
								size_t index1;
								size_t index2;

								switch (submeshData.indices[j] % 3)
								{
								case 0:	// �|���S���̍ŏ��̒��_�������ꍇ
									index0 = submeshData.indices[j - 2];
									index1 = submeshData.indices[j - 1];
									index2 = submeshData.indices[j];
									break;
								case 1:	// �|���S����2�Ԗڂ̒��_�������ꍇ
									if (j > indexAccessor.count - 2) continue;
									index0 = submeshData.indices[j - 1];
									index1 = submeshData.indices[j];
									index2 = submeshData.indices[j + 1];
									break;
								case 2:	// �|���S����3�Ԗڂ̒��_�������ꍇ
									if (j > indexAccessor.count - 3) continue;
									index0 = submeshData.indices[j];
									index1 = submeshData.indices[j + 1];
									index2 = submeshData.indices[j + 2];
									break;
								default:
									break;
								}

								p0 = submeshData.vertices[index0].position;
								p1 = submeshData.vertices[index1].position;
								p2 = submeshData.vertices[index2].position;
								uv0 = submeshData.vertices[index0].uv;
								uv1 = submeshData.vertices[index1].uv;
								uv2 = submeshData.vertices[index2].uv;

								tangent = CalcTangent(p0, p1, p2, uv0, uv1, uv2);
								if (tangent.x == 0 && tangent.y == 0 && tangent.z == 0) continue;

								XMStoreFloat3(&tangentSum, XMVectorAdd(XMLoadFloat3(&tangentSum), XMLoadFloat3(&tangent)));
								tangentCount++;
								break;
							}
						}
						XMStoreFloat3(&tangentSum, XMVector3Normalize(XMLoadFloat3(&tangentSum)));
						submeshData.vertices[i].tangent =
							XMFLOAT4(tangentSum.x / tangentCount, tangentSum.y / tangentCount, tangentSum.z / tangentCount, 1.f);
					}
				}
				else {
					const tinygltf::BufferView& tangentBufferView = model.bufferViews[tangentAccessor.bufferView];
					const tinygltf::Buffer& tangentBuffer = model.buffers[tangentBufferView.buffer];
					const float* tangents = reinterpret_cast<const float*>(&tangentBuffer.data[tangentBufferView.byteOffset + tangentAccessor.byteOffset]);

					for (size_t i = 0; i < tangentAccessor.count; i++) {
						XMFLOAT4 tangent = XMFLOAT4(tangents[i * 3 + 0], tangents[i * 3 + 1], tangents[i * 3 + 2], tangents[i * 3 + 3]);
						submeshData.vertices[i].tangent = tangent;
					}
				}

				// �}�e���A���C���f�b�N�X
				if (primitive.material < 0) {
					submeshData.materialIndex = 0;
				}
				else {
					submeshData.materialIndex = primitive.material;
				}

				// �T�u���b�V������
				Component::SubMesh submesh;
				if (submesh.Create(device, submeshData) == Utility::RESULT::FAILED) {
					MessageBox(NULL, L"SubMesh�̐����Ɏ��s���܂����B", L"�G���[���b�Z�[�W", MB_OK);
					return Utility::RESULT::FAILED;
				}
				submeshes.push_back(submesh);
			}
		}

		return Utility::RESULT::SUCCESS;
	}
	Utility::RESULT GLBLoader::LoadMaterial(ID3D12Device& device, tinygltf::Model& model, std::vector<Component::Material>& materials)
	{
		if (model.materials.size() == 0) {
			model.materials.resize(1);
		}

		materials.resize(model.materials.size());
		size_t index = 0;
		for (auto& material : model.materials) {

			// �x�[�X�J���[
			if (material.pbrMetallicRoughness.baseColorTexture.index >= 0) {
				const tinygltf::Texture& baseColorTexture = model.textures[material.pbrMetallicRoughness.baseColorTexture.index];
				tinygltf::Image& baseColorImage = model.images[baseColorTexture.source];

				if (materials[index].baseColorTexture == nullptr)
					materials[index].baseColorTexture = new DX12API::Texture();

				uint8_t* data = baseColorImage.image.data();

				materials[index].baseColorTexture->CreateTextureFromConstantData(
					Core::Dx12GraphicsEngine::Instance(), data, sizeof(unsigned char) * 4,
					static_cast<size_t>(baseColorImage.width) * baseColorImage.height,
					baseColorImage.width, baseColorImage.height, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
			}
			else {
				if (materials[index].baseColorTexture == nullptr)
					materials[index].baseColorTexture = new DX12API::Texture();

				std::vector<unsigned char> dummyBaseColor = { 128, 128, 128, 255 };

				materials[index].baseColorTexture->CreateTextureFromConstantData(
					Core::Dx12GraphicsEngine::Instance(), dummyBaseColor.data(), sizeof(unsigned char) * 4,
					1, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
			}
			// ���^���b�N���t�l�X
			if (material.pbrMetallicRoughness.metallicRoughnessTexture.index >= 0) {
				const tinygltf::Texture& metalRoughTexture = model.textures[material.pbrMetallicRoughness.metallicRoughnessTexture.index];
				tinygltf::Image& metalRoughImage = model.images[metalRoughTexture.source];

				if (materials[index].metalRoughTexture == nullptr)
					materials[index].metalRoughTexture = new DX12API::Texture();

				uint8_t* data = metalRoughImage.image.data();

				materials[index].metalRoughTexture->CreateTextureFromConstantData(
					Core::Dx12GraphicsEngine::Instance(), data, sizeof(unsigned char) * 4,
					static_cast<size_t>(metalRoughImage.width) * metalRoughImage.height,
					metalRoughImage.width, metalRoughImage.height, DXGI_FORMAT_R8G8B8A8_UNORM);

			}
			else {
				std::vector<unsigned char> dummyMetalRough = { 0, 255, 0, 255 };

				if (materials[index].metalRoughTexture == nullptr)
					materials[index].metalRoughTexture = new DX12API::Texture();

				materials[index].metalRoughTexture->CreateTextureFromConstantData(
					Core::Dx12GraphicsEngine::Instance(), dummyMetalRough.data(), sizeof(unsigned char) * 4,
					1, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM);
			}
			// �m�[�}��
			if (material.normalTexture.index >= 0) {
				const tinygltf::Texture& normalTexture = model.textures[material.normalTexture.index];
				tinygltf::Image& normalImage = model.images[normalTexture.source];

				if (materials[index].normalTexture == nullptr)
					materials[index].normalTexture = new DX12API::Texture();

				uint8_t* data = normalImage.image.data();

				materials[index].normalTexture->CreateTextureFromConstantData(
					Core::Dx12GraphicsEngine::Instance(), data, sizeof(unsigned char) * 4,
					static_cast<size_t>(normalImage.width) * normalImage.height,
					normalImage.width, normalImage.height, DXGI_FORMAT_R8G8B8A8_UNORM);
			}
			else {
				if (materials[index].normalTexture == nullptr)
					materials[index].normalTexture = new DX12API::Texture();
				// �V�F�[�_�[���� -1�`1�ɕϊ�����̂� �@��(0,0,1.f)�ɂ������ꍇ�͐�(0.5,0.5,1.f) �Ƃ���
				std::vector<unsigned char> dummyNormal = { 128,128,255,255 };

				materials[index].normalTexture->CreateTextureFromConstantData(
					Core::Dx12GraphicsEngine::Instance(), dummyNormal.data(), sizeof(unsigned char) * 4,
					1, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM);
			}
			// �A���r�G���g�I�N���[�W����
			if (material.occlusionTexture.index >= 0) {
				const tinygltf::Texture& occlusionTexture = model.textures[material.occlusionTexture.index];
				tinygltf::Image& occlusionImage = model.images[occlusionTexture.source];

				if (materials[index].occlusionTexture == nullptr)
					materials[index].occlusionTexture = new DX12API::Texture();

				uint8_t* data = occlusionImage.image.data();

				materials[index].occlusionTexture->CreateTextureFromConstantData(
					Core::Dx12GraphicsEngine::Instance(), data, sizeof(unsigned char) * 4,
					static_cast<size_t>(occlusionImage.width) * occlusionImage.height,
					occlusionImage.width, occlusionImage.height, DXGI_FORMAT_R8G8B8A8_UNORM);
			}
			else {
				if (materials[index].occlusionTexture == nullptr)
					materials[index].occlusionTexture = new DX12API::Texture();

				std::vector<unsigned char> dummyOcclusion = { 255, 255, 255, 255 };

				materials[index].occlusionTexture->CreateTextureFromConstantData(
					Core::Dx12GraphicsEngine::Instance(), dummyOcclusion.data(), sizeof(unsigned char) * 4,
					1, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM);
			}
			// �G�~�b�V�u
			if (material.emissiveTexture.index >= 0) {
				const tinygltf::Texture& emissiveTexture = model.textures[material.emissiveTexture.index];
				tinygltf::Image& emissiveImage = model.images[emissiveTexture.source];

				if (materials[index].emissiveTexture == nullptr)
					materials[index].emissiveTexture = new DX12API::Texture();

				uint8_t* data = emissiveImage.image.data();

				materials[index].emissiveTexture->CreateTextureFromConstantData(
					Core::Dx12GraphicsEngine::Instance(), data, sizeof(unsigned char) * 4,
					static_cast<size_t>(emissiveImage.width) * emissiveImage.height,
					emissiveImage.width, emissiveImage.height, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
			}
			else {
				if (materials[index].emissiveTexture == nullptr)
					materials[index].emissiveTexture = new DX12API::Texture();

				std::vector<unsigned char> dummyEmissive = { 0, 0, 0, 0 };

				materials[index].emissiveTexture->CreateTextureFromConstantData(
					Core::Dx12GraphicsEngine::Instance(), dummyEmissive.data(), sizeof(unsigned char) * 4,
					1, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM);
			}
			index++;
		}

		return Utility::RESULT::SUCCESS;
	}

	// �Q�l http://marupeke296.com/DXPS_No12_CalcTangentVectorSpace.html 
	XMFLOAT3 GLBLoader::CalcTangent(const XMFLOAT3& p0, const XMFLOAT3& p1, const XMFLOAT3& p2, const XMFLOAT2& uv0, const XMFLOAT2& uv1, const XMFLOAT2& uv2)
	{
		// 5������3�������_��
		XMFLOAT3 cp0[3] = {
			XMFLOAT3(p0.x,uv0.x,uv0.y),
			XMFLOAT3(p0.y,uv0.x,uv0.y),
			XMFLOAT3(p0.z,uv0.x,uv0.y)
		};
		XMFLOAT3 cp1[3] = {
			XMFLOAT3(p1.x,uv1.x,uv1.y),
			XMFLOAT3(p1.y,uv1.x,uv1.y),
			XMFLOAT3(p1.z,uv1.x,uv1.y)
		};
		XMFLOAT3 cp2[3] = {
			XMFLOAT3(p2.x,uv2.x,uv2.y),
			XMFLOAT3(p2.y,uv2.x,uv2.y),
			XMFLOAT3(p2.z,uv2.x,uv2.y)
		};

		// ���ʃp�����[�^����UV�����W�Z�o
		// �����ł�u��(�ڐ��x�N�g��)�̂݌v�Z����
		float u[3];
		for (size_t i = 0; i < 3; i++) {
			XMVECTOR v1 = XMVectorSubtract(XMLoadFloat3(&cp1[i]), XMLoadFloat3(&cp0[i]));
			XMVECTOR v2 = XMVectorSubtract(XMLoadFloat3(&cp2[i]), XMLoadFloat3(&cp1[i]));
			XMFLOAT3 abc;
			XMStoreFloat3(&abc, XMVector3Cross(v1, v2));

			// ���_��������uv���������Z�b�g����Ă��Ȃ�
			if (abc.x == 0.f) {
				//MessageBox(NULL, L"�ڐ��x�N�g���̐����Ɏ��s���܂����B", L"�G���[���b�Z�[�W", MB_OK);
				return XMFLOAT3(0.f, 0.f, 0.f);
			}
			u[i] = -abc.y / abc.x;
		}

		XMFLOAT3 tangent = XMFLOAT3(u[0], u[1], u[2]);
		XMStoreFloat3(&tangent, XMVector3Normalize(XMLoadFloat3(&tangent)));

		return tangent;
	}
}