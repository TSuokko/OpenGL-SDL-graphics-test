#include <SDL/SDL.h>
#include <GL/glew.h>

#include "PhysicsGame.h"
int main(int argc, char** argv)
{
	PhysicsGame game;
	game.run();
	return 0;
}