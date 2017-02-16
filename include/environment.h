#include "gridhandler.h"
#include <list>
#include <map>

typedef Eigen::Matrix<position, Eigen::Dynamic, 1> PosVector;


class Environment {
public:

	Environment();

	Environment(int grid_size, int grid_id);

	position reset();

	position reset(int init_row, int end_row, int init_col, int end_col);
	
	observation step(Actions a);

	observation prediction(int state, Actions a);

	position getCurrState();

	std::shared_ptr<MatrixXi> getGrid();

	int getNumActions();

	std::shared_ptr<PosVector> getStatesList();

	std::shared_ptr<std::map<std::pair<int, int>, int>> getPositionsList();

	void printGridEnv();
	
	virtual ~Environment(){};

private:
	
	std::shared_ptr<MatrixXi> grid;
	
	position curr_state;

	std::shared_ptr<PosVector> states_list;

	/*
	list used to map position to state number
	instead of the position it is used a pair of integer.
	the first int is the row, the second is the col as for the position
	*/
	std::shared_ptr<std::map<std::pair<int, int>, int>> positions_list;
};