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

unsigned int moves = -1;
//static int frameCounter = 0;
void Zombie::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies)
{
	Human* playerClose = chasePlayer(humans);

	if (playerClose != nullptr)
	{
		if (mapread == false)
		{
			moves = -1;
			readMap("Level1.txt", levelData, humans);
		}
		if (moves != -1)
		{
			movement();
		}
		else
		{
			//mapread = false;
			NodeCoords.clear();
			NodeDirection.clear();
		}
	}
	else
	{
		mapread = false;		
	}
	collideWithLevel(levelData);

}


Human* Zombie::chasePlayer(std::vector<Human*>& humans)
{
	Human* playerNear = nullptr;
	float smallestDistance = 1200.0f;

	for (unsigned int i = 0; i < humans.size(); i++)
	{
		glm::vec2 distVec = humans[i]->getPosition() - _position;
		// Get distance
		float distance = glm::length(distVec);

		// If this person is closer than the closest person, this is the new closest
		if (distance < smallestDistance)
		{
			//smallestDistance = 1500.0f;
			smallestDistance = distance;
			playerNear = humans[i];
		}
		else
		{
			mapread = false;
			//NodeCoords.clear();
			//NodeDirection.clear();
			//moves = -1;
		}

	}
	return playerNear;
}

void Zombie::readMap(const std::string& FileName, const std::vector<std::string>& levelData, std::vector<Human*>& humans)
{
	mapread = true;

	int endX = humans[0]->getPosition().x / 64;
	int endY = humans[0]->getPosition().y / 64;
	int startX = _position.x / 64;
	int startY = _position.y / 64;
	std::string route = pathFind(FileName, startX, startY, endX, endY);

	if (route == "")
	{
		std::cout << "An empty route generated!" << std::endl;
	}
	std::cout << route << std::endl << std::endl;

	

	if (route.length() > 0)
	{
		int j; char c;
		int x = startX;
		int y = startY;
		map[x][y] = 2;

		for (unsigned int i = 0; i < route.length(); i++)
		{
			c = route.at(i);
			j = atoi(&c);
			x = x + dx[j];
			y = y + dy[j];
			map[x][y] = 3;
		}
		//std::cout << x << y << std::endl;
		map[x][y] = 4;

		
		
		
		
		//int framecounter = 0;

		//_direction.x = dx[NodeDirection[i]];
		//_direction.y = dy[NodeDirection[i]];

		//std::cout << "dirX " << _direction.x << " dirY " << _direction.y << std::endl;

		//_position += _direction * _speed;
			
		

		/*for (int y = 0; y<m; y++)
		{
			
			for (int x = 0; x<n; x++)
				if (map[x][y] == 0)
				{
					std::cout << " ";
				}
				else if (map[x][y] == 1)
					std::cout << "#"; //obstacle
				else if (map[x][y] == 2)
					std::cout << "S"; //start
				else if (map[x][y] == 3)
				{				
					std::cout << "O"; //route
				}
				else if (map[x][y] == 4)
					std::cout << "F"; //finish
			std::cout << std::endl;
		}*/
	}
}



void Zombie::movement()
{
	//std::cout << moves << std::endl;

	//moves = NodeDirection.size() - 1;
	//std::cout << "Direction Nodes:" << std::endl;
	//std::cout << "test";


	//TODO: EACH ZOMBIE HAS THEIR OWN UNIQUE SET OF MOVES
	std::cout <<"\nNodeCoords: " <<NodeCoords[moves].x << " " << NodeCoords[moves].y << std::endl;

	glm::vec2 delta(NodeCoords[moves].x - (int)_position.x / 64.f, NodeCoords[moves].y - (int)_position.y / 64.f);


	/*if (delta.x > 1)
	{
		delta.x = 1;
	}
	if (delta.x < -1)
	{
		delta.x = -1;
	}
	if (delta.y > 1)
	{
		delta.y = 1;
	}
	if (delta.y < -1)
	{
		delta.y = -1;
	}*/
	
	//std::cout << delta.x << " " << delta.y << std::endl;
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
		std::cout <<"dx: " <<dx[NodeDirection[moves]] << std::endl;
		_direction.x = dx[NodeDirection[moves]];
	}
	if (delta.y == 0)
	{
		_direction.y = 0;
	}
	else
	{
		std::cout << "dy: " << dy[NodeDirection[moves]] << std::endl;
		_direction.y = dy[NodeDirection[moves]];
		/*if (_direction.y > 1 || _direction.y < -1)
		{
			glm::normalize(_direction.y);
			std::cout << "New: " << _direction.y;
		}*/
	}
	std::cout <<"\ndelta: x:"  <<delta.x<<" y: "<<delta.y;
	std::cout << "\ndirX: " << _direction.x;
	std::cout << "\ndirY: " << _direction.y;
	_position += _direction * _speed;


	//TODO: FIX when the zombie moves past the move check, and continues on.
	if (delta.x == 0 && delta.y == 0)
	{
		//because of how the nodes are generated in the pathfinding, 
		//we must start from the back of the nodes and go backwards in order
		moves--;
	}
	/*if (moves == -1)
	{
		mapread = false;
	}*/
	

}

std::string Zombie::pathFind(const std::string& FileName, const int & xStart, const int & yStart,
	const int & xFinish, const int & yFinish)
{

	std::string line;										//current line 
	char type;											//current character type on the map
	std::ifstream inputFile(FileName.c_str());				//read the given .txt file

															//write the values for the algorithm
	if (inputFile)										//if reading the inputted file
	{
		std::cout << "Input file success" << std::endl;
		std::getline(inputFile, line);
		for (unsigned int y = 0; y < n; ++y)	// Loops through the y-coordinate
		{
			std::getline(inputFile, line);
			for (unsigned int x = 0; x < m; ++x)	// Loops through the x-coordinate
			{
				type = line.at(x);
				if (type == 35)
				{
					map[x][y] = 1; //   # symbol = value 1
				}
				if (type == 46)
				{
					map[x][y] = 0; //   . symbol = value 0
				}

			}
			line.clear();								//erases the contents of the line
		}
		inputFile.close();
	}

	//check if either point is surrounded
	if (map[xStart + 1][yStart] == 1 &&
		map[xStart + 1][yStart + 1] == 1 &&
		map[xStart][yStart + 1] == 1 &&
		map[xStart - 1][yStart + 1] == 1 &&
		map[xStart - 1][yStart] == 1 &&
		map[xStart - 1][yStart - 1] == 1 &&
		map[xStart][yStart - 1] == 1 &&
		map[xStart + 1][yStart - 1] == 1)
	{
		std::cout << "Start Point surrounded by walls\n";
		return "";
	}
	if (map[xFinish + 1][yFinish] == 1 &&
		map[xFinish + 1][yFinish + 1] == 1 &&
		map[xFinish][yFinish + 1] == 1 &&
		map[xFinish - 1][yFinish + 1] == 1 &&
		map[xFinish - 1][yFinish] == 1 &&
		map[xFinish - 1][yFinish - 1] == 1 &&
		map[xFinish][yFinish - 1] == 1 &&
		map[xFinish + 1][yFinish - 1] == 1)
	{
		std::cout << "Finish Point surrounded by walls\n";
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

/*unsigned int i = 0;

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
}*/
