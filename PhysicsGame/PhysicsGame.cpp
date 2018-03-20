#include "PhysicsGame.h"
#include <DevyEngine/Errors.h>
#include <DevyEngine/Window.h>
#include <DevyEngine/DevyEngine.h>
#include <DevyEngine/ResourceManager.h>


PhysicsGame::PhysicsGame() :
	_screenWidth(800),
	_screenHeight(600),
	_game(GameState::PLAY),
	_maxFPS(60.0f),
	_time(0)
	//,_player(nullptr)
{
	_camera.init(_screenWidth, _screenHeight);
}
//run the game and start initializing
void PhysicsGame::run()
{	
	initSystems();	
	gameloop();

}

//Shader initialization
void PhysicsGame::initShaders()
{
	_colorProgram.compileShaders("Shaders/VertexShader.txt", "Shaders/FragmentShader.txt");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
}

void PhysicsGame::initSystems()
{
	DevyEngine::init();

	_window.create("GameEngine", _screenWidth, _screenHeight, 0);
	glClearColor(0.6, 0.7f, 0.7f, 1.0f); //grey background
	initShaders();

	_agentSpriteBatch.init();
	_spriteBatch.init();

	_fpslimiter.init(_maxFPS);
}

void PhysicsGame::processInput()
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
	{
		_camera.setScale(_camera.getScale() - scaleSpeed);
	}
	if (_input.isKeyPressed(SDLK_e))
	{
		_camera.setScale(_camera.getScale() + scaleSpeed);
	}
}

void PhysicsGame::drawGame()
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

	_colorProgram.unuse();

	_window.swapBuffer();

}

void PhysicsGame::gameloop()
{
	while (_game != GameState::EXIT)
	{
		_fpslimiter.begin();

		processInput();
		_time += 0.1f;

		_camera.update();

		drawGame();

		//readMap("Level1.txt");

		_fps = _fpslimiter.end();

		//print only once every 10 frames
		static int frameCounter = 0;
		frameCounter++;

		if (frameCounter == 100)
		{
			frameCounter = 0;
		}
	}
}

PhysicsGame::~PhysicsGame()
{
}