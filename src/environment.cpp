#include "environment.h"
#include <iostream>
#include <thread>

Environment::Environment()
{
	std::shared_ptr<MatrixXi> param_grid = std::make_shared<MatrixXi>(0,0);
	grid = param_grid;
	srand(time(0));
}

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
	int nstate = 0;
	position p;
	std::pair<int, int> p1;
	std::shared_ptr<std::map<std::pair<int, int>, int>> pos_list = std::make_shared<std::map<std::pair<int, int>, int>>();
	for (int i = 0; i < grid->rows(); ++i)
	{
		for (int j = 0; j < grid->cols(); ++j)
		{
			if ((*grid)(i,j)==0) 
			{
				p.row = i;
				p.col = j;
				p1 = std::make_pair(p.row, p.col);
				(*pos_list)[p1] = nstate;
				nstate++;
			}
		}
	}

	std::shared_ptr<PosVector> param_list = std::make_shared<PosVector>(nstate);
	int s = 0;
	for (int i = 0; i < grid->rows(); ++i)
	{
		for (int j = 0; j < grid->cols(); ++j)
		{
			if ((*grid)(i,j) == 0) 
			{
				p.row = i;
				p.col = j;
				(*param_list)(s) = p;
				s++;
			}
		}
	}

	positions_list = pos_list;
	states_list = param_list;

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
	if (0 <= pos.row < grid->rows() &&
		0 <= pos.col < grid->cols())
	{
		//inside the grid
		if ((*grid)(pos.row, pos.col) != -1)
			//next state != wall
			curr_state = pos;
		if ((*grid)(pos.row, pos.col) == 1)
		{
			//next state == goal

			ob.next_state = (*positions_list)[std::make_pair(curr_state.row, curr_state.col)];
			ob.reward = 1000;
			ob.done = true;
			return ob;
		}
	}

	ob.next_state = (*positions_list)[std::make_pair(curr_state.row, curr_state.col)];;
	ob.reward = 0;
	ob.done = false;
	return ob;
}

observation Environment::prediction(int state, Actions a)
{

	observation ob;
	position n_state, c_state;
	n_state = (*states_list)(state);
	c_state = n_state;
	switch (a)
	{
		case up: n_state.row--; break;
		case down: n_state.row++; break;
		case left: n_state.col--; break;
		case right: n_state.col++; break;
	}
	if (n_state.row >= 0 && n_state.row < grid->rows() &&
		n_state.col >= 0 && n_state.col < grid->cols())
	{
		//inside the grid
		if ((*grid)(n_state.row, n_state.col) != -1)
			//next state != wall
			c_state = n_state;
		if ((*grid)(n_state.row, n_state.col) == 1)
		{
			//next state == goal
			ob.next_state = (*positions_list)[std::make_pair(c_state.row, c_state.col)];;
			ob.reward = 1000;
			ob.done = true;
			return ob;
		}
	}

	ob.next_state = (*positions_list)[std::make_pair(c_state.row, c_state.col)];;
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

int Environment::getNumActions()
{
	return 4;
}

std::shared_ptr<PosVector> Environment::getStatesList()
{
	return states_list;
}


std::shared_ptr<std::map<std::pair<int, int>, int>> Environment::getPositionsList()
{
	return positions_list;
}

void Environment::printGridEnv()
{
	printGrid(grid, curr_state.row, curr_state.col);
}





