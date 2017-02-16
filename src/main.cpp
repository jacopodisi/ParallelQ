#include "dynamicprog.h"
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

	Environment env (14, 0);
	env.printGridEnv();
	std::shared_ptr<Eigen::VectorXd> V;
	V = Dynamicprog::valueIteration(env, 0.8);
	std::cout << (*V) << '\n';
}