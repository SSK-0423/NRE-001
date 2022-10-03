#include "FBXLoader.h"
#include "EngineUtility.h"
#include "PhongMaterial.h"
#include "PBRMaterial.h"

#include <Windows.h>
#include <cassert>

namespace NamelessEngine::Graphics
{
	FBXLoader::FBXLoader()
		: fbxManager(nullptr), fbxImporter(nullptr), fbxScene(nullptr)
	{
		// ��Ō������K�v����
		if (Init() == false) {
			MessageBox(NULL, L"FBX_SDK�̏������Ɏ��s���܂����B", L"�G���[���b�Z�[�W", MB_OK);
			return;
		}
	}

	FBXLoader::~FBXLoader()
	{
		if (fbxImporter != nullptr) {
			fbxImporter->Destroy();
		}
		if (fbxScene != nullptr) {
			fbxScene->Destroy();
		}
		if (fbxManager != nullptr) {
			fbxManager->Destroy();
		}
		_materials.clear();
	}

	bool FBXLoader::Init()
	{
		// FbxManager����
		fbxManager = FbxManager::Create();
		if (fbxManager == nullptr) {
			return false;
		}

		// FbxImporter����
		fbxImporter = FbxImporter::Create(fbxManager, "Importer");
		if (fbxImporter == nullptr) {
			fbxManager->Destroy();
			return false;
		}

		// FbxScene����
		fbxScene = FbxScene::Create(fbxManager, "Scene");
		if (fbxScene == nullptr) {
			fbxImporter->Destroy();
			fbxManager->Destroy();
			return false;
		}

		return true;
	}

	FBXMeshData FBXLoader::CreateMesh(FbxMesh* mesh)
	{
		FBXMeshData meshData;
		LoadVertices(meshData, mesh);
		LoadIndices(meshData, mesh);
		LoadNormals(meshData, mesh);
		LoadUV(meshData, mesh);
		SetMaterial(meshData, mesh);

		return meshData;
	}

	void FBXLoader::LoadVertices(FBXMeshData& meshData, FbxMesh* mesh)
	{
		// ���_�o�b�t�@�̎擾 
		FbxVector4* vertices = mesh->GetControlPoints();
		// �C���f�b�N�X�o�b�t�@�̎擾
		int* indices = mesh->GetPolygonVertices();
		// ���_���W�̐�(���_��)�̎擾
		unsigned long long polygonVertexCount = mesh->GetPolygonVertexCount();

		for (size_t idx = 0; idx < polygonVertexCount; idx++) {

			FBXMeshVertex vertex;

			// �C���f�b�N�X�o�b�t�@���璸�_�ԍ����擾
			int index = indices[idx];

			// ���_���W���X�g������W���擾����
			vertex.position.x = -vertices[index][0]; // FBX�͉E��n DirectX�͍���n�Ȃ̂�X���W�𔽓]
			vertex.position.y = vertices[index][1];
			vertex.position.z = vertices[index][2];

			// �ǉ�
			meshData.vertices.push_back(vertex);
		}
	}

	void FBXLoader::LoadIndices(FBXMeshData& meshData, FbxMesh* mesh)
	{
		// �|���S�����̎擾
		int polygonCount = mesh->GetPolygonCount();

		// �|���S���̐������A�ԂƂ��ĕۑ�����
		for (unsigned int i = 0; i < polygonCount; i++) {
			// �C���f�b�N�X�o�b�t�@�̏��Ԃŕ���ł���̂ł��̂܂�
			// ����n�΍�
			meshData.indices.push_back(i * 3 + 2);
			meshData.indices.push_back(i * 3 + 1);
			meshData.indices.push_back(i * 3);
		}
	}

	void FBXLoader::LoadNormals(FBXMeshData& meshData, FbxMesh* mesh)
	{
		FbxArray<FbxVector4> normals;
		// �@�����X�g�擾
		mesh->GetPolygonVertexNormals(normals);

		// �@���ݒ�
		for (size_t idx = 0; idx < normals.Size(); idx++) {
			meshData.vertices[idx].normal.x = (float)-normals[idx][0];
			meshData.vertices[idx].normal.y = (float)normals[idx][1];
			meshData.vertices[idx].normal.z = (float)normals[idx][2];
		}
	}

	void FBXLoader::SetMaterial(FBXMeshData& meshData, FbxMesh* mesh)
	{
		// �}�e���A����������ΏI���
		if (mesh->GetElementMaterialCount() == 0) {
			meshData.material = nullptr;
			return;
		}

		// FbxMesh���̃}�e���A�����擾
		FbxLayerElementMaterial* material = mesh->GetElementMaterial();
		int index = material->GetIndexArray().GetAt(0);
		FbxSurfaceMaterial* surfaceMaterial = mesh->GetNode()->GetSrcObject<FbxSurfaceMaterial>(index);

		if (surfaceMaterial != nullptr) {
			for (auto& mat : _materials) {
				if (mat.first == surfaceMaterial->GetName()) {
					meshData.material = mat.second;
				}
			}
		}
		else {
			meshData.material = nullptr;
		}
	}

	bool FBXLoader::LoadPhongMaterial(FbxSurfaceMaterial* material, std::wstring textureFolderPath)
	{
		PhongMaterial* phongMaterial = new PhongMaterial();

		enum class MATERIAL
		{
			DIFFUSE,
			AMBIENT,
			SPECULAR,
			MATERIAL_NUM
		};

		FbxDouble3 materialColors[static_cast<int>(MATERIAL::MATERIAL_NUM)];
		FbxDouble factors[static_cast<int>(MATERIAL::MATERIAL_NUM)];
		FbxProperty prop;

		// FbxSurfacePhong�N���X������
		if (material->GetClassId().Is(FbxSurfacePhong::ClassId)) {
			// �擾����}�e���A���f�[�^
			const char* colorElements[] = {
				FbxSurfaceMaterial::sDiffuse,
				FbxSurfaceMaterial::sAmbient,
				FbxSurfaceMaterial::sSpecular,
			};

			// �擾����t�@�N�^�[(�d��)
			const char* factorElements[] = {
				FbxSurfaceMaterial::sDiffuseFactor,
				FbxSurfaceMaterial::sAmbientFactor,
				FbxSurfaceMaterial::sSpecularFactor,
			};

			for (size_t idx = 0; idx < _countof(colorElements); idx++) {
				prop = material->FindProperty(colorElements[idx]);
				if (prop.IsValid()) {
					materialColors[idx] = prop.Get<FbxDouble3>();
				}
				else {
					materialColors[idx] = FbxDouble3(1.f, 1.f, 1.f);
				}
			}

			for (size_t idx = 0; idx < _countof(factorElements); idx++) {
				prop = material->FindProperty(factorElements[idx]);
				if (prop.IsValid()) {
					factors[idx] = prop.Get<FbxDouble>();
				}
				else {
					factors[idx] = 1.f;
				}
			}

			// Shiness�擾
			prop = material->FindProperty(FbxSurfaceMaterial::sShininess);
			if (prop.IsValid()) {
				phongMaterial->SetShininess(prop.Get<FbxDouble>());
			}
			else {
				phongMaterial->SetShininess(1.f);
			}

			FbxDouble3 color = materialColors[static_cast<int>(MATERIAL::DIFFUSE)];
			FbxDouble factor = factors[static_cast<int>(MATERIAL::DIFFUSE)];
			phongMaterial->SetDiffuse(
				static_cast<float>(color[0]),
				static_cast<float>(color[1]),
				static_cast<float>(color[2]),
				static_cast<float>(factor));

			color = materialColors[static_cast<int>(MATERIAL::AMBIENT)];
			factor = factors[static_cast<int>(MATERIAL::AMBIENT)];
			phongMaterial->SetAmbient(
				static_cast<float>(color[0]),
				static_cast<float>(color[1]),
				static_cast<float>(color[2]),
				static_cast<float>(factor));

			color = materialColors[static_cast<int>(MATERIAL::SPECULAR)];
			factor = factors[static_cast<int>(MATERIAL::SPECULAR)];
			phongMaterial->SetSpecular(
				static_cast<float>(color[0]),
				static_cast<float>(color[1]),
				static_cast<float>(color[2]),
				static_cast<float>(factor));
		}

		// FbxSurfaceLambert�N���X������
		else if (material->GetClassId().Is(FbxSurfaceLambert::ClassId)) {
			// �擾����}�e���A���J���[
			const char* colorElements[] = {
				FbxSurfaceMaterial::sDiffuse,
				FbxSurfaceMaterial::sAmbient,
			};
			// �擾����t�@�N�^�[(�d��)
			const char* factorElements[] = {
				FbxSurfaceMaterial::sDiffuseFactor,
				FbxSurfaceMaterial::sAmbientFactor,
			};

			for (size_t idx = 0; idx < _countof(colorElements); idx++) {
				prop = material->FindProperty(colorElements[idx]);
				if (prop.IsValid()) {
					materialColors[idx] = prop.Get<FbxDouble3>();
				}
				else {
					materialColors[idx] = FbxDouble3(1.f, 1.f, 1.f);
				}
			}

			for (size_t idx = 0; idx < _countof(factorElements); idx++) {
				prop = material->FindProperty(factorElements[idx]);
				if (prop.IsValid()) {
					factors[idx] = prop.Get<FbxDouble>();
				}
				else {
					factors[idx] = 1.f;
				}
			}

			FbxDouble3 color = materialColors[static_cast<int>(MATERIAL::DIFFUSE)];
			FbxDouble factor = factors[static_cast<int>(MATERIAL::DIFFUSE)];
			phongMaterial->SetDiffuse(
				static_cast<float>(color[0]),
				static_cast<float>(color[1]),
				static_cast<float>(color[2]),
				static_cast<float>(factor));

			color = materialColors[static_cast<int>(MATERIAL::AMBIENT)];
			factor = factors[static_cast<int>(MATERIAL::AMBIENT)];
			phongMaterial->SetAmbient(
				static_cast<float>(color[0]),
				static_cast<float>(color[1]),
				static_cast<float>(color[2]),
				static_cast<float>(factor));
		}

		// �e�N�X�`���ǂݍ���
		// Diffuse�v���p�e�B���擾
		prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
		FbxFileTexture* texture = nullptr;
		int textureNum = prop.GetSrcObjectCount<FbxFileTexture>();
		// �V���O���e�N�X�`���̏ꍇ
		if (textureNum > 0) {
			texture = prop.GetSrcObject<FbxFileTexture>(0);
		}
		// �}���`�e�N�X�`���̏ꍇ
		else {
			int layerNum = prop.GetSrcObjectCount<FbxLayeredTexture>();
			if (layerNum > 0) {
				// TODO:�}���`�e�N�X�`���ւ̑Ή�
				texture = prop.GetSrcObject<FbxFileTexture>();
			}
		}

		// �t�@�C���l�[���̂ݎ擾
		phongMaterial->SetTextureName(GetTextureFileName(texture));
		// �e�N�X�`���t�H���_�ւ̃p�X�Z�b�g
		phongMaterial->SetTextureFolderPath(textureFolderPath);
		// �}�e���A���𗘗p�\�ȏ�Ԃɂ���
		if (phongMaterial->Commit() == Utility::MYRESULT::FAILED) { return false; }

		// �}�e���A�����X�g�ɓo�^
		_materials[material->GetName()] = phongMaterial;

		return true;
	}

	bool FBXLoader::LoadPBRMaterial(
		FbxSurfaceMaterial* material, std::wstring textureFolderPath,
		std::wstring baseColorTextureName, std::wstring metallicTextureName,
		std::wstring roughnessTextureName, std::wstring normalTextureName, std::wstring occulusionTextureName)
	{
		// PBR�e�N�X�`���ǂݍ���
		PBRMaterial* pbrMaterial = new PBRMaterial();
		pbrMaterial->SetTextureFolderPath(textureFolderPath);
		// �}�e���A���Ƀe�N�X�`���Z�b�g
		pbrMaterial->SetTextureName(PBRTEXTURETYPE::BASE_COLOR, baseColorTextureName);
		pbrMaterial->SetTextureName(PBRTEXTURETYPE::METALLIC, metallicTextureName);
		pbrMaterial->SetTextureName(PBRTEXTURETYPE::ROUGHNESS, roughnessTextureName);
		pbrMaterial->SetTextureName(PBRTEXTURETYPE::NORMAL, normalTextureName);
		pbrMaterial->SetTextureName(PBRTEXTURETYPE::OCCULUSION, occulusionTextureName);
		// �}�e���A���𗘗p�\�ɂ���
		if (pbrMaterial->Commit() == Utility::MYRESULT::FAILED) { return false; }
		// �}�e���A�����X�g�ɓo�^
		_materials[material->GetName()] = pbrMaterial;

		return true;
	}

	bool FBXLoader::LoadMaterial(FBXLoadData& data)
	{
		// �}�e���A�����擾
		int materialNum = fbxScene->GetSrcObjectCount<FbxSurfaceMaterial>();

		// �t�H���̔��˃��f���̃}�e���A���ǂݍ���
		if (data.materialType == MATERIAL_TYPE::PHONG) {
			for (int idx = 0; idx < materialNum; idx++) {
				bool result = LoadPhongMaterial(
					fbxScene->GetSrcObject<FbxSurfaceMaterial>(idx), data.textureFolderPath);
				if (!result) { return false; }
			}
		}

		// PBR�}�e���A���ǂݍ���
		else if (data.materialType == MATERIAL_TYPE::PBR) {
			for (int idx = 0; idx < materialNum; idx++) {
				bool result = LoadPBRMaterial(
					fbxScene->GetSrcObject<FbxSurfaceMaterial>(idx), data.textureFolderPath,
					data.baseColorName, data.metallicName, data.roughnessName, data.normalName, data.occlusionName);
				if (!result) { return false; }
			}
		}
		return true;
	}

	void FBXLoader::LoadUV(FBXMeshData& meshData, FbxMesh* mesh)
	{
		FbxStringList uvsetNames;
		// UVSet�̖��O���X�g���擾
		mesh->GetUVSetNames(uvsetNames);

		FbxArray<FbxVector2> uvBuffer;

		// UVSet�̖��O����UVset���擾����
		// ����̓}���`�e�N�X�`���ɂ͑Ή����Ȃ��̂ōŏ��̖��O���g��
		mesh->GetPolygonVertexUVs(uvsetNames.GetStringAt(0), uvBuffer);

		for (int idx = 0; idx < uvBuffer.Size(); idx++) {
			FbxVector2& uv = uvBuffer[idx];

			meshData.vertices[idx].uv.x = static_cast<float>(uv[0]);
			// DirectX�͍��㌴�_ �e�N�X�`���͍������_�ɓ\���Ă���̂�v�l�𔽓]
			meshData.vertices[idx].uv.y = static_cast<float>(1.f - uv[1]);
		}
	}

	std::wstring FBXLoader::GetTextureFileName(FbxFileTexture* texture)
	{
		if (texture == nullptr) { return L""; }

		// ���΃p�X�擾
		std::string path = texture->GetRelativeFileName();

		// �u��
		path = Utility::ReplaceString(path, "\\", "/");

		// �g���q���݂̃t�@�C�����擾
		std::string::size_type pos = path.rfind("/");
		// �u/�v��������������擾
		path = path.substr(pos + 1);

		// ���������΍�
		char* charFileName;
		// �����R�[�h�ϊ�
		FbxUTF8ToAnsi(path.c_str(), charFileName);

		// wstring�ɕϊ�
		wchar_t wchartFileName[256];
		errno_t err = Utility::charToWchar(charFileName, wchartFileName, 256);
		if (err != 0) {
			return L"";
		}

		return std::wstring(wchartFileName);
	}

	bool FBXLoader::Load(std::vector<FBXMeshData>& meshDataList, FBXLoadData& data)
	{
		assert(fbxManager != nullptr && fbxScene != nullptr && fbxImporter != nullptr);

		// �t�@�C��������������
		if (fbxImporter->Initialize(data.meshPath) == false) {
			fbxImporter->Destroy();
			fbxScene->Destroy();
			fbxManager->Destroy();
			return false;
		}

		// �C���|�[�g
		if (fbxImporter->Import(fbxScene) == false) {
			fbxImporter->Destroy();
			fbxScene->Destroy();
			fbxManager->Destroy();
			return false;
		}

		FbxGeometryConverter converter(fbxManager);
		// �S�Ă�FbxMesh���}�e���A�����Ƃɕ���
		converter.SplitMeshesPerMaterial(fbxScene, true);
		// �|���S�����O�p�`�ɂ���
		converter.Triangulate(fbxScene, true);

		// �}�e���A���ǂݍ���
		if (LoadMaterial(data) == false) { return false; }

		// FbxMesh�̐����擾
		int meshNum = fbxScene->GetSrcObjectCount<FbxMesh>();

		// ���b�V�������̃������m��
		meshDataList.resize(meshNum);

		// ���b�V������
		for (int idx = 0; idx < meshNum; idx++) {
			meshDataList[idx] = CreateMesh(fbxScene->GetSrcObject<FbxMesh>(idx));
		}

		return true;
	}
}

///
/// PBR�e�N�X�`���ǂݍ���
/// �d�l
///	�EPhong�̔��˃��f��
/// �EPBR
/// �E�@���}�b�v+Phong�̔��˃��f���Ƃ�
/// �������g����������(���f���P��)
/// ������1
///		bool�ŕ���
/// ������2
///		�e�N�X�`���P�ʂœǂݍ��݁@�t�@�C�������w�肳��Ă��Ȃ����͓̂ǂݍ��܂Ȃ�
/// 