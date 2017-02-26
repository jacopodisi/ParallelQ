#include "gridhandler.h"
#include <map>
#include <cmath>

typedef Eigen::Matrix<position, Eigen::Dynamic, 1> PosVector;


class Environment {
public:

	
	/*
	empty inizialization of the object
	*/
	Environment();

	/*
	initialization of Environment
	@param grid_size: size of the row of the grid
		   grid_id:   index of the grid in the file
	*/
	Environment(int grid_size, int grid_id);

	/*
	reset the value of curr_state to a consistent position
	@return position as integer of the value curr_state
	*/
	int reset();

	/*
	reset the value of curr_state to a consistent position given
	some constraints
	@param the first and last column/row in which initialize curr_state
	@return position as integer of the value curr_state
	*/
	int reset(int init_row, int end_row, int init_col, int end_col);
		
	/*
	make a step in the grid (modify also the curr_state variable)
	@param action to be performed
	@return the observation: next_state (integer (-1 if the state is goal)), reward, done
	*/
	observation step(Actions a);

	/*
	make a step in the grid (does not modify the curr_state variable)
	@param state as integer of the state from which to predict
	       action is the action to be predicted
	@return the observation: next_state (integer (-1 if the state is goal)), reward, done
	*/
	observation prediction(int state, Actions a);

	/*
	@return the value of curr_state as a position variable
	*/
	position getCurrState();
	int getCurrStateNumber()
	{
		return (*positions_list)[std::make_pair(curr_state.row, curr_state.col)];
	};
	int reset(int init, int end)
	{
		int state = init + (rand() % (end - init + 1));
		curr_state = (*states_list)(state);
		return 	state;
	};

	/*
	@return the pointer to the grid
	*/
	std::shared_ptr<MatrixXi> getGrid();

	/*
	@return the maximum number of actions that can be performed
	*/
	int getNumActions();

	/*
	@return the vector states_list (as a smart pointer to the vector of position)
	*/
	std::shared_ptr<PosVector> getStatesList();

	/*
	@return the vector positions_list 
	(as a smart pointer to the map<KEY: pair(int, int), VALUE: state number>)
	*/
	std::shared_ptr<std::map<std::pair<int, int>, int>> getPositionsList();

	int getId(){return id;};
	/*
	save the value function of the environment in a file
	called value_function_list_size(size).bin
	*/
	void saveValueFunction(std::shared_ptr<Eigen::VectorXd> value_function);
	std::shared_ptr<Eigen::VectorXd> readValueFunction();
	
	/*
	print the grid
	*/
	void printGridEnv();

	virtual ~Environment(){};

private:
	
	std::shared_ptr<MatrixXi> grid;

	int id;
	
	position curr_state;

	std::shared_ptr<PosVector> states_list;

	/*
	list used to map position to state number
	instead of the position it is used a pair of integer.
	the first int is the row, the second is the col as for the position
	*/
	std::shared_ptr<std::map<std::pair<int, int>, int>> positions_list;
};