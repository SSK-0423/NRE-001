#pragma once
#include <Windows.h>
#include <tchar.h>

namespace NamelessEngine::Core
{
	/// <summary>
/// ウィンドウの設定
/// </summary>
	struct AppWindowInitData
	{
		const TCHAR* windowName;	// ウィンドウ名
		LONG windowWidth;	        // ウィンドウ幅
		LONG windowHeight;	        // ウィンドウ高

		AppWindowInitData(const TCHAR* name = L"DX12アプリケーション", LONG width = 1024, LONG height = 768);
	};

	/// <summary>
	/// ウィンドウクラス
	/// </summary>
	class AppWindow
	{
	public:
		AppWindow() = default;
		~AppWindow() = default;
		/// <summary>
		/// ウィンドウ生成
		/// </summary>
		/// <param name="initData">ウィンドウ設定の構造体</param>
		void CreateAppWindow(AppWindowInitData initData);

		/// <summary>
		/// メッセージをディスパッチしてアプリを続行するか判断する
		/// </summary>
		/// <returns>true: アプリ続行 false: アプリ終了</returns>
		bool DispatchWindowMessage();

	private:
		HWND _hwnd;
		WNDCLASSEX _wndClassEx;
	public:
		/// <summary>
		/// ウィンドウハンドル取得
		/// </summary>
		/// <returns>ウィンドウハンドル</returns>
		HWND& GetHwnd() { return _hwnd; }

		/// <summary>
		/// ウィンドウサイズ取得
		/// </summary>
		/// <returns>成功: ウィンドウサイズ 失敗: 縦横0のサイズを返す</returns>
		SIZE GetWindowSize();
	};
}