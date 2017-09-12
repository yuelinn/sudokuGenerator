#include <iostream>
#include <ctime>
#include <cstdlib>
#include "sudokuHeader.h"

using namespace std;

int grid[9][9];

int main()
{	
	int level = 0;
	int numOfPuzzles = 0;
	int numOfSpaces = 0;
	int tempGrid[9][9]; 
	// int seed;

	cout << "input level: 1 = easy, 2 = medium, 3 = hard" << endl;  // check if the level is valid !!
	cin >> level;

	cout << "input number of puzzles required" << endl;
	cin >> numOfPuzzles;

	fileOpen(level, "final_puzzles.html");
	fileOpen(level, "solutions.html");
	srand(time(NULL) % 1000);

	for(int i = 0; i < numOfPuzzles; ++i)
	{
		generateSeed(rand());

		switch(level)
		{
			case 1:
			{
				numOfSpaces = remove_boxes(rand()%30 + 20);
				break;
			}
			case 2:
			{
				numOfSpaces = remove_boxes(rand()%6 + 44);
				break;
			}
			case 3:
			{
				numOfSpaces = remove_boxes(55);
				break;
			}
			cout << "error 1: invalid level input" << endl;
		}

		printPuzzle();

		if(decideLevel(numOfSpaces) != level)
		{
			// redo
			i--;
			break;
		}

		while((rand()%2 || rand()%2) && i < numOfPuzzles)
		{
			srand(time(NULL));
			permutate(rand());
			// send to print
			subFile(i+1, "final_puzzles.html");
			copyGrid(grid, tempGrid);
			solve();
			subFile(i+1, "solutions.html");
			copyGrid(tempGrid, grid);

			i++;
		}
	}
	fileClose("final_puzzles.html");
	fileClose("solutions.html");

	return 0;
}
