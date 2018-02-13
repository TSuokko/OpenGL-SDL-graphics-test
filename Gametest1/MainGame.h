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
#include "Player.h"
#include "Zombie.h"
enum class GameState{PLAY, EXIT};

class Zombie;

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();
	
private:
	void initSystems();
	void initShaders();
	void initLevels();
	void processInput();
	void gameloop();
	void drawGame();
	void drawDungeon();

	void updateAgents();

	

	DevyEngine::Window _window;

	int _screenWidth;
	int _screenHeight;
	GameState _game;
	SDL_Event _event;

	DevyEngine::Sprite _sprite;
	DevyEngine::GLSLprogram _colorProgram;
	DevyEngine::Camera2D _camera;
	DevyEngine::SpriteBatch _agentSpriteBatch;

	DevyEngine::InputManager _input;
	DevyEngine::SpriteBatch _spriteBatch;
	DevyEngine::FpsLimiter _fpslimiter;
	
	std::vector<Bullet> _bullets;
	std::vector<Level*> _levels;

	Player* _player;
	std::vector<Agent*> _agents;
	std::vector<Zombie*> _zombies;
	std::vector<Human*> _humans;
	int _currentLevel;

	float _fps;
	float _maxFPS;
	float _time;


};

