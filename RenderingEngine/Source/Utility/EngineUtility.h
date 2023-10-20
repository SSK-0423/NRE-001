#pragma once
#include <Windows.h>
#include <wrl.h>
#include <vector>
#include <string>

namespace NamelessEngine::Utility
{
	/// <summary>
	/// �֐��̐���/���s
	/// </summary>
	enum class RESULT
	{
		SUCCESS,	//����
		FAILED		//���s
	};

	/// <summary>
	/// RESULT�^�̈�����������\���Ă��邩���ׂ�
	/// </summary>
	/// <param name="result">�֐��̎��s����</param>
	/// <returns>true: RESULT::SUCCESS false: RESULT::FAILED</returns>
	inline bool IsResultSuccess(RESULT result) { return result == RESULT::SUCCESS; }

	/// <summary>
	/// RESULT�^�̈��������s��\���Ă��邩���ׂ�
	/// </summary>
	/// <param name="result">�֐��̎��s����</param>
	/// <returns>true: RESULT::FAILED false: RESULT::SUCCESS</returns>
	inline bool IsResultFailed(RESULT result) { return result == RESULT::FAILED; }

	/// <summary>
	/// HRESULT�^�̈��������s��\���Ă��邩���ׂ�
	/// </summary>
	/// <param name="result">�֐��̎��s����</param>
	/// <returns>true: ���s false: S_OK</returns>
	inline bool IsHresultFailed(HRESULT result) { return FAILED(result); }

	/// <summary>
	/// �֐��̎��s�����s�����ꍇ�Ɏ��s��\���l��Ԃ��A�֐��̎��s�𒆒f����
	/// </summary>
	/// <param name="result">�֐��̎��s����</param>
	/// <returns></returns>
	inline RESULT ReturnFailed(HRESULT result) { if (FAILED(result)) return RESULT::FAILED; }

	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="obj"></param>
	template <class T>
	inline void ReleaseComObj(Microsoft::WRL::ComPtr<T> obj) {
		if (obj.Get() != nullptr) {}
	}

	/// <summary>
	/// 
	/// </summary>
	struct ColorRGBA
	{
		float r;
		float g;
		float b;
		float a;
		/// <summary>
		/// �R���X�g���N�^ �f�t�H���g�F��
		/// </summary>
		/// <param name="r">��</param>
		/// <param name="g">��</param>
		/// <param name="b">��</param>
		/// <param name="a">�����x</param>
		ColorRGBA(float r = 0.f, float g = 0.f, float b = 0.f, float a = 1.f)
			: r(r), g(g), b(b), a(a)
		{}
	};

	/// <summary>
	/// �w��T�C�Y�ɃA���C�����g����
	/// </summary>
	/// <param name="size">�f�[�^�T�C�Y</param>
	/// <param name="alignment">�A���C�����g��</param>
	/// <returns>�A���C�����g���ꂽ�T�C�Y</returns>
	inline size_t AlignmentedSize(size_t size, size_t alignment) {
		return size + alignment - size % alignment;
	}

	/// <summary>
	/// vector�̑��T�C�Y��Ԃ�
	/// </summary>
	/// <typeparam name="T">�v�f�̌^</typeparam>
	/// <param name="vec">vector</param>
	/// <returns>vector�̑��T�C�Y</returns>
	template <typename T>
	inline UINT SizeofVector(const std::vector<T>& vec) {
		return static_cast<UINT>(sizeof(T) * vec.size());
	}

	/// <summary>
	/// ������u��
	/// </summary>
	/// <param name="str">������</param>
	/// <param name="target">�u���Ώ�</param>
	/// <param name="replacement">�u����̕�����</param>
	/// <returns>�u��������̕�����</returns>
	std::string ReplaceString(std::string str, std::string target, std::string replacement);

	/// <summary>
	/// �t�@�C���g���q�擾
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	/// <returns>�u.�v���������g���q</returns>
	std::string GetExtension(std::string fileName);

	/// <summary>
	/// �t�@�C���g���q�擾(���C�h����)
	/// </summary>
	/// <param name="fileName">�t�@�C����(���C�h����)</param>
	/// <returns>�u.�v���������g���q</returns>
	std::wstring GetExtension(std::wstring fileName);

	/// <summary>
	/// char�^��wchar_t�ւ̕ϊ�
	/// </summary>
	/// <param name="src">�ϊ���������</param>
	/// <param name="dst">�ϊ���̕�����̊i�[��</param>
	/// <param name="dstSize">�ϊ��㕶����̃T�C�Y</param>
	/// <returns>����I��:0 ���s:�G���[�R�[�h</returns>
	errno_t charToWchar(const char* src, wchar_t* dst, const size_t dstSize);

	/// <summary>
	/// nullptr�`�F�b�N���s��delete
	/// </summary>
	/// <typeparam name="T">�������|�C���^�^�C�v</typeparam>
	/// <param name="ptr>�������|�C���^</param>
	template <typename T>
	inline void SafetyDelete(T* ptr) {
		if (ptr != nullptr) {
			delete ptr;
			ptr = nullptr;
		}
	}
}