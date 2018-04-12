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
#include <Windows.h>


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


static int frameCounter = 0;
void Zombie::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies)
{
	Human* playerClose = chasePlayer(humans);


	if (playerClose != nullptr)
	{
		if (mapread == false)
		{
			frameCounter = 0;
			readMap("level1.txt", levelData, humans);
		}
		
		aStar(humans);

	}
	else
	{
		mapread = false;
	}
	collideWithLevel(levelData);
	//path.clear();
}

Human* Zombie::chasePlayer(std::vector<Human*>& humans)
{
	Human* playerNear = nullptr;
	float smallestDistance = 1000.0f;

	for (unsigned int i = 0; i < humans.size(); i++)
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




unsigned int i = 0;

void Zombie::aStar(std::vector<Human*>& humans)
{
	frameCounter++;
	if (frameCounter == 500)
	{
		frameCounter = 0;
		
		//i++;
		std::cout << "did not move lol, i = " << i << " path size"<<path.size()<<std::endl;
		
		mapread = false;
		
	}

		//THIS MOVES THE ZOMBIE
		glm::vec2 delta(path[i].x - (int)_position.x / 64.f, path[i].y - (int)_position.y / 64.f);

		/*std::cout << "\ndeltax: "<<delta.x<<" deltay: " << delta.y << std::endl;
		std::cout << "posx: " << _position.x/64.f << " posy: " << _position.y/64.f << std::endl;
		std::cout << "path: "<< "x: "<<path[i].x << " y: " << path[i].y << std::endl;*/

		if (delta.x == 0.f && delta.y == 0.f)
		{
			_direction.x = 0.f;
			_direction.y = 0.f;
		}

		if (delta.x == 0)
		{
			_direction.x = 0;
		}
		else
		{
			_direction.x = glm::normalize(delta.x);
		}

		if (delta.y == 0)
		{
			_direction.y = 0;
		}
		else
		{
			_direction.y = glm::normalize(delta.y);
		}
		
		_position += _direction * _speed;
		
		//std::cout <<"i: "<< i <<" dirx: " << _direction.x << "diry:: " << _direction.y << std::endl;


		//std::cout << "length: " << glm::length(delta) << std::endl;
		if (glm::length(delta) <= 0.1f)
		{	
			++i;
			//std::cout <<"i: "<< i << std::endl;
		}

		if (i >= path.size())
		{
			//std::cout << "kierros" << std::endl;
			_direction.x = 0.f;
			_direction.y = 0.f;
			mapread = false;
			i = 0;
		}

}
		

	




SquareGraph Zombie::readMap(const std::string& FileName, const std::vector<std::string>& levelData, std::vector<Human*>& humans)
{

	const int mapDimension = levelData.size();			//dimension of the map ([200][200])

	//std::cout <<"\nmapdim: " <<mapDimension << std::endl;

	string line;										//current line 
	char type;											//current character type on the map
	ifstream inputFile(FileName.c_str());				//read the given .txt file
	SquareGraph graph(mapDimension);					//gives the constructor the map dimension
	if (inputFile)										//if reading the inputted file
	{
		std::cout << "Input file success" << std::endl;
		std::getline(inputFile, line);
		for (int i = 0; i < mapDimension; i++)			//for loop the size of the dimension
		{												//looping the Y-coordinate
			getline(inputFile, line);					//disregard the first line with the number
			for (int j = 0; j < mapDimension; j++)		//and the X-coordinate
			{
				type = line.at(j);						//read the character on file
				graph.setCellValue(make_pair(i, j), type);//sets the value as either blank or wall
			}

			line.clear();								//erases the contents of the line
		}
		int px = humans[0]->getPosition().x / 64;
		int py = humans[0]->getPosition().y / 64;
		graph.setSecondRobotPos(make_pair(px, py));		//set the player coordinates



		int x = _position.x / 64;
		int y = _position.y / 64;
		graph.setFirstRobotPos(make_pair(x, y));	//set zombie coordinates

		//std::cout << "Player: " << px << " " << py << " Zombie: " << x << " " << y << std::endl;

		path = graph.executeAStar();

		graph.printPath(path);

		inputFile.close();

		mapread = true;

		return graph;
	}
	else
		return graph;
}
