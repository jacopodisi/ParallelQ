#include "agent.h"

std::shared_ptr<Eigen::MatrixXd> Agent::global_q = std::make_shared<Eigen::MatrixXd>();

Agent::Agent(Environment param_env, agent_options param_opt)
{
	parallel = false;
	env = param_env;
	states_list = env.getStatesList();
	num_actions = env.getNumActions();
	//global_q->setZero(states_list->rows(), env.getNumActions());
	q_function = std::make_shared<Eigen::MatrixXd>(states_list->rows(), env.getNumActions());
	q_function->setZero();
	q_cache = std::make_shared<Eigen::VectorXd>(states_list->rows());
	q_cache->setZero();
	init_row = 0;
	end_row = env.getStatesList()->rows() - 1;
	cache_size = 0;
	opt = param_opt;
}

Agent::Agent(Environment param_env, int param_init, int param_end, int param_cache_size, agent_options param_opt)
{
	parallel = true;
	env = param_env;
	states_list = env.getStatesList();
	num_actions = env.getNumActions();
	global_q->setZero(states_list->rows(), env.getNumActions());
	q_cache = std::make_shared<Eigen::VectorXd>(states_list->rows());
	q_cache->setZero();
	init_row = param_init;
	end_row = param_end;
	cache_size = param_cache_size;
	opt = param_opt;
	if(opt.shared_mem)
	{
		q_function = global_q;
	} else
	{
		q_function = std::make_shared<Eigen::MatrixXd>(states_list->rows(), env.getNumActions());
		q_function->setZero();
	}
}

int Agent::epsilonGreedyPolicy(int state, double epsilon)
{
	std::shared_ptr<Eigen::VectorXd> policy = std::make_shared<Eigen::VectorXd>(num_actions);
	(*policy).fill(epsilon / num_actions);
	int max = 0;
	for (int i = 1; i < num_actions; ++i)
	{
		if ((*q_function)(state, i) > (*q_function)(state, max))
			max = i;
	}
	(*policy)(max) += 1 - epsilon;
	double val = ((double) rand() / RAND_MAX);
	double prob = 0;
	int action = 0;
	for(int i = 0; i < num_actions; i++)
	{
		prob += (*policy)(i);
		if (val < prob) {action = i; break;}
	}
	return action;
}

void * Agent::learn(void * agent)
{
	Agent ag = (*(Agent *)agent);
	bool outside = false;
	double epsilon = ag.opt.eps;
	std::chrono::steady_clock::time_point start;
	start = std::chrono::steady_clock::now();
	for (int episode = 0; episode < ag.opt.num_ep; episode++)
	{
		outside = false;
		if (ag.opt.eps < 0) epsilon = pow(1-episode/ag.opt.num_ep, 2);
		int state = ag.env.reset(ag.init_row, ag.end_row);
		//calculate the action to be performed 
		//based on the e-greedy policy derived from the Q function
		int action = ag.epsilonGreedyPolicy(state, epsilon);
		//first condition
		for (int i = 0; i < ag.opt.mse; ++i)
		{
		    observation ob = ag.env.step(static_cast<Actions>(action));
		    if (ag.parallel)
		    {
				int pos = ag.env.getCurrStateNumber();
				if (pos<ag.init_row && pos>ag.end_row)
				{
					if (!ag.opt.shared_mem)
					{
						if ((*ag.q_cache)(ob.next_state) == 0)
						{
							(*ag.q_function).row(ob.next_state) = (*global_q).row(ob.next_state);
							(*ag.q_cache)(ob.next_state) = ag.cache_size;
						} else 
						{
							(*ag.q_cache)(ob.next_state)--;
						}
					}
					outside = true;
				}
			}
			//second condition
		    if (ob.done)
		    {
		    	double delta = ob.reward - (*ag.q_function)(state, action);
		    	(*ag.q_function)(state, action) += ag.opt.alpha * delta;
		    	if(!ag.opt.shared_mem && ag.parallel)
		    	{
			    	if ((std::chrono::steady_clock::now() - start) >= std::chrono::microseconds(8))
			    	{
			    		(*global_q).row(state) = (*ag.q_function).row(state);
			    		start = std::chrono::steady_clock::now();
			    	}
			    }
		    	break;
		    }
		    int next_action = ag.epsilonGreedyPolicy(ob.next_state, epsilon);
			//improve the Q function for the current (state, action)
			//from the deterministic policy, following the e-greedy one
		    double delta = ob.reward + ag.opt.discount * (ag.q_function->row(ob.next_state)).maxCoeff() - (*ag.q_function)(state, action);
		    (*ag.q_function)(state, action) += ag.opt.alpha * delta;
		    //third condition
		    if(!ag.opt.shared_mem && ag.parallel)
		    {
			    if ((std::chrono::steady_clock::now() - start) >= std::chrono::microseconds(8))
		    	{
		    		(*global_q).row(state) = (*ag.q_function).row(state);
		    		start = std::chrono::steady_clock::now();
		    	}
		    }
		    if (outside) break;
		    state = ob.next_state;
		    action = next_action;
		}
	}
	return (void *) &(*ag.q_function);
}

std::shared_ptr<Eigen::MatrixXd> Agent::getQ()
{
	return q_function;
}
std::shared_ptr<Eigen::MatrixXd> Agent::getGlobalQ()
{
	return global_q;
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
	for (std::size_t i = 0; i < size; i++)
	{
		for (int j = 0; j < num_actions; j++)
			fread(&(*Q)(i,j), sizeof(double), 1, fs);
	}

	if(std::fclose(fs) != 0) std::cout << "Error in closing file" << '\n';
	return Q;
}