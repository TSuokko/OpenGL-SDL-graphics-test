
//	LUO DEBUG TILANNE JOSSA ON 10X10 MAP JA TOISTUVA SAMA PATHFIND BUGI, JOTTA DEBUGGAUS OLIS HELPOMPAA. MAHDOLLISESTI MYÖS PIIRTO OMINAISUUS
//making games with ben --> 21:00

#include "MainGame.h"

#include <Windows.h>
#include <cstdio>
#undef Rectangle


#include <DevyEngine/Errors.h>
#include <DevyEngine/Window.h>
#include <DevyEngine/DevyEngine.h>
#include <DevyEngine\ResourceManager.h>

#include "Point.h"
#include "RectangleMap.h"
#include "Leaf.h"
//#include "Node.h"

#include <iomanip>
#include <queue>
#include <math.h>

#include <src/game.h>

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

#define TEST_SEED 1

typedef void(*LoopType)();
LoopType LoopPtr;
HMODULE GameDLL;
FILETIME GameDLLWriteTime;


//Constructor
MainGame::MainGame() :
	_screenWidth(800),
	_screenHeight(600),
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
	drawDungeon(time(NULL));
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
	glClearColor(0.6, 0.6f, 0.7f, 1.0f); //grey background
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
	_player->init(10.0f, _levels[_currentLevel]->getStartPlayerPos(), &_input);
	_humans.push_back(_player);

	std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));
	
	std::uniform_int_distribution<int> randX(2, _levels[_currentLevel]->getWidth() - 1);
	std::uniform_int_distribution<int> randY(2, _levels[_currentLevel]->getHeight() - 1);

	const float ZOMBIE_SPEED = 4.f;
	//add NPC:s
	for (int i = 0; i < _levels[_currentLevel]->getNumNPC(); i++)
	{
		_zombies.push_back(new Zombie);
		glm::vec2 pos(randX(randomEngine) * TILE_WIDTH, randY(randomEngine) * TILE_WIDTH);
		//glm::vec2 pos(120 *TILE_WIDTH , 120 *TILE_WIDTH );
		std::cout << "\nX: " << pos.x / TILE_WIDTH << " Y:" << pos.y / TILE_WIDTH<< std::endl;
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
			std::cout <<_player->getPosition().x/64 + _input.getMouseCoords().x - 426 << " " << (int)_player->getPosition().y/64 + _input.getMouseCoords().y - 270<< std::endl;
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

	for (unsigned int i = 0; i < _humans.size(); i++)
	{
		_humans[i]->draw(_agentSpriteBatch);
	}

	for (unsigned int i = 0; i < _zombies.size(); i++)
	{
		_zombies[i]->draw(_agentSpriteBatch);
	}
	
	_agentSpriteBatch.end();
	
	_agentSpriteBatch.renderBatch();

	_colorProgram.unuse();
	
	_window.swapBuffer();

}

FILETIME Win32GetLastWriteTime(char* path)
{
	FILETIME time = {};
	WIN32_FILE_ATTRIBUTE_DATA data;

	if (GetFileAttributesEx(path, GetFileExInfoStandard, &data))
		time = data.ftLastWriteTime;

	return time;
}

void UnloadGameDLL()
{
	FreeLibrary(GameDLL);
	GameDLL = 0;
	LoopPtr = 0;
}

void LoadGameDLL()
{
	WIN32_FILE_ATTRIBUTE_DATA unused;
	if (!GetFileAttributesEx("lock.tmp", GetFileExInfoStandard, &unused))
	{
		UnloadGameDLL();
		CopyFile("game.dll", "game_temp.dll", 0);
		GameDLL = LoadLibrary("game_temp.dll");

		if (!GameDLL)
		{
			DWORD err = GetLastError();
			printf("Can't load lib: %d\n", err);
			return;
		}

		LoopPtr = (LoopType)GetProcAddress(GameDLL, "Loop");
		if (!LoopPtr)
		{
			DWORD err = GetLastError();
			printf("Cant load func: %d\n", err);
			return;
		}

		GameDLLWriteTime = Win32GetLastWriteTime("game.dll");
	}
}



void MainGame::gameloop()
{
	LoadGameDLL();
	while (_game != GameState::EXIT)
	{
		FILETIME newTime = Win32GetLastWriteTime("game.dll");

		if (CompareFileTime(&newTime, &GameDLLWriteTime))
			LoadGameDLL();

		LoopPtr();

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
			//std::cout << "Current pos: "<<_player->getPosition().x/64 << " " << _player->getPosition().y/64 << std::endl;
			//std::cout << PlayerScore << std::endl;
			
			frameCounter = 0;
		}
	}
}


void MainGame::updateAgents()
{	
	for (unsigned int i = 0; i < _humans.size(); i++)
	{
		_humans[i]->update(_levels[_currentLevel]->getLevelData(),
			_humans,
			_zombies);	
	}
	for (unsigned int i = 0; i < _zombies.size(); i++)
	{
		_zombies[i]->update(_levels[_currentLevel]->getLevelData(),
			_humans,
			_zombies);
		
		//_zombies[i]->aStar(path);
		
	}


	for (unsigned int i = 0; i < _humans.size(); i++)
	{
		// Collide with other humans
		for (unsigned int j = i + 1; j < _zombies.size(); j++)
		{
			(_humans[i]->collideWithAgent(_zombies[j]));		
		}		
	}
	
	for (unsigned int i = 0; i < _zombies.size(); i++)
	{
		// Collide with other zombies
		for (unsigned int j = i + 1; j < _zombies.size(); j++)
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
void MainGame::drawDungeon(unsigned int seed)
{
	srand(seed);

	

	int MAX_LEAF_SIZE = 30;
	Leaf* root = new Leaf(0, 0, 200, 200);
	std::list<Leaf> leaf_edge_nodes;
	std::list<RectangleMap> halls;
	root->generate(MAX_LEAF_SIZE);

	root->createRooms(&leaf_edge_nodes, &halls);

	// need a char map (space is char 40, wall is char 179);
	
	for (int i = 0; i < 200; i++) 
	{
		for (int j = 0; j < 200; j++) 
		{
			levelMap[i][j] = 35; //creates ASCII wall symbol
		}
	}

	for (std::list<Leaf>::iterator l = leaf_edge_nodes.begin(); l != leaf_edge_nodes.end(); ++l) {

		RectangleMap* room = l->getRoom();
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
					levelMap[i][j] = 46;
				}
			}
		}
	}

	for (std::list<RectangleMap>::iterator hh = halls.begin(); hh != halls.end(); ++hh)
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
				levelMap[i][j] = 46;
			}
		}
	}

	//surround map with walls
	for (int i = 0; i < 200; i++) //y
	{
		if (levelMap[i][200 - 1] == 46)
			levelMap[i][200 - 1] = 35; //creates ASCII wall symbol
		if (levelMap[i][200 - 1] == 46)
			levelMap[i][200 - 1] = 35; //creates ASCII wall symbol
		if (levelMap[0][i] == 46)
			levelMap[0][i] = 35; //creates ASCII wall symbol
		if (levelMap[200 - 1][i] == 46)
			levelMap[200 - 1][i] = 35; //creates ASCII wall symbol
	}

	std::cout << "\n=== DUNGEON GENERATOR v0.2 ===\n\n";
	std::ofstream myfile("Level1.txt");

	myfile << "_numNPC: 25\n";

	for (int j = 0; j < 200; j++)
	{
		for (int i = 0; i < 200; i++)
		{
			//print the level data to a .txt file
			if (myfile.is_open())
			{
				myfile << levelMap[i][j];
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

MainGame::~MainGame()
{
	for (unsigned int i = 0; i < _levels.size(); i++)
	{
		delete _levels[i];
	}
}