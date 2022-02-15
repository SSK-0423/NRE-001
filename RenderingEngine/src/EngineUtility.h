#pragma once
#include <Windows.h>
#include <wrl.h>

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