#include "Bullet.h"
#include <DevyEngine\ResourceManager.h>
#include "Agent.h"
#include "Human.h"
#include "Zombie.h"
Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float speed, float damage):_position(pos), _direction(dir), _speed(speed), _damage(damage)
{}


Bullet::~Bullet()
{
}


bool Bullet::update(const std::vector<std::string>& levelData)
{
	_position += _direction *_speed;
	return collideWithWorld(levelData);
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

bool Bullet::collideWithAgent(Agent* agent)
{
	const float MIN_DISTANCE = AGENT_RADIUS + 5;

	glm::vec2 centerPosA = _position;
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(5);

	glm::vec2 distanceVec = centerPosA - centerPosB;

	float distance = glm::length(distanceVec);

	float collisionDepth = MIN_DISTANCE - distance;

	if (collisionDepth > 0)
	{		
		return true;
	}
	return false;
}

bool Bullet::collideWithWorld(const std::vector<std::string>& levelData)
{
	glm::ivec2 gridPosition;
	gridPosition.x = floor(_position.x / 64);
	gridPosition.y = floor(_position.y / 64);

	//if agents are outside the world, return with no error
	if (gridPosition.x < 0 || gridPosition.x >= levelData[0].size() ||
		gridPosition.y < 0 || gridPosition.y >= levelData.size()){
		return true;
	}
	return (levelData[gridPosition.y][gridPosition.x] != '.');
}