#include "sudokuHeader.h"
#include <cstdlib>
#include <iostream>

// poke holes in seed
// poke a random hole
// check if solution is avail
// !! check if solution is unique !!

int remove_boxes(int numOfSpaces)
{
	int seed = 20;
	srand(seed);
	struct Entree temp; //  to store digit to be removed in case it cannot be removed
	int currentSpaces = 0;
	int iter = 0;
	int tempGrid[9][9] = {0}; // store the answer
	copyGrid(grid, tempGrid);

	while(numOfSpaces > currentSpaces && iter < 1000 )
	{
		++iter;
		// if(iter > 10)
			// std::cout << iter << std::endl;
		temp.row = rand() % 9;
		temp.col = rand() % 9;
		temp.digit = grid[temp.row][temp.col];
		// std::cout << temp.row << temp.col << std::endl;

		// check if the spot is alr empty or not
		if(grid[temp.row][temp.col] == 0)
		{
			// std::cout << "overlap" << std::endl;
			continue;
		}
			
		grid[temp.row][temp.col] = 0;

		if(!solve())
		{
			grid[temp.row][temp.col] = temp.digit;
		}
		else
		{
			++currentSpaces;
			iter = 0;
			grid[temp.row][temp.col] = 0;
			// std::cout << temp.row << temp.col << std::endl;
			tempGrid[temp.row][temp.col] = 0;
		}		
		copyGrid(tempGrid, grid); // reset the holes because solve() has filled them
	}

	return currentSpaces;
}
