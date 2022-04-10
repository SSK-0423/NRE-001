#pragma once

#include "EngineUtility.h"


class Dx12GraphicsEngine;
class AppWindow;

/// <summary>
/// DirectX12のアプリケーションのインターフェース
/// </summary>
class Dx12ApplicationImpl {
public:
	/// <summary>
	/// アプリケーションの初期化
	/// </summary>
	/// <param name="graphicsEngine">グラフィックスエンジン</param>
	/// <param name="appWindow">ウィンドウ</param>
	/// <returns></returns>
	virtual MYRESULT Init(Dx12GraphicsEngine& graphicsEngine, AppWindow& appWindow) = 0;

	/// <summary>
	/// アプリケーションの更新処理
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// アプリケーションの描画処理
	/// </summary>
	virtual void Draw(Dx12GraphicsEngine& graphicsEngine) = 0;

	/// <summary>
	/// アプリケーションの終了処理
	/// </summary>
	virtual void Final() = 0;
};