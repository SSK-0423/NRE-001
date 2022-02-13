#pragma once
#include "EngineUtility.h"

class Dx12GraphicsEngine
{
private:
	Dx12GraphicsEngine();
	~Dx12GraphicsEngine();
	Dx12GraphicsEngine(const Dx12GraphicsEngine& inst) = delete;
	void operator=(const Dx12GraphicsEngine& inst) = delete;

public:
	/// <summary>
	/// クラスのシングルトンインスタンス取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static Dx12GraphicsEngine& Instance();

	/// <summary>
	/// DirectX12の初期化
	/// </summary>
	/// <returns>MYRESULT::SUCCESS: 成功 MYRESULT::FAILED: 失敗</returns>
	MYRESULT Init();
};

/// メモ
/// MiniEngineはグローバル変数でポインタを持っていたのでよろしくない
/// 同様のことをするならシングルトンの方が安全→デストラクタがprivateになっているのでどこかでdeleteされる心配がない