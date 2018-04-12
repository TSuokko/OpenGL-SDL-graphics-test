#pragma once
#include <glm\glm.hpp>
#include <DevyEngine\SpriteBatch.h>

class Human;
class Zombie;
class Bullet
{
public:
	Bullet() {};
	Bullet(glm::vec2 pos, glm::vec2 dir, float speed, float damage);
	~Bullet();

	void draw(DevyEngine::SpriteBatch& spriteBatch);
	void update(std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies);


private:
	int _damage;


	int _lifeTime;
	float _speed;
	glm::vec2 _direction;
	glm::vec2 _position;
};

