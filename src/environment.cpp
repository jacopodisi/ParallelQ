#include "environment.h"
#include "functions.h"

Environment::Environment()
{
	std::shared_ptr<MatrixXi> param_grid = std::make_shared<MatrixXi>(0,0);
	grid = param_grid;
	id = 0;
}

Environment::Environment(int grid_size, int grid_id)
{
	while (true)
	{	
		std::string fn = "grid/grid_matrix_size" + std::to_string(grid_size) + "id0.bin";
		if (Functions::fileExists(fn)) break;
	    std::cout << "incorrect grid size, specify a new size value: ";
		std::cin >> grid_size;
		std::cout << '\n';
	}
	int max_id;
	for (max_id = 0; ; ++max_id)
	{
		std::string fn = "grid/grid_matrix_size" + std::to_string(grid_size) + "id" + std::to_string(max_id) + ".bin";
		if (!Functions::fileExists(fn)) {max_id--; break;}
	}
	while (true)
	{
		if(grid_id<=max_id) break;
	    std::cout << "incorrect grid id \n";
		std::cout << "specify a new id value (MAX id: " + std::to_string(max_id) + "): ";
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
	id = grid_id;
}

int Environment::reset()
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
	return 	(*positions_list)[std::make_pair(curr_state.row, curr_state.col)];
}

int Environment::reset(int init_row, int end_row, int init_col, int end_col)
{
	position pos;
	while(true) 
	{
	    pos.row = (rand() % (end_row - init_row + 1)) + init_row;
	    pos.col = (rand() % (end_col - init_col + 1)) + init_col;
	    if ((*grid)(pos.row, pos.col) == 0) break;
	}
	curr_state = pos;
	return 	(*positions_list)[std::make_pair(curr_state.row, curr_state.col)];
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
	if (pos.row >= 0 && pos.row < grid->rows() &&
		pos.col >= 0 && pos.col < grid->cols())
	{
		//inside the grid
		if ((*grid)(pos.row, pos.col) != -1)
			//next state != wall
			curr_state = pos;
		if ((*grid)(pos.row, pos.col) == 1)
		{
			//next state == goal
			ob.next_state = -1;
			ob.reward = 100;
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
			ob.next_state = -1;
			ob.reward = 100;
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

void Environment::saveValueFunction(std::shared_ptr<Eigen::VectorXd> value_function)
{
	int gridsize = grid->rows();
	std::string dir = "grid";
	std::string fn = "value_function_grid_size" + std::to_string(gridsize) + "id" + std::to_string(id);
	std::shared_ptr<Eigen::MatrixXd> mat = std::make_shared<Eigen::MatrixXd>(value_function->rows(), 1);
	mat->col(0) = value_function->col(0);
	Functions::saveMat(mat, fn, dir);
}

std::shared_ptr<Eigen::VectorXd> Environment::readValueFunction()
{
	std::cout << "1\n";
	std::string fn = "grid/value_function_grid_size" + std::to_string(grid->rows()) + "id" + std::to_string(id) + ".bin";
	if (!Functions::fileExists(fn)) 
	{
		std::cout << "Calculating Value function of grid " << id << '\n';
		Environment::saveValueFunction(Environment::valueIteration());
	}
	FILE *fs = fopen(fn.c_str(), "rb");
	if(!fs)
	{
		std::cout << "File opening failed: ";
		std::perror(fn.c_str());
	}
	uint rows, cols;
	fread(&rows, sizeof(uint), 1, fs);
	fread(&cols, sizeof(uint), 1, fs);
    std::shared_ptr<Eigen::VectorXd> V = std::make_shared<Eigen::VectorXd>(rows);
    for (std::size_t i = 0; i < rows; i++)
	{
		fread(&(*V)(i), sizeof(double), 1, fs);
	}
	if(std::fclose(fs) != 0) std::cout << "Error in closing file" << '\n';
	return V;
}


void Environment::printGridEnv()
{
	printGrid(grid, curr_state.row, curr_state.col);
}

std::shared_ptr<Eigen::VectorXd> Environment::valueIteration(double discount_factor, double theta)
{
	std::shared_ptr<PosVector> states_list = getStatesList();
	std::shared_ptr<std::map<std::pair<int, int>, int>> p_list = getPositionsList();
	std::shared_ptr<Eigen::VectorXd> value_function = std::make_shared<Eigen::VectorXd>(states_list->rows());
	std::shared_ptr<Eigen::VectorXd> A;
	if (debugDynamic) std::cout << std::to_string(states_list->rows()) << '\n';
	(*value_function).setZero();
	while(true) {
	    double delta = 0;
	    if (debugDynamic) std::cout << "             u d l r" << '\n';
	    for (int state = 0; state < value_function->rows(); state++)
	    {
	    	if (debugDynamic) std::cout << "pos: " + std::to_string((*states_list)(state).row) + " " + std::to_string((*states_list)(state).col) + " --> ";
	    	A = lookahead(state, value_function, discount_factor);
	    	if (debugDynamic) std::cout << (*A).transpose() <<   '\n';
	    	double best_action_value = (*A).maxCoeff();
	    	delta = std::fmax(delta, std::abs(best_action_value - (*value_function)(state)));
	    	(*value_function)(state) = best_action_value;
	    }
	    if (delta <= theta) break;
	    if (debugDynamic) std::cout << "delta = " + std::to_string(delta) << '\n';
	}
	return value_function;
}

std::shared_ptr<Eigen::VectorXd> Environment::lookahead(int state, std::shared_ptr<Eigen::VectorXd> V, double discount_factor)
{
	std::shared_ptr<Eigen::VectorXd> A = std::make_shared<Eigen::VectorXd>(getNumActions());
	(*A).setZero();
	observation ob;
	for (int i = 0; i < getNumActions(); ++i)
	{
		ob = prediction(state, static_cast<Actions>(i));
		if (ob.done) (*A)(i) = ob.reward;
		else (*A)(i) = ob.reward + discount_factor * (*V)(ob.next_state);
	}
	return A;
}