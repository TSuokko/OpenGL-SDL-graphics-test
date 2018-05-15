#include "Zombie.h"
#include "Human.h"
#include <ctime>
#include <random>
#include <glm/gtx/rotate_vector.hpp>
#include <DevyEngine/ResourceManager.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <list>
#include <queue>
#include <math.h>
#include <fstream>
#include <Windows.h>
#include <queue>
#include <sstream>
#include "node.h"

Zombie::Zombie()
{
}

Zombie::~Zombie()
{
}

void Zombie::init(float speed, glm::vec2 pos)
{
	_health = 150;
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);
	_color.setColor(255, 55, 55, 255);
	moves = -1;
	surroundedByWalls = false;
	_speed = speed;
	_position = pos;
	// Get random direction
	_direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
	// Make sure direction isn't zero
	if (_direction.length() == 0) _direction = glm::vec2(1.0f, 0.0f);
	_direction = glm::normalize(_direction);
}


//static int frameCounter = 0;
void Zombie::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies)
{
	Human* playerClose = chasePlayer(humans);

	//if the player IS close
	if (playerClose != nullptr)
	{
		//and the map hasn't been read
		if (mapread == false)
		{
			//the amount of moves will be set to -1, since when moves are added to their vector, the first move will be move number 0 (since arrays start at 0)
			moves = -1;
			//read the map for pathfinding
			readMap("Level1.txt", humans);
		}
		if (moves != -1)
		{
			//when the algorithm has gone it's course and the zombie has movement vectors, it's time to move the zombie.
			movement(humans);
		}
		else
		{
			//as long as the zombie is not surrounded by walls, and movement has gone to -1, it's time to start over.
			if (surroundedByWalls == false)
			{
				NodeCoords.clear();
				NodeDirection.clear();
				mapread = false;
			}
		}
	}
	//level collision detection
	collideWithLevel(levelData);

}


Human* Zombie::chasePlayer(std::vector<Human*>& humans)
{
	Human* playerNear = nullptr;
	
	smallestDistance = 1200.0f;
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

void Zombie::readMap(const std::string& FileName, std::vector<Human*>& humans)
{
	//the map has been read, so it won't be read again until needed. 
	mapread = true;
	int startX = (int)_position.x / 64;
	int startY = (int)_position.y / 64;
	int endX = (int)humans[0]->getPosition().x / 64;
	int endY = (int)humans[0]->getPosition().y / 64;
	
	std::string route = pathFind(FileName, startX, startY, endX, endY);
	if (route == "")
	{
		//std::cout << "An empty route generated!" << std::endl;
		surroundedByWalls = true;
	}
}



void Zombie::movement(std::vector<Human*>& humans)
{
	glm::vec2 delta(NodeCoords[moves].x - (int)_position.x / 64.f, NodeCoords[moves].y - (int)_position.y / 64.f);
	
	//vertical movement
	if (delta.x == 0){
		_direction.x = 0;
	}
	else{	
		//the pathfinding algorithm also stores a vector of directions
		_direction.x = dx[NodeDirection[moves]];
	}
	//horizontal movement
	if (delta.y == 0){
		_direction.y = 0;
	}
	else{
		_direction.y = dy[NodeDirection[moves]];
	}
	//if for some reason the zombie has bumped itself to a wrong location, for now as a quick fix 
	//the zombie will teleport to it's original location before the bump
	if (delta.x < -1 || delta.x > 1 || delta.y < -1 || delta.y > 1)
	{
		_direction = glm::vec2(0, 0);
		_position = glm::vec2(NodeCoords[moves].x * 64, NodeCoords[moves].y * 64);
		moves = -1;
	}
	//main movement function
	_position += _direction * _speed;
	
	//Most commonly occures when two zombies bumb into each other
	if (delta.x == 0 && delta.y == 0){
		//because of how the nodes are generated in the pathfinding, 
		//it must start from the back of the nodes and go backwards in order
		moves--;
	}	
}

std::string Zombie::pathFind(const std::string& FileName, const int & xStart, const int & yStart,
	const int & xFinish, const int & yFinish)
{
	std::string line;										//current line 
	char type;												//current character type on the map
	std::ifstream inputFile(FileName.c_str());				//read the given .txt file
															//write the values for the algorithm
	if (inputFile)											//if reading the inputted file
	{
		//std::cout << "Input file success" << std::endl;
		std::getline(inputFile, line);
		for (unsigned int y = 0; y < n; ++y)				// Loops through the y-coordinate
		{
			std::getline(inputFile, line);
			for (unsigned int x = 0; x < m; ++x)			// Loops through the x-coordinate
			{
				type = line.at(x);
				if (type == 35)
				{
					map[x][y] = 1;  //   # symbol = value 1
				}
				if (type == 46)
				{
					map[x][y] = 0;  //   . symbol = value 0
				}

			}
			line.clear();			//erases the contents of the line
		}
		inputFile.close();
	}
	
	//check if either point is surrounded
	if (map[xStart + 1][yStart	  ] == 1 &&
		map[xStart + 1][yStart + 1] == 1 &&
		map[xStart	  ][yStart + 1] == 1 &&
		map[xStart - 1][yStart + 1] == 1 &&
		map[xStart - 1][yStart	  ] == 1 &&
		map[xStart - 1][yStart - 1] == 1 &&
		map[xStart	  ][yStart - 1] == 1 &&
		map[xStart + 1][yStart - 1] == 1)
	{
		//std::cout << "Start Point surrounded by walls\n";
		surroundedByWalls = true;
		return "";
	}
	if (map[xFinish][yFinish] == 1)
	{
		//std::cout << "Finish Point in in wall\n";
		return "";
	}
	
	static std::priority_queue<node> pq[2]; // list of open (not-yet-tried) nodes
	static int pqi; // pq index
	static node* n0;
	static node* m0;
	static int i, j, x, y, xdx, ydy;
	static char c;
	pqi = 0;

	// reset the node maps
	for (y = 0; y<m; y++)
	{
		for (x = 0; x<n; x++)
		{
			closed_nodes_map[x][y] = 0;
			open_nodes_map[x][y] = 0;
		}
	}

	// create the start node and push into list of open nodes
	n0 = new node(xStart, yStart, 0, 0);
	n0->updatePriority(xFinish, yFinish);
	pq[pqi].push(*n0);

	//open_nodes_map[x][y] = n0->getPriority(); // mark it on the open nodes map
	open_nodes_map[xStart][yStart] = n0->getPriority();

	//a* search
	delete n0;
	while (!pq[pqi].empty())
	{
		// get the current node w/ the highest priority
		// from the list of open nodes
		n0 = new node(pq[pqi].top().getxPos(), pq[pqi].top().getyPos(),
			pq[pqi].top().getLevel(), pq[pqi].top().getPriority());

		x = n0->getxPos(); y = n0->getyPos();

		pq[pqi].pop(); // remove the node from the open list
		open_nodes_map[x][y] = 0;
		// mark it on the closed nodes map
		closed_nodes_map[x][y] = 1;

		// quit searching when the goal state is reached
		//if((*n0).estimate(xFinish, yFinish) == 0)
		if (x == xFinish && y == yFinish)
		{
			//std::cout << x <<" "<< y << std::endl;
			// generate the path from finish to start
			// by following the directions
			std::string path = "";
			while (!(x == xStart && y == yStart))
			{
				j = dir_map[x][y];
				c = '0' + (j + dir / 2) % dir;
				//std::cout << "x: "<< x << "y: " <<y << std::endl;
				NodeCoords.push_back(glm::vec2(x,y));
				NodeDirection.push_back((j + dir / 2) % dir);
				moves++;
				path = c + path;
				x += dx[j];
				y += dy[j];
			}

			// garbage collection
			delete n0;
			// empty the leftover nodes
			while (!pq[pqi].empty()) pq[pqi].pop();
			return path;
		}

		// generate moves (child nodes) in all possible directions
		for (i = 0; i<dir; i++)
		{
			xdx = x + dx[i]; ydy = y + dy[i];
			if (!(xdx<0 || xdx>n - 1 || ydy<0 || ydy>m - 1 || map[xdx][ydy] == 1
				|| closed_nodes_map[xdx][ydy] == 1))
			{
				// generate a child node
				m0 = new node(xdx, ydy, n0->getLevel(),
					n0->getPriority());
				m0->nextLevel(i);
				m0->updatePriority(xFinish, yFinish);

				// if it is not in the open list then add into that
				if (open_nodes_map[xdx][ydy] == 0)
				{
					open_nodes_map[xdx][ydy] = m0->getPriority();
					pq[pqi].push(*m0);
					// mark its parent node direction
					delete m0;
					dir_map[xdx][ydy] = (i + dir / 2) % dir;

				}
				else if (open_nodes_map[xdx][ydy]>m0->getPriority())
				{
					// update the priority info
					open_nodes_map[xdx][ydy] = m0->getPriority();
					// update the parent direction info
					dir_map[xdx][ydy] = (i + dir / 2) % dir;
					// replace the node
					// by emptying one pq to the other one
					// except the node to be replaced will be ignored
					// and the new node will be pushed in instead
					while (!(pq[pqi].top().getxPos() == xdx &&
						pq[pqi].top().getyPos() == ydy))
					{
						pq[1 - pqi].push(pq[pqi].top());
						pq[pqi].pop();
					}
					pq[pqi].pop(); // remove the wanted node

								   // empty the larger size pq to the smaller one
					if (pq[pqi].size()>pq[1 - pqi].size()) pqi = 1 - pqi;
					while (!pq[pqi].empty())
					{
						pq[1 - pqi].push(pq[pqi].top());
						pq[pqi].pop();
					}
					pqi = 1 - pqi;
					pq[pqi].push(*m0); // add the better node instead
				}
				else delete m0; // garbage collection
			}
		}
		delete n0; // garbage collection
	}
	return ""; // no route found
}


