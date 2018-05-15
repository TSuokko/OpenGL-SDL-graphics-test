#pragma once
#include <glm\glm.hpp>
#include <DevyEngine\SpriteBatch.h>
class Agent;
class Human;
class Zombie;
class Bullet
{
public:
	Bullet() {};
	Bullet(glm::vec2 pos, glm::vec2 dir, float speed, float damage);
	~Bullet();

	void draw(DevyEngine::SpriteBatch& spriteBatch);
	bool update(const std::vector<std::string>& levelData);
	float getDamage() const { return _damage; }

	bool collideWithAgent(Agent* agent);
private:

	bool collideWithWorld(const std::vector<std::string>& levelData);

	int _damage;
	int _lifeTime;
	float _speed;
	glm::vec2 _direction;
	glm::vec2 _position;
};

