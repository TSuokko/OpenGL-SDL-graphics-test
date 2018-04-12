#ifndef SQUAREGRAPH_H_
#define SQUAREGRAPH_H_

#include <vector>
#include <algorithm>
#include <set>
#include <list>
#include <queue>
#include <math.h>
#include <iostream>
#include "Node.h"
#include <DevyEngine\SpriteBatch.h>

class SquareGraph 
{
private:
	std::vector< std::vector< Node > > map;
	pair<int, int> firstRobotPos;
	pair<int, int> secondRobotPos;
public:

	//debug
	void debugDraw(DevyEngine::SpriteBatch& spritebatch, Node currentNode);


	SquareGraph(int dimension);
	Node* getCellValue(pair<int, int> coord);
	void setCellValue(pair<int, int> coord, char value);
	void setFirstRobotPos(pair<int, int> coord);
	void setSecondRobotPos(pair<int, int> coord);
	pair<int, int> getFirstRobotPos();
	pair<int, int> getSecondRobotPos();
	float calculateDistance(pair<int, int> from, pair<int, int> to);
	void calculateAllCosts();
	bool isInsideMap(pair<int, int> coord);
	class compareNodes {
	public:
		bool operator()(Node n1, Node n2);
	};
	set<Node> getNeighbours(Node n);
	vector<Node> executeAStar();
	vector<Node> reconstructPath(Node* to, Node* from);
	void printPath(vector<Node> path);
	priority_queue<Node, vector<Node>, compareNodes> openNodes;		//A priority queue is a container adaptor that provides
	priority_queue<Node, vector<Node>, compareNodes> closedNodes;	//constant time lookup of the largest (by default) element
};

#endif