#include "Bullet.h"
#include <DevyEngine\ResourceManager.h>


Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float speed, float damage):_position(pos), _direction(dir), _speed(speed), _damage(damage)
{}


Bullet::~Bullet()
{
}


void Bullet::update(std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies)
{
	_position += _direction *_speed;
}

void Bullet::draw(DevyEngine::SpriteBatch& spriteBatch)
{
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static DevyEngine::GLTexture texture = DevyEngine::ResourceManager::getTexture("Textures/jumpgame/PNG/Bullet.png");

	DevyEngine::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	glm::vec4 posAndSize = glm::vec4(_position.x + 5, _position.y + 5, 10, 10);

	spriteBatch.draw(posAndSize, uv, texture.id, 0.0f, color);
}
