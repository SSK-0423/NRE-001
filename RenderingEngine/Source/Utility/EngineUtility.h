#pragma once
#include <Windows.h>
#include <wrl.h>
#include <vector>
#include <string>

namespace NamelessEngine::Utility
{
	/// <summary>
	/// 関数の成功/失敗
	/// </summary>
	enum class RESULT
	{
		SUCCESS,	//成功
		FAILED		//失敗
	};

	/// <summary>
	/// RESULT型の引数が成功を表しているか調べる
	/// </summary>
	/// <param name="result">関数の実行結果</param>
	/// <returns>true: RESULT::SUCCESS false: RESULT::FAILED</returns>
	inline bool IsResultSuccess(RESULT result) { return result == RESULT::SUCCESS; }

	/// <summary>
	/// RESULT型の引数が失敗を表しているか調べる
	/// </summary>
	/// <param name="result">関数の実行結果</param>
	/// <returns>true: RESULT::FAILED false: RESULT::SUCCESS</returns>
	inline bool IsResultFailed(RESULT result) { return result == RESULT::FAILED; }

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
		/// コンストラクタ デフォルト：黒
		/// </summary>
		/// <param name="r">赤</param>
		/// <param name="g">緑</param>
		/// <param name="b">青</param>
		/// <param name="a">透明度</param>
		ColorRGBA(float r = 0.f, float g = 0.f, float b = 0.f, float a = 1.f)
			: r(r), g(g), b(b), a(a)
		{}
	};

	/// <summary>
	/// 指定サイズにアライメントする
	/// </summary>
	/// <param name="size">データサイズ</param>
	/// <param name="alignment">アライメント数</param>
	/// <returns>アライメントされたサイズ</returns>
	inline size_t AlignmentedSize(size_t size, size_t alignment) {
		return size + alignment - size % alignment;
	}

	/// <summary>
	/// vectorの総サイズを返す
	/// </summary>
	/// <typeparam name="T">要素の型</typeparam>
	/// <param name="vec">vector</param>
	/// <returns>vectorの総サイズ</returns>
	template <typename T>
	inline UINT SizeofVector(const std::vector<T>& vec) {
		return static_cast<UINT>(sizeof(T) * vec.size());
	}

	/// <summary>
	/// 文字列置換
	/// </summary>
	/// <param name="str">文字列</param>
	/// <param name="target">置換対象</param>
	/// <param name="replacement">置換後の文字列</param>
	/// <returns>置換処理後の文字列</returns>
	std::string ReplaceString(std::string str, std::string target, std::string replacement);

	/// <summary>
	/// ファイル拡張子取得
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <returns>「.」を除いた拡張子</returns>
	std::string GetExtension(std::string fileName);

	/// <summary>
	/// ファイル拡張子取得(ワイド文字)
	/// </summary>
	/// <param name="fileName">ファイル名(ワイド文字)</param>
	/// <returns>「.」を除いた拡張子</returns>
	std::wstring GetExtension(std::wstring fileName);

	/// <summary>
	/// char型⇒wchar_tへの変換
	/// </summary>
	/// <param name="src">変換元文字列</param>
	/// <param name="dst">変換後の文字列の格納先</param>
	/// <param name="dstSize">変換後文字列のサイズ</param>
	/// <returns>正常終了:0 失敗:エラーコード</returns>
	errno_t charToWchar(const char* src, wchar_t* dst, const size_t dstSize);

	/// <summary>
	/// nullptrチェックを行うdelete
	/// </summary>
	/// <typeparam name="T">解放するポインタタイプ</typeparam>
	/// <param name="ptr>解放するポインタ</param>
	template <typename T>
	inline void SafetyDelete(T* ptr) {
		if (ptr != nullptr) {
			delete ptr;
			ptr = nullptr;
		}
	}
}