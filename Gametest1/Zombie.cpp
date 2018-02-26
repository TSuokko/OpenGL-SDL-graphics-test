#include "Zombie.h"
#include "Human.h"
#include <ctime>
#include <random>
#include <glm/gtx/rotate_vector.hpp>
#include <DevyEngine/ResourceManager.h>

#include <vector>
#include <algorithm>
#include <set>
#include <list>
#include <queue>
#include <math.h>
#include <fstream>


Zombie::Zombie()
{
}


Zombie::~Zombie()
{
}

void Zombie::init(float speed, glm::vec2 pos)
{
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

	

	_color.r = 255;
	_color.g = 55;
	_color.b = 55;
	_color.a = 255;

	_speed = speed;
	_position = pos;
	// Get random direction
	_direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
	// Make sure direction isn't zero
	if (_direction.length() == 0) _direction = glm::vec2(1.0f, 0.0f);

	_direction = glm::normalize(_direction);
}

void Zombie::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies)
{
	Human* playerClose = chasePlayer(humans);
	
	
	if (playerClose != nullptr)
	{
		if (mapread == false)
		{
			readMap("level1.txt", levelData, humans);
		}
		aStar();
		
	}
	collideWithLevel(levelData);
	//path.clear();
}

Human* Zombie::chasePlayer(std::vector<Human*>& humans)
{
	Human* playerNear = nullptr;
	float smallestDistance = 1000.0f;

	for (int i = 0; i < humans.size(); i++)
	{
		glm::vec2 distVec = humans[i]->getPosition() - _position;
		// Get distance
		float distance = glm::length(distVec);

		// If this person is closer than the closest person, this is the new closest
		if (distance < smallestDistance) 
		{
			smallestDistance = distance;
			playerNear = humans[i];
		}

	}
	return playerNear;
}

void Zombie::aStar()
{
	//TODO:
	//FIND OUT WHY IT'S LOCKED IN DIRECTION (-1, -1)
	//IMPLEMENT NODE TO NODE MOVEMENT
	for (int i = 0; i < path.size(); i++)
	{
		
		//std::cout << "Path: " << path[i].x << " " << path[i].y << std::endl;
		if (_position.x / 64 != path[i].x && _position.y / 64 != path[i].y)
		{		
			_direction.x = glm::normalize(path[i].x - _position.x);
			_direction.y = glm::normalize(path[i].y - _position.y);
			_position += _direction * _speed;

			std::cout << "\npath test1: " << i << " " << path[i].x << " " << path[i].y << std::endl;
			std::cout << "position test1: " << " " << _position.x / 64 << " " << _position.y / 64 << std::endl;
			std::cout << "direction test2: " <<" "<< _direction.x << " " <<_direction.y << "\n"<< std::endl;
		}
		
		//std::cout << "Pos: " << _position.x / 64 << " " << _position.y / 64 << std::endl;

		if (_position.x / 64 == path[i].x && _position.y / 64 == path[i].y)
		{
			std::cout << "Pos: "<< i << " "<<_position.x / 64 << " " << _position.y / 64 << std::endl;
		}
	}

}

SquareGraph Zombie::readMap(const std::string& FileName, const std::vector<std::string>& levelData, std::vector<Human*>& humans)
{

	const int mapDimension = levelData.size();	//dimension of the map ([200][200])
	string line;										//current line 
	char type;											//current character type on the map
	ifstream inputFile(FileName.c_str());				//read the given .txt file
	SquareGraph graph(mapDimension);					//gives the constructor the map dimension
	if (inputFile)										//if reading the inputted file
	{
		std::getline(inputFile, line);
		for (int i = 0; i < mapDimension; i++)			//for loop the size of the dimension
		{												//looping the Y-coordinate
			getline(inputFile, line);					//disregard the first line with the number
			for (int j = 0; j < mapDimension; j++)		//and the X-coordinate
			{
				type = line.at(j);						//read the character on file
				graph.setCellValue(make_pair(i, j), type);	//sets the value as either blank or wall
			}
			line.clear();								//erases the contents of the line
		}
		int px = humans[0]->getPosition().x / 64;
		int py = humans[0]->getPosition().y / 64;
		graph.setSecondRobotPos(make_pair(px, py));		//set the player coordinates

		//for (int i = 0; i < _zombies.size(); i++)
		//{

			int x = _position.x  / 64;
			int y = _position.y / 64;
			graph.setFirstRobotPos(make_pair(x, y));	//set zombie coordinates

		//}
		path = graph.executeAStar();

		graph.printPath(path);

		inputFile.close();

		mapread = true;

		

		return graph;
	}
}
