#include <math.h>
#include "Zombie.h"

//A-star variables
const int n = 200; // horizontal size of the map
const int m = 200; // vertical size size of the map

static int map[n][m];
static int pictureMap[n][m];
static int closed_nodes_map[n][m]; // map of closed (tried-out) nodes
static int open_nodes_map[n][m]; // map of open (not-yet-tried) nodes
static int dir_map[n][m]; // map of directions

const int dir = 8; // number of possible directions to go at any position
				   // if dir==4
				   //static int dx[dir]={1, 0, -1, 0};
				   //static int dy[dir]={0, 1, 0, -1};
				   // if dir==8
static int dx[dir] = { 1, 1, 0, -1, -1, -1, 0, 1 };
static int dy[dir] = { 0, 1, 1, 1, 0, -1, -1, -1 };

class node
{
	// current position
	int xPos;
	int yPos;
	// total distance already travelled to reach the node
	int level;
	// priority=level+remaining distance estimate
	int priority;  // smaller: higher priority

public:
	node(int xp, int yp, int d, int p)
	{
		xPos = xp; yPos = yp; level = d; priority = p;
	}

	int getxPos() const { return xPos; }
	int getyPos() const { return yPos; }
	int getLevel() const { return level; }
	int getPriority() const { return priority; }

	void updatePriority(const int & xDest, const int & yDest)
	{
		priority = level + estimate(xDest, yDest) * 10; //A*
	}

	// give better priority to going strait instead of diagonally
	void nextLevel(const int & i) // i: direction
	{
		level += (dir == 8 ? (i % 2 == 0 ? 10 : 14) : 10);
	}

	// Estimation function for the remaining distance to the goal.
	const int & estimate(const int & xDest, const int & yDest) const
	{
		static int xd, yd, d;
		xd = xDest - xPos;
		yd = yDest - yPos;

		// Euclidian Distance
		d = static_cast<int>(sqrt(xd*xd + yd*yd));

		// Manhattan distance
		//d=abs(xd)+abs(yd);

		// Chebyshev distance
		//d=max(abs(xd), abs(yd));

		return(d);
	}
};


bool operator<(const node & a, const node & b)
{
	return a.getPriority() > b.getPriority();
}