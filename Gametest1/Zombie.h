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
	//void aStar(std::vector<Human*>& humans);
////////////////////////////////////////////////////////
private:
	glm::vec2 _direction;
	int frames;
	Human* chasePlayer(std::vector<Human*>& humans);
////////////////////////////////////////////////////////
	void readMap(const std::string& FileName, const std::vector<std::string>& levelData, std::vector<Human*>& humans);
	//std::vector<Node> path;
	
	bool mapread = false;
	bool pathfound = false;
////////////////////////////////////////////////////////


	std::string pathFind(const std::string& FileName, const int & xStart, const int & yStart,
		const int & xFinish, const int & yFinish);

	
};











