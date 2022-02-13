#pragma once

#include <Windows.h>
#include <memory>

#include "EngineUtility.h"

class Dx12GraphicsEngine;

/// <summary>
/// レンダリング開発中のテスト用クラス
/// </summary>
class Dx12Application
{
public:
	Dx12Application();
	~Dx12Application();

	/// <summary>
	/// アプリケーションの初期化
	/// </summary>
	/// <returns>成功：MYRESULT::SUCCESS 失敗：MYRESULT::FAILED</returns>
	MYRESULT Init();

	/// <summary>
	/// メインループ
	/// </summary>
	void Run();

	/// <summary>
	/// 終了処理
	/// </summary>
	void End();

private:
	Dx12GraphicsEngine* _graphicsEngine = nullptr;
};

/// メモ
/// DirectXの初期化にウィンドウハンドル必要(HWND)
/// →シングルトン化してウィンドウサイズ取得できるようにしたい
/// →色んなクラスからAppicationクラスを呼び出す羽目になる
/// →アプリケーションクラスはアプリケーション固有の処理を記述するためのクラス
///	→システム側のクラスがアプリケーションクラスと関係を持つことは避ける
/// →main.cppでのみ呼び出すような設計にする→シングルトン化する必要なし