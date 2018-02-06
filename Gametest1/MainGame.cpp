#include "MainGame.h"
#include <iostream>
#include <string>
#include <DevyEngine/Errors.h>
#include <DevyEngine/Window.h>
#include <DevyEngine/DevyEngine.h>
#include <DevyEngine\ResourceManager.h>

#include <string>
#include <functional>
#include <stdlib.h>
#include <vector>
#include <list>
#include <cmath>
#include <time.h>       /* time */
#include "Point.h"
#include "Rectangle.h"
#include "Leaf.h"

#include<fstream>

MainGame::MainGame(): 
	_screenWidth(1024), 
	_screenHeight(768),
	_game(GameState::PLAY), 
	_maxFPS(60.0f),
	_time(0)
{
	_camera.init(_screenWidth, _screenHeight);
}
	
void MainGame::initSystems()
{
	DevyEngine::init();

	_window.create("GameEngine", _screenWidth, _screenHeight, 0);

	initShaders();

	_spriteBatch.init();
	_fpslimiter.init(_maxFPS);
}

MainGame::~MainGame(){}

void MainGame::processInput()
{
	const float cameraSpeed = 7.0f;
	const float scaleSpeed = 0.1f;

	while (SDL_PollEvent(&_event))
	{
		switch (_event.type)
		{
		case SDL_QUIT:
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

	if (_input.isKeyPressed(SDLK_w))
	{
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0, -cameraSpeed));
	}
	if (_input.isKeyPressed(SDLK_s))
	{
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0, cameraSpeed));
	}
	if (_input.isKeyPressed(SDLK_a))
	{
		_camera.setPosition(_camera.getPosition() + glm::vec2(cameraSpeed, 0.0f));
	}
	if (_input.isKeyPressed(SDLK_d))
	{
		_camera.setPosition(_camera.getPosition() + glm::vec2(-cameraSpeed, 0.0f));
	}
	if (_input.isKeyPressed(SDLK_q))
	{
		_camera.setScale(_camera.getScale() - scaleSpeed);
	}
	if (_input.isKeyPressed(SDLK_e))
	{
		_camera.setScale(_camera.getScale() + scaleSpeed);
	}
	if (_input.isKeyPressed(SDL_BUTTON_LEFT))
	{
		glm::vec2 mouseCoords = _input.getMouseCoords();
		mouseCoords = _camera.convertScreenToWorld(mouseCoords);
		
		glm::vec2 playerPosition(0.0f);
		glm::vec2 direction = mouseCoords - playerPosition;
		direction = glm::normalize(direction);
		_bullets.emplace_back(playerPosition, direction, 5.00f, 1000);
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
	
	for ( int i = 0; i < _bullets.size(); i++)
	{
		_bullets[i].draw(_spriteBatch);
	}

	
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
		_fpslimiter.begin();

		processInput();
		_time += 0.1f;

		_camera.update();

		//update bullets
		for ( int i = 0; i < _bullets.size();)
		{
			if (_bullets[i].update() == true)
			{
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
			} else {
				i++;
			}
		}

		drawGame();


		_fps = _fpslimiter.end();

		//print only once every 10 frames
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 1000)
		{
			std::cout << _fps << std::endl;
			frameCounter = 0;
		}
	}
}
void MainGame::run()
{
	initSystems();
	drawDungeon();
	gameloop();

}


void MainGame::initShaders()
{
	_colorProgram.compileShaders("Shaders/VertexShader.txt", "Shaders/FragmentShader.txt");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
}

void MainGame::drawDungeon()
{
	srand(time(NULL));

	int MAX_LEAF_SIZE = 30;
	Leaf* root = new Leaf(0, 0, 100, 100);
	std::list<Leaf> leaf_edge_nodes;
	std::list<Rectangle> halls;
	root->generate(MAX_LEAF_SIZE);

	root->createRooms(&leaf_edge_nodes, &halls);


	// need a char map (space is char 40, wall is char 179);
	char map[100][100]; // same width / height as the root leaf
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			map[i][j] = 178;
		}
	}

	for (std::list<Leaf>::iterator l = leaf_edge_nodes.begin(); l != leaf_edge_nodes.end(); ++l) {

		Rectangle* room = l->getRoom();
		int left = room->left();
		int right = room->right();
		int top = room->top();
		int bottom = room->bottom();

		if (left < 1) left = 1;
		if (right < 1) right = 1;
		if (top < 1) top = 1;
		if (bottom < 1) bottom = 1;

		if (right - left > 3 && bottom - top > 3) {

			for (int i = left; i <= right; i++) {
				for (int j = top; j <= bottom; j++) {
					map[i][j] = 32;
				}
			}
		}
	}

	for (std::list<Rectangle>::iterator hh = halls.begin(); hh != halls.end(); ++hh) {

		int left = hh->left();
		int right = hh->right();
		int top = hh->top();
		int bottom = hh->bottom();

		if (left < 1) left = 1;
		if (right < 1) right = 1;
		if (top < 1) top = 1;
		if (bottom < 1) bottom = 1;

		if (left > 99) left = 98;
		if (right > 99) right = 98;
		if (top > 99) top = 98;
		if (bottom > 99) bottom = 98;

		for (int i = left; i <= right; i++) {
			for (int j = top; j <= bottom; j++) {
				map[i][j] = 32;
			}
		}
	}

	std::cout << "\n=== DUNGEON GENERATOR v0.1 ===\n\n";
	std::ofstream myfile("Level1.txt");

	for (int j = 0; j < 100; j++) {
		std::cout << "  ";
		for (int i = 0; i < 100; i++) {
			std::cout << map[i][j];
			
			if (myfile.is_open())
			{
				myfile << map[i][j];
				if (i == 99)
				{
					myfile << "\n";
				}
				
			}
			else std::cout << "Unable to open file";
		}
		std::cout << "\n" << std::endl;
	}

	int d;
	std::cin >> d;
}

