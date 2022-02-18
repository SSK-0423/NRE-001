#pragma once
#include <Windows.h>
#include <wrl.h>

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

/// <summary>
/// HRESULT型の引数が失敗を表しているか調べる
/// </summary>
/// <param name="result">関数の実行結果</param>
/// <returns>true: 失敗 false: S_OK</returns>
inline bool IsHresultFailed(HRESULT result) { return FAILED(result); }

/// <summary>
/// 関数の実行が失敗した場合に失敗を表す値を返し、関数の実行を中断する
/// </summary>
/// <param name="result">関数の実行結果</param>
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
	float _color[4];

	/// <summary>
	/// コンストラクタ デフォルト：黒
	/// </summary>
	/// <param name="r">赤</param>
	/// <param name="g">緑</param>
	/// <param name="b">青</param>
	/// <param name="a">透明度</param>
	ColorRGBA(float r = 0.f, float g = 0.f, float b = 0.f, float a = 1.f)
	{
		_color[0] = r;
		_color[1] = g;
		_color[2] = b;
		_color[3] = a;
	}
};