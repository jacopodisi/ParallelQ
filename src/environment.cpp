#include "environment.h"
#include <iostream>
#include <thread>

Environment::Environment(int grid_size, int grid_id)
{
	int l_size = listSize(grid_size);
	while (true)
	{
		if (!(grid_id >= l_size)) break;
		std::cout << "incorrect grid id for grid size " + std::to_string(grid_size) << '\n';
		std::cout << "specify a new id value (MAX id: " + std::to_string(l_size - 1) + "): ";
		std::cin >> grid_id;
		std::cout << '\n';
	}
	grid = openGrid(grid_size, grid_id);
	srand(time(0));
}

position Environment::reset()
{
	int size = grid->cols();
	position pos;
	while(true) 
	{
	    pos.row = rand() % size;
	    pos.col = rand() % size;
	    if ((*grid)(pos.row, pos.col) == 0) break;
	}
	curr_state = pos;
	return curr_state;
}

position Environment::reset(int init_row, int end_row, int init_col, int end_col)
{
	position pos;
	while(true) 
	{
	    pos.row = (rand() % (end_row - init_row + 1)) + init_row;
	    pos.col = (rand() % (end_col - init_col + 1)) + init_col;
	    if ((*grid)(pos.row, pos.col) == 0) break;
	}
	curr_state = pos;
	return curr_state;
}

observation Environment::step(Actions a)
{
	observation ob;
	position pos;
	pos = curr_state;
	switch (a)
	{
		case up: pos.row--; break;
		case down: pos.row++; break;
		case left: pos.col--; break;
		case right: pos.col++; break;
	}
	if (0 < pos.row < grid->rows() &&
		0 < pos.col < grid->cols())
	{
		//inside the grid
		if ((*grid)(pos.row, pos.col) != -1)
			//next state != wall
			curr_state = pos;
		if ((*grid)(pos.row, pos.col) == 1)
		{
			//next state == goal
			ob.next_state = curr_state;
			ob.reward = 1000;
			ob.done = true;
			return ob;
		}
	}

	ob.next_state = curr_state;
	ob.reward = 0;
	ob.done = false;
	return ob;
}

position Environment::getCurrState()
{
	return curr_state;
}

std::shared_ptr<MatrixXi> Environment::getGrid()
{
	return grid;
}

void Environment::printGridEnv()
{
	printGrid(grid, curr_state.row, curr_state.col);
}





