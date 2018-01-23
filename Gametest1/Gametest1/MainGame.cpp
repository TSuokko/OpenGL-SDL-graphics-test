#include "MainGame.h"
#include <iostream>
#include <string>
#include "Errors.h"
#include "ImageLoader.h"

MainGame::MainGame(): _window(nullptr), 
	_screenWidth(1024), 
	_screenHeight(768),
	_game(GameState::PLAY), 
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
		case SDL_MOUSEMOTION:
			std::cout << _event.motion.x<<" "<< _event.motion.y<<std::endl;
		}
	}
}

void MainGame::drawGame()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_colorProgram.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _playerTexture.id);
	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	GLuint timeLocation = _colorProgram.getUniformLocation("time");
	glUniform1f(timeLocation, _time);

	_sprite.draw();

	glBindTexture(GL_TEXTURE_2D, 0);
	_colorProgram.unuse();
	
	SDL_GL_SwapWindow(_window);
}

void MainGame::gameloop()
{
	while (_game != GameState::EXIT)
	{
		processInput();
		_time += 0.1;
		drawGame();
	}
}
void MainGame::run()
{
	initSystems();
	_sprite.init(-1.0f, -1.0f, 2.0f, 2.0f);

	_playerTexture = ImageLoader::loadPNG("Textures/jumpgame/PNG/Tails.png");

	gameloop();

}
void MainGame::initSystems()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	_window = SDL_CreateWindow("GameTest", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_OPENGL);
	if (_window == nullptr)
	{
		fatalError("SDL window could not open");
	}
	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	if (glContext == nullptr)
	{
		fatalError("SDL_GL context could not be created");
	}

	GLenum error = glewInit();

	if (error != GLEW_OK)
	{
		fatalError("Could not init glew");
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	glClearColor(0.0f,1.0f,0.0f,0.0f);

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