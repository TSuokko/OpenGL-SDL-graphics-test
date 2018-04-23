#pragma once
#include <glm/glm.hpp>
#include <DevyEngine\SpriteBatch.h>

const float AGENT_WIDTH = 60.0f;
const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;
class Zombie;
class Human;


class Agent
{
public:
	Agent();
	virtual ~Agent();
	virtual void update(const std::vector<std::string>& levelData,
						std::vector<Human*>& humans, 
						std::vector<Zombie*>& zombies) = 0;

	bool collideWithLevel(const std::vector<std::string>& levelData);

	bool collideWithAgent(Agent* agent);

	void draw(DevyEngine::SpriteBatch& _spriteBatch);

	glm::vec2 getPosition() const { return _position; };

protected:
	void checkTilePosition(const std::vector<std::string>& levelData, 
						   std::vector<glm::vec2>& collideTilePosition, 
						   float x, float y);

	void collideWithTile(glm::vec2 tilePos);

	bool surroundedByWalls;
	int moves;
	float smallestDistance;
	glm::vec2 _position;
	glm::vec2 _direction;
	DevyEngine::Color _color;
	float _speed;

};

