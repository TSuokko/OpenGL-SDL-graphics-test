#include "Zombie.h"
#include "Human.h"
#include <ctime>
#include <random>
#include <glm/gtx/rotate_vector.hpp>
#include <DevyEngine/ResourceManager.h>

#include <vector>
#include <algorithm>
#include <set>
#include <list>
#include <queue>
#include <math.h>


Zombie::Zombie()
{
}


Zombie::~Zombie()
{
}

void Zombie::init(float speed, glm::vec2 pos)
{
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

	_color.r = 255;
	_color.g = 55;
	_color.b = 55;
	_color.a = 255;

	_speed = speed;
	_position = pos;
	// Get random direction
	_direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
	// Make sure direction isn't zero
	if (_direction.length() == 0) _direction = glm::vec2(1.0f, 0.0f);

	_direction = glm::normalize(_direction);
}

void Zombie::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies)
{
	/*Human* playerClose = chasePlayer(humans);

	if (playerClose != nullptr)
	{
		_direction = glm::normalize(playerClose->getPosition() - _position);
		_position += _direction * _speed;
	}*/
	collideWithLevel(levelData);

}

Human* Zombie::chasePlayer(std::vector<Human*>& humans)
{
	Human* playerNear = nullptr;
	float smallestDistance = 999999.0f;

	for (int i = 0; i < humans.size(); i++)
	{
		glm::vec2 distVec = humans[i]->getPosition() - _position;
		// Get distance
		float distance = glm::length(distVec);

		// If this person is closer than the closest person, this is the new closest
		if (distance < smallestDistance) 
		{
			smallestDistance = distance;
			playerNear = humans[i];
		}

	}
	return playerNear;
}

void Zombie::aStar(std::vector<Node> path)
{
	for (int i = 0; i < path.size(); i++)
	{
		_direction.x = path[i].x;
		_direction.y = path[i].y;
		//Luo metodi jolla se liikkuu nodesta nodeen.
	}
	
}
