#include "Input.h"

/// <summary>
/// Constructor
/// </summary>
Input::Input() {
	//Set all key states to none
	ZeroMemory(_asciiKeys, sizeof(_asciiKeys));
}

/// <summary>
/// Destructor
/// </summary>
Input::~Input() {
	
}

/// <summary>
/// Returns true if a key is pressed, false if not
/// </summary>
/// <param name="key">Key to check</param>
/// <returns></returns>
bool Input::isKeyDown(KEYS key) {
	const auto state = _asciiKeys[static_cast<int>(key)];
	return state >= pressed;
}

/// <summary>
/// Returns true, only if a key is held down
/// </summary>
/// <param name="key">Key to check</param>
/// <returns></returns>
bool Input::isKeyHeld(KEYS key) {
	const auto state = _asciiKeys[static_cast<int>(key)];
	return state == held;
}

/// <summary>
/// Returns the instance of this class
/// </summary>
/// <returns></returns>
Input& Input::getInstance() {
	static Input instance;
	return instance;
}

/// <summary>
/// Handles the given keyboard message
/// </summary>
/// <param name="keyboard_message"></param>
bool Input::tryHandleKeyboardMessage(const MSG& keyboard_message) {
	auto wasHandled = false;
	switch(keyboard_message.message) {
		case WM_KEYDOWN: 
		{
			//Key was pressed
			const auto c            = MapVirtualKey(keyboard_message.wParam, MAPVK_VK_TO_CHAR);
			const auto code         = static_cast<int>(c);
			const auto currentState = static_cast<int>(_asciiKeys[code]);

			_asciiKeys[code] = currentState + 1 > static_cast<int>(held) ?
				held : static_cast<KEY_STATE>(currentState + 1);
			wasHandled = true;
			break;
		}
		case WM_KEYUP: 
		{
			//Key was released
			const auto c = MapVirtualKey(keyboard_message.wParam, MAPVK_VK_TO_CHAR);
			const auto code = static_cast<int>(c);

			_asciiKeys[code] = none;
			wasHandled = true;
			break;
		}
	}
	return wasHandled;
}


