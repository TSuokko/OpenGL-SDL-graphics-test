#pragma once
#include <glm\glm.hpp>
#include "Agent.h"
#include <DevyEngine\InputManager.h>
#include "Human.h"
#include "Node.h"
class Gun;
class Player : public Human
{
public:
	Player();
	~Player();

	void init(float speed, glm::vec2 position, DevyEngine::InputManager* input);
	void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies);

	void addGun(Gun* gun);

private:
	DevyEngine::InputManager* _input;
	std::vector <Gun*>_guns;
	int _currentGunIndex;
};

