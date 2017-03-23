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
	Agent(Environment env, agent_options param_opt, bool save_ep_val = false);
	Agent(Environment env, int init, int end, int param_cache_size, agent_options param_opt, bool save_ep_val = false);
	int epsilonGreedyPolicy(int state, double epsilon);
	static void * learn(void * agent);
	std::shared_ptr<Eigen::MatrixXd> getQ();
	std::shared_ptr<Eigen::MatrixXd> getGlobalQ();
	void saveQ(std::string fopt);
	void saveEpVF(std::string fopt);
	std::shared_ptr<Eigen::MatrixXd> readQ(std::string fn);
	~Agent(){};

private:
	int init_row;
	int end_row;
	int num_actions;
	int cache_size;
	Environment env;
	std::shared_ptr<PosVector> states_list;
	std::shared_ptr<Eigen::MatrixXd> q_function;
	std::shared_ptr<Eigen::VectorXd> q_cache;
	std::shared_ptr<Eigen::VectorXd> value_function;
	bool parallel;
	bool leader_agent;
	bool save_ep_val;
	agent_options opt;

	static int num_agents;
	static std::shared_ptr<Eigen::MatrixXd> global_q;
	static std::shared_ptr<Eigen::MatrixXd> ep_value_function;
};

#endif //AGENT