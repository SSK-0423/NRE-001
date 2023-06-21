#include "Input.h"

#define SafetyRelease(x) { if(x != nullptr) {x->Release(); x = nullptr;} }
#define SafetyReleaseDevice(x) { if(x != nullptr) { x->Unacquire(); x->Release(); x = nullptr; }}

using namespace NamelessEngine::Utility;

constexpr BYTE ON_VALUE = 0x80;

namespace NamelessEngine::Core
{
	Input::Input()
	{
		memset(_currentKeys, 0, sizeof(_currentKeys));
		memset(_prevKeys, 0, sizeof(_prevKeys));
		memset(_keysState, static_cast<int>(BUTTON_STATE::NONE), sizeof(_keysState));

		memset(&_currentMouse, 0, sizeof(_currentMouse));
		memset(&_prevMouse, 0, sizeof(_prevMouse));
		memset(_mouseButtonState, static_cast<int>(BUTTON_STATE::NONE), sizeof(_mouseButtonState));
	}
	Input::~Input()
	{
		SafetyReleaseDevice(_keyboard);
		SafetyReleaseDevice(_mouse);
		SafetyRelease(_directInputInterface);
	}
	Input& Input::Instance()
	{
		static Input input;
		return input;
	}

	RESULT Input::Init(HWND hwnd)
	{
		_hwnd = hwnd;

		// IDirectInputインターフェース生成
		HRESULT result = DirectInput8Create(
			GetModuleHandle(0), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&_directInputInterface, NULL);
		if (FAILED(result)) { return RESULT::FAILED; }

		result = SetupKeyboard(hwnd);
		if (FAILED(result)) { return RESULT::FAILED; }

		result = SetupMouse(hwnd);
		if (FAILED(result)) { return RESULT::FAILED; }

		return RESULT::SUCCESS;
	}
	HRESULT Input::SetupKeyboard(HWND& hwnd)
	{
		// キーボードデバイス生成
		HRESULT result = _directInputInterface->CreateDevice(GUID_SysKeyboard, &_keyboard, NULL);
		if (FAILED(result)) { return result; }
		// デバイスのフォーマット指定
		result = _keyboard->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(result)) { return result; }
		// 協調モード設定
		result = _keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(result)) { return result; }
		// キーボードの入力読み取り開始
		result = _keyboard->Acquire();
		if (FAILED(result)) { return result; }

		return result;
	}
	HRESULT Input::SetupMouse(HWND& hwnd)
	{
		// マウスデバイス生成
		HRESULT result = _directInputInterface->CreateDevice(GUID_SysMouse, &_mouse, NULL);
		if (FAILED(result)) { return result; }
		// マウスのフォーマット指定
		result = _mouse->SetDataFormat(&c_dfDIMouse);
		if (FAILED(result)) { return result; }
		// 協調モード設定
		result = _mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(result)) { return result; }
		// マウスの入力読み取り開始
		result = _mouse->Acquire();
		if (FAILED(result)) { return result; }
		// ポーリング開始
		result = _mouse->Poll();
		if (FAILED(result)) { return result; }

		return result;
	}

	void Input::Update()
	{
		UpdateKeyStates();
		UpdateMouseStates();
	}

	void Input::UpdateKeyStates()
	{
		// 1フレーム前のキー情報を退避する
		memcpy_s(_prevKeys, sizeof(_prevKeys), _currentKeys, sizeof(_currentKeys));
		// 最新のキーの押下状態を取得する
		HRESULT result = _keyboard->GetDeviceState(sizeof(_currentKeys), _currentKeys);
		if (FAILED(result)) {
			_keyboard->Acquire();
			_keyboard->GetDeviceState(sizeof(_currentKeys), _currentKeys);
		}

		// キーの状態更新
		for (UINT index = 0; index < sizeof(_currentKeys); index++) {
			// ボタンが押されている場合は1バイトの最上位ビットが1となるため,0x80とアンド演算を取る
			if (_currentKeys[index] & ON_VALUE) {
				// キーが押され続けている
				if (_prevKeys[index] & ON_VALUE)
					_keysState[index] = BUTTON_STATE::HOLD;
				// キーが押された
				else
					_keysState[index] = BUTTON_STATE::DOWN;
			}
			else {
				// キーが離された
				if (_prevKeys[index] & ON_VALUE)
					_keysState[index] = BUTTON_STATE::UP;
				// キーが押されていない
				else
					_keysState[index] = BUTTON_STATE::NONE;
			}
		}
	}
	void Input::UpdateMouseStates()
	{
		// 1フレーム前のキー情報を退避する
		_prevMouse = _currentMouse;
		// 最新のキーの押下状態を取得する
		HRESULT result = _mouse->GetDeviceState(sizeof(DIMOUSESTATE), &_currentMouse);
		if (FAILED(result)) { return; }

		for (UINT index = 0; index < sizeof(_currentMouse.rgbButtons); index++) {
			if (_currentMouse.rgbButtons[index] & ON_VALUE) {
				// ボタンが押され続けている
				if (_prevMouse.rgbButtons[index] & ON_VALUE)
					_mouseButtonState[index] = BUTTON_STATE::HOLD;
				// ボタンが押された
				else
					_mouseButtonState[index] = BUTTON_STATE::DOWN;
			}
			else {
				// キーが離された
				if (_prevMouse.rgbButtons[index] & ON_VALUE)
					_mouseButtonState[index] = BUTTON_STATE::UP;
				// キーが押されていない
				else
					_mouseButtonState[index] = BUTTON_STATE::NONE;
			}
		}

		_mouseXMovement = _currentMouse.lX;
		_mouseYMovement = _currentMouse.lY;
		_mouseWheelMovement = _currentMouse.lZ;	// マウスホイールこれであってるかわからない
	}

	BUTTON_STATE Input::GetMouseState(MOUSECODE mouseCode)
	{
		return _mouseButtonState[static_cast<UINT>(mouseCode)];
	}
	BUTTON_STATE Input::GetKeyboradState(KEYCODE keyCode)
	{
		return _keysState[keyCode];
	}

	float Input::GetMouseXMovement()
	{
		return _mouseXMovement;
	}

	float Input::GetMouseYMovement()
	{
		return _mouseYMovement;
	}

	float Input::GetMouseWheelMovement()
	{
		return _mouseWheelMovement;
	}

	void Input::ViewMouseCursor()
	{
		HRESULT result = _mouse->SetCooperativeLevel(_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		result = _mouse->Acquire();
		result = _mouse->Poll();
	}

}
