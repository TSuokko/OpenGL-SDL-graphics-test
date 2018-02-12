#include "Human.h"
#include <ctime>
#include <random>
#include <glm/gtx/rotate_vector.hpp>
#include <DevyEngine/ResourceManager.h>

Human::Human() :
	frames(0)
{
}
Human::~Human()
{
}

void Human::init(float speed, glm::vec2 pos) {

	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

	//_health = 20;

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

void Human::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies)
{

	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randRotate(-10.0f, 10.0f);

	_position += _direction * _speed;

	if (frames == 60)
	{
		_direction = glm::rotate(_direction, randRotate(randomEngine));
		frames = 0;
	}
	else
	{
		frames++;
	}
	if (collideWithLevel(levelData))
	{
		_direction = glm::rotate(_direction, randRotate(randomEngine));
	}
	
}