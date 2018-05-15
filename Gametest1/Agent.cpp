#include "Agent.h"

Agent::Agent()
{
}

Agent::~Agent()
{
}

void Agent::draw(DevyEngine::SpriteBatch& _spriteBatch)
{
	static int textureID = DevyEngine::ResourceManager::getTexture("Textures/pepe_1.png").id;
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 destRect(_position.x, _position.y, AGENT_WIDTH, AGENT_WIDTH);
	_spriteBatch.draw(destRect, uvRect, textureID, 0.0, _color);
}

bool Agent::collideWithLevel(const std::vector<std::string>& levelData)
{
	std::vector<glm::vec2> collideTilePosition;

	//check four corners
	checkTilePosition(levelData, collideTilePosition, _position.x, _position.y);
	checkTilePosition(levelData, collideTilePosition, _position.x + AGENT_WIDTH, _position.y);
	checkTilePosition(levelData, collideTilePosition, _position.x, _position.y + AGENT_WIDTH);
	checkTilePosition(levelData, collideTilePosition, _position.x + AGENT_WIDTH, _position.y + AGENT_WIDTH);

	if (collideTilePosition.size() == 0)
	{
		return false;
	}

	for (int i = 0; i < collideTilePosition.size(); i++)
	{
		collideWithTile(collideTilePosition[i]);
	}

	return true;
}

//Collision between agents
bool Agent::collideWithAgent(Agent* agent)
{
	const float MIN_DISTANCE = AGENT_RADIUS * 2.0;

	glm::vec2 centerPosA = _position + glm::vec2(AGENT_WIDTH / 2);
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distanceVec = centerPosA - centerPosB;

	float distance = glm::length(distanceVec);

	float collisionDepth = MIN_DISTANCE - distance;

	if (collisionDepth > 0)
	{
		glm::vec2 collisionDepthVec = glm::normalize(distanceVec) * collisionDepth;

		_position += collisionDepthVec / 2.0f;
		agent->_position -= collisionDepthVec / 2.0f;
		return true;
	}
	return false;
}

bool Agent::applyDamage(float damage)
{
	_health -= damage;
	if (_health <= 0)
	{
		return true;
	}
	return false;
}

void Agent::checkTilePosition(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePosition, float x, float y)
{
	glm::vec2 cornerPos = glm::vec2(floor(x / (float)TILE_WIDTH), floor(y / (float)TILE_WIDTH));
	//if agents are outside the world, return with no error
	if (cornerPos.x < 0 || cornerPos.x >= levelData[0].size() ||
		cornerPos.y < 0 || cornerPos.y >= levelData.size())
	{
		return;
	}

	if (levelData[cornerPos.y][cornerPos.x] != '.')
	{
		collideTilePosition.push_back(cornerPos * (float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH / 2.f));
	}


}

void Agent::collideWithTile(glm::vec2 tilePos)
{
	const float TILE_RADIUS = TILE_WIDTH / 2.f;
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;

	glm::vec2 centerPlayerPos = _position + glm::vec2(AGENT_RADIUS);
	glm::vec2 distanceVec = centerPlayerPos - tilePos;
	
	float xDepth = MIN_DISTANCE - abs(distanceVec.x);
	float yDepth = MIN_DISTANCE - abs(distanceVec.y);

	if ( (xDepth > 0.0f) || (yDepth > 0))
	{
		if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f))
		{			
			if (distanceVec.x < 0)
			{
				_position.x -= xDepth;
			}
			else 
			{
				_position.x += xDepth;
			}	
		}
		else 
		{
			if (distanceVec.x == distanceVec.y)
			{
				_position.y -= yDepth;
			}
			if (-distanceVec.x == distanceVec.y)
			{
				_position.y -= yDepth;
			}
			if (distanceVec.y < 0)
			{
				_position.y -= yDepth;
			}
			else 
			{
				_position.y += yDepth;
			}
		}
	}
}