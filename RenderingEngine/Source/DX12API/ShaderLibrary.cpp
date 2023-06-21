#include "ShaderLibrary.h"

using namespace NamelessEngine::Utility;

namespace NamelessEngine::DX12API
{
	static const TCHAR* SHADER_FOLDER_PASS = L"Shaders/";
	static const UINT BUFFSIZE = 128;

	ShaderLibrary::ShaderLibrary()
	{
		_shaderTypeMap[SHADERTYPE::VERTEX] = "vs_5_0";
		_shaderTypeMap[SHADERTYPE::PIXEL] = "ps_5_0";

		RegistShader(L"GBuffer", "GBufferVS", SHADERTYPE::VERTEX, "VSMain");
		RegistShader(L"GBuffer", "GBufferPS", SHADERTYPE::PIXEL, "PSMain");
		RegistShader(L"PBR", "PBRVS", SHADERTYPE::VERTEX, "VSMain");
		RegistShader(L"PBR", "PBRPS", SHADERTYPE::PIXEL, "PSMain");
		RegistShader(L"SkyBox", "SkyBoxVS", SHADERTYPE::VERTEX, "VSMain");
		RegistShader(L"SkyBox", "SkyBoxPS", SHADERTYPE::PIXEL, "PSMain");
		RegistShader(L"Blend", "BlendVS", SHADERTYPE::VERTEX, "VSMain");
		RegistShader(L"Blend", "BlendPS", SHADERTYPE::PIXEL, "PSMain");
		RegistShader(L"IBL", "IBLVS", SHADERTYPE::VERTEX, "VSMain");
		RegistShader(L"IBL", "IBLPS", SHADERTYPE::PIXEL, "PSMain");
		RegistShader(L"ShadowMap", "ShadowMapVS", SHADERTYPE::VERTEX, "VSMain");
		RegistShader(L"ShadowMap", "ShadowMapPS", SHADERTYPE::PIXEL, "PSMain");
		RegistShader(L"Shadowing", "ShadowVS", SHADERTYPE::VERTEX, "VSMain");
		RegistShader(L"Shadowing", "ShadowPS", SHADERTYPE::PIXEL, "PSMain");
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