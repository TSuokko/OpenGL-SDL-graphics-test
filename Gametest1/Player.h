#pragma once
#include <glm\glm.hpp>
#include "Agent.h"
#include <DevyEngine\InputManager.h>

class Player : public Agent
{
public:
	Player();
	~Player();

	void init(float speed, glm::vec2 position, DevyEngine::InputManager* input);
	void update(const std::vector<std::string>& levelData,
		std::vector<Zombie*>& zombies);

private:
	DevyEngine::InputManager* _input;

};

