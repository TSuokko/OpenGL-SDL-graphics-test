#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <DevyEngine/Sprite.h>
#include <DevyEngine/GLSLprogram.h>
#include <DevyEngine/GLTexture.h>
#include <vector>
#include <DevyEngine/Window.h>
#include <DevyEngine/Camera2D.h>
#include <DevyEngine\SpriteBatch.h>
#include <DevyEngine\InputManager.h>
#include <DevyEngine\Timing.h>
#include "Bullet.h"
#include "Level.h"
enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();
	
private:
	void initSystems();
	void initShaders();
	void processInput();
	void gameloop();
	void drawGame();
	void drawDungeon();



	DevyEngine::Window _window;

	int _screenWidth;
	int _screenHeight;
	GameState _game;
	SDL_Event _event;

	DevyEngine::Sprite _sprite;
	DevyEngine::GLSLprogram _colorProgram;
	DevyEngine::Camera2D _camera;
	DevyEngine::InputManager _input;
	DevyEngine::SpriteBatch _spriteBatch;
	DevyEngine::FpsLimiter _fpslimiter;
	
	std::vector<Bullet> _bullets;
	std::vector<Level*> _levels;
	int _currentLevel;

	float _fps;
	float _maxFPS;
	float _time;


};

