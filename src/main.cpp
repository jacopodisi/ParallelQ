#include "environment.h"
#include <iostream>

int main(int argc, char* argv[])
{
	if (argc>2)
	{
		int gridsize = atof(argv[1]);
		int wallsize = gridsize*gridsize*0.312;
		int numgrid = atof(argv[2]);
		for (int i=2; i<numgrid; i++)
		{
			GridMatrix_pointer grid = gridGenerator(gridsize, wallsize);
			printGrid(grid);
		}
	}

	Environment env(14, 2);
	env.printGridEnv();
	env.reset(0,7,0,4);
	env.printGridEnv();
	Actions bo = static_cast<Actions> (0);
	env.step(bo);
	env.printGridEnv();
}