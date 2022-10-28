#include "ShaderLibrary.h"

using namespace NamelessEngine::Utility;

namespace NamelessEngine::DX12API
{
	static const TCHAR* SHADER_FOLDER_PASS = L"../RenderingEngine/Shaders/";
	static const UINT BUFFSIZE = 128;

	ShaderLibrary::ShaderLibrary()
	{
		_shaderTypeMap[SHADERTYPE::VERTEX] = "vs_5_0";
		_shaderTypeMap[SHADERTYPE::PIXEL] = "ps_5_0";

		RegistShader(L"GeometryVS", "GeometryVS", SHADERTYPE::VERTEX);
		RegistShader(L"GeometryPS", "GeometryPS", SHADERTYPE::PIXEL);
		RegistShader(L"SkyBoxVS", "SkyBoxVS", SHADERTYPE::VERTEX);
		RegistShader(L"SkyBoxPS", "SkyBoxPS", SHADERTYPE::PIXEL);
		RegistShader(L"GBuffer", "GBufferVS", SHADERTYPE::VERTEX, "VSMain");
		RegistShader(L"GBuffer", "GBufferPS", SHADERTYPE::PIXEL, "PSMain");
		RegistShader(L"PBR", "PBRVS", SHADERTYPE::VERTEX, "VSMain");
		RegistShader(L"PBR", "PBRPS", SHADERTYPE::PIXEL, "PSMain");
	};
	ShaderLibrary::~ShaderLibrary()
	{
	}
	ShaderLibrary& ShaderLibrary::Instance()
	{
		static ShaderLibrary inst;
		return inst;
	}
	void ShaderLibrary::RegistShader(
		const TCHAR* shaderFileName, const std::string& shaderTag, 
		SHADERTYPE shaderType, const char* entryPointName)
	{
		TCHAR shaderFilePass[BUFFSIZE];
		lstrcpyn(shaderFilePass, SHADER_FOLDER_PASS, BUFFSIZE);
		lstrcat(shaderFilePass, shaderFileName);
		lstrcat(shaderFilePass, L".hlsl");

		Shader shader;
		RESULT result = shader.Create(shaderFilePass, entryPointName, _shaderTypeMap[shaderType]);
		if (result == RESULT::FAILED) {
			TCHAR errorMessage[BUFFSIZE];
			lstrcpyn(errorMessage, L"ShaderCompileFailed FileName \"", BUFFSIZE);
			lstrcat(errorMessage, shaderFileName);
			lstrcat(errorMessage, L"\"");
			MessageBoxW(NULL, errorMessage, L"エラーメッセージ", MB_OK);
		}

		_shaderMap[shaderTag] = shader;
	}
	Shader* ShaderLibrary::GetShader(const std::string& shaderTag)
	{
		if (_shaderMap.count(shaderTag) == 0)
			return nullptr;
		return &_shaderMap[shaderTag];
	}
}