#include <Windows.h>
#include <iostream>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "MainGame.h"
#include <DevyEngine/Sprite.h>
//test
#include "InputHandler.h"
#include "SquareGraph.h"

int main(int argc, char** argv)
{
	MainGame game;
	game.run();
	
	return 0;
}