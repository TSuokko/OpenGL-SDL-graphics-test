#pragma once
#include "Agent.h"
#include "Node.h"
#include "SquareGraph.h"
class Zombie: public Agent
{
public:
	Zombie();
	~Zombie();
	void init(float speed, glm::vec2 pos);
	virtual void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies);

////////////////////////////////////////////////////////
	void aStar(std::vector<Node> path);
////////////////////////////////////////////////////////
private:
	glm::vec2 _direction;
	int frames;
	Human* chasePlayer(std::vector<Human*>& humans);
////////////////////////////////////////////////////////
	
////////////////////////////////////////////////////////
};





