#pragma once
#include <vector>
#include <string>
#include <GL\glew.h>
#include <DevyEngine\SpriteBatch.h>

const int TILE_WIDTH = 64;
const int  MAP_SIZE = 200;

class Level
{
public:
	Level(const std::string& FileName );
	~Level();
	void draw();
	const std::vector<std::string>& getLevelData() const { return _levelData; };
	int getNumNPC() const { return _numNPC; }
	int getWidth() const { return _levelData[0].size(); }
	int getHeight() const { return _levelData.size(); }

	glm::vec2 getStartPlayerPos() const { return _startPlayerPos; }
	const std::vector<glm::vec2>& getZombiePos() const { return _zombieStartPosition; };
private:
	std::vector<std::string> _levelData;
	int _numNPC;
	DevyEngine::SpriteBatch _spriteBatch;

	glm::vec2 _startPlayerPos;
	std::vector<glm::vec2> _zombieStartPosition;
};

