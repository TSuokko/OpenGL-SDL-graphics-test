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
	file >> tmp >> _numNPC;
	std::getline(file, tmp);

	while (std::getline(file, tmp))
	{
		_levelData.push_back(tmp);
	}
	for (int i = 0; i < _levelData.size(); i++)
	{
		std::cout << _levelData[i] << std::endl;
	}

	_spriteBatch.init();
	_spriteBatch.begin();

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	DevyEngine::Color whiteColor;
	whiteColor.r = 255;
	whiteColor.g = 255;
	whiteColor.b = 255;
	whiteColor.a = 255;


	//render tiles
	for (int y = 0; y < _levelData.size(); y++)
	{
		for (int x = 0; x < _levelData[y].size(); x++)
		{
			//grab tile
			char tile = _levelData[y][x];

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
			case '@':
				_levelData[y][x] = '.';
				_startPlayerPos.x = x * TILE_WIDTH;
				_startPlayerPos.y = y * TILE_WIDTH;
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
