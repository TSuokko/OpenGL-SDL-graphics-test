#include "Human.h"
#include <DevyEngine\Vertex.h>

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

	//we set a different color for humans
	_color.setColor(255, 55, 55, 255);

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