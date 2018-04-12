#include "Gun.h"



Gun::Gun(std::string name, int fireRate, int bps, float spread, float bd, float bs):
	_name(name), _fireRate(fireRate), _bulletsPerShot(bps), _spread(spread), _bulletDamage(bd), _bulletSpeed(bs), _frameCounter(0){}


Gun::~Gun()
{
}

void Gun::fire(const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets)
{
	for (int i = 0; i < _bulletsPerShot; i++)
	{
		bullets.emplace_back(position,direction, _bulletSpeed, _bulletDamage);
	}
}

void Gun::update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets)
{
	_frameCounter++;
	if (_frameCounter > _fireRate && isMouseDown)
	{
		fire(position,direction,bullets);
		_frameCounter = 0;
	}
}