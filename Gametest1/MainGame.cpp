#include "MainGame.h"

#include <DevyEngine/Errors.h>
#include <DevyEngine/Window.h>
#include <DevyEngine/DevyEngine.h>
#include <DevyEngine\ResourceManager.h>
#include <DevyEngine\Errors.h>

#include "Point.h"
#include "Rectangle.h"
#include "Leaf.h"
#include "Node.h"

#include <iomanip>
#include <queue>
#include <math.h>



#include <iostream>
#include <string>
#include <functional>
#include <random>
#include <ctime>
#include <fstream>
#include <vector>
#include <list>
#include <cmath>

#include <stdlib.h>
#include <time.h>       

int PLAYER_HP = 20;
int PlayerScore = 0;

//Constructor
MainGame::MainGame() :
	_screenWidth(1024),
	_screenHeight(768),
	_game(GameState::PLAY),
	_maxFPS(60.0f),
	_time(0),
	_player(nullptr)
{
	_camera.init(_screenWidth, _screenHeight);
}
//run the game and start initializing
void MainGame::run()
{
	drawDungeon();
	PathfindStart();
	initSystems();
	initLevels();
	gameloop();
}

//Shader initialization
void MainGame::initShaders()
{
	_colorProgram.compileShaders("Shaders/VertexShader.txt", "Shaders/FragmentShader.txt");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
}

void MainGame::initSystems()
{
	DevyEngine::init();

	_window.create("GameEngine", _screenWidth, _screenHeight, 0);
	glClearColor(0.6, 0.7f, 0.7f, 1.0f); //grey background
	initShaders();
	
	_agentSpriteBatch.init();
	_spriteBatch.init();

	_fpslimiter.init(_maxFPS);
}

void MainGame::initLevels()
{
	_levels.push_back(new Level("Level1.txt")); //pushes back the level data that was created during the level creation method
	_currentLevel = 0;

	_player = new Player();
	_player->init(7.0f, _levels[_currentLevel]->getStartPlayerPos(), &_input);
	_humans.push_back(_player);

	std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));
	std::uniform_int_distribution<int> randX(2, _levels[_currentLevel]->getWidth() - 1);
	std::uniform_int_distribution<int> randY(2, _levels[_currentLevel]->getHeight() - 1);

	const float ZOMBIE_SPEED = 4.0f;
	//add NPC:s
	for (int i = 0; i < _levels[_currentLevel]->getNumNPC(); i++)
	{
		_zombies.push_back(new Zombie);
		glm::vec2 pos(randX(randomEngine) * TILE_WIDTH, randY(randomEngine) * TILE_WIDTH);
		_zombies.back()->init(ZOMBIE_SPEED, pos);
	}
}

void MainGame::processInput()
{
	const float scaleSpeed = 0.01f; //map scaling speed
	
	while (SDL_PollEvent(&_event))
	{
		switch (_event.type)
		{
		case SDL_QUIT:
			_game = GameState::EXIT; //allows you to quit the game
			break;
		case SDL_MOUSEMOTION:
			_input.setMouseCoords(_event.motion.x, _event.motion.y);
			break;
		case SDL_KEYDOWN:
			_input.keyPress(_event.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_input.keyRelease(_event.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_input.keyPress(_event.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_input.keyRelease(_event.button.button);
			break;
		}
	}
	//scale the map with 'q(smaller)' or 'e(larger)'
	if (_input.isKeyPressed(SDLK_q))
	{_camera.setScale(_camera.getScale() - scaleSpeed);}
	if (_input.isKeyPressed(SDLK_e))
	{_camera.setScale(_camera.getScale() + scaleSpeed);}	
}
	
void MainGame::drawGame()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_colorProgram.use();
	glActiveTexture(GL_TEXTURE0);

	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	GLuint pLocation = _colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();

	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	_levels[_currentLevel]->draw();

	_agentSpriteBatch.begin();
	
	const glm::vec2 agentDims(AGENT_RADIUS * 2.0f);

	for (int i = 0; i < _humans.size(); i++)
	{
		_humans[i]->draw(_agentSpriteBatch);
	}

	for (int i = 0; i < _zombies.size(); i++)
	{
		_zombies[i]->draw(_agentSpriteBatch);
	}
	
	_agentSpriteBatch.end();
	
	_agentSpriteBatch.renderBatch();

	_colorProgram.unuse();
	
	_window.swapBuffer();

}

void MainGame::gameloop()
{
	while (_game != GameState::EXIT)
	{
		_fpslimiter.begin(); 

		processInput();
		_time += 0.1f;

		updateAgents();
		
		_camera.setPosition(_player->getPosition());

		_camera.update();

		drawGame();

		_fps = _fpslimiter.end();

		//print only once every 10 frames
		static int frameCounter = 0;
		frameCounter++;

		if (frameCounter == 100)
		{
			PlayerScore += 10;
			
			frameCounter = 0;
		}
	}
}


void MainGame::updateAgents()
{	
	for (int i = 0; i < _humans.size(); i++) 
	{
		_humans[i]->update(_levels[_currentLevel]->getLevelData(),
			_humans,
			_zombies);	
	}
	for (int i = 0; i < _humans.size(); i++) 
	{
		// Collide with other humans
		for (int j = i + 1; j < _zombies.size(); j++) 
		{
			(_humans[i]->collideWithAgent(_zombies[j]));		
		}		
	}
	for (int i = 0; i < _zombies.size(); i++)
	{
		_zombies[i]->update(_levels[_currentLevel]->getLevelData(),
			_humans,
			_zombies);
	}
	for (int i = 0; i < _zombies.size(); i++)
	{
		// Collide with other zombies
		for (int j = i + 1; j < _zombies.size(); j++)
		{
			(_zombies[i]->collideWithAgent(_zombies[j]));
		}
		if (_zombies[i]->collideWithAgent(_player))
		{
			PLAYER_HP--;
			std::cout << "HP: " << PLAYER_HP << std::endl;
			if (PLAYER_HP == 0)
			{
				std::cout << "\nYour Score: " << PlayerScore << std::endl;
				DevyEngine::fatalError("Game Over");

			}
		}
	}
}


///////////////////////////////DUNGEON BSP GENERATOR ALGORITHM
void MainGame::drawDungeon()
{
	srand(time(NULL));

	int MAX_LEAF_SIZE = 30;
	Leaf* root = new Leaf(0, 0, 200, 200);
	std::list<Leaf> leaf_edge_nodes;
	std::list<Rectangle> halls;
	root->generate(MAX_LEAF_SIZE);

	root->createRooms(&leaf_edge_nodes, &halls);

	// need a char map (space is char 40, wall is char 179);
	char map[200][200]; // same width / height as the root leaf
	for (int i = 0; i < 200; i++) 
	{
		for (int j = 0; j < 200; j++) 
		{
			map[i][j] = 35; //creates ASCII wall symbol
		}
	}

	for (std::list<Leaf>::iterator l = leaf_edge_nodes.begin(); l != leaf_edge_nodes.end(); ++l) {

		Rectangle* room = l->getRoom();
		int left = room->left();
		int right = room->right();
		int top = room->top();
		int bottom = room->bottom();

		if (left < 1) left = 1;
		if (right < 1) right = 1;
		if (top < 1) top = 1;
		if (bottom < 1) bottom = 1;

		if (right - left > 3 && bottom - top > 3) {

			for (int i = left; i <= right; i++) {
				for (int j = top; j <= bottom; j++) 
				{
					map[i][j] = 46;
				}
			}
		}
	}

	for (std::list<Rectangle>::iterator hh = halls.begin(); hh != halls.end(); ++hh) 
	{
		int left = hh->left();
		int right = hh->right();
		int top = hh->top();
		int bottom = hh->bottom();

		if (left < 1) left = 1;
		if (right < 1) right = 1;
		if (top < 1) top = 1;
		if (bottom < 1) bottom = 1;

		if (left > 199) left = 198;
		if (right > 199) right = 198;
		if (top > 199) top = 198;
		if (bottom > 199) bottom = 198;

		for (int i = left; i <= right; i++) {
			for (int j = top; j <= bottom; j++) {
				map[i][j] = 46;
			}
		}
	}

	std::cout << "\n=== DUNGEON GENERATOR v0.2 ===\n\n";
	std::ofstream myfile("Level1.txt");

	myfile << "_numNPC: 150\n";

	for (int j = 0; j < 200; j++)
	{
		for (int i = 0; i < 200; i++)
		{
			//print the level daa to a .txt file
			if (myfile.is_open())
			{
				myfile << map[i][j];
				if (i == 199)
				{
					//divide the map into 99 character lengths
					myfile << "\n";
				}
			}
			else std::cout << "Unable to open file";
		}
	}
	myfile.close();
}

/////////////A* ALGORITHM////////////////////////////

bool operator<(const Node & a, const Node & b)
{
	return a.getPriority() > b.getPriority();
}

// A-star algorithm.
// The route returned is a string of direction digits.
std::string MainGame::pathFind(const int & xStart, const int & yStart,
	const int & xFinish, const int & yFinish)
{
	static std::priority_queue<Node> pq[2]; // list of open (not-yet-tried) nodes
	static int pqi; // pq index
	static Node* n0;
	static Node* m0;
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
	n0 = new Node(xStart, yStart, 0, 0);
	n0->updatePriority(xFinish, yFinish);
	pq[pqi].push(*n0);
	open_nodes_map[x][y] = n0->getPriority(); // mark it on the open nodes map

											  // A* search
	while (!pq[pqi].empty())
	{
		// get the current node w/ the highest priority
		// from the list of open nodes
		n0 = new Node(pq[pqi].top().getxPos(), pq[pqi].top().getyPos(),
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
			// generate the path from finish to start
			// by following the directions
			std::string path = "";
			while (!(x == xStart && y == yStart))
			{
				j = dir_map[x][y];
				c = '0' + (j + dir / 2) % dir;
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
				m0 = new Node(xdx, ydy, n0->getLevel(),
					n0->getPriority());
				m0->nextLevel(i);
				m0->updatePriority(xFinish, yFinish);

				// if it is not in the open list then add into that
				if (open_nodes_map[xdx][ydy] == 0)
				{
					open_nodes_map[xdx][ydy] = m0->getPriority();
					pq[pqi].push(*m0);
					// mark its parent node direction
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

int MainGame::PathfindStart()
{
	srand(time(NULL));
	// create empty map
	for (int y = 0; y<m; y++)
	{
		for (int x = 0; x<n; x++) map[x][y] = 0;
	}

	// fillout the map matrix with a '+' pattern
	for (int x = n / 8; x<n * 7 / 8; x++)
	{
		map[x][m / 2] = 1;
	}
	for (int y = m / 8; y<m * 7 / 8; y++)
	{
		map[n / 2][y] = 1;
	}

	// randomly select start and finish locations
	int xA, yA, xB, yB;
	switch (rand() % 8)
	{
	case 0: xA = 0; yA = 0; xB = n - 1; yB = m - 1; break;
	case 1: xA = 0; yA = m - 1; xB = n - 1; yB = 0; break;
	case 2: xA = n / 2 - 1; yA = m / 2 - 1; xB = n / 2 + 1; yB = m / 2 + 1; break;
	case 3: xA = n / 2 - 1; yA = m / 2 + 1; xB = n / 2 + 1; yB = m / 2 - 1; break;
	case 4: xA = n / 2 - 1; yA = 0; xB = n / 2 + 1; yB = m - 1; break;
	case 5: xA = n / 2 + 1; yA = m - 1; xB = n / 2 - 1; yB = 0; break;
	case 6: xA = 0; yA = m / 2 - 1; xB = n - 1; yB = m / 2 + 1; break;
	case 7: xA = n - 1; yA = m / 2 + 1; xB = 0; yB = m / 2 - 1; break;
	}

	std::cout << "Map Size (X,Y): " << n << "," << m << std::endl;
	std::cout << "Start: " << xA << "," << yA << std::endl;
	std::cout << "Finish: " << xB << "," << yB << std::endl;
	// get the route
	clock_t start = clock();
	std::string route = pathFind(xA, yA, xB, yB);
	if (route == "") std::cout << "An empty route generated!" << std::endl;
	clock_t end = clock();
	double time_elapsed = double(end - start);
	std::cout << "Time to calculate the route (ms): " << time_elapsed << std::endl;
	std::cout << "Route:" << std::endl;
	std::cout << route << std::endl;

	// follow the route on the map and display it 
	if (route.length()>0)
	{
		int j; char c;
		int x = xA;
		int y = yA;
		map[x][y] = 2;
		for (int i = 0; i<route.length(); i++)
		{
			c = route.at(i);
			j = atoi(&c);
			x = x + dx[j];
			y = y + dy[j];
			map[x][y] = 3;
		}
		map[x][y] = 4;

		// display the map with the route
		for (int y = 0; y<m; y++)
		{
			for (int x = 0; x<n; x++)
				if (map[x][y] == 0)
					std::cout << ".";
				else if (map[x][y] == 1)
					std::cout << "O"; //obstacle
				else if (map[x][y] == 2)
					std::cout << "S"; //start
				else if (map[x][y] == 3)
					std::cout << "R"; //route
				else if (map[x][y] == 4)
					std::cout << "F"; //finish
			std::cout << "\n" << std::endl;
		}
	}
	getchar(); // wait for a (Enter) keypress  
	return (0);
}


////////////////////////////////////////////////////
MainGame::~MainGame()
{
	for (int i = 0; i < _levels.size(); i++)
	{
		delete _levels[i];
	}
}