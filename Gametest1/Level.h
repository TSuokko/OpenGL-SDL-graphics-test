#pragma once
#include <vector>
#include <string>
#include <GL\glew.h>


class Level
{
public:
	Level(const std::string& FileName );
	~Level();

private:
	std::vector<std::string> _levelData;

};

