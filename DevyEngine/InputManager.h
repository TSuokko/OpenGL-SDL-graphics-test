#pragma once
#include <unordered_map>
#include <glm\glm.hpp>
namespace DevyEngine
{
	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		void setMouseCoords(float x, float y);
		void keyPress(unsigned int keyID);
		void keyRelease(unsigned int keyID);
		bool isKeyPressed(unsigned int keyID);

		glm::vec2 getMouseCoords() const { return _mouseCoords; }
	private:
		std::unordered_map<unsigned int, bool> _keyMap;
		glm::vec2 _mouseCoords;

	};
}
