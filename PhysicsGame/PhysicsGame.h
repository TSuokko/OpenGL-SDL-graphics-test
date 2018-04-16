#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <DevyEngine/Sprite.h>
#include <DevyEngine/GLSLprogram.h>
#include <DevyEngine/GLTexture.h>
#include <DevyEngine/Window.h>
#include <DevyEngine/Camera2D.h>
#include <DevyEngine/SpriteBatch.h>
#include <DevyEngine/InputManager.h>
#include <DevyEngine/Timing.h>
#include <DevyEngine/ParticleEngine2D.h>
#include <DevyEngine/ParticleBatch2D.h>

enum class GameState { PLAY, EXIT };

class PhysicsGame
{
public:
	PhysicsGame();
	~PhysicsGame();

	void run();

private:
	void initSystems();
	void initShaders();
	void processInput();
	void gameloop();
	void drawGame();

	DevyEngine::Window _window;

	int _screenWidth;
	int _screenHeight;
	GameState _game;
	SDL_Event _event;

	DevyEngine::Sprite _sprite;
	DevyEngine::GLSLprogram _colorProgram;
	DevyEngine::Camera2D _camera;
	DevyEngine::SpriteBatch _agentSpriteBatch;

	DevyEngine::ParticleEngine2D m_particleEngine;
	DevyEngine::ParticleBatch2D* m_waterParticles;
	void addWater(const glm::vec2& position, int numParticles);


	DevyEngine::InputManager _input;
	DevyEngine::SpriteBatch _spriteBatch;
	DevyEngine::FpsLimiter _fpslimiter;

	float _fps;
	float _maxFPS;
	float _time;
};