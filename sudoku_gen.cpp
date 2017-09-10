#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

int grid[9][9];

// make struct to represent an entree
struct Entree
{
	int row;
	int col;
	int digit;
};

// make a class of sudoku?

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
bool alreadySolved()
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

// int** recur_solve(int** puzzle, int digit, int row, int col)
// {
// 	if(alreadySolved())
// 	{

// 		return puzzle;
// 	}
// 	else
// 	{
// 		puzzle[row][col] = digit + 1;
// 		return;
// 	}
// }

void copyGrid(int from[][9], int to[][9])
{
	for(int i = 0; i < 9; ++i)
		for(int j = 0; j < 9; ++j)
		{
			to[i][j] = from[i][j];
		}
	return;
}

bool solve()
{
	bool isSolved = false;
	
	int tempGrid[9][9] = {0};
	copyGrid(grid, tempGrid); // store grid in case cannot be solved or is not unique

	// solve recursively

	if(!isSolved)
		copyGrid(tempGrid, grid);
	return isSolved;
}

int remove_boxes(int numOfSpaces)
{
	int seed = 1;
	srand(seed);
	int random_row = rand() % 9 + 1;
	int random_col = rand() % 9 + 1;
	int temp = 0; // store digit to be removed in case it cannot be removed
	int currentSpaces = 0;
	int iter = 0;

	while(numOfSpaces > currentSpaces && iter < 1000)
	{
		++iter;
		temp = grid[random_row][random_col];
		grid[random_row][random_col] = 0;
		if(!solve())
			grid[random_row][random_col] = temp;
		else
			++currentSpaces;
	}
	return currentSpaces;
}

void printPuzzle()
{
	cout << "Sudoku Puzzle" << endl << endl;

	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			cout << grid[i][j] << ' ';
			if ((j + 1) % 3 == 0)
				cout << " ";
		}			
		cout << endl;
		if ((i + 1) % 3 == 0)
				cout << endl;
	}
	return;
}

void nullGrid()
{
	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 9; ++j)
			grid[i][j] = 0;
	}
	return;
}

bool isValidDigit (Entree guess)
{
	bool isValid = true;
	if(guess.digit < 1 || guess.digit > 9)
		isValid = false;
	else
	{
		// check repeats in column and row
		for(int j = 0; j < 9; ++j) 
			if(grid[guess.row][j] == guess.digit || grid[j][guess.col] == guess.digit)
			{
				isValid = false;
				break;
			}

		// determine box
		int box_row = guess.row / 3; 
		int box_col = guess.col / 3;
		box_row = box_row * 3;
		box_col *= 3;
		// check box
		for(int j = 0; j < 3; ++j && isValid)
			for(int i = 0; i < 3; ++i && isValid)
			{
				if(grid[box_row+i][box_col+j] == guess.digit)
					isValid = false;
			}

	}
	return isValid;
}

int main()
{	
	bool isSeedValid = false;
	int seed = 0; // first seed found to be valid is 350
	int random_digit = 0;
	Entree guess;

	do // change to do while loop
	{
		++seed;
		srand(seed);
		isSeedValid = true;
		random_digit = 0;
		for(int i = 0; i < 9; ++i)
		{
			for(int j = 0; j < 9; ++j)
			{
				int loopCheck = 0;
				guess.digit = random_digit;
				guess.row = i;
				guess.col = j;
				while(!isValidDigit(guess))
				{
					if(loopCheck > 10000)
					{
						isSeedValid = false;
						nullGrid();
						break;
					}
					else
					{
						random_digit = rand();
						guess.digit = random_digit % 9 + 1;	
						++loopCheck;
					}
				}
				if(!isSeedValid)
					break;
				grid[i][j] = guess.digit;
			}
		}
	}
	while(!isSeedValid);
	cout << "Seed: " << seed << endl;
	printPuzzle();

	cout << boolalpha << alreadySolved() << endl;
	return 0;
}