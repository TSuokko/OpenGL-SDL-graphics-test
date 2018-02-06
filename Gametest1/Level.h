#pragma once
#include <vector>
#include <string>
#include <GL\glew.h>
#include <DevyEngine\SpriteBatch.h>

const int TILE_WIDTH = 64;

class Level
{
public:
	Level(const std::string& FileName );
	~Level();
	void draw();



private:
	std::vector<std::string> _levelData;
	int _numNPC;
	DevyEngine::SpriteBatch _spriteBatch;

	glm::ivec2 _startPlayerPos;
	std::vector<glm::ivec2> _zombieStartPosition;
};

