#pragma once

#include <d3d12.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")

#include "EngineUtility.h"

/// <summary>
/// �V�F�[�_�[�N���X
/// </summary>
class Shader {
public:
	Shader() = default;
	~Shader() = default;

private:
	Microsoft::WRL::ComPtr<ID3DBlob> _shaderBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> _errorBlob = nullptr;

	/// <summary>
	/// �V�F�[�_�[���R���p�C��
	/// </summary>
	/// <param name="shaderFilePass">�V�F�[�_�[�̃t�@�C���p�X</param>
	/// <param name="entoryPointName">�V�F�[�_�[�̃G���g���֐���</param>
	/// <param name="shaderType">�V�F�[�_�[�̎��_�o�[�W����</param>
	/// <returns></returns>
	HRESULT CompileShader(const TCHAR* shaderFilePass, const char* entoryPointName, const char* shaderTypeAndVersion);

public:
	/// <summary>
	/// �V�F�[�_�[����
	/// </summary>
	/// <param name="shaderFilePass">�V�F�[�_�[�̃t�@�C���p�X</param>
	/// <param name="entoryPointName">�V�F�[�_�[�̃G���g���֐���</param>
	/// <param name="shaderType">�V�F�[�_�[�̎��_���f��</param>
	/// <returns></returns>
	MYRESULT Create(const TCHAR* shaderFilePass, const char* entoryPointName, const char* shaderType);

	/// <summary>
	/// �V�F�[�_�[�擾
	/// </summary>
	/// <returns>�V�F�[�_�[</returns>
	ID3DBlob& GetShader() const {
		return *_shaderBlob.Get();
	}
};