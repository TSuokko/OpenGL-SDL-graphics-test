#include "Level.h"
#include <iostream>
#include <DevyEngine\Errors.h>
#include <DevyEngine\ResourceManager.h>
#include <fstream>

Level::Level(const std::string& FileName)
{
	std::ifstream file;
	file.open(FileName);
	if (file.fail())
	{
		DevyEngine::fatalError("Failed to open " + FileName);
	}
	std::string tmp;
	//get the amount of NPCs that have been written on the first line of the file
	file >> tmp >> _numNPC;
	std::getline(file, tmp);

	while (std::getline(file, tmp))
	{
		_levelData.push_back(tmp);
	}

	_spriteBatch.init();
	_spriteBatch.begin();

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	DevyEngine::Color whiteColor(255,255,255,255);

	_startPlayerPos = glm::vec2(MAP_SIZE / 2 * TILE_WIDTH, MAP_SIZE / 2 * TILE_WIDTH);

	//render tiles
	for (unsigned int y = 0; y < _levelData.size(); y++)
	{
		for (unsigned int x = 0; x < _levelData[y].size(); x++)
		{
			//grab tile
			char tile = _levelData[y][x];
			glm::vec2 currentCoordinate(_levelData[x].size(), _levelData.size());
			//destination rectangle
			glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);
			
			//process tile
			switch(tile)
			{
			case '#':
				_spriteBatch.draw(destRect, uvRect, DevyEngine::ResourceManager::getTexture("Textures/brick.png").id, 0.0f, whiteColor);
				break;
			case '.':
				_spriteBatch.draw(destRect, uvRect, DevyEngine::ResourceManager::getTexture("Textures/grass.png").id, 0.0f, whiteColor);
				break;
			default:
				std::printf("Unexpected symbol %c at (%d, %d)", tile, x, y);
			}
		}
	}
	_spriteBatch.end();
}

Level::~Level()
{
}

void Level::draw()
{
	_spriteBatch.renderBatch();
}
