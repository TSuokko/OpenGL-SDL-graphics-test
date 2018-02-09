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
	const std::vector<std::string>& getLevelData() const { return _levelData; };
	
	glm::vec2 getStartPlayerPos() const { return _startPlayerPos; }
	const std::vector<glm::vec2>& getZombiePos() const { return _zombieStartPosition; };
private:
	std::vector<std::string> _levelData;
	int _numNPC;
	DevyEngine::SpriteBatch _spriteBatch;

	glm::vec2 _startPlayerPos;
	std::vector<glm::vec2> _zombieStartPosition;
};

