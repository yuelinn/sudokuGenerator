# makefile

#VARNAME = value
CXX = g++
CXXFLAGS = -Wall -g

sudoku: main.o solve.o generate.o level.o transform.o grid.o
	$(CXX) $(CXXFLAGS) -o sudoku main.o solve.o generate.o level.o transform.o grid.o

main.o: main.cpp sudokuHeader.h 
	$(CXX) $(CXXFLAGS) -c main.cpp

solve.o: solve.cpp sudokuHeader.h 
	$(CXX) $(CXXFLAGS) -c solve.cpp

generate.o: generate.cpp sudokuHeader.h
	$(CXX) $(CXXFLAGS) -c generate.cpp

level.o: level.cpp sudokuHeader.h
	$(CXX) $(CXXFLAGS) -c level.cpp

transform.o: transform.cpp sudokuHeader.h
	$(CXX) $(CXXFLAGS) -c transform.cpp

grid.o: grid.cpp sudokuHeader.h
	$(CXX) $(CXXFLAGS) -c grid.cpp