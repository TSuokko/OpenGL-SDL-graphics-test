#include "PhysicsGame.h"
#include <DevyEngine/Errors.h>
#include <DevyEngine/Window.h>
#include <DevyEngine/DevyEngine.h>
#include <DevyEngine/ResourceManager.h>
#include <DevyEngine/ParticleBatch2D.h>
#include <DevyEngine/Vertex.h>
#include <random>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <glm/gtx/rotate_vector.hpp>

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

	m_waterParticles = new DevyEngine::ParticleBatch2D;
	m_waterParticles->init(100.f,0.005f, DevyEngine::ResourceManager::getTexture("C:/Users/TuomasSuo/Desktop/OpenGL-SDL-graphics-test/PhysicsGame/Textures/jumpgame/PNG/Bubble_Small.png"));
	m_particleEngine.addParticleBatch(m_waterParticles);

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
			std::cout << _input.getMouseCoords().x << _input.getMouseCoords().y << std::endl;
			_input.keyPress(_event.button.button);
			addWater(glm::vec2(_input.getMouseCoords().x - _screenWidth/2, -_input.getMouseCoords().y + _screenHeight/2), 500);
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


	//render particles
	m_particleEngine.draw(&_agentSpriteBatch);

	_colorProgram.unuse();

	_window.swapBuffer();

}

void PhysicsGame::addWater(const glm::vec2& position, int numParticles)
{
	static std::mt19937 randEngine(time(nullptr));
	static std::uniform_real_distribution<float> randAngle(0.f, 360.f);
	glm::vec2 vel(1.f, 0.0f);
	DevyEngine::Color col;
	col.r = 0;
	col.g = 0;
	col.b = 255;
	col.a = 255;
	for (int i = 0; i < numParticles; i++) {
		m_waterParticles->addParticle(position, glm::rotate(vel, randAngle(randEngine)), col, 10);
	}
}


void PhysicsGame::gameloop()
{
	const float DESIRED_FPS = 60.0f; // FPS the game is designed to run at
	const float MS_PER_SECOND = 1000; // Number of milliseconds in a second
	const float MAX_DELTA_TIME = 1.0f;
	const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS; // The desired frame time per frame
	float previousTicks = SDL_GetTicks();
	const int MAX_PHYSICS_STEPS = 6; // Max number of physics steps per frame

	while (_game != GameState::EXIT)
	{
		_fpslimiter.begin();

		// Calculate the frameTime in milliseconds
		float newTicks = SDL_GetTicks();
		float frameTime = newTicks - previousTicks;
		previousTicks = newTicks; // Store newTicks in previousTicks so we can use it next frame
								  // Get the total delta time
		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;


		int i = 0; // This counter makes sure we don't spiral to death!
				   // Loop while we still have steps to process.
		while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS) {
			// The deltaTime should be the the smaller of the totalDeltaTime and MAX_DELTA_TIME
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
			// Update all physics here and pass in deltaTime
			m_particleEngine.update(deltaTime);
			// Since we just took a step that is length deltaTime, subtract from totalDeltaTime
			totalDeltaTime -= deltaTime;
			// Increment our frame counter so we can limit steps to MAX_PHYSICS_STEPS
			i++;
		}

		processInput();

		_camera.update();

		drawGame();

		

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