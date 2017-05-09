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

#ifndef AGENT
#define AGENT

#include "environment.h"
#include <pthread.h>

struct agent_options{
	double eps = 0.1;
	int num_ep = 32000;
	double discount = 0.95;
	double alpha = 1;
	int mse = 10000;
	bool shared_mem = true;
};

class Agent
{
public:
	
	/*
	Constructor for the agent, used when the algorithms works
	in linear mode (only 1 agent)
	@param env: object Environment where the agent should learn
	       param_opt: used to specify some options like epsilon ecc..
	       compute_ep_val: used to specify if the agent should compute
	                    the value function per episode
	*/
	Agent(Environment env, agent_options param_opt, bool compute_ep_val = false);
	
	/*
	Constructor for the agent, used when the algorithms works
	in parallel mode (more than 1 agent)
	@param env: object Environment where the agent should learn
	       init: initial state from where to learn
	       end: last state from where to learn
	       param_cach_size: size of the cache used there is no shared
	                        memory
	       param_opt: used to specify some options like epsilon ecc..
	       compute_ep_val: used to specify if the agent should compute
	                    the value function per episode
	*/
	Agent(Environment env, int init, int end, int param_cache_size, agent_options param_opt, bool compute_ep_val = false);
	
	/*
	function used to compute the epsilon greedy policy from a given
	state
	@param state: index of the state from where to compute the policy
	       epsilon
	@return played action (0 1 2 3)
	*/
	int epsilonGreedyPolicy(int state, double epsilon);

	/*
	static function used to run the q learning function by an agent
	@param agent: agent which learn the function passed as a pointer
	              of void just to satisfy some constraints of pthread
	*/
	static void * learn(void * agent);

	/*
	for the value function of each episode (columns of ep_value_function),
	compute the distance from the true value function of each state and save
	the average of this distance per state in a file in statistics folder
	*/
	void computeSaveStatistics(std::string fopt, bool incremental = false);

	/*
	@return the pointer to local q function
	*/
	std::shared_ptr<Eigen::MatrixXd> getQ();

	/*
	@return the pointer to the global q function
	*/
	std::shared_ptr<Eigen::MatrixXd> getGlobalQ();

	/*
	save the local/global q in a file in qfunc folder
	@param fopt: options of the name of the file, except the default name
	             (ex fopt = "default" --> grid_size14id0default)
	*/
	void saveQ(std::string fopt, bool incremental = false);

	/*
	save the ep_value_function in a file in valfuncepisodes folder
	@param fopt: options of the name of the file, except the default name
	             (ex fopt = "default" --> grid_size14id0default)
	*/
	void saveEpVF(std::string fopt, bool incremental = false);

	/*
	read the q function of a file in qfunc folder
	@param fopt: options of the name of the file, except the default name
	             (ex fopt = "default" --> grid_size14id0default)
	*/
	std::shared_ptr<Eigen::MatrixXd> readQ(std::string fopt);

	~Agent(){};

private:
	int init_row;
	int end_row;
	int num_actions;
	int cache_size;
	Environment env;

	/*
	list of the states of environment as a pointer to an eigen vector of position
	*/
	std::shared_ptr<PosVector> states_list;
	std::shared_ptr<Eigen::MatrixXd> q_function;

	/*
	vector used to handle the cache 
	*/
	std::shared_ptr<Eigen::VectorXd> q_cache;
	std::shared_ptr<Eigen::VectorXd> value_function;

	/*
	logic variable set to true if the algorithm is working in parallel version
	*/
	bool parallel;

	/*
	logic variable set to true if the agent is the leader of a pool of agents
	*/
	bool leader_agent;

	/*
	logic variable set to true if the agent should compute the value function per episode
	*/
	bool compute_ep_val;
	agent_options opt;

	static int num_agents;
	static std::shared_ptr<Eigen::MatrixXd> global_q;

	/*
	pointer to a matrix in which each column correspond to the value function compute
	at an episode
	*/
	static std::shared_ptr<Eigen::MatrixXd> ep_value_function;
};

#endif //AGENT