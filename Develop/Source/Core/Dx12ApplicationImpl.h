#pragma once

#include "NamelessEngine.h"

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
	virtual NamelessEngine::Utility::RESULT Init(
		NamelessEngine::Core::Dx12GraphicsEngine& graphicsEngine,
		NamelessEngine::Core::AppWindow& appWindow) = 0;

	/// <summary>
	/// アプリケーションの更新処理
	/// </summary>
	virtual void Update(float deltaTime) = 0;

	/// <summary>
	/// アプリケーションの描画処理
	/// </summary>
	virtual void Draw(NamelessEngine::Core::Dx12GraphicsEngine& graphicsEngine) = 0;

	/// <summary>
	/// アプリケーションの終了処理
	/// </summary>
	virtual void Final() = 0;
};