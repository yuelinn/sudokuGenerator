#include "sudokuHeader.h"
#include <fstream>
#include <string>

using namespace std;

void generateSeed(int seed)
{
	bool isSeedValid = false;
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
	return;
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

bool isValidDigit (struct Entree guess)
{
	bool isValid = true;
	if(guess.digit < 1 || guess.digit > 9)
		isValid = false;
	else
	{
		// check repeats in column and row
		for(int j = 0; j < 9; ++j) 
		{
			if(((j != guess.row && grid[j][guess.col] == guess.digit)
				|| (j != guess.col && grid[guess.row][j] == guess.digit)))
			{
				isValid = false;
				break;
			}
		}
		// determine box
		int box_row = guess.row / 3; 
		int box_col = guess.col / 3;
		box_row = box_row * 3;
		box_col *= 3;
		// check box
		for(int j = 0; j < 3 && isValid; ++j)
			for(int i = 0; i < 3 && isValid; ++i)
			{
				if(!(box_row+i == guess.row && box_col+j == guess.col))
					if(grid[box_row+i][box_col+j] == guess.digit)
						isValid = false;
			}

	}
	return isValid;
}

// print puzzle to html file so that the web site can be made automatically
bool subFile(int num, string file)
{
	ofstream outFile;
	outFile.open(file.c_str(), ios::app);
	outFile << "<h2>Puzzle #" << num << "</h2>" << endl;

	outFile << "<table align = \"center\"; style=\"page-break-after: always; font-size: 50px;\">" << endl;

	for(int i = 0; i < 9; ++i)
	{
		outFile << "<tr>" << endl;
		for(int j = 0; j < 9; ++j)
		{
			if(grid[i][j] == 0)
			{
				outFile << "<th>&ensp;</th>" << endl;
			}
			else
			{
				outFile << "<th>" << grid[i][j] << "</th>" << endl;
			}
		}

		outFile << "</tr>" << endl;
		outFile << endl;
	}
	outFile << "</table> <br />" << endl;
	outFile.close();
	return true;
}

void fileOpen(int level, string file)
{
	ofstream outFile;
	outFile.open(file.c_str());
	ifstream inFile;
	inFile.open("puzzles_head.txt");
	string line;
	// copy in the head
	while(getline(inFile, line))
	{
		outFile << line << endl;
	}
	inFile.close();

	switch(level)
	{
		case 1 :
		{
			outFile << " <b>Easy</b></h1>" << endl; // change according to level !!
			break;
		}
		case 2 :
		{
			outFile << " <b>Medium</b></h1>" << endl; // change according to level !!
			break;
		}
		case 3 :
		{
			outFile << " <b>Hard</b></h1>" << endl; // change according to level !!
			break;
		}
	}
	
	inFile.close();
	outFile.close();
	return;
}

void fileClose(string file)
{
	ofstream outFile;
	outFile.open(file.c_str(), ios::app); // change passing char array to stream!!
	ifstream inFile;
	string line;
	// copy from butt
	inFile.open("puzzles_butt.txt");
	while(getline(inFile, line))
	{
		outFile << line << endl;
	}
	inFile.close();
	outFile.close();
	return;
}