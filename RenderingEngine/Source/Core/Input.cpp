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

		// IDirectInput�C���^�[�t�F�[�X����
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
		// �L�[�{�[�h�f�o�C�X����
		HRESULT result = _directInputInterface->CreateDevice(GUID_SysKeyboard, &_keyboard, NULL);
		if (FAILED(result)) { return result; }
		// �f�o�C�X�̃t�H�[�}�b�g�w��
		result = _keyboard->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(result)) { return result; }
		// �������[�h�ݒ�
		result = _keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(result)) { return result; }
		// �L�[�{�[�h�̓��͓ǂݎ��J�n
		result = _keyboard->Acquire();
		if (FAILED(result)) { return result; }

		return result;
	}
	HRESULT Input::SetupMouse(HWND& hwnd)
	{
		// �}�E�X�f�o�C�X����
		HRESULT result = _directInputInterface->CreateDevice(GUID_SysMouse, &_mouse, NULL);
		if (FAILED(result)) { return result; }
		// �}�E�X�̃t�H�[�}�b�g�w��
		result = _mouse->SetDataFormat(&c_dfDIMouse);
		if (FAILED(result)) { return result; }
		// �������[�h�ݒ�
		result = _mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(result)) { return result; }
		// �}�E�X�̓��͓ǂݎ��J�n
		result = _mouse->Acquire();
		if (FAILED(result)) { return result; }
		// �|�[�����O�J�n
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
		// 1�t���[���O�̃L�[����ޔ�����
		memcpy_s(_prevKeys, sizeof(_prevKeys), _currentKeys, sizeof(_currentKeys));
		// �ŐV�̃L�[�̉�����Ԃ��擾����
		HRESULT result = _keyboard->GetDeviceState(sizeof(_currentKeys), _currentKeys);
		if (FAILED(result)) {
			_keyboard->Acquire();
			_keyboard->GetDeviceState(sizeof(_currentKeys), _currentKeys);
		}

		// �L�[�̏�ԍX�V
		for (UINT index = 0; index < sizeof(_currentKeys); index++) {
			// �{�^����������Ă���ꍇ��1�o�C�g�̍ŏ�ʃr�b�g��1�ƂȂ邽��,0x80�ƃA���h���Z�����
			if (_currentKeys[index] & ON_VALUE) {
				// �L�[�������ꑱ���Ă���
				if (_prevKeys[index] & ON_VALUE)
					_keysState[index] = BUTTON_STATE::HOLD;
				// �L�[�������ꂽ
				else
					_keysState[index] = BUTTON_STATE::DOWN;
			}
			else {
				// �L�[�������ꂽ
				if (_prevKeys[index] & ON_VALUE)
					_keysState[index] = BUTTON_STATE::UP;
				// �L�[��������Ă��Ȃ�
				else
					_keysState[index] = BUTTON_STATE::NONE;
			}
		}
	}
	void Input::UpdateMouseStates()
	{
		// 1�t���[���O�̃L�[����ޔ�����
		_prevMouse = _currentMouse;
		// �ŐV�̃L�[�̉�����Ԃ��擾����
		HRESULT result = _mouse->GetDeviceState(sizeof(DIMOUSESTATE), &_currentMouse);
		if (FAILED(result)) { return; }

		for (UINT index = 0; index < sizeof(_currentMouse.rgbButtons); index++) {
			if (_currentMouse.rgbButtons[index] & ON_VALUE) {
				// �{�^���������ꑱ���Ă���
				if (_prevMouse.rgbButtons[index] & ON_VALUE)
					_mouseButtonState[index] = BUTTON_STATE::HOLD;
				// �{�^���������ꂽ
				else
					_mouseButtonState[index] = BUTTON_STATE::DOWN;
			}
			else {
				// �L�[�������ꂽ
				if (_prevMouse.rgbButtons[index] & ON_VALUE)
					_mouseButtonState[index] = BUTTON_STATE::UP;
				// �L�[��������Ă��Ȃ�
				else
					_mouseButtonState[index] = BUTTON_STATE::NONE;
			}
		}

		_mouseXMovement = _currentMouse.lX;
		_mouseYMovement = _currentMouse.lY;
		_mouseWheelMovement = _currentMouse.lZ;	// �}�E�X�z�C�[������ł����Ă邩�킩��Ȃ�
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
