#include "environment.h"

class Dynamicprog
{
public:
	Dynamicprog(){};
	static std::shared_ptr<Eigen::VectorXd> valueIteration(Environment env, double discount_factor, double theta = 0);
	static std::shared_ptr<Eigen::VectorXd> lookahead(Environment env, int state, std::shared_ptr<Eigen::VectorXd> V, double discount_factor);

	~Dynamicprog();
};
