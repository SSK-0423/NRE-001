#pragma once
#include "EngineUtility.h"

class Dx12GraphicsEngine
{
public:
	Dx12GraphicsEngine();
	~Dx12GraphicsEngine();

	/// <summary>
	/// DirectX12の初期化
	/// </summary>
	/// <returns>成功：MYRESULT::SUCCESS 失敗：MYRESULT::FAILED</returns>
	MYRESULT Init();
};

/// メモ
/// MiniEngineはグローバル変数でポインタを持っていたのでよろしくない
/// 同様のことをするならシングルトンの方が安全→デストラクタがprivateになっているのでどこかでdeleteされる心配がない