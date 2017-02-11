#include "environment.h"
#include <iostream>

int main(int argc, char* argv[])
{
	int gridsize = 14;
	int wallsize = gridsize*gridsize*0.312;
	GridMatrix_pointer grid = gridGenerator(gridsize, wallsize);
	printGrid(grid);
}