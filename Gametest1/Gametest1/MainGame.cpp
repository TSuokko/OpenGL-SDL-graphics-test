#include "MainGame.h"
#include <iostream>
#include <string>
#include <DevyEngine/Errors.h>
#include <DevyEngine/Window.h>
#include <DevyEngine/DevyEngine.h>

MainGame::MainGame(): 
	_screenWidth(1024), 
	_screenHeight(768),
	_game(GameState::PLAY), 
	_maxFPS(60.0f),
	_time(0){}
	
	

MainGame::~MainGame(){}

void MainGame::processInput()
{
	while (SDL_PollEvent(&_event))
	{
		switch (_event.type)
		{
		case SDL_QUIT:
			_game = GameState::EXIT;
			break;
		//case SDL_MOUSEMOTION:
			//std::cout << _event.motion.x<<" "<< _event.motion.y<<std::endl;
		}
	}
}

void MainGame::drawGame()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_colorProgram.use();
	glActiveTexture(GL_TEXTURE0);

	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	GLuint timeLocation = _colorProgram.getUniformLocation("time");
	glUniform1f(timeLocation, _time);


	for (int i = 0; i < _sprites.size(); i++)
	{
		_sprites[i]->draw();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	_colorProgram.unuse();
	
	_window->swapBuffer();
}

void MainGame::gameloop()
{
	while (_game != GameState::EXIT)
	{
		float startTicks = SDL_GetTicks();

		processInput();
		_time += 0.1f;
		drawGame();
		calculateFPS();
		//print only once every 10 frames
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 5)
		{
			std::cout << _fps << std::endl;
			frameCounter = 0;
		}
		//limit fps to max fps

		float frameTicks = SDL_GetTicks() - startTicks;
		if (1000.0f / _maxFPS > frameTicks)
		{
			SDL_Delay(1000.0f / _maxFPS - frameTicks);
		}
	}
}
void MainGame::run()
{
	initSystems();

	_sprites.push_back(new DevyEngine::Sprite());
	_sprites.back()->init(-1.0f, -1.0f, 1.0f, 1.0f, "Textures/jumpgame/PNG/Tails.png");
	_sprites.push_back(new DevyEngine::Sprite());
	_sprites.back()->init(0.0f, -1.0f, 1.0f, 1.0f, "Textures/jumpgame/PNG/Tails.png");
	_sprites.push_back(new DevyEngine::Sprite());
	_sprites.back()->init(-1.0f, 0.0f, 1.0f, 1.0f, "Textures/jumpgame/PNG/Tails.png");
	_sprites.push_back(new DevyEngine::Sprite());
	_sprites.back()->init(0.0f, 0.0f, 1.0f, 1.0f, "Textures/jumpgame/PNG/Tails.png");

	gameloop();

}
void MainGame::initSystems()
{
	DevyEngine::init();

	_window->create("Game Engine", _screenWidth, _screenHeight, 0);

	initShaders();
}

void MainGame::initShaders()
{
	_colorProgram.compileShaders("Shaders/VertexShader.txt", "Shaders/FragmentShader.txt");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
}

void MainGame::calculateFPS()
{
	static const int NUM_SAMPLES = 10;
	static float frameTimes[NUM_SAMPLES];
	static int currentFrame = 0;
	
	static float prevTicks = SDL_GetTicks();
	float currentTicks;
	currentTicks = SDL_GetTicks();

	_frameTime = currentTicks - prevTicks;
	frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;
	prevTicks = currentTicks;

	int count;
	currentFrame++;

	if (currentFrame < NUM_SAMPLES)
	{
		count = currentFrame;
	}
	else 
	{
		count = NUM_SAMPLES;
	}

	float frameTimeAverage = 0;
	for (int i = 0; i < count; i++)
	{
		frameTimeAverage += frameTimes[i];
	}
	frameTimeAverage /= count;
	if (frameTimeAverage > 0)
	{
		_fps = 1000.0f / frameTimeAverage;
	}
	else
	{
		_fps = 60.0f;
	}
	
}