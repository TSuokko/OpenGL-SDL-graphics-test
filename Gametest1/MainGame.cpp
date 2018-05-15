#include "MainGame.h"

int PLAYER_HP = 20;
int PlayerScore = 0;


//#include <src/game.h>
//typedef void(*LoopType)();
//LoopType LoopPtr;
//HMODULE GameDLL;
//FILETIME GameDLLWriteTime;


//Constructor
MainGame::MainGame() :
	_screenWidth(800),
	_screenHeight(800),
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
	//randomly generated dungeon with random seed
	drawDungeon((int)time(NULL)); 
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
	//initialize game engine
	DevyEngine::init();
	_window.create("Another Zombie Survival Game", _screenWidth, _screenHeight, 0);
	//grey background
	glClearColor((GLclampf)0.6, (GLclampf)0.6, (GLclampf)0.7, (GLclampf)1.0);
	initShaders();
	_agentSpriteBatch.init();
	_spriteBatch.init();
	_fpslimiter.init(_maxFPS);
}

void MainGame::initLevels()
{
	//pushes back the level data that was created during the level creation method
	_levels.push_back(new Level("Level1.txt")); 
	_currentLevel = 0;
	
	_player = new Player();
	//initialize player with speed = 7, position in the middle of the map, and working keyboard & mouse input
	_player->init(3.5f, _levels[_currentLevel]->getStartPlayerPos(), &_input, &_camera, &_bullets);
	_humans.push_back(_player);

	//random positioning for the zombies. 
	std::mt19937 randomEngine;
	randomEngine.seed((int)time(nullptr));
	std::uniform_int_distribution<int> randX(2, MAP_SIZE - 1);
	std::uniform_int_distribution<int> randY(2, MAP_SIZE - 1);

	const float ZOMBIE_SPEED = 4.f;
	//add Zombies to the map
	for (int i = 0; i < _levels[_currentLevel]->getNumNPC(); i++)
	{
		_zombies.push_back(new Zombie);
		glm::vec2 pos(randX(randomEngine) * TILE_WIDTH, randY(randomEngine) * TILE_WIDTH);
		_zombies.back()->init(ZOMBIE_SPEED, pos);
	}

	//set up players guns
	_player->addGun(new Gun("Pistol", 10, 1, 0.2f, 50, 20.f));
	_player->addGun(new Gun("Shotgun", 30, 12, 0.4f, 5, 20.f));
	_player->addGun(new Gun("Machine Gun", 2, 1, 0.4f, 15, 20.f));

}

void MainGame::processInput()
{
	//map scaling speed, needs to be quite small
	const float scaleSpeed = 0.005f; 
	
	while (SDL_PollEvent(&_event))
	{
		switch (_event.type)
		{
		case SDL_QUIT:
			//allows you to quit the game
			_game = GameState::EXIT; 
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
	//opengl graphics stuff 
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//using the engines color program 
	_colorProgram.use();
	glActiveTexture(GL_TEXTURE0);
	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);
	GLuint pLocation = _colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	//draw the first generated level, since currentLevel is equal to 0
	_levels[_currentLevel]->draw();

	//player and zombie spritebatch initialization 
	_agentSpriteBatch.begin();
	//character sizes
	const glm::vec2 agentDims(AGENT_RADIUS * 2.0f);

	//currently the only "human" is the player
	for (unsigned int i = 0; i < _humans.size(); i++)
	{
		_humans[i]->draw(_agentSpriteBatch);
	}
	//draw all the zombies that were pushed into the vector
	for (unsigned int i = 0; i < _zombies.size(); i++)
	{
		_zombies[i]->draw(_agentSpriteBatch);
	}
	//draw bullets
	for (unsigned int i = 0; i < _bullets.size(); i++)
	{
		_bullets[i].draw(_agentSpriteBatch);
	}

	//end the opengl graphics stuff
	_agentSpriteBatch.end();
	_agentSpriteBatch.renderBatch();
	_colorProgram.unuse();
	_window.swapBuffer();
}

void MainGame::updateBullets()
{
	//update with world
	for (int i = 0; i < _bullets.size();)
	{
		if (_bullets[i].update(_levels[_currentLevel]->getLevelData()))
		{
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
		}else{
			i++;
		}
	}

	for (int i = 0; i < _bullets.size();i++)
	{
	
		//collide with humans and zombies
		for (int j = 0; j < _zombies.size(); j++)
		{
			if (_bullets[i].collideWithAgent(_zombies[j]))
			{
				//damage zombie
				if (_zombies[j]->applyDamage(_bullets[i].getDamage()))
				{
					delete _zombies[j];
					_zombies[j] = _zombies.back();
					_zombies.pop_back();
				}else{
					j++;
				}
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
			
				i--;
				break;
			}else{
				j++;
			}
		}
	}
}


//experimentation wih code using c-hotloading, didn't really work, but still wanna save it. 
/*FILETIME Win32GetLastWriteTime(char* path)
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
}*/

void MainGame::gameloop()
{
	//LoadGameDLL();
	while (_game != GameState::EXIT)
	{
		/*//more c-hotloading stuff
		FILETIME newTime = Win32GetLastWriteTime("game.dll");

		if (CompareFileTime(&newTime, &GameDLLWriteTime))
			LoadGameDLL();

		LoopPtr();
		*/

		//fps initialization
		_fpslimiter.begin(); 
		processInput();
		_time += 0.1f;
		//agent movement and collision updates
		updateAgents();

		updateBullets();

		//track player position, camera is targeted to the middle of the player
		_camera.setPosition(_player->getPosition()+glm::vec2(TILE_WIDTH/2, TILE_WIDTH/2));
		_camera.update();

		drawGame();
		//current fps
		_fps = _fpslimiter.end();

		//frame timer that calculates the time (points) the player has survived
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 100)
		{
			//also shows current fps
			//std::cout <<"FPS: " <<_fps<<"\n";
			PlayerScore += 10;
			frameCounter = 0;
		}
	}
}


void MainGame::updateAgents()
{	
	//movement updates for all humans (currently only the player)
	for (unsigned int i = 0; i < _humans.size(); i++)
	{
		_humans[i]->update(_levels[_currentLevel]->getLevelData(),
			_humans,
			_zombies);	
	}
	//movement updates for all the zombies
	for (unsigned int i = 0; i < _zombies.size(); i++)
	{
		_zombies[i]->update(_levels[_currentLevel]->getLevelData(),
			_humans,
			_zombies);
	}
	//human collision updates
	for (unsigned int i = 0; i < _humans.size(); i++)
	{
		// Collide with other humans
		for (unsigned int j = i + 1; j < _zombies.size(); j++)
		{
			_humans[i]->collideWithAgent(_zombies[j]);		
		}		
	}
	//zombie collision updates
	for (unsigned int i = 0; i < _zombies.size(); i++)
	{
		// Collide with other zombies
		for (unsigned int j = i + 1; j < _zombies.size(); j++)
		{
			_zombies[i]->collideWithAgent(_zombies[j]);
		}
		//player collision reduces player hit points
		if (_zombies[i]->collideWithAgent(_player))
		{
			PLAYER_HP--;
			std::cout << "HP: " << PLAYER_HP << std::endl;
			if (PLAYER_HP == 0)
			{
				//game end screen
				std::cout << "\nYour Score: " << PlayerScore << std::endl;
				DevyEngine::fatalError("Game Over");

			}
		}
	}
}


///////////////////////////////DUNGEON BSP GENERATOR ALGORITHM
void MainGame::drawDungeon(unsigned int seed)
{
	//creates a "random" char map the size of 200x200, currently only with walls and empty spaces. 
	srand(seed);

	int MAX_LEAF_SIZE = 30;
	Leaf* root = new Leaf(0, 0, MAP_SIZE, MAP_SIZE);
	std::list<Leaf> leaf_edge_nodes;
	std::list<RectangleMap> halls;
	root->generate(MAX_LEAF_SIZE);
	root->createRooms(&leaf_edge_nodes, &halls);

	for (int i = 0; i < MAP_SIZE; i++) 
	{
		for (int j = 0; j < MAP_SIZE; j++) 
		{
			levelMap[i][j] = 35; //creates ASCII wall symbol
		}
	}

	for (std::list<Leaf>::iterator l = leaf_edge_nodes.begin(); l != leaf_edge_nodes.end(); ++l)
	{
		RectangleMap* room = l->getRoom();
		int left	= room->left();
		int right	= room->right();
		int top		= room->top();
		int bottom	= room->bottom();

		if (left	< 1) left	= 1;
		if (right	< 1) right	= 1;
		if (top		< 1) top	= 1;
		if (bottom	< 1) bottom = 1;

		if (right - left > 3 && bottom - top > 3) 
		{
			for (int i = left; i <= right; i++) 
			{
				for (int j = top; j <= bottom; j++) 
				{
					levelMap[i][j] = 46;
				}
			}
		}
	}

	for (std::list<RectangleMap>::iterator hh = halls.begin(); hh != halls.end(); ++hh)
	{
		int left	= hh->left();
		int right	= hh->right();
		int top		= hh->top();
		int bottom	= hh->bottom();

		if (left	< 1) left	= 1;
		if (right	< 1) right	= 1;
		if (top		< 1) top	= 1;
		if (bottom	< 1) bottom = 1;

		if (left	> MAP_SIZE - 1) left	= MAP_SIZE - 2;
		if (right	> MAP_SIZE - 1) right	= MAP_SIZE - 2;
		if (top		> MAP_SIZE - 1) top		= MAP_SIZE - 2;
		if (bottom	> MAP_SIZE - 1) bottom	= MAP_SIZE - 2;

		for (int i = left; i <= right; i++) 
		{
			for (int j = top; j <= bottom; j++)
			{
				levelMap[i][j] = 46;
			}
		}
	}

	//surround map with walls, so nothing can go outside the borders
	for (int i = 0; i < MAP_SIZE; i++) //y
	{
		//upper wall
		if (levelMap[i][0] == 46)//if it is empty space
			levelMap[i][0] = 35; //this turns it into a ASCII wall symbol
		//left wall
		if (levelMap[0][i] == 46)
			levelMap[0][i] = 35;
		//lower wall
		if (levelMap[i][MAP_SIZE - 1] == 46)
			levelMap[i][MAP_SIZE - 1] = 35;
		//right wall
		if (levelMap[MAP_SIZE - 1][i] == 46)
			levelMap[MAP_SIZE - 1][i] = 35;
	}

	//now that the char map has been generated above, it's time to turn it into a .txt file, which can be read later on. 
	std::cout << "\n=== DUNGEON GENERATOR v0.2 ===\n\n";
	std::ofstream myfile("Level1.txt");
	//change the number for the amount of zombies you want in game
	myfile << "_numNPC: 100\n";

	for (int j = 0; j < MAP_SIZE; j++)
	{
		for (int i = 0; i < MAP_SIZE; i++)
		{
			//print the level data to a .txt file
			if (myfile.is_open())
			{
				myfile << levelMap[i][j];
				if (i == MAP_SIZE - 1)
				{
					//divide the map into mapsize character lengths (for example if mapsize is 200, 0 -> 199)
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