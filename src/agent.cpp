#include "agent.h"

Agent::Agent(Environment param_env)
{
	env = param_env;
	states_list = env.getStatesList();
	std::shared_ptr<QList> q_param = std::make_shared<QList>(states_list->rows(), env.getNumActions());
	q_function = q_param;
}

