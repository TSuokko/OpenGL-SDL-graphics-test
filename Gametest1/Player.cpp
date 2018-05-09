#include "Player.h"
#include <SDL\SDL.h>
#include <DevyEngine/ResourceManager.h>
#include "Gun.h"
#include <iostream>
Player::Player():_currentGunIndex(-1)
{
}


Player::~Player()
{
}

void Player::init(float speed, glm::vec2 pos, DevyEngine::InputManager* input, DevyEngine::Camera2D* camera, std::vector<Bullet>* bullets)
{
	_speed = speed;
	_position = pos;
	_input = input;
	_color.setColor(255, 255, 255, 255);
	_camera = camera;
	_bullets = bullets;
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

	if (_input->isKeyPressed(SDLK_1) && _guns.size() >= 0){
		_currentGunIndex = 0;
	}	else if (_input->isKeyPressed(SDLK_2) && _guns.size() >= 1){
		_currentGunIndex = 1;
	}	else if (_input->isKeyPressed(SDLK_3) && _guns.size() >= 2){
		_currentGunIndex = 2;
	}


	glm::vec2 mouseCoords = _input->getMouseCoords();
	mouseCoords = _camera->convertScreenToWorld(mouseCoords);
	//printf("Mouse Coords: %d %d\n", mouseCoords.x, mouseCoords.y);

	//std::cout << mouseCoords.x << " " << mouseCoords.y<<std::endl;

	glm::vec2 centerPosition = _position + glm::vec2(AGENT_RADIUS);
	glm::vec2 m_direction = glm::normalize(mouseCoords - centerPosition);

	if (_currentGunIndex != -1)
	{
		_guns[_currentGunIndex]->update(_input->isKeyPressed(SDL_BUTTON_LEFT), centerPosition, m_direction, *_bullets);
	}


	collideWithLevel(levelData);

}
