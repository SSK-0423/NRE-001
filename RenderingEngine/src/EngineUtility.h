#pragma once
#include <Windows.h>
#include <wrl.h>
#include <vector>
#include <string>

/// <summary>
/// �֐��̐���/���s
/// </summary>
enum class MYRESULT
{
	SUCCESS,	//����
	FAILED		//���s
};

/// <summary>
/// MYRESULT�^�̈�����������\���Ă��邩���ׂ�
/// </summary>
/// <param name="result">�֐��̎��s����</param>
/// <returns>true: MYRESULT::SUCCESS false: MYRESULT::FAILED</returns>
inline bool IsResultSuccess(MYRESULT result) { return result == MYRESULT::SUCCESS; }

/// <summary>
/// MYRESULT�^�̈��������s��\���Ă��邩���ׂ�
/// </summary>
/// <param name="result">�֐��̎��s����</param>
/// <returns>true: MYRESULT::FAILED false: MYRESULT::SUCCESS</returns>
inline bool IsResultFailed(MYRESULT result) { return result == MYRESULT::FAILED; }

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
inline MYRESULT ReturnFailed(HRESULT result) { if (FAILED(result)) return MYRESULT::FAILED; }

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
	float color[4];

	/// <summary>
	/// �R���X�g���N�^ �f�t�H���g�F��
	/// </summary>
	/// <param name="r">��</param>
	/// <param name="g">��</param>
	/// <param name="b">��</param>
	/// <param name="a">�����x</param>
	ColorRGBA(float r = 0.f, float g = 0.f, float b = 0.f, float a = 1.f)
	{
		color[0] = r;
		color[1] = g;
		color[2] = b;
		color[3] = a;
	}
};

/// <summary>
/// �w��T�C�Y�ɃA���C�����g����
/// </summary>
/// <param name="size">�f�[�^�T�C�Y</param>
/// <param name="alignment">�A���C�����g��</param>
/// <returns></returns>
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
/// <typeparam name="T"></typeparam>
/// <param name="ptr"></param>
template <typename T>
inline void SafetyDelete(T* ptr) {
	if (ptr != nullptr) {
		delete ptr;
		ptr = nullptr;
	}
}