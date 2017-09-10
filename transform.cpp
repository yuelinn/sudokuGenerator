#include "sudokuHeader.h"
#include <cstdlib>

using namespace std;

void rotateCCW()
{
	int tempGrid[9][9];
	copyGrid(grid, tempGrid);
	for(int i = 0; i < 9; ++i)
		for(int j = 0; j < 9; ++j)
		{
			grid[i][j] = tempGrid[j][8-i];
		}
	return;
}

void rotateCW()
{
	for (int i = 0; i < 3; ++i)
	{
		rotateCCW();
	}
	return;
}

// swap all digits a to b, vice versa
void swap(int a, int b)
{
	for(int i = 0 ; i < 9; ++i)
		for(int j = 0 ; j < 9; ++j)
		{
			if(grid[i][j] == a)
				grid[i][j] = b;
			else if(grid[i][j] == b)
				grid[i][j] = a;
		}

	return;
}

// reflect along col
void reflect(void)
{
	int temp;
	for(int i = 0; i < 9 ; ++i)
		for(int j = 0; j < 4 ; ++j)
		{
			temp = grid[i][j];
			grid[i][j] = grid[i][8-j];
			grid[i][8-j] = temp;
		}

	return;
}

// swap row
void swapRow(int a, int b)
{
	int temp;

	for(int i = 0 ; i < 9 ; ++i)
	{
		temp = grid[a][i];
		grid[a][i] = grid[b][i];
		grid[b][i] = temp;
	}

	return;
}

// swap col
void swapCol(int a, int b)
{
	int temp;

	for(int i = 0 ; i < 9 ; ++i)
	{
		temp = grid[i][a];
		grid[i][a] = grid[i][b];
		grid[i][b] = temp;
	}

	return;
}

// randomly permutate grid
void permutate(int seed)
{
	srand(seed);
	
	if(rand() % 2) // if reflect
	{
		reflect();
	}

	int times = rand() % 4;
	for(int i = 0; i < times; ++i)
		rotateCCW();

	times = rand() % 9;
	for(int i = 0; i < times; ++i)
	{
		int a = rand() % 9 + 1;
		int b = rand() % 9 + 1;
		if(a != b)
			swap(a,b);
	}

	// swap row & col
	times = rand() % 9;
	for(int i = 0; i < times; ++i)
	{
		int box = rand()%3;
		int row_a = rand()%3;
		int row_b = rand()%3;
		if(row_a != row_b)
		{
			swapRow(box*3+row_a, box*3+row_b);
			if(rand()%2)
				swapCol(box*3+row_a, box*3+row_b);
		}
	}
	return;
}