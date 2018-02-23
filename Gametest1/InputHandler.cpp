/*#include "InputHandler.h"

SquareGraph InputHandler::readMap(string filename) {

	int mapDimension;									//dimension of the map ([200][200])
	string line;										//current line 
	char type;											//current character type on the map
	ifstream inputFile(filename.c_str());				//read the given .txt file
	if (inputFile)										//if reading the inputted file
	{
		try 
		{
			inputFile >> mapDimension;					//reads the first line of the txt file, 
			getline(inputFile, line);					//which gives the map dimensions
			SquareGraph graph(mapDimension);			//gives the constructor the map dimension
			for (int i = 0; i<mapDimension; i++)		//for loop the size of the dimension
			{											//looping the Y-coordinate
				getline(inputFile, line);				//disregard the first line with the number
				for (int j = 0; j<mapDimension; j++)	//and the X-coordinate
				{
					if (line.at(j) == 'T')				//If it finds a 'T' character in the file
					{									//TODO: change it so it reads the zombie position on the map
						graph.setFirstRobotPos(make_pair(i, j));	//sets the starting position
						type = ' ';						//changes it to a blank space for pathfinding
					}
					else if (line.at(j) == 'G')			//Same applies here, but instead the 'G' character
					{									//TODO: change it so it read the player position
						graph.setSecondRobotPos(make_pair(i, j));	//sets the finishing position
						type = ' ';						//changes it to a blank space for pathfinding
					}
					else
						type = line.at(j);				//read the character on file
					graph.setCellValue(make_pair(i, j), type);	//sets the value as either blank or wall
				}
				line.clear();							//erases the contents of the line
			}
			vector<Node> path = graph.executeAStar();	//starts the algorithm, go to SquareGraph.cpp 
			cout << "The total number of moves from distance to the target are : " 
				 << path.size() << endl;;				//checks how many times path nodes were pushed into the vector
			cout << "You want to see the whole path to the target ? (y/n) " << endl;
			string response;
			cin >> response;
			if (response.compare("y") == 0) 
			{
				graph.printPath(path);
			}
			inputFile.close();
			return graph;
		}
		catch (exception &e) 
		{
			inputFile.close();
			throw runtime_error("Input file is not well formatted.");
		}
	}
	else
	{
		throw runtime_error("Could not open file");
	}
}*/
