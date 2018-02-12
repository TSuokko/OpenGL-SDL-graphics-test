#include "MainGame.h"
#include <iostream>
#include <string>
#include <DevyEngine/Errors.h>
#include <DevyEngine/Window.h>
#include <DevyEngine/DevyEngine.h>
#include <DevyEngine\ResourceManager.h>

#include <string>
#include <functional>
#include <stdlib.h>
#include <vector>
#include <list>
#include <cmath>
#include <time.h>       /* time */
#include "Point.h"
#include "Rectangle.h"
#include "Leaf.h"
#include <random>
#include <ctime>

#include<fstream>
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
	_player->init(8.0f, _levels[_currentLevel]->getStartPlayerPos(), &_input);
	_humans.push_back(_player);

	std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));
	std::uniform_int_distribution<int> randX(2, _levels[_currentLevel]->getWidth() - 1);
	std::uniform_int_distribution<int> randY(2, _levels[_currentLevel]->getHeight() - 1);

	const float HUMAN_SPEED = 1.0f;
	//add NPC:s
	for (int i = 0; i < _levels[_currentLevel]->getNumNPC(); i++)
	{
		//std::cout << "dude created"<<std::endl; //debug
		_humans.push_back(new Human);
		glm::vec2 pos(randX(randomEngine) * TILE_WIDTH, randY(randomEngine) * TILE_WIDTH);
		_humans.back()->init(HUMAN_SPEED, pos);
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
		if (frameCounter == 1000)
		{
			std::cout << _fps << std::endl;
			frameCounter = 0;
		}
	}
}


void MainGame::updateAgents()
{
	for (int i = 0; i < _humans.size(); i++) {
		_humans[i]->update(_levels[_currentLevel]->getLevelData(),
			_humans,
			_zombies);
	}
	for (int i = 0; i < _humans.size(); i++) {
		// Collide with other humans
		for (int j = i + 1; j < _humans.size(); j++) {
			_humans[i]->collideWithAgent(_humans[j]);
		}
	}
	
}



void MainGame::drawDungeon()
{
	//DUNGEON BSP GENERATOR ALGORITHM
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

	for (std::list<Rectangle>::iterator hh = halls.begin(); hh != halls.end(); ++hh) {

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
				/*if (i == 2 && j == 98)
				{
					//print the player 
					myfile << "@";
					
				}*/
			}
			else std::cout << "Unable to open file";
		}
	}
	myfile.close();
}

MainGame::~MainGame()
{
	for (int i = 0; i < _levels.size(); i++)
	{
		delete _levels[i];
	}
}