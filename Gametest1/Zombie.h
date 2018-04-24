#pragma once
#include "Agent.h"
//#include "Node.h"
//#include "SquareGraph.h"
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
private:
	glm::vec2 _direction;
	Human* chasePlayer(std::vector<Human*>& humans);
////////////////////////////////////////////////////////
	int moves;
	float smallestDistance;
	bool surroundedByWalls;
	void readMap(const std::string& FileName, std::vector<Human*>& humans);
	bool mapread = false;
	bool pathfound = false;
	bool positionChanged = false;
	std::vector<int> NodeDirection;
	std::vector<glm::vec2> NodeCoords;
	void movement(std::vector<Human*>& humans);
	std::string pathFind(const std::string& FileName, const int & xStart, const int & yStart,
		const int & xFinish, const int & yFinish);
	
	
	
};











