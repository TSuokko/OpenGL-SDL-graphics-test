#include "Player.h"
#include <SDL\SDL.h>
#include <DevyEngine/ResourceManager.h>

Player::Player():_currentGunIndex(-1)
{
}


Player::~Player()
{
}

void Player::init(float speed, glm::vec2 pos, DevyEngine::InputManager* input)
{
	_speed = speed;
	_position = pos;
	_input = input;
	_color.r = 255;
	_color.g = 255;
	_color.b = 255;
	_color.a = 255;
}

void Player::addGun(Gun* gun)
{
	_guns.push_back(gun);
	if (_currentGunIndex == -1)
	{
		_currentGunIndex = 0;
	}
}

void Player::update(const std::vector<std::string>& levelData, std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies)
{
	if (_input->isKeyPressed(SDLK_w))
	{
		_position.y += _speed;
	} else if (_input->isKeyPressed(SDLK_s))
	{
		_position.y -= _speed;
	}
	if (_input->isKeyPressed(SDLK_a))
	{
		_position.x -= _speed;
	}
	else if (_input->isKeyPressed(SDLK_d))
	{
		_position.x += _speed;
	}

	if (_input->isKeyPressed(SDLK_1) && _guns.size() >= 0)
	{
		_currentGunIndex = 0;
	}
	if (_input->isKeyPressed(SDLK_2) && _guns.size() >= 1)
	{
		_currentGunIndex = 1;
	}

	

	collideWithLevel(levelData);

}
