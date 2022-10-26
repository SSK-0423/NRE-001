#pragma once
#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include "AppWindow.h"
#include "EngineUtility.h"

namespace NamelessEngine::Core {

	constexpr UINT KEY_NUM = 256;
	constexpr UINT MOUSE_BUTTON_NUM = 4;

	typedef BYTE KEYCODE;

	enum class BUTTON_STATE {
		DOWN = 0,
		UP,
		HOLD,
		NONE = -1,
	};

	enum class MOUSECODE {
		LEFT = 0,
		RIGHT = 1,
		WHEEL = 2,
	};

	class Input {
	private:
		Input();
		~Input();
		Input(const Input& inst) = delete;
		void operator=(const Input& inst) = delete;

	public:
		static Input& Instance();

		Utility::RESULT Init(AppWindow& window);

		void Update();

		BUTTON_STATE GetMouseState(MOUSECODE mouseCode);
		BUTTON_STATE GetKeyboradState(KEYCODE keyCode);
		float GetMouseXMovement();
		float GetMouseYMovement();
		float GetMouseWheelMovement();

	private:
		// IDirectInput8インターフェース
		LPDIRECTINPUT8 _directInputInterface = nullptr;
		// キーボード用デバイス
		LPDIRECTINPUTDEVICE8 _keyboard = nullptr;
		// マウス用デバイス
		LPDIRECTINPUTDEVICE8 _mouse = nullptr;

		HRESULT SetupKeyboard(HWND& hwnd);
		HRESULT SetupMouse(HWND& hwnd);

		void UpdateKeyStates();
		void UpdateMouseStates();

		BYTE _currentKeys[KEY_NUM];
		BYTE _prevKeys[KEY_NUM];
		BUTTON_STATE _keysState[KEY_NUM];
		
		DIMOUSESTATE _currentMouse;
		DIMOUSESTATE _prevMouse;
		BUTTON_STATE _mouseButtonState[MOUSE_BUTTON_NUM];
		float _mouseXMovement;
		float _mouseYMovement;
		float _mouseWheelMovement;
	};
}