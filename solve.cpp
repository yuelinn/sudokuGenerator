// solving sudoku
// listed in sudokuHeader.h to link to main.cpp
#include "sudokuHeader.h"
#include <iostream>
#include <cstdlib>

using namespace std;

void copyGrid(int from[][9], int to[][9])
{
	for(int i = 0; i < 9; ++i)
		for(int j = 0; j < 9; ++j)
		{
			to[i][j] = from[i][j];
		}
	return;
}

bool checkUnique(int array[9])
{
	bool isValid = true;
	for(int i = 1; i <= 9 && isValid; ++i) // check for digits 1 to 9
	{
		isValid = false;
		for(int j = 0; j < 9; ++j)
		{
			// scan through array to check if digit is present
			if(i == array[j])
				isValid = true;
		}
	}
	return isValid;
}

// checks if grid is solved completely
bool alreadySolved(void)
{
	bool isSolved = false;
	int temp[9] = {0};
	int tempCol[9] = {0};

	
	for(int i = 0 ; i < 9; ++i)
	{
		for(int j = 0; j < 9; ++j)
		{
			temp[j] = grid[i][j]; // check unique row
			tempCol[j] = grid[j][i]; // check unique col
		}
		// check for 9 unique digits
		isSolved = checkUnique(temp) && checkUnique(tempCol);
	}

	// check boxes
	for(int i = 0; i < 3 && isSolved; ++i) // for each box row
		for(int j = 0; j < 3 && isSolved; ++j)
		{
			for(int k = 0 ; k < 9; ++k) //for each entree in said box
				temp[k] = grid[i*3+ k/3][j*3 + k%3];
			isSolved = checkUnique(temp) && isSolved;
		}

	return isSolved;
}

// check if there are any more zeros/spaces in the puzzle
// returns -1 if there is no space, else returns the coordinate
struct Entree isThereSpace(void) 
{
	struct Entree emptySpot;

	emptySpot.digit = 0;
	emptySpot.row = -1;
	emptySpot.col = -1;

	for(int i = 0; i < 9; ++i)
		for(int j = 0; j < 9; ++j)
			if(grid[i][j] == 0)
			{
				emptySpot.row = i;
				emptySpot.col = j;
				return emptySpot;
			}

	//if no empty space
	emptySpot.row = -1;
	emptySpot.col = -1;
	return emptySpot;
}

bool recur_solve(void)
{
	if(alreadySolved())
		return true;
	else
	{
		struct Entree guess = isThereSpace();

		if(guess.row < 0) // catch here in case there is no space
			return false;

		// catch if the current entrees are correct
		for(int k = 0 ; k < 9; ++k)
			for(int l = 0; l < 9; ++l)
			{
				struct Entree entree;
				entree.digit = grid[k][l];
				if(entree.digit == 0)
					continue; // ignore spaces
				entree.row = k;
				entree.col = l;
				if(!isValidDigit(entree))
					return false;
			}
		
		for(int i = 1; i <= 9 ; ++i) // try to guess 
		{
			guess.digit = i;
			// check if valid guess
			if(isValidDigit(guess))
			{
				grid[guess.row][guess.col] = guess.digit;
				if(alreadySolved())
				{
					return true;
				}
					
				else
				{
					// feed to little bro
					if(recur_solve())
						return true;
				}
			}			
		}

		// if the for loop did not yield
		grid[guess.row][guess.col] = 0;
		return false;
	}
	return false;
}

// returns true if grid question has unique solution, changes grid to the answer
// returns false if no unique solution/ solution is not unique, no changes to grid
bool solve(void)
{
	bool isSolved = false;

	if(alreadySolved())
		return true;
	
	int tempGrid[9][9] = {0};
	copyGrid(grid, tempGrid); // store grid in case cannot be solved
	int solution[9][9] = {0};

	// solve recursively
	// cout << boolalpha << recur_solve() << endl;
	
	isSolved = recur_solve();

	if(isSolved) // check for uniqueness
	{
		//tempGrid has the holes, grid has the soln
		copyGrid(grid, solution); // store the solution from recur solve
		copyGrid(tempGrid, grid); // reset grid

		struct Entree space = isThereSpace();
		while(space.row >= 0) // while there is space
		{
			if(solution[space.row][space.col] == 9)
			{
				grid[space.row][space.col] = solution[space.row][space.col];
				space = isThereSpace();
				continue;
			}
			for(int i = solution[space.row][space.col] + 1; i <= 9 ; ++i)
			{
				grid[space.row][space.col] = i;
				if(recur_solve()) //if not unique 
				{
					isSolved = false; 
					break;
				}
			}
			grid[space.row][space.col] = solution[space.row][space.col];
			space = isThereSpace();
		}
		if(isSolved)
		{
			copyGrid(solution, grid);
		}
	}

	if(!isSolved)
		copyGrid(tempGrid, grid);

	return isSolved; 
}