#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"

#include "GLBLoader.h"
#include "../Utility/EngineUtility.h"

namespace NamelessEngine::Graphics
{
	Utility::RESULT GLBLoader::LoadModel(ID3D12Device& device, const std::string& path, std::vector<Component::SubMesh>& submeshes)
	{
		tinygltf::TinyGLTF loader;
		tinygltf::Model model;
		std::string err;
		std::string warn;

		std::string extension = Utility::GetExtension(path);
		if (extension != "glb") {
			MessageBox(NULL, L"��Ή��`���̃t�H�[�}�b�g�ł��B(�Ή��`���F.glb)", L"�G���[���b�Z�[�W", MB_OK);
		}

		bool result = loader.LoadBinaryFromFile(&model, &err, &warn, path);

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

		if (!result)
			MessageBox(NULL, L"���f���ǂݍ��݂Ɏ��s���܂����B", L"�G���[���b�Z�[�W", MB_OK);


		// Mesh�N���X�𐶐�����
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
					DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(positions[i * 3 + 0], positions[i * 3 + 1], positions[i * 3 + 2]);
					submeshData.vertices[i].position = pos;
				}

				// �@���f�[�^
				const tinygltf::Accessor& normalAccessor = model.accessors[primitive.attributes["NORMAL"]];
				const tinygltf::BufferView& normalBufferView = model.bufferViews[normalAccessor.bufferView];
				const tinygltf::Buffer& normalBuffer = model.buffers[normalBufferView.buffer];
				const float* normals = reinterpret_cast<const float*>(&normalBuffer.data[normalBufferView.byteOffset + normalAccessor.byteOffset]);

				for (size_t i = 0; i < normalAccessor.count; i++) {
					DirectX::XMFLOAT3 normal = DirectX::XMFLOAT3(normals[i * 3 + 0], normals[i * 3 + 1], normals[i * 3 + 2]);
					submeshData.vertices[i].normal = normal;
				}

				// UV�f�[�^
				const tinygltf::Accessor& uvAccessor = model.accessors[primitive.attributes["TEXCOORD_0"]];
				const tinygltf::BufferView& uvBufferView = model.bufferViews[uvAccessor.bufferView];
				const tinygltf::Buffer& uvBuffer = model.buffers[uvBufferView.buffer];
				const float* uvs = reinterpret_cast<const float*>(&uvBuffer.data[uvBufferView.byteOffset + uvAccessor.byteOffset]);

				for (size_t i = 0; i < uvAccessor.count; i++) {
					DirectX::XMFLOAT2 uv = DirectX::XMFLOAT2(uvs[i * 2 + 0], uvs[i * 2 + 1]);
					submeshData.vertices[i].uv = uv;
				}

				// �C���f�b�N�X�f�[�^
				const tinygltf::Accessor& indexAccessor = model.accessors[primitive.indices];
				const tinygltf::BufferView& indexBufferView = model.bufferViews[indexAccessor.bufferView];
				const tinygltf::Buffer& indexBuffer = model.buffers[indexBufferView.buffer];
				const unsigned short* indi = reinterpret_cast<const unsigned short*>(&indexBuffer.data[indexBufferView.byteOffset + indexAccessor.byteOffset]);

				submeshData.indices.resize(indexAccessor.count);
				for (size_t i = 0; i < indexAccessor.count; i++) {
					submeshData.indices[i] = indi[i];
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
}