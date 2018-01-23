#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "Sprite.h"
#include "GLSLprogram.h"
#include "GLTexture.h"
#include <vector>
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
	void calculateFPS();


	SDL_Window* _window;
	int _screenWidth;
	int _screenHeight;
	GameState _game;
	SDL_Event _event;
	Sprite _sprite;
	GLSLprogram _colorProgram;

	float _maxFPS;
	float _time;
	float _fps;
	float _frameTime;

	std::vector<Sprite*> _sprites;


};

