#pragma once
/// <summary>
/// シングルトンクラス
/// </summary>
/// <typeparam name="T">シングルトン化するクラス名</typeparam>
template <typename T>
class Singleton
{
protected:
	Singleton() {}
	virtual ~Singleton() {}
	Singleton(const Singleton& inst) {}
	Singleton& operator=(const Singleton& inst){}
public:
	/// <summary>
	/// クラスのシングルトンインスタンス取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static T& Instance()
	{
		static T inst;
		return inst;
	}
};