#include "environment.h"
#include <cmath>

class Dynamicprog
{
public:
	Dynamicprog(){};

	/*
	@param env: environment to learn
		   discount factor
		   theta: precision of the algorithm (default 0)
	@return value function as a (pointer to) vector of double
	*/
	static std::shared_ptr<Eigen::VectorXd> valueIteration(Environment env, double discount_factor = 0.95, double theta = 0);
	static std::shared_ptr<Eigen::VectorXd> lookahead(Environment env, int state, std::shared_ptr<Eigen::VectorXd> V, double discount_factor);

	~Dynamicprog();
};
