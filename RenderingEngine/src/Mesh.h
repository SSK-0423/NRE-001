#pragma once

#include "EngineUtility.h"
#include "RenderingContext.h"
#include "FBXLoader.h"

/// <summary>
/// ���b�V���N���X
/// </summary>
class Mesh {
public:
	Mesh();
	~Mesh();
public:
	/// <summary>
	/// ���b�V���ǂݍ���
	/// </summary>
	/// <param name="modelPath">�t�@�C���p�X</param>
	/// <returns></returns>
	MYRESULT LoadMesh(const char* modelPath);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="renderContext"></param>
	void Draw(RenderingContext& renderContext);
};