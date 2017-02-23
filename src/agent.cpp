#include "agent.h"

Agent::Agent(Environment param_env)
{
	env = param_env;
	states_list = env.getStatesList();
	num_actions = env.getNumActions();
	global_q = std::make_shared<Eigen::MatrixXd>(states_list->rows(), env.getNumActions());
	global_q->setZero();
	q_function = std::make_shared<Eigen::MatrixXd>(states_list->rows(), env.getNumActions());
	q_function->setZero();
	q_cache = std::make_shared<Eigen::VectorXd>(states_list->rows());
	q_cache->setZero();
	init_row = 0;
	end_row = 0;
	init_col = 0;
	end_col = 0;
	cache_size = 0;
}

Agent::Agent(Environment param_env, int param_init_row, int param_end_row, int param_init_col, int param_end_col, int param_cache_size)
{
	env = param_env;
	states_list = env.getStatesList();
	num_actions = env.getNumActions();
	global_q = std::make_shared<Eigen::MatrixXd>(states_list->rows(), env.getNumActions());
	global_q->setZero();
	q_function = std::make_shared<Eigen::MatrixXd>(states_list->rows(), env.getNumActions());
	q_function->setZero();
	q_cache = std::make_shared<Eigen::VectorXd>(states_list->rows());
	q_cache->setZero();
	init_row = param_init_row;
	end_row = param_end_row;
	init_col = param_init_col;
	end_col = param_end_col;
	cache_size = param_cache_size;
}

int Agent::epsilonGreedyPolicy(int state, double epsilon)
{
	std::shared_ptr<Eigen::VectorXd> policy = std::make_shared<Eigen::VectorXd>(num_actions);
	(*policy).fill(epsilon / num_actions);
	if (debugAgent) std::cout << (*policy).transpose() << " ";
	int max = 0;
	for (int i = 1; i < num_actions; ++i)
	{
		if ((*q_function)(state, i) > (*q_function)(state, max))
			max = i;
	}
	if (debugAgent) std::cout << std::to_string(max) << " ";
	(*policy)(max) += 1 - epsilon;
	double val = ((double) rand() / RAND_MAX);
	if (debugAgent) std::cout << "val: " + std::to_string(val) + " ";
	if (debugAgent) std::cout << (*policy).transpose() << " ";
	double prob = 0;
	int action = 0;
	for(int i = 0; i < num_actions; i++)
	{
		prob += (*policy)(i);
		if (debugAgent) std::cout << std::to_string(prob) + " ";
		if (val < prob) {action = i; break;}
	}
	if (debugAgent) std::cout << std::to_string(action) + " ";
	return action;
}

void Agent::learn(double eps, int num_episodes, double discount_factor, double alpha, int MSE)
{
	bool outside = false;
	double epsilon = eps;
	std::chrono::steady_clock::time_point start;
	std::chrono::steady_clock::time_point end;
	start = std::chrono::steady_clock::now();
	for (int episode = 0; episode < num_episodes; episode++)
	{
		if (eps < 0) epsilon = pow(1-episode/num_episodes, 2);
		int state = env.reset(init_row, end_row, init_col, end_col);
		if (debugAgent) std::cout << std::to_string(state);
		//calculate the action to be performed 
		//based on the e-greedy policy derived from the Q function
		int action = epsilonGreedyPolicy(state, epsilon);
		//first condition
		for (int i = 0; i < MSE; ++i)
		{
			if (debugAgent) std::cout << std::to_string(state) << ' ';
		    observation ob = env.step(static_cast<Actions>(action));
			if (debugAgent) std::cout << std::to_string(static_cast<Actions>(action)) << ' ';
			position pos = env.getCurrState();
			if (!(pos.row >= init_row &&
					pos.row <= end_row &&
					pos.col >= init_col &&
					pos.col <= end_col))
			{
				if ((*q_cache)(ob.next_state) == 0)
				{
					//TODO mutex
					(*q_function).row(ob.next_state) = (*global_q).row(ob.next_state);
					(*q_cache)(ob.next_state) = cache_size;
				} else 
				{
					(*q_cache)(ob.next_state)--;
				}
				outside = true;
			}
			//second condition
		    if (ob.done)
		    {
				if (debugAgent) std::cout << std::to_string(state);
		    	double delta = ob.reward - (*q_function)(state, action);
		    	(*q_function)(state, action) += alpha * delta;
		    	if ((start-std::chrono::steady_clock::now()) >= std::chrono::microseconds(8))
		    	{
		    		//TODO mutex
		    		(*global_q).row(state) = (*q_function).row(state);
		    		start = std::chrono::steady_clock::now();
		    	}
		    	break;
		    }
		    int next_action = epsilonGreedyPolicy(ob.next_state, epsilon);
			//improve the Q function for the current (state, action)
			//from the deterministic policy, following the e-greedy one
		    double delta = ob.reward + discount_factor * (q_function->row(ob.next_state)).maxCoeff() - (*q_function)(state, action);
		    (*q_function)(state, action) += alpha * delta;
		    //third condition
		    if ((start-std::chrono::steady_clock::now()) >= std::chrono::microseconds(8))
	    	{
	    		//TODO mutex
	    		(*global_q).row(state) = (*q_function).row(state);
	    		start = std::chrono::steady_clock::now();
	    	}
		    if (outside) break;
		    state = ob.next_state;
		    action = next_action;
		    if (debugAgent) std::cout << "fs" << '\n';
		}
		if (debugAgent) std::cout << '\n';
		if (debugAgent) std::cout << (*q_function) << '\n';
	}
}

std::shared_ptr<Eigen::MatrixXd> Agent::getQ()
{
	return q_function;
}

void Agent::saveQ(std::string fopt)
{
	std::string fn = "qfunc/grid_size" + std::to_string(env.getGrid()->rows()) + "id" + std::to_string(env.getId()) + fopt + ".bin";
	std::string choice;
	while (fileExists(fn)) 
	{
		std::cout << "file " + fn + " already exist. Do you want to overwrite it?(y/n): ";
		std::cin >> choice;
		std::cout  << '\n';
		while(choice != "y" && choice != "n")
		{
			std::cout << "Invalid option (y/n): ";
			std::cin >> choice;
		}
		if (choice == "y") break;
		else std::cout << "specify a new file name: qfunc/grid_size" + std::to_string(env.getGrid()->rows()) + "id" + std::to_string(env.getId());
		std::cin >> fopt;
		fn = "qfunc/grid_size" + std::to_string(env.getGrid()->rows()) + "id" + std::to_string(env.getId()) + fopt + ".bin";
	}
	int size = q_function->cols();
	double output[size];
	FILE *fs = fopen(fn.c_str(), "wb");
	if(!fs) std::perror("File opening failed");
	for (int i = 0; i < q_function->rows(); ++i)
	{
		for (int j = 0; j < size; j++) output[j] = (*q_function)(i,j);
		std::fwrite(output, sizeof(double), size, fs);
	}
	if(std::fflush(fs) != 0)
	{
		std::cout << "Error in flushing file" << '\n';
		return;
	}
	if(std::fclose(fs) != 0) std::cout << "Error in closing file" << '\n';
}

std::shared_ptr<Eigen::MatrixXd> Agent::readQ(std::string fopt)
{
	std::string fn = "qfunc/grid_size" + std::to_string(env.getGrid()->rows()) + "id" + std::to_string(env.getId()) + fopt + ".bin";	
	while (!fileExists(fn)) 
	{
		std::cout << "file does not exist: " + fn << '\n';
		std::cout << "Specify a new file name (size=" + std::to_string(env.getGrid()->rows()) + ", id=" + std::to_string(env.getId()) + "): ";
		std::cin >> fopt;
		std::string fn = "qfunc/grid_size" + std::to_string(env.getGrid()->rows()) + "id" + std::to_string(env.getId()) + fopt + ".bin";	
	}
	FILE *fs = fopen(fn.c_str(), "rb");
	if(!fs)
	{
		std::cout << "File opening failed: ";
		std::perror(fn.c_str());
	}
	std::fseek(fs, 0, SEEK_END);
    std::size_t size = std::ftell(fs)/(sizeof(double)*num_actions);
    std::shared_ptr<Eigen::MatrixXd> Q = std::make_shared<Eigen::MatrixXd>(size,num_actions);
    std::fseek(fs, 0, SEEK_SET);
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < num_actions; j++)
			fread(&(*Q)(i,j), sizeof(double), 1, fs);
	}

	if(std::fclose(fs) != 0) std::cout << "Error in closing file" << '\n';
	return Q;
}

static void parallelAgents()
{

}





