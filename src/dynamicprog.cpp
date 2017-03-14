#include "dynamicprog.h"

std::shared_ptr<Eigen::VectorXd> Dynamicprog::valueIteration(Environment env, double discount_factor, double theta)
{
	std::shared_ptr<PosVector> states_list = env.getStatesList();
	std::shared_ptr<std::map<std::pair<int, int>, int>> p_list = env.getPositionsList();
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
	    	A = Dynamicprog::lookahead(env, state, value_function, discount_factor);
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

std::shared_ptr<Eigen::VectorXd> Dynamicprog::lookahead(Environment env, int state, std::shared_ptr<Eigen::VectorXd> V, double discount_factor)
{
	std::shared_ptr<Eigen::VectorXd> A = std::make_shared<Eigen::VectorXd>(env.getNumActions());
	(*A).setZero();
	observation ob;
	for (int i = 0; i < env.getNumActions(); ++i)
	{
		ob = env.prediction(state, static_cast<Actions>(i));
		if (ob.done) (*A)(i) = ob.reward;
		else (*A)(i) = ob.reward + discount_factor * (*V)(ob.next_state);
	}
	return A;
}