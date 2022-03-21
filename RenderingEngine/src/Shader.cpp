#include "Shader.h"
#include <d3dcompiler.h>
#include <string>

#pragma comment(lib,"d3dcompiler.lib")

HRESULT Shader::CompileShader(
	const TCHAR* shaderFilePass, const char* entoryPointName, const char* shaderTypeAndVersion)
{
#ifdef _DEBUG
	// �V�F�[�_�[�̃R���p�C��
	HRESULT result = D3DCompileFromFile(
		shaderFilePass,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entoryPointName, shaderTypeAndVersion,
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&_shaderBlob, &_errorBlob);
#else
	// �V�F�[�_�[�̃R���p�C��
	HRESULT result = D3DCompileFromFile(
		shaderFilePass,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entoryPointName, shaderTypeAndVersion,
		D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&_shaderBlob, &_errorBlob);
#endif // _DEBUG

	if (FAILED(result)) {
		if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND)) {
			::OutputDebugStringA("�t�@�C������������܂���");
		}
		else {
			std::string errstr;	// �G���[���b�Z�[�W�󂯎��pstring
			errstr.resize(_errorBlob->GetBufferSize()); // �K�v�ȃT�C�Y���m��
			// �f�[�^�R�s�[
			std::copy_n((char*)_errorBlob->GetBufferPointer(),
				_errorBlob->GetBufferSize(),
				errstr.begin());
			errstr += "\n";

			::OutputDebugStringA(errstr.c_str());
		}
	}

	return result;
}

MYRESULT Shader::Create(
	const TCHAR* shaderFilePass, const char* entoryPointName, const char* shaderTypeAndVersion)
{
	// �V�F�[�_�[���R���p�C��
	if (FAILED(CompileShader(shaderFilePass, entoryPointName, shaderTypeAndVersion))) {
		return MYRESULT::FAILED;
	}

	return MYRESULT::SUCCESS;
}

MYRESULT Shader::Create(const ShaderData& shaderData)
{
	MYRESULT result = 
		Create(shaderData.shaderFilePass, shaderData.entoryPointName, shaderData.shaderType);
	if (result == MYRESULT::FAILED) { return result; }

	return MYRESULT::SUCCESS;
}
