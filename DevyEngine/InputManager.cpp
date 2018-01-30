#include "InputManager.h"
namespace DevyEngine
{
	InputManager::InputManager(): _mouseCoords(0,0)
	{
	}


	InputManager::~InputManager()
	{
	}

	void InputManager::keyPress(unsigned int keyID)
	{
		_keyMap[keyID] = true;
	}

	void InputManager::keyRelease(unsigned int keyID)
	{
		_keyMap[keyID] = false;
	}

	void InputManager::setMouseCoords(float x, float y)
	{
		_mouseCoords.x = x;
		_mouseCoords.y = y;
	}

	bool InputManager::isKeyPressed(unsigned int keyID)
	{
		auto it = _keyMap.find(keyID);
		if (it != _keyMap.end())
		{
			return it->second;
		}
		else {
			return false;
		}
	}


}
