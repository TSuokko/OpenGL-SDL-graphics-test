#include "Gun.h"
#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>

Gun::Gun(std::string name, int fireRate, int bps, float spread, float bd, float bs):
	_name(name), 
	_fireRate(fireRate),
	_bulletsPerShot(bps), 
	_spread(spread),
	_bulletDamage(bd), 
	_bulletSpeed(bs), 
	_frameCounter(0){}


Gun::~Gun()
{
}


void Gun::update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets)
{
	_frameCounter++;
	if (_frameCounter > _fireRate && isMouseDown)
	{
		
		fire(position, direction, bullets);
		_frameCounter = 0;
	}
}

void Gun::fire(const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets)
{
	static std::mt19937 randomEngine(time(nullptr));
	std::uniform_real_distribution<float> randRotate(-_spread, _spread);

	//printf("Direction: %d %d\n", direction.x, direction.y);
	std::cout << direction.x << " " << direction.y << std::endl;

	for (int i = 0; i < _bulletsPerShot; i++)
	{
		bullets.emplace_back(position,
			glm::rotate(direction, randRotate(randomEngine)),
			_bulletDamage,
			_bulletSpeed);
	}
}
