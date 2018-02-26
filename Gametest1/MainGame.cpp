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
#include <stdexcept>
#include <fstream>

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
	initSystems();
	initLevels();
	gameloop();
	//readMap("Level1.txt");
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

	const float ZOMBIE_SPEED = 5.0f;
	//add NPC:s
	for (int i = 0; i < _levels[_currentLevel]->getNumNPC(); i++)
	{
		_zombies.push_back(new Zombie);
		//glm::vec2 pos(randX(randomEngine), randY(randomEngine));
		glm::vec2 pos(100*TILE_WIDTH, 110*TILE_WIDTH);
		_zombies.back()->init(ZOMBIE_SPEED, pos);
	}

	//readMap("Level1.txt");
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

		//readMap("Level1.txt");

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
	for (int i = 0; i < _zombies.size(); i++)
	{
		_zombies[i]->update(_levels[_currentLevel]->getLevelData(),
			_humans,
			_zombies);
		
		//_zombies[i]->aStar(path);
		
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
		// Collide with other zombies
		for (int j = i + 1; j < _zombies.size(); j++)
		{
			_zombies[i]->collideWithAgent(_zombies[j]);
		}
		if (_zombies[i]->collideWithAgent(_player))
		{
			PLAYER_HP--;
			std::cout << "HP: " << PLAYER_HP << std::endl;
			if (PLAYER_HP == 0)
			{
				std::cout << "\nYour Score: " << PlayerScore << std::endl;
				//DevyEngine::fatalError("Game Over");

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

	myfile << "_numNPC: 1\n";

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

/*SquareGraph MainGame::readMap(const std::string& FileName)
{

	const int mapDimension = _levels[_currentLevel]->getWidth();	//dimension of the map ([200][200])
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
		int px = _humans[0]->getPosition().x / 64;
		int py = _humans[0]->getPosition().y / 64;
		graph.setSecondRobotPos(make_pair(px, py));		//set the player coordinates

		for (int i = 0; i < _zombies.size(); i++)
		{

			int x = _zombies[i]->getPosition().x / 64;
			int y = _zombies[i]->getPosition().y / 64;
			graph.setFirstRobotPos(make_pair(x, y));	//set zombie coordinates

		}
		path = graph.executeAStar();

		graph.printPath(path);

		inputFile.close();
		return graph;
	}
}*/

////////////////////////////////////////////////////
MainGame::~MainGame()
{
	for (int i = 0; i < _levels.size(); i++)
	{
		delete _levels[i];
	}
}