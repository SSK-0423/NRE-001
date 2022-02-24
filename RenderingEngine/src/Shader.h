#pragma once

#include <d3d12.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")

#include "EngineUtility.h"

/// <summary>
/// シェーダークラス
/// </summary>
class Shader {
public:
	Shader() = default;
	~Shader() = default;

private:
	Microsoft::WRL::ComPtr<ID3DBlob> _shaderBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> _errorBlob = nullptr;

	/// <summary>
	/// シェーダーをコンパイル
	/// </summary>
	/// <param name="shaderFilePass">シェーダーのファイルパス</param>
	/// <param name="entoryPointName">シェーダーのエントリ関数名</param>
	/// <param name="shaderType">シェーダーの種類_バージョン</param>
	/// <returns></returns>
	HRESULT CompileShader(const TCHAR* shaderFilePass, const char* entoryPointName, const char* shaderTypeAndVersion);

public:
	/// <summary>
	/// シェーダー生成
	/// </summary>
	/// <param name="shaderFilePass">シェーダーのファイルパス</param>
	/// <param name="entoryPointName">シェーダーのエントリ関数名</param>
	/// <param name="shaderType">シェーダーの種類_モデル</param>
	/// <returns></returns>
	MYRESULT Create(const TCHAR* shaderFilePass, const char* entoryPointName, const char* shaderType);

	/// <summary>
	/// シェーダー取得
	/// </summary>
	/// <returns>シェーダー</returns>
	ID3DBlob& GetShader() const {
		return *_shaderBlob.Get();
	}
};