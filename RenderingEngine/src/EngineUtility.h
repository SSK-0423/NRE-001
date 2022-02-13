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
/// <returns>true: MYRESULT::SUCCESS false: MYRESULT::FAILED</returns>
inline bool IsResultSuccess(MYRESULT result) { return result == MYRESULT::SUCCESS; }

/// <summary>
/// MYRESULT型の引数が失敗を表しているか調べる
/// </summary>
/// <param name="result">関数の実行結果</param>
/// <returns>true: MYRESULT::FAILED false: MYRESULT::SUCCESS</returns>
inline bool IsResultFailed(MYRESULT result) { return result == MYRESULT::FAILED; }