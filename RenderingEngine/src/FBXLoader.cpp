#include "FBXLoader.h"
#include <Windows.h>
#include <cassert>

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

void FBXLoader::LoadMaterial(FbxSurfaceMaterial* material)
{
	FBXMaterial fbxMaterial;

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
			fbxMaterial.shiness = prop.Get<FbxDouble>();
		}
		else {
			fbxMaterial.shiness = 1.f;
		}

		FbxDouble3 color = materialColors[static_cast<int>(MATERIAL::DIFFUSE)];
		FbxDouble factor = factors[static_cast<int>(MATERIAL::DIFFUSE)];
		fbxMaterial.SetDiffuse(
			static_cast<float>(color[0]),
			static_cast<float>(color[1]),
			static_cast<float>(color[2]),
			static_cast<float>(factor));

		color = materialColors[static_cast<int>(MATERIAL::AMBIENT)];
		factor = factors[static_cast<int>(MATERIAL::AMBIENT)];
		fbxMaterial.SetAmbient(
			static_cast<float>(color[0]),
			static_cast<float>(color[1]),
			static_cast<float>(color[2]),
			static_cast<float>(factor));

		color = materialColors[static_cast<int>(MATERIAL::SPECULAR)];
		factor = factors[static_cast<int>(MATERIAL::SPECULAR)];
		fbxMaterial.SetSpecular(
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
		fbxMaterial.SetDiffuse(
			static_cast<float>(color[0]),
			static_cast<float>(color[1]),
			static_cast<float>(color[2]),
			static_cast<float>(factor));

		color = materialColors[static_cast<int>(MATERIAL::AMBIENT)];
		factor = factors[static_cast<int>(MATERIAL::AMBIENT)];
		fbxMaterial.SetAmbient(
			static_cast<float>(color[0]),
			static_cast<float>(color[1]),
			static_cast<float>(color[2]),
			static_cast<float>(factor));
	}

	_materials[material->GetName()] = fbxMaterial;
}

void FBXLoader::SetMaterial(FBXMeshData& meshData, FbxMesh* mesh)
{
	// �}�e���A����������ΏI���
	if (mesh->GetElementMaterialCount() == 0) {
		//TODO: �Ȃ��ꍇ�̃}�e���A�����ǂ����邩
		return;
	}

	// FbxMesh���̃}�e���A�����擾
	FbxLayerElementMaterial* material = mesh->GetElementMaterial();
	int index = material->GetIndexArray().GetAt(0);
	FbxSurfaceMaterial* surfaceMaterial = mesh->GetNode()->GetSrcObject<FbxSurfaceMaterial>(index);

	if (surfaceMaterial != nullptr) {
		for (auto mat : _materials) {
			if (mat.first == surfaceMaterial->GetName()) {
				meshData.material = mat.second;
			}
		}
	}
	else {
		//TODO: �Ȃ��ꍇ�̃}�e���A�����ǂ����邩
	}
}

bool FBXLoader::Load(const char* meshPath, std::vector<FBXMeshData>& meshDataList)
{
	assert(fbxManager != nullptr && fbxScene != nullptr && fbxImporter != nullptr);

	// �t�@�C��������������
	if (fbxImporter->Initialize(meshPath) == false) {
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

	// �}�e���A�����擾
	int materialNum = fbxScene->GetSrcObjectCount<FbxSurfaceMaterial>();
	for (int idx = 0; idx < materialNum; idx++) {
		LoadMaterial(fbxScene->GetSrcObject<FbxSurfaceMaterial>(idx));
	}

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