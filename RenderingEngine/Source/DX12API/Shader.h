#pragma once

#include <d3d12.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")

#include "EngineUtility.h"

namespace NamelessEngine::DX12API
{
	/// <summary>
	/// �V�F�[�_�[�����ɕK�v�ȏ��
	/// </summary>
	struct ShaderData
	{
		const TCHAR* shaderFilePass = nullptr;	// �V�F�[�_�[�t�@�C���̃p�X
		const char* entoryPointName = nullptr;	// �V�F�[�_�[�̃G���g���֐�
		const char* shaderType = nullptr;       // �V�F�[�_�[�̎�ނƃo�[�W����

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="shaderFilePass">�V�F�[�_�[�t�@�C���̃p�X</param>
		/// <param name="entoryPointName">�V�F�[�_�[�̃G���g���֐�</param>
		/// <param name="shaderType">�V�F�[�_�[�̎�ނƃo�[�W����</param>
		ShaderData(const TCHAR* shaderFilePass, const char* entoryPointName, const char* shaderType)
			: shaderFilePass(shaderFilePass), entoryPointName(entoryPointName), shaderType(shaderType)
		{}

		ShaderData() {};
	};

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
		Utility::MYRESULT Create(const TCHAR* shaderFilePass, const char* entoryPointName, const char* shaderType);

		/// <summary>
		/// �V�F�[�_�[����
		/// </summary>
		/// <param name="shaderData">�V�F�[�_�[�f�[�^</param>
		/// <returns></returns>
		Utility::MYRESULT Create(const ShaderData& shaderData);

		/// <summary>
		/// �V�F�[�_�[�擾
		/// </summary>
		/// <returns>�V�F�[�_�[</returns>
		ID3DBlob& GetShader() const {
			return *_shaderBlob.Get();
		}
	};
}