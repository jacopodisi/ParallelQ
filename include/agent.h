#include "environment.h"

typedef  Eigen::Matrix<Q_value, Eigen::Dynamic, Eigen::Dynamic> QList;

class Agent
{
public:
	Agent(Environment env);
	~Agent();

private:
	int num_actions;
	Environment env;
	std::shared_ptr<PosVector> states_list;
	std::shared_ptr<QList> q_function;
};