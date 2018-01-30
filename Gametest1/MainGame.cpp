#include "MainGame.h"
#include <iostream>
#include <string>
#include <DevyEngine/Errors.h>
#include <DevyEngine/Window.h>
#include <DevyEngine/DevyEngine.h>
#include <DevyEngine\ResourceManager.h>

MainGame::MainGame(): 
	_screenWidth(1024), 
	_screenHeight(768),
	_game(GameState::PLAY), 
	_maxFPS(60.0f),
	_time(0)
{
	_camera.init(_screenWidth, _screenHeight);
}
	
	

MainGame::~MainGame(){}

void MainGame::processInput()
{
	const float cameraSpeed = 20.0f;
	const float scaleSpeed = 0.1f;

	while (SDL_PollEvent(&_event))
	{
		switch (_event.type)
		{
		case SDL_QUIT:
			_game = GameState::EXIT;
			break;
		case SDL_KEYDOWN:
			switch (_event.key.keysym.sym)
			{
			case SDLK_w:
				_camera.setPosition(_camera.getPosition() + glm::vec2(0.0, -cameraSpeed));
				break;
			
			case SDLK_s:
				_camera.setPosition(_camera.getPosition() + glm::vec2(0.0, cameraSpeed));
				break;
			case SDLK_a:
				_camera.setPosition(_camera.getPosition() + glm::vec2(cameraSpeed, 0.0f));
				break;
			case SDLK_d:
				_camera.setPosition(_camera.getPosition() + glm::vec2(-cameraSpeed, 0.0f));
				break;
			case SDLK_q:
				_camera.setScale(_camera.getScale() - scaleSpeed);
				break;
			case SDLK_e:
				_camera.setScale(_camera.getScale() + scaleSpeed);
				break;
			}
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

	GLuint pLocation = _colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();

	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	_spriteBatch.begin();

	glm::vec4 pos(0.0f, 0.0f, 50.0f, 50.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static DevyEngine::GLTexture texture = DevyEngine::ResourceManager::getTexture("Textures/jumpgame/PNG/Tails.png");
	DevyEngine::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;


	_spriteBatch.draw(pos, uv, texture.id, 0.0f, color);
	_spriteBatch.end();
	_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);
	_colorProgram.unuse();
	
	_window.swapBuffer();
}

void MainGame::gameloop()
{
	while (_game != GameState::EXIT)
	{
		float startTicks = SDL_GetTicks();

		processInput();
		_time += 0.1f;

		_camera.update();

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

	gameloop();

}
void MainGame::initSystems()
{
	DevyEngine::init();

	_window.create("GameEngine", _screenWidth, _screenHeight, 0);

	initShaders();

	_spriteBatch.init();
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