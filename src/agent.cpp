/***************************************************************************
 *   Copyright (C) 2017 by Jacopo Di Simone                                *
 *                                                                         *
 *   This file is part of ParallelQ.                                      *
 *                                                                         *
 *   ParallelQ is free software; you can redistribute it and/or modify    *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   ParallelQ is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with ParallelQ; if not, see <http://www.gnu.org/licenses/>    *
 ***************************************************************************/

#include "agent.h"
#include "functions.h"

std::shared_ptr<Eigen::MatrixXd> Agent::global_q = std::make_shared<Eigen::MatrixXd>();
std::shared_ptr<Eigen::MatrixXd> Agent::ep_value_function = std::make_shared<Eigen::MatrixXd>();
int Agent::num_agents = 0;

Agent::Agent(Environment param_env, agent_options param_opt, bool param_compute_ep_val)
{
	compute_ep_val = param_compute_ep_val;
	leader_agent = false;
	parallel = false;
	env = param_env;
	states_list = env.getStatesList();
	num_actions = env.getNumActions();
	q_function = std::make_shared<Eigen::MatrixXd>(states_list->rows(), env.getNumActions());
	q_function->setZero();
	q_cache = std::make_shared<Eigen::VectorXd>(states_list->rows());
	q_cache->setZero();
	init_row = 0;
	end_row = env.getStatesList()->rows() - 1;
	cache_size = 0;
	opt = param_opt;
	opt.shared_mem = false;
	value_function = env.readValueFunction();
	ep_value_function->setZero(value_function->rows(), opt.num_ep);
}

Agent::Agent(Environment param_env, int param_init, int param_end, int param_cache_size, agent_options param_opt, bool param_compute_ep_val)
{
	compute_ep_val = param_compute_ep_val;
	leader_agent = false;
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
	value_function = env.readValueFunction();
	ep_value_function->setZero(value_function->rows(), opt.num_ep);
	num_agents++;
	if (num_agents == 1) leader_agent = true;
}

int Agent::epsilonGreedyPolicy(int state, double epsilon)
{
	std::shared_ptr<Eigen::VectorXd> policy = std::make_shared<Eigen::VectorXd>(num_actions);
	(*policy).fill(epsilon / num_actions);
	//if (debugAgent) std::cout << (*policy).transpose() << " ";
	int max = 0;
	for (int i = 1; i < num_actions; ++i)
	{
		if ((*q_function)(state, i) > (*q_function)(state, max))
			max = i;
	}
	//if (debugAgent) std::cout << std::to_string(max) << " ";
	(*policy)(max) += 1 - epsilon;
	double val = ((double) rand() / RAND_MAX);
	//if (debugAgent) std::cout << "val: " + std::to_string(val) + " ";
	//if (debugAgent) std::cout << (*policy).transpose() << " ";
	double prob = 0;
	int action = 0;
	for(int i = 0; i < num_actions; i++)
	{
		prob += (*policy)(i);
		//if (debugAgent) std::cout << std::to_string(prob) + " ";
		if (val < prob) {action = i; break;}
	}
	//if (debugAgent) std::cout << std::to_string(action) + " ";
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
		//save current value function
		if (debugAgent) std::cout << "episode " << episode << '\n';
		if(ag.compute_ep_val)
		{
			if (!ag.parallel) (*ep_value_function).col(episode) = ag.q_function->rowwise().maxCoeff();
			else if (ag.leader_agent) (*ep_value_function).col(episode) = global_q->rowwise().maxCoeff();
		}
		
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
			if (debugAgent) ag.env.printGridEnv();
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
		    	if (debugAgent) std::cout << "\nWIN\n";
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
		    if (debugAgent) std::cout << '\n';
		}
		if (debugAgent) std::cout << "\n --------------------------------------------------- \n" << (*ag.q_function) << "\n\n\n";
	}
	return (void *) &(*ag.q_function);
}

void Agent::computeSaveStatistics(std::string fopt, bool incremental)
{
	std::shared_ptr<Eigen::MatrixXd> med = std::make_shared<Eigen::MatrixXd>(ep_value_function->cols(), 1);
	for (int ep = 0; ep < ep_value_function->cols(); ep++)
	{
		(*med)(ep, 0) = (value_function->col(0) - ep_value_function->col(ep)).sum() / ep_value_function->rows();
	}
	std::string dir = "statistics";
	std::string fn = "grid_size" + std::to_string(env.getGrid()->rows()) + "id" + std::to_string(env.getId()) + fopt;
	Functions::saveMat(med, fn, dir, incremental);
}

std::shared_ptr<Eigen::MatrixXd> Agent::getQ()
{
	return q_function;
}
std::shared_ptr<Eigen::MatrixXd> Agent::getGlobalQ()
{
	return global_q;
}

void Agent::saveQ(std::string fopt, bool incremental)
{
	std::string dir = "qfunc";
	std::string fn = "grid_size" + std::to_string(env.getGrid()->rows()) + "id" + std::to_string(env.getId()) + fopt;
	std::shared_ptr<Eigen::MatrixXd> mat = std::make_shared<Eigen::MatrixXd>();
	if (parallel) mat = global_q;
	else mat = q_function;
	Functions::saveMat(mat, fn, dir, incremental);
}

void Agent::saveEpVF(std::string fopt, bool incremental)
{
	std::string dir = "valfuncepisodes";
	std::string fn = "grid_size" + std::to_string(env.getGrid()->rows()) + "id" + std::to_string(env.getId()) + fopt;
	Functions::saveMat(ep_value_function, fn, dir, incremental);
}

std::shared_ptr<Eigen::MatrixXd> Agent::readQ(std::string fopt)
{
	int gridsize = env.getGrid()->rows();
	std::string dir = "qfunc";
	std::string fn = "grid_size" + std::to_string(gridsize) + "id" + std::to_string(env.getId()) + fopt;
	std::shared_ptr<Eigen::MatrixXd> Q = std::make_shared<Eigen::MatrixXd>();
	if (!Functions::readMat(Q, fn, dir))
		exit(0);
	return Q;
}