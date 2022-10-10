#pragma once

#include <unordered_map>

#include "Shader.h"
#include "EngineUtility.h"

namespace NamelessEngine::DX12API {
	class ShaderLibrary {
	private:
		ShaderLibrary();
		~ShaderLibrary();
		ShaderLibrary(const ShaderLibrary& inst) = delete;
		void operator=(const ShaderLibrary& inst) = delete;

		std::unordered_map<std::string, Shader> _shaderMap;

	public:
		static ShaderLibrary& Instance();

		void RegistShader(
			const TCHAR* shaderFilePass, const char* entoryPointName, 
			const char* shaderType, const std::string& shaderTag);
		
		Shader* GetShader(const std::string& shaderTag);
	};
}