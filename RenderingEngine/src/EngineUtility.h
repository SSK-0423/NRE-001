#pragma once
/// <summary>
/// 関数の成功/失敗
/// </summary>
enum class MYRESULT
{
	SUCCESS,	//成功
	FAILED		//失敗
};
/// <summary>
/// MYRESULT型の引数が成功を表しているか調べる
/// </summary>
/// <param name="result">関数の実行結果</param>
/// <returns>成功：true 失敗：false</returns>
inline bool IsResultSuccess(MYRESULT result) { return result == MYRESULT::SUCCESS; }

/// <summary>
/// MYRESULT型の引数が失敗を表しているか調べる
/// </summary>
/// <param name="result">関数の実行結果</param>
/// <returns>失敗：true 成功：false</returns>
inline bool IsResultFailed(MYRESULT result) { return result == MYRESULT::FAILED; }