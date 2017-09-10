// sudoku header file

#ifndef SUDOKU_HEADER
#define SUDOKU_HEADER
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;
	
	// from main.cpp
	extern int grid[9][9];

	// make struct to represent an entree
	struct Entree
	{
		int row;
		int col;
		int digit;
	};

	// from grid.cpp
	void printPuzzle();
	void generateSeed(int);
	void nullGrid();
	bool isValidDigit (struct Entree guess);
	bool subFile(int i, string);
	void fileOpen(int level, string);
	void fileClose(string);

	// functions in solve.cpp
	bool solve();
	void copyGrid(int from[][9], int to[][9]);
	bool checkUnique(int array[9]);
	bool alreadySolved();
	bool isValidDigit (struct Entree guess);
	struct Entree isThereSpace(void);

	// from generate.cpp
	int remove_boxes(int numOfSpaces);

	//from level.cpp
	int decideLevel(int numOfSpaces);

	//from transform.cpp
	void rotateCW();
	void rotateCCW();
	void permutate(int seed);

	

	// make struct for valid testing in logic solve
	struct Valids
	{
		std::vector<int> valids[9][9];

		bool fillValids(void)
		{
			struct Entree guess;
			for(int i = 0 ; i < 9; ++i)
			{
				for(int j = 0; j < 9; ++j)
				{
					guess.row = i;
					guess.col = j;
					if(grid[guess.row][guess.col] != 0)
						continue;
					else
					{
						for(guess.digit = 1; guess.digit <= 9 ; ++guess.digit)
						{
							if(isValidDigit(guess)) // grid[guess.row][guess.col])
								valids[i][j].push_back(guess.digit);
						}
					}
				}
			}
				
			return true;
		}

		// print valids
		void printValids(void)
		{
			for(int i = 0; i < 9; ++i)
			{
				for(int j = 0; j < 9; ++j)
				{
					int len = valids[i][j].size();
					if(!valids[i][j].empty())
					{
						std::cout << "(";
						for(int k = 0; k < len; ++k)
						{
							if(k != len - 1)
								std::cout << valids[i][j][k] << ", ";
							else
								std::cout << valids[i][j][k];
						}
						std::cout << ")" << '\t';
					}
					else
					{
						std::cout << "(0) ";
					}
				}
				std::cout << std::endl;
				std::cout << std::endl;
			}
			std::cout << std::endl;
			return;
		}

		// remove valid after updated on grid by placing confirmed guess
		void updateValid(struct Entree guess)
		{
			valids[guess.row][guess.col].erase(valids[guess.row][guess.col].begin(), valids[guess.row][guess.col].end());
			std::vector<int>::iterator found = valids[0][0].end();

			for(int i = 0; i < 9; ++i)
			{
				//check row for overlap
				found = find(valids[guess.row][i].begin(), valids[guess.row][i].end(), guess.digit);
				if(found != valids[guess.row][i].end())
					valids[guess.row][i].erase(found);
				
				//check col for overlap
				found = find(valids[i][guess.col].begin(), valids[i][guess.col].end(), guess.digit);
				if(found != valids[i][guess.col].end())
					valids[i][guess.col].erase(found);
			}

			//check box for overlap
			int box_row = guess.row / 3; 
			int box_col = guess.col / 3;
			box_row = box_row * 3; // first row val of the box
			box_col *= 3; // first col val
			for(int i = 0 ; i < 3; ++i)
			{
				for(int j = 0; j < 3; ++j)
				{
					found = find(valids[box_row+i][box_col+j].begin(), valids[box_row+i][box_col+j].end(), guess.digit);
					if(found != valids[box_row+i][box_col+j].end())
						valids[box_row+i][box_col+j].erase(found);
				}
			}
		}

		void rotateValidsCW(void)
		{
			std::vector<int> temp[9][9];

			// copy to temp 
			for(int i = 0; i < 9; ++i)
				for(int j = 0; j <9 ; ++j)
				{
					temp[i][j] = valids[i][j];
				}

			for(int i = 0; i < 9; ++i)
				for(int j = 0; j <9 ; ++j)
				{
					valids[i][j] = temp[j][8-i];
				}
			return;
		}
	};
	
#endif