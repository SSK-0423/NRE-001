#include "ShaderLibrary.h"

using namespace NamelessEngine::Utility;

namespace NamelessEngine::DX12API
{
	static const TCHAR* SHADER_FOLDER_PASS = L"../Shaders/";
	static const UINT BUFFSIZE = 128;

	ShaderLibrary::ShaderLibrary()
	{
	}
	ShaderLibrary::~ShaderLibrary()
	{
	}
	ShaderLibrary& ShaderLibrary::Instance()
	{
		static ShaderLibrary inst;
		return inst;
	}
	void ShaderLibrary::RegistShader(const TCHAR* shaderFileName, const char* entryPointName, const char* shaderType, const std::string& shaderTag)
	{
		Shader shader;
		TCHAR shaderFilePass[BUFFSIZE];
		lstrcpyn(shaderFilePass, SHADER_FOLDER_PASS, BUFFSIZE);
		lstrcat(shaderFilePass, shaderFileName);

		RESULT result = shader.Create(shaderFilePass, entryPointName, shaderType);
		if (result == RESULT::FAILED) {
			TCHAR errorMessage[BUFFSIZE];
			lstrcpyn(errorMessage, L"ShaderCompileFailed FileName \"", BUFFSIZE);
			lstrcat(errorMessage, shaderFileName);
			lstrcat(errorMessage, L"\"");
			MessageBoxW(NULL, errorMessage, L"エラーメッセージ", MB_OK);
		}

	}
	Shader* ShaderLibrary::GetShader(const std::string& shaderTag)
	{
		if (_shaderMap.count(shaderTag) == 0)
			return nullptr;
		return &_shaderMap[shaderTag];
	}
}