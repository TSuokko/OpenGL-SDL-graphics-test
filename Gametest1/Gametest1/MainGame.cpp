#include "MainGame.h"
#include <iostream>
#include <string>
#include "Errors.h"


MainGame::MainGame(): _window(nullptr), 
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
	
	SDL_GL_SwapWindow(_window);
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

	_sprites.push_back(new Sprite());
	_sprites.back()->init(-1.0f, -1.0f, 1.0f, 1.0f, "Textures/jumpgame/PNG/Tails.png");
	_sprites.push_back(new Sprite());
	_sprites.back()->init(0.0f, -1.0f, 1.0f, 1.0f, "Textures/jumpgame/PNG/Tails.png");
	_sprites.push_back(new Sprite());
	_sprites.back()->init(-1.0f, 0.0f, 1.0f, 1.0f, "Textures/jumpgame/PNG/Tails.png");
	_sprites.push_back(new Sprite());
	_sprites.back()->init(0.0f, 0.0f, 1.0f, 1.0f, "Textures/jumpgame/PNG/Tails.png");

	gameloop();

}
void MainGame::initSystems()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

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
	//check opengl version
	std::printf("*** OpenGL Version: %s ***", glGetString(GL_VERSION));
	//set background color
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	//set vsync
	SDL_GL_SetSwapInterval(0);

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