#pragma once
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