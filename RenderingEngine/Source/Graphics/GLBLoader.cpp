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
			MessageBox(NULL, L"非対応形式のフォーマットです。(対応形式：.glb)", L"エラーメッセージ", MB_OK);
			return Utility::RESULT::FAILED;
		}

		bool res = loader.LoadBinaryFromFile(&model, &err, &warn, path);

		if (!warn.empty()) {
			wchar_t message[128];
			Utility::charToWchar(warn.c_str(), message, warn.length());
			MessageBox(NULL, message, L"警告メッセージ", MB_OK);
		}

		if (!err.empty()) {
			wchar_t message[128];
			Utility::charToWchar(err.c_str(), message, err.length());
			MessageBox(NULL, message, L"エラーメッセージ", MB_OK);
		}

		if (!res) {
			MessageBox(NULL, L"モデル読み込みに失敗しました。", L"エラーメッセージ", MB_OK);
			return Utility::RESULT::FAILED;
		}

		// メッシュ読み込み
		if (LoadMesh(device, model, submeshes) == Utility::RESULT::FAILED) {
			MessageBox(NULL, L"メッシュ読み込みに失敗しました。", L"エラーメッセージ", MB_OK);
			return Utility::RESULT::FAILED;
		}

		// マテリアル読み込み
		if (LoadMaterial(device, model, materials) == Utility::RESULT::FAILED) {
			MessageBox(NULL, L"マテリアル読み込みに失敗しました。", L"エラーメッセージ", MB_OK);
			return Utility::RESULT::FAILED;
		}

		return Utility::RESULT::SUCCESS;
	}

	Utility::RESULT GLBLoader::LoadMesh(ID3D12Device& device, tinygltf::Model& model, std::vector<Component::SubMesh>& submeshes)
	{
		// メッシュ読み込み
		for (auto& mesh : model.meshes) {
			for (auto& primitive : mesh.primitives) {
				Component::MeshData submeshData;
				// 位置データ
				const tinygltf::Accessor& posAccessor = model.accessors[primitive.attributes["POSITION"]];
				const tinygltf::BufferView& posBufferView = model.bufferViews[posAccessor.bufferView];
				const tinygltf::Buffer& posBuffer = model.buffers[posBufferView.buffer];
				const float* positions = reinterpret_cast<const float*>(&posBuffer.data[posBufferView.byteOffset + posAccessor.byteOffset]);

				submeshData.vertices.resize(posAccessor.count);

				for (size_t i = 0; i < posAccessor.count; i++) {
					XMFLOAT3 pos = XMFLOAT3(positions[i * 3 + 0], positions[i * 3 + 1], positions[i * 3 + 2]);
					submeshData.vertices[i].position = pos;
				}

				// 法線データ
				const tinygltf::Accessor& normalAccessor = model.accessors[primitive.attributes["NORMAL"]];
				const tinygltf::BufferView& normalBufferView = model.bufferViews[normalAccessor.bufferView];
				const tinygltf::Buffer& normalBuffer = model.buffers[normalBufferView.buffer];
				const float* normals = reinterpret_cast<const float*>(&normalBuffer.data[normalBufferView.byteOffset + normalAccessor.byteOffset]);

				for (size_t i = 0; i < normalAccessor.count; i++) {
					XMFLOAT3 normal = XMFLOAT3(normals[i * 3 + 0], normals[i * 3 + 1], normals[i * 3 + 2]);
					submeshData.vertices[i].normal = normal;
				}

				// UVデータ
				const tinygltf::Accessor& uvAccessor = model.accessors[primitive.attributes["TEXCOORD_0"]];
				const tinygltf::BufferView& uvBufferView = model.bufferViews[uvAccessor.bufferView];
				const tinygltf::Buffer& uvBuffer = model.buffers[uvBufferView.buffer];
				const float* uvs = reinterpret_cast<const float*>(&uvBuffer.data[uvBufferView.byteOffset + uvAccessor.byteOffset]);

				for (size_t i = 0; i < uvAccessor.count; i++) {
					XMFLOAT2 uv = XMFLOAT2(uvs[i * 2 + 0], uvs[i * 2 + 1]);
					submeshData.vertices[i].uv = uv;
				}

				// インデックスデータ
				const tinygltf::Accessor& indexAccessor = model.accessors[primitive.indices];
				const tinygltf::BufferView& indexBufferView = model.bufferViews[indexAccessor.bufferView];
				const tinygltf::Buffer& indexBuffer = model.buffers[indexBufferView.buffer];
				const unsigned short* indi = reinterpret_cast<const unsigned short*>(&indexBuffer.data[indexBufferView.byteOffset + indexAccessor.byteOffset]);

				submeshData.indices.resize(indexAccessor.count);
				for (size_t i = 0; i < indexAccessor.count; i++) {
					submeshData.indices[i] = indi[i];
				}

				// 接線データ(存在しない場合がある)
				// 存在しない場合は他のAccessorとcount変数の値が異なる
				const tinygltf::Accessor& tangentAccessor = model.accessors[primitive.attributes["TANGENT"]];
				if (tangentAccessor.count != posAccessor.count) {
					for (size_t i = 0; i < posAccessor.count; i++) {
						// ポリゴン頂点とUV抽出
						// 頂点接線ベクトルなので共有面を全てリストアップして平均取る必要ありそう
						XMFLOAT3 p0;
						XMFLOAT3 p1;
						XMFLOAT3 p2;
						XMFLOAT2 uv0;
						XMFLOAT2 uv1;
						XMFLOAT2 uv2;

						for (size_t j = 0; j < indexAccessor.count; j++) {
							// 接線ベクトルを求めたい頂点を含むポリゴンを見つける
							if (XMVector3Equal(XMLoadFloat3(&submeshData.vertices[i].position), XMLoadFloat3(&submeshData.vertices[j].position))) {
								switch (j % 3)
								{
								case 0:	// ポリゴンの最初の頂点だった場合
									p0 = submeshData.vertices[j].position;
									p1 = submeshData.vertices[j + 1].position;
									p2 = submeshData.vertices[j + 2].position;
									uv0 = submeshData.vertices[j].uv;
									uv1 = submeshData.vertices[j + 1].uv;
									uv2 = submeshData.vertices[j + 2].uv;
									break;
								case 1:	// ポリゴンの2番目の頂点だった場合
									p0 = submeshData.vertices[j - 1].position;
									p1 = submeshData.vertices[j].position;
									p2 = submeshData.vertices[j + 1].position;
									uv0 = submeshData.vertices[j - 1].uv;
									uv1 = submeshData.vertices[j].uv;
									uv2 = submeshData.vertices[j + 1].uv;
									break;
								case 2:	// ポリゴンの3番目の頂点だった場合
									p0 = submeshData.vertices[j - 2].position;
									p1 = submeshData.vertices[j - 1].position;
									p2 = submeshData.vertices[j].position;
									uv0 = submeshData.vertices[j - 2].uv;
									uv1 = submeshData.vertices[j - 1].uv;
									uv2 = submeshData.vertices[j].uv;
								default:
									break;
								}
								break;
							}
						}

						submeshData.vertices[i].tangent = CalcTangent(p0, p1, p2, uv0, uv1, uv2);
					}
				}
				else {
					const tinygltf::BufferView& tangentBufferView = model.bufferViews[tangentAccessor.bufferView];
					const tinygltf::Buffer& tangentBuffer = model.buffers[tangentBufferView.buffer];
					const float* tangents = reinterpret_cast<const float*>(&tangentBuffer.data[tangentBufferView.byteOffset + tangentAccessor.byteOffset]);

					for (size_t i = 0; i < tangentAccessor.count; i++) {
						XMFLOAT3 tangent = XMFLOAT3(tangents[i * 3 + 0], tangents[i * 3 + 1], tangents[i * 3 + 2]);
						submeshData.vertices[i].tangent = tangent;
					}
				}

				// マテリアルインデックス
				submeshData.materialIndex = primitive.material;

				// サブメッシュ生成
				Component::SubMesh submesh;
				if (submesh.Create(device, submeshData) == Utility::RESULT::FAILED) {
					MessageBox(NULL, L"SubMeshの生成に失敗しました。", L"エラーメッセージ", MB_OK);
					return Utility::RESULT::FAILED;
				}
				submeshes.push_back(submesh);
			}
		}

		return Utility::RESULT::SUCCESS;
	}
	Utility::RESULT GLBLoader::LoadMaterial(ID3D12Device& device, tinygltf::Model& model, std::vector<Component::Material>& materials)
	{
		materials.resize(model.materials.size());
		size_t index = 0;
		for (auto& material : model.materials) {
			// ベースカラー
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
			// メタリックラフネス
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
			// ノーマル
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
			// アンビエントオクルージョン
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
			// エミッシブ
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

			index++;
		}

		return Utility::RESULT::SUCCESS;
	}

	XMFLOAT3 GLBLoader::CalcTangent(const XMFLOAT3& p0, const XMFLOAT3& p1, const XMFLOAT3& p2, const XMFLOAT2& uv0, const XMFLOAT2& uv1, const XMFLOAT2& uv2)
	{
		return XMFLOAT3();
	}
}