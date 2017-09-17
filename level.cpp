// decide level of the question puzzle
/* 
	level 0 = too easy
	level 1 = easy
	level 2 = med
	level 3 = hard
	level 4 = extremly hard
*/

#include "sudokuHeader.h"
#include <iostream>
#include <vector>

using namespace std;

struct Valids guesses;
	
// performs basic elimination by checking if any square has only one option only
bool basicElimination(void)
{
	bool hasDoneSomething = false;
	// by elimination of other possible digits
	bool stillWorks = true; // if the loop has gone through and there are no more squares I can fill
	struct Entree space = isThereSpace(); 

	while(stillWorks && space.row >= 0)
	{
		stillWorks = false;
		for(int i = 0; i < 9; ++i)
		{
			for(int j = 0 ; j < 9; ++j)
			{
				if(guesses.valids[i][j].size() == 1)
				{
					grid[i][j] = guesses.valids[i][j].back();
					stillWorks = true;
					space.row = i;
					space.col = j;
					space.digit = grid[i][j];
					guesses.updateValid(space);
					hasDoneSomething = true;
				}
			}
		}
		space = isThereSpace(); 
	}
	return hasDoneSomething;
}

// decide by being the only possible square for a digit
bool onlySquare()
{
	bool hasDoneSomething = false;
	
	std::vector<int> encountered;
	std::vector<int> encountered_col;
	bool isFound = false;
	struct Entree space;

	for(int i = 0; i < 9 ; ++i) // check row by row
	{
		for(int j = 0; j < 9; ++j)
		{
			unsigned int len = guesses.valids[i][j].size();
			if(len > 0)
			{
				for(unsigned int k = 0; k < len; ++k)
				{
					if(find(encountered.begin(), encountered.end(), guesses.valids[i][j][k]) == encountered.end()) // if is not already in the encountered
					{
						encountered.push_back(guesses.valids[i][j][k]);
						for(int l = j+1; l < 9; ++l) // look for it in the remaining elements of the row
						{
							if(find(guesses.valids[i][l].begin(), guesses.valids[i][l].end(), guesses.valids[i][j][k])
								!= guesses.valids[i][l].end()) // found in other vectors
							{
								isFound = true;
								break;
							}	
						}
						if(!isFound) // if not found in other remaining vectors
						{
							grid[i][j] = guesses.valids[i][j][k]; // put it on grid
							space.row = i;
							space.col = j;
							space.digit = grid[i][j];
							guesses.updateValid(space); // update valids
							hasDoneSomething = true;
						}
						isFound = false;
					}
				}
			}

			// now repeat for col
			len = guesses.valids[j][i].size();
			if(len > 0)
			{
				for(unsigned int k = 0; k < len; ++k)
				{
					if(find(encountered_col.begin(), encountered_col.end(), guesses.valids[j][i][k]) == encountered_col.end()) // if is not already in the encountered
					{
						encountered_col.push_back(guesses.valids[j][i][k]);
						for(int l = j+1; l < 9; ++l) // look for it in the remaining elements of the col
						{
							if(find(guesses.valids[l][i].begin(), guesses.valids[l][i].end(), guesses.valids[j][i][k])
								!= guesses.valids[l][i].end()) // found in other vectors
							{
								isFound = true;
								break;
							}	
						}
						if(!isFound) // if not found in other remaining vectors
						{
							grid[j][i] = guesses.valids[j][i][k]; // put it on grid
							space.row = j;
							space.col = i;
							space.digit = grid[j][i];
							guesses.updateValid(space); // update valids
							hasDoneSomething = true;
						}
						isFound = false;
					}
				}
			}
			
					
		}
		encountered.erase(encountered.begin(), encountered.end()); //reset encounter after each row
		encountered_col.erase(encountered_col.begin(), encountered_col.end()); //reset encounter after each col
	}

	// now for box check
	for(int j = 0; j < 3; ++j)
	{
		for(int i = 0; i < 3; ++i) // transversing boxes
		{
			for(int k = 0 ; k < 3; ++k) // transversing small squares in box
			{
				for(int l = 0; l < 3 ; ++l)
				{
					if(!guesses.valids[i*3+k][j*3+l].empty())
					{
						for(unsigned int m = 0; m < guesses.valids[i*3+k][j*3+l].size(); m++) // transverse valids at this square
						{
							if(find(encountered.begin(), encountered.end(), guesses.valids[i*3+k][j*3+l][m]) 
								== encountered.end()) // if not found
							{
								isFound = false;
								encountered.push_back(guesses.valids[i*3+k][j*3+l][m]);
								// search for it in remaining squares
								for(int n = 0; n < 3 && !isFound; ++n)
								{
									for(int p = 0; p < 3; ++p)
									{
										if(!(n == k && p == l) &&
											find(guesses.valids[i*3+n][j*3+p].begin(), guesses.valids[i*3+n][j*3+p].end(), 
											guesses.valids[i*3+k][j*3+l][m]) != guesses.valids[i*3+n][j*3+p].end()) 
											// if found
										{
											isFound = true;
											break;
										}
									}
								}
								if(!isFound)
								{
									hasDoneSomething = true;
									// update grid
									grid[i*3+k][j*3+l] = guesses.valids[i*3+k][j*3+l][m];
									space.row = i*3+k;
									space.col = j*3+l;
									space.digit = grid[i*3+k][j*3+l];
									guesses.updateValid(space); // update valids
									isFound = false;
									break;
								}
								
							}
						}
					}
				}
			}
			encountered.erase(encountered.begin(), encountered.end());
		}
	}
	while(basicElimination());
	return hasDoneSomething;
}

// sub group exclusion eliminates valids based on the rule that some other square must take that value instead
bool subGpExclusion(void)
{
	bool hasDoneSomething = false;
	std::vector<int> encountered;
	bool isInBox = false;
	bool isOutBox = false;

	for(int row = 0; row < 2; ++row)
	{
		for (int i = 0; i < 9; ++i) // for each row
		{
			for(int j = 0 ; j < 9; ++j) // each element of the row
			{
				if(!guesses.valids[i][j].empty())
				{
					for(unsigned int k = 0; k < guesses.valids[i][j].size(); ++k)
					{
						if(find(encountered.begin(), encountered.end(), guesses.valids[i][j][k]) 
							== encountered.end()) // if not found in encountered
						{
							encountered.push_back(guesses.valids[i][j][k]);
							for(int l = j+1; l < 9; ++l) // check the rest of the row
							{
								if(find(guesses.valids[i][l].begin(),  guesses.valids[i][l].end(), guesses.valids[i][j][k])
									!= guesses.valids[i][l].end()) // if found a repeat
								{
									if(l/3 == j/3) // if repeat is in the same box
									{
										isInBox = true;
									}
									else
									{
										isOutBox = true;
										break;
									}
								}
							}
							if(isInBox && !isOutBox)
							{
								// remove from the box any other valids of the same value
								for(int m = 0 ; m < 3; ++m)
									for(int n = 0; n < 3; ++n)
									{
										if((i/3)*3+m != i) // skip original row
										{
											std::vector<int>::iterator found = find(guesses.valids[(i/3)*3+m][(j/3)*3+n].begin(), 
												guesses.valids[(i/3)*3+m][(j/3)*3+n].end(), guesses.valids[i][j][k]);
											if(found != guesses.valids[(i/3)*3+m][(j/3)*3+n].end()) // if there is an overlap
											{
												 // remove the overlap
												guesses.valids[(i/3)*3+m][(j/3)*3+n].erase(found);
												hasDoneSomething = true;
											}
										}
									}
							}
							isInBox = false;
							isOutBox = false;
						}
					}
					
				}
			}
			encountered.erase(encountered.begin(), encountered.end());
		}
		// rotate valids!!
		guesses.rotateValidsCW();
	}
	// rotate valids x2 !!
	guesses.rotateValidsCW();
	guesses.rotateValidsCW();
	while(onlySquare());
	return hasDoneSomething;
}

bool twinExclusion(int numOfChildren = 2)
{
	bool hasDoneSomething = false;
	std::vector<int> encountered;
	// int numOfOverlaps = 0;
	std::vector<struct Entree> overlaps;
	struct Entree entree;


	// for a constrain 
	// if there are exactly 2 squares with 2 overlapping valids that is not present in the constraint
	// remove other valids from said constrain
	for(int row = 0; row < 2; ++row)
	{
		for (int i = 0; i < 9; ++i) // for each row
		{
			encountered.erase(encountered.begin(), encountered.end());
			for (int j = 0; j < 9; ++j) // for each element in row
			{
				overlaps.erase(overlaps.begin(), overlaps.end());
				if(!guesses.valids[i][j].empty())
				{
					for(unsigned int k = 0; k < guesses.valids[i][j].size(); ++k)
					{
						if(find(encountered.begin(), encountered.end(), guesses.valids[i][j][k]) == encountered.end()) // if not found
						{
							encountered.push_back(guesses.valids[i][j][k]);
							int numOfOverlaps = 0;
							int reset = overlaps.size();

							for(int l = j+1; l < 9; ++l) // check the rest of the row
							{
								if(find(guesses.valids[i][l].begin(), guesses.valids[i][l].end(), guesses.valids[i][j][k])
								 != guesses.valids[i][l].end())// found
								{
									// store into overlaps
									entree.row = i;
									entree.col = l;
									entree.digit = guesses.valids[i][j][k];
									overlaps.push_back(entree);
									numOfOverlaps++;
								}
							}
							if(numOfOverlaps >= numOfChildren) //2 for twins
							{
								overlaps.erase(overlaps.begin() + reset, overlaps.end());
							}
						}
					}


					for(unsigned int l = 0; l < overlaps.size() && !overlaps.empty(); ++l)
					{
						for(unsigned int m = l + 1; m < overlaps.size() ; ++m)
							if(overlaps[l].row == overlaps[m].row && overlaps[l].col == overlaps[m].col ) 
							// need to find more for more numofchildren !!!
							{
								// found the twin
								
								// remove other valids from the two squares
								int len = guesses.valids[i][j].size(); // seg fault!!!
								for(int n = 0; n < len ; ++n)
								{
									if(guesses.valids[i][j][n] != overlaps[l].digit 
										&&  guesses.valids[i][j][n] != overlaps[m].digit)
									{
										// not the twin
										guesses.valids[i][j].erase(guesses.valids[i][j].begin() + n);
										len--;
										n--;
										hasDoneSomething = true;
									}
								}
								len = guesses.valids[overlaps[l].row][overlaps[l].col].size();
								for(int n = 0; n < len ; ++n)
								{
									if(guesses.valids[overlaps[l].row][overlaps[l].col][n] != overlaps[l].digit 
										&&  guesses.valids[overlaps[l].row][overlaps[l].col][n] != overlaps[m].digit)
									{
										// not the twin
										guesses.valids[overlaps[l].row][overlaps[l].col].erase(guesses.valids[overlaps[l].row][overlaps[l].col].begin() + n);
										len--;
										n--;
										hasDoneSomething = true;
									}
								}
								break; //idk this breaks a diff loop... >< ?!
							}
					}
					
				}
			}
		}
		guesses.rotateValidsCW();
	}
	guesses.rotateValidsCW();
	guesses.rotateValidsCW();
	subGpExclusion();
	return hasDoneSomething;
}

bool triplet(void)
{
	bool hasDoneSomething = false;

	// write this later, not very common !!

	return hasDoneSomething;
}

bool alternatePairEx()
{
	bool hasDoneSomething = false;
	int tempGrid[9][9] = {0};
	int digits[10] = {0};
	std::vector<int> tempValids[9][9] = guesses.valids;

	copyGrid(grid, tempGrid);

	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			if(guesses.valids[i][j].size() == 2)
			{
				for(int k = 0; k < 2; ++k)
				{
					digits[guesses.valids[i][j][k]] ++;
				}
			}
		}
	}
	
	int mostCommonDigit = 0;
	for(int i = 0; i < 10; ++i)
	{
		if(digits[i] > mostCommonDigit)
			mostCommonDigit = i;
	}

	for(int i = 0; i < 2 && !hasDoneSomething; ++i)
	{
		for(int j = 0; j < 2 && !hasDoneSomething; ++j)
		{
			if(guesses.valids[i][j].size() == 2)
			{
				for(int k = 0; k < 2 && !hasDoneSomething; ++k)
				{
					if(guesses.valids[i][j][k] == mostCommonDigit)
					{
						grid[i][j] = mostCommonDigit;
						Entree space;
						space.row = i;
						space.col = j;
						space.digit = grid[i][j];
						guesses.updateValid(space);

						// check if guess is valid
						while(subGpExclusion() || onlySquare() || basicElimination() || twinExclusion());
						if(alreadySolved())
						{
							hasDoneSomething = true;
							break;
						}
						else
						{
							// reset grid
							copyGrid(tempGrid, grid);

							// reset valids
							for(int m = 0; m < 9; ++m)
							{
								for(int n = 0; n < 9; ++n)
								{
									guesses.valids[m][n].clear();
									for(unsigned int l = 0 ; l < tempValids[m][n].size(); ++l)
										guesses.valids[m][n].push_back(tempValids[m][n][l]);
								}
							}

							// try the other digit !!! change this ot a separate function for modularity
							grid[i][j] = guesses.valids[i][j][abs(k - 1)];
							Entree space;
							space.row = i;
							space.col = j;
							space.digit = grid[i][j];
							guesses.updateValid(space);

							// check if guess is valid
							while(subGpExclusion() || onlySquare() || basicElimination() || twinExclusion());
							if(alreadySolved())
							{
								hasDoneSomething = true;
								break;
							}
							else
							{
								// reset grid
								copyGrid(tempGrid, grid);

								// reset valids
								for(int m = 0; m < 9; ++m)
								{
									for(int n = 0; n < 9; ++n)
									{
										guesses.valids[m][n].clear();
										for(unsigned int l = 0 ; l < tempValids[m][n].size(); ++l)
											guesses.valids[m][n].push_back(tempValids[m][n][l]);
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return hasDoneSomething;
}



int decideLevel(int numOfSpaces)
{
	int level = 0;
	int tempGrid[9][9]={0};
	copyGrid(grid, tempGrid);

	// check range of empty spots for each level; bigger num of spaces = easier
	if(numOfSpaces >= 45) 
	{
		if(numOfSpaces <= (81-35))
			level = 1;
		else
		{
			if(numOfSpaces <= (81-32))
				level = 2;
			else
				if(numOfSpaces <= (81-28))
					level = 3;
				else
					level = 4;
		}
	}
		
	// check the max spaces in a criteria ie a row/row/box
	// check row
	int maxEmpty = 0;
	for(int i = 0 ; i < 9; ++i)
	{
		int empty = 0;
		for(int j = 0; j < 9; ++j)
		{
			if(grid[i][j] == 0)
				++empty;
		}
		if(empty > maxEmpty) 
			maxEmpty = empty;
	}
	//check columns
	for(int i = 0 ; i < 9; ++i)
	{
		int empty = 0;
		for(int j = 0; j < 9; ++j)
		{
			if(grid[j][i] == 0)
				++empty;
		}
		if(empty > maxEmpty) 
			maxEmpty = empty;
	}
	//check boxes
	for(int i = 0 ; i < 3; ++i)
		for(int j = 0; j < 3; ++j)
		{
			int empty = 0;
			for(int k = 0 ; k < 3; ++k)
				for(int l = 0 ; l < 3; ++l)
				{
					if(grid[k+i*3][l+j*3] == 0)
						++empty;
				}
			if(empty > maxEmpty) 
				maxEmpty = empty;
		}
	// decide level based on max num of spaces in a criteria
	if(maxEmpty == 9)
		level += 4;
	else if(maxEmpty >= 7)
		level += 3;
	else if(maxEmpty >= 6)
		level += 2;
	else if(maxEmpty >= 5)
		level += 1;
	else
		level += 0;

	// check techniques used:
	// logical techniques from: http://www.sudokudragon.com/sudokustrategy.htm
	// score from: https://www.sudokuoftheday.com/about/difficulty/

	guesses.fillValids();  // get valid candidates in 3d vector in spaces

	basicElimination(); // level 0;
	onlySquare();
	
	if(subGpExclusion()) // easy
		level += 1;

	if(twinExclusion()) // med
		level += 1;

	bool sub = false, sq =  false, be = false, tw = false;
	do
	{
		sub = subGpExclusion();
		sq = onlySquare();
		be =basicElimination();
		tw = twinExclusion();
	}while (sub || sq || be || tw); // split for debugging

	if(alternatePairEx()) // med hard // triplet!! & alt pair ex!!!
		level = 10; 

	while(subGpExclusion() || onlySquare() || basicElimination() || twinExclusion() || alternatePairEx());

	if(!alreadySolved()) // crazy hard
		level += 3;

	if(level < 3)
		level = 0; // too easy
	else if(level < 6)
		level = 1; // easy
	else if(level < 9)
		level = 2; // med
	else if(level < 12)
		level = 3; // hard
	else
		level = 4; // extra hard
	
	copyGrid(tempGrid, grid);

	return level;
}
