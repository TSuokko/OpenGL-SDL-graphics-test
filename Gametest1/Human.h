#pragma once
#include "Agent.h"
#include <ctime>
#include <random>
#include <glm/gtx/rotate_vector.hpp>
#include <DevyEngine/ResourceManager.h>
class Player;

class Human :public Agent
{
public:
	Human();
	~Human();

	void init(float speed, glm::vec2 pos);

	virtual void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies);

private:
	glm::vec2 _direction;
	int frames;
};

