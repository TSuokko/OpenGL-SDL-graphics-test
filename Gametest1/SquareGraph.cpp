#include "SquareGraph.h"

SquareGraph::SquareGraph(int dimension) : 
map(dimension, vector<Node>(dimension)),
openNodes(), closedNodes() {}

Node* SquareGraph::getCellValue(pair<int, int> coord)
{
	if (coord.first < 0 || coord.first >= 200 || coord.second < 0 || coord.second >= 200)
	{
		printf("ERROR ERROR: Invalid coord %d %d\n", coord.first, coord.second);
	}
	return &(this->map[coord.first][coord.second]);
}

void SquareGraph::setCellValue(pair<int, int> coord, char value) 
{
	this->map[coord.first][coord.second].setState(Node::UNVISITED);
	this->map[coord.first][coord.second].setType(value);
	this->map[coord.first][coord.second].x = coord.first;
	this->map[coord.first][coord.second].y = coord.second;
}

pair<int, int> SquareGraph::getFirstRobotPos() {
	return this->firstRobotPos;
}

pair<int, int> SquareGraph::getSecondRobotPos() {
	return this->secondRobotPos;
}

void SquareGraph::setFirstRobotPos(pair<int, int> coord) {
	this->firstRobotPos = coord;
}

void SquareGraph::setSecondRobotPos(pair<int, int> coord) {
	this->secondRobotPos = coord;
}

float SquareGraph::calculateDistance(pair<int, int> from, pair<int, int> to) {
	return pow(pow((from.first - to.first), 2) + pow((from.second - to.second), 2), 0.5);
}

void SquareGraph::calculateAllCosts() 
{
	for (int i = 0; i<map.size(); i++) 
	{
		for (int j = 0; j<map[0].size(); j++) 
		{
			map[i][j].setCostFromStart(calculateDistance(getFirstRobotPos(), make_pair(i, j)));
			map[i][j].setCostToTarget(calculateDistance(getSecondRobotPos(), make_pair(i, j)));
			map[i][j].calculateTotalCost();
		}
	}
}

bool SquareGraph::compareNodes::operator()(Node n1, Node n2) 
{
	return n1.getTotalCost() > n2.getTotalCost();
}

bool SquareGraph::isInsideMap(pair<int, int> coord)
{
	return (coord.first >= 0) && (coord.second >= 0) && (coord.first < map[0].size()) && (coord.second < map.size());
}

set<Node> SquareGraph::getNeighbours(Node n)
{
	set<Node> neighbours;
	Node* temp;
	list<int> values = { -1, 0, 1 };

	for (int i : values) 
	{
		for (int j : values) 
		{
			if (!(i == 0 && j == 0)) 
			{
				if ((isInsideMap(make_pair(n.x + i, n.y + j)))) 
				{
					temp = getCellValue(make_pair((n.x + i), (n.y + j)));

					if ((!temp->isObstacle())) 
					{
						neighbours.insert(*temp);
					}
				}
			}
		}
	}
	return neighbours;
}

vector<Node> SquareGraph::reconstructPath(Node* to, Node* from)
{
	vector<Node> path;
	Node* tmp = from;
	Node* parent;

	path.push_back(*tmp);
	while (tmp->getParent() != nullptr)
	{
		parent = (tmp->getParent());
		path.push_back(*parent);
		tmp = parent;
	}
	reverse(path.begin(), path.end());
	return path;
}

void SquareGraph::printPath(vector<Node> path) {
	cout << "--- Path to target ---" << endl;
	for (auto i = path.begin(); i != path.end(); i++)
	{
		Node node = *i;
		cout << "node : (" << node.x << "," << node.y << ")" << endl;
	}
}
																//NOTE: ptr == "Pointer"
vector<Node> SquareGraph::executeAStar() 
{
	
	pair<int, int> start = this->getFirstRobotPos();			//gets the starting position
	std::cout << start.first << start.second << std::endl;
	pair<int, int> target = this->getSecondRobotPos();			//gets the finishing position
	
	Node* startNodePtr = getCellValue(start);					//sets the starting Node's position to the pointer
	Node* targetNodePtr = getCellValue(target);					//sets the finishing Node's position
	
	vector<Node> path;											//establish a vector of Nodes		
	Node currentNode;											//the current node
																//"sets" are containers that store unique elements following a specific order.
	set<Node> neighbours;										//in a set, the value of an element also identifies it
	openNodes.push(*startNodePtr);								//push the startNode to the priority_queue
	startNodePtr->setOpen();									//makes the state of the node "Open"
	if (openNodes.size() == 0)
	{
		std::cout << "empty" << std::endl;
	}
	while (openNodes.size() > 0)								//while the OpenNodes are NOT empty
	{		
		//std::cout << "open nodes: "<<openNodes.size() << std::endl;
		currentNode = openNodes.top();							//.top returns the OpenNode reference to the top element in the priority queue
		Node* currentPtr = getCellValue(make_pair(currentNode.x, currentNode.y));	//the current Pointer checks the current Node's position
		
		//if the current Pointers position is exactly the same as the target final Nodes position
		if ((currentPtr->x == targetNodePtr->x) && (currentPtr->y == targetNodePtr->y)) 
		{   //reconstruct the created path from the current to the beginning 
			return reconstructPath(startNodePtr, currentPtr);	
		}
		//std::cout << "test6" << std::endl;
		openNodes.pop();										//removes the top element from the priority queue.
		closedNodes.push(*currentPtr);							//push the currentPointer to the closedNodes
		currentPtr->setClosed();								//set the nodes state to "closed"
		neighbours = getNeighbours(*currentPtr);				//retrieves the information of the current pointers neighbours
		//std::cout << "test7" << std::endl;
		for (auto i = neighbours.begin(); i != neighbours.end(); ++i) //loop though the neighbours of the pointer
		{
			Node* neighbourPtr = getCellValue(make_pair(i->x, i->y));	//makes the position of the neighbourpointer
			//std::cout << "test8" << std::endl;
			if (!(neighbourPtr->isClosed()))					//if the neighbour pointer is NOT a closed state
			{
				//std::cout << "test9" << std::endl;
				//"tentative" == "alustava" in Finnish
				//the score of the current neighbor is equal to the starting cost and distance cost from the pointer to the neighbour
				int tentativeScore = currentNode.getCostFromStart() + this->calculateDistance(make_pair(currentPtr->x, currentPtr->y), make_pair(neighbourPtr->x, neighbourPtr->y));
				
				if ((!neighbourPtr->isOpen()) || (tentativeScore < currentNode.getCostFromStart())) 
				{
					//std::cout << "test10" << std::endl;
					neighbourPtr->setParent(currentPtr);		//sets the parent to the current pointer 
					neighbourPtr->setCostFromStart(tentativeScore);		//sets the cost from the start as the score
					neighbourPtr->setCostToTarget(this->calculateDistance(make_pair(neighbourPtr->x, neighbourPtr->y), target)); //calculates the cost from the neighbour to the final target
					neighbourPtr->calculateTotalCost();			//calculates the total cost of the neighbour
					if (!neighbourPtr->isOpen())				//if the neightbouring pointer is not open, 
					{
						//std::cout << "test11" << std::endl;
						openNodes.push(*neighbourPtr);			//push the neighbouring pointer to the open nodes list
						neighbourPtr->setOpen();				//and set it as open.
					}
				}
			}
		}

	}
	//openNodes.push(*startNodePtr);
	std::cout << "break test: " << openNodes.size() <<std::endl;
	//TODO: FIND OUT THE "_CRT IS VALID HEAP POINTER" PROBLEM
	delete startNodePtr;
	delete targetNodePtr;

	return path;
}