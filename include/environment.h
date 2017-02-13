#include "gridhandler.h"
#include <list>


class Environment {
public:
	Environment(int grid_size, int grid_id);

	position reset();

	position reset(int init_row, int end_row, int init_col, int end_col);
	
	observation step(Actions a);

	position getCurrState();

	std::shared_ptr<MatrixXi> getGrid();

	void printGridEnv();
	
	virtual ~Environment(){};

private:
	
	std::shared_ptr<MatrixXi> grid;
	
	position curr_state;
};