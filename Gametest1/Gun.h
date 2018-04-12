#pragma once
#include <string>
#include <vector>
#include <glm\glm.hpp>
#include "Bullet.h"
class Gun
{
public:
	Gun(std::string name,int fireRate, int bps, float spread, float bd, float bs );
	~Gun();
	
	void update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets);

private:
	void fire(const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets);
	std::string _name;
	int _fireRate; //firerate in terms of frames
	int _bulletsPerShot; //how many bullets are fired at a time
	float _spread; //accuracy

	float _bulletSpeed;
	float _bulletDamage;

	int _frameCounter;
};

